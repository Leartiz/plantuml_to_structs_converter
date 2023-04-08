#include <map>
#include <regex>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "usecasegraph.h"
#include "robustnessgraph.h"
#include "sequencegraph.h"

#include "constructhelper.h"
#include "grapherror.h"

#include "nlohmann/json.hpp"

using namespace std;
using namespace nlohmann;

UseCaseGraph::UcNode::UcNode(string id, string name, Type tp)  {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

UseCaseGraph::UcEdge::UcEdge(string id, string name, Type tp) {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

// -----------------------------------------------------------------------

namespace {

const auto ch{ make_shared<ConstructHelper>() };

UseCaseGraph::UcEdge::Type edge_type_from_arrow_parts(const string& head,
                                                      const string& body,
                                                      const string& note) {
    const auto it{ find(begin(body), end(body), '.') };
    const auto detected_dot{ it != end(body) ? true : false };

    if ((head == "<" || head == ">") && !detected_dot && note.empty())
        return UseCaseGraph::UcEdge::Association;
    if ((head == "<|" || head == "|>") && !detected_dot && note.empty())
        return UseCaseGraph::UcEdge::Generalization;

    if ((head == "<" || head == ">") && detected_dot && note == "extend")
        return UseCaseGraph::UcEdge::Extend;
    if ((head == "<" || head == ">") && detected_dot && note == "include")
        return UseCaseGraph::UcEdge::Include;
    throw GraphError(ch->line_number, "unknown edge type");
}

shared_ptr<UseCaseGraph::UcNode> create_node_if_need(const string& str) {
    shared_ptr<UseCaseGraph::UcNode> res_node;
    if (!ch->id_node.count(str)) {
        res_node = make_shared<UseCaseGraph::UcNode>(str, str, UseCaseGraph::UcNode::Actor);
        ch->id_node[str] = res_node;
    }
    else {
        res_node = static_pointer_cast<UseCaseGraph::UcNode>(
                    ch->id_node[str]);
    }
    return res_node;
}

// *** json

string node_type_to_str(const UseCaseGraph::UcNode::Type tp) {
    switch (tp) {
    case UseCaseGraph::UcNode::Actor: return "actor";
    case UseCaseGraph::UcNode::UseCase: return "usecase";
    }

    throw runtime_error{ "node type unknown" };
}

string edge_type_to_str(const UseCaseGraph::UcEdge::Type tp) {
    switch (tp) {
    case UseCaseGraph::UcEdge::Association: return "association";
    case UseCaseGraph::UcEdge::Generalization: return "generalization";
    case UseCaseGraph::UcEdge::Include: return "include";
    case UseCaseGraph::UcEdge::Extend: return "extend";
    }

    throw runtime_error{ "edge type unknown" };
}

// TODO: создать новый тип исключений?
json edge_to_json(UseCaseGraph::UcEdge& edge) {
    if (edge.beg.expired() || edge.end.expired()) {
        throw runtime_error{ "beg or end expired" };
    }

    json result;
    result["id"] = edge.id;
    result["name"] = edge.name;
    result["type"] = edge_type_to_str(edge.type);
    result["beg"] = { { "id", edge.beg.lock()->id } };
    result["end"] = { { "id", edge.end.lock()->id } };
    return result;
}

// TODO: вектор слабых ссылок, можно изменить?
json edges_to_short_json(vector<weak_ptr<Graph::Edge>> edges) {
    const string id_key{ "id" };
    auto json_edges = json::array();
    for_each(begin(edges), end(edges), [&](const weak_ptr<Graph::Edge> edge_wp) {
        if (const auto edge_sp = edge_wp.lock(); edge_sp) {
            json_edges.push_back({ { id_key, edge_sp->id } });
            return;
        }
        throw runtime_error{ "inn/out edge expired" };
    });

    sort(begin(json_edges), end(json_edges), [&](const json& lhs, const json& rhs) {
        return lhs[id_key] < rhs[id_key];
    });
    return json_edges;
}

json node_to_json(UseCaseGraph::UcNode& node) {
    json result;
    result["id"] = node.id;
    result["name"] = node.name;
    result["type"] = node_type_to_str(node.type);

    result["inn_edges"] = edges_to_short_json(node.inns);
    result["out_edges"] = edges_to_short_json(node.outs);

    result["rob_dia"] = node.id;
    result["seq_dia"] = node.id;
    if (!node.rob_graph) result["rob_dia"] = nullptr;
    if (!node.seq_graph) result["seq_dia"] = nullptr;

    return result;
}

}

// -----------------------------------------------------------------------

void UseCaseGraph::read_puml(istream& in) {
    ch->reset();
    while (in) {
        string line;
        getline(in, line);
        ch->line_number++;

        if (
                !try_actor_node(line) &&
                !try_usecase_node(line) &&
                !try_connection(line) &&
                !try_whitespaces(line) &&
                !try_grouping(line, in) &&

                !try_directive(line) &&
                !try_skinparam(line) &&
                !try_direction(line)) {
            throw GraphError(ch->line_number, "unknown line");
        }
    }

    nodes = ch->to_nodes();
    edges = ch->to_edges();
}

void UseCaseGraph::write_json(ostream& out) {
    using namespace nlohmann;

    json json_graph;
    json_graph["id"] = "use_case_dia";

    /* edges */
    {
        json::array_t json_edges;
        for (const auto& edge : edges) {
            auto uc_edge = static_pointer_cast<UcEdge>(edge);
            json_edges.push_back(edge_to_json(*uc_edge));
        }
        json_graph["edges"] = json_edges;
    }

    /* nodes */
    {
        json::array_t json_nodes;
        for (const auto& node : nodes) {
            auto uc_node = static_pointer_cast<UcNode>(node);
            json_nodes.push_back(node_to_json(*uc_node));
        }
        json_graph["nodes"] = json_nodes;
    }

    out << setw(2) << json_graph;
}

// -----------------------------------------------------------------------

bool UseCaseGraph::try_actor_node(string& line) {
    smatch match;
    if (!regex_match(line, match, regex("^\\s*actor\\s+(:(.+):|\\\"(.+)\\\")\\s+as\\s+(\\S+)\\s*$"))) {
        return false;
    }

    const auto node_id{ match[4].str() };
    const string node_name{ match[2].str().empty() ? match[3].str() : match[2].str()};
    const auto node = make_shared<UcNode>(node_id, node_name, UcNode::Actor);

    ch->id_node[node_id] = node;
    return true;
}

bool UseCaseGraph::try_usecase_node(string& line) {
    smatch match;
    if (!regex_match(line, match, regex("^\\s*usecase\\s+(\\((.+)\\)|\\\"(.+)\\\")\\s+as\\s+(\\S+)\\s*$"))) {
        return false;
    }

    const string node_id{ match[4].str() };
    const string node_name{ match[2].str().empty() ? match[3].str() : match[2].str()};
    const auto node{ make_shared<UcNode>(node_id, node_name, UcNode::UseCase) };

    ch->id_node[node->id] = node;
    return true;
}

// -----------------------------------------------------------------------

bool UseCaseGraph::try_connection(string& line) {
    smatch match;

    if (!regex_match(line, match, regex("^\\s*(\\S+)\\s+((<|<\\|)?([-\\.]+([lrdu]|left|right|up|down)[-\\.]+|[-\\.]+)(\\|>|>)?)"
                                        "\\s+(\\S+)\\s*(:\\s*(<<(include|extend)>>))?\\s*$"))) {
        return false;
    }

    auto left_head_arrow{ match[3].str() };
    auto rght_head_arrow{ match[6].str() };

    if (!left_head_arrow.empty() && !rght_head_arrow.empty()) {
        throw GraphError(ch->line_number, "double-sided arrow");
    }

    string arrow_head;
    bool is_left_to_rght{ true };
    // <--
    if (rght_head_arrow.empty()) {
        arrow_head = left_head_arrow;
        is_left_to_rght = false;
    }
    // -->
    else {
        arrow_head = rght_head_arrow;
    }

    auto arrow_body{ match[4].str() };
    auto etype{ edge_type_from_arrow_parts(arrow_head, arrow_body, match[10].str()) };

    const auto left_node{ create_node_if_need(match[1].str()) };
    const auto rght_node{ create_node_if_need(match[7].str()) };

    const auto edge{ make_shared<UcEdge>(ch->next_edge_id(), match[9].str(), etype) };
    if (is_left_to_rght) {
        edge->beg = left_node;
        edge->end = rght_node;

        left_node->outs.push_back(edge);
        rght_node->inns.push_back(edge);
    }
    else {
        edge->beg = rght_node;
        edge->end = left_node;

        left_node->inns.push_back(edge);
        rght_node->outs.push_back(edge);
    }
    ch->id_edge[edge->id] = edge;
    return true;
}

// -----------------------------------------------------------------------

bool UseCaseGraph::try_grouping(string& line, istream& in) {
    if (!try_beg_grouping(line)) {
        return false;
    }

    bool is_closed{ false };
    while (in) {
        string line;
        getline(in, line);
        ch->line_number++;

        if (try_end_curly_brace(line)) {
            is_closed = true;
            break;
        }

        if (
                !try_actor_node(line) &&
                !try_usecase_node(line) &&
                !try_connection(line) &&
                !try_whitespaces(line) &&
                !try_grouping(line, in)) {
            throw GraphError(ch->line_number, "unknown line");
        }
    }

    if (!is_closed) {
        throw GraphError(ch->line_number, "group is not closed");
    }

    return true;
}
