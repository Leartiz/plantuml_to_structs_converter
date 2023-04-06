#include <map>
#include <regex>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "usecasegraph.h"
#include "constructhelper.h"
#include "grapherror.h"

#include "nlohmann/json.hpp"

using namespace std;

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

shared_ptr<UseCaseGraph::UcNode> add_node(const string& str) {
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

}

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
                !try_directive(line) &&
                !try_skinparam(line)) {
            throw GraphError(ch->line_number, "unknown line");
        }
    }

    nodes = ch->to_nodes();
    edges = ch->to_edges();
}

void UseCaseGraph::write_json(ostream& out) {
    using namespace nlohmann;

    json json_graph;
    json_graph["id"] = "use_case_graph";

    json::array_t json_nodes;
    for (auto node : nodes) {
        auto uc_node = static_pointer_cast<UcNode>(node);

        json uc_node_json;


    }
    json::array_t json_edges;


    out << setw(2) << json_graph;
}

// -----------------------------------------------------------------------

bool UseCaseGraph::try_actor_node(string& line) {
    std::smatch match;
    if (!regex_match(line, match, regex("^\\s*actor\\s*(:.+:|\".+\")\\s*as\\s*(\\S*)\\s*$"))) {
        return false;
    }

    const auto node_id{ match[2].str() };
    const auto node = make_shared<UcNode>(node_id, match[1].str(), UcNode::Actor);

    ch->id_node[node_id] = node;
    return true;
}

bool UseCaseGraph::try_usecase_node(string& line) {
    std::smatch match;
    if (!regex_match(line, match, regex("^\\s*usecase\\s*(\\(.+\\)|\\\".+\\\")\\s*as\\s*(\\S*)\\s*$"))) {
        return false;
    }

    const auto node_id{ match[2].str() };
    const auto node{ make_shared<UcNode>(node_id, match[1].str(), UcNode::UseCase) };

    ch->id_node[node->id] = node;
    return true;
}

bool UseCaseGraph::try_connection(string& line)
{
    std::smatch match;
    if (!regex_match(line, match, regex("^\\s*(\\S*)\\s+((<|<\\|)?([-\\.]+)(>|\\|>)?)\\s+(\\S*)\\s*(:\\s*(<<(include|extend)>>))?\\s*$"))) {
        return false;
    }

    auto left_head_arrow{ match[3].str() };
    auto rght_head_arrow{ match[5].str() };

    if (!left_head_arrow.empty() && !rght_head_arrow.empty()) {
        throw GraphError(ch->line_number, "double-sided arrow");
    }

    auto arrow_body{ match[4].str() };
    auto cm_head_arrow{ left_head_arrow.empty() ? rght_head_arrow : left_head_arrow };
    auto tp = edge_type_from_arrow_parts(cm_head_arrow, arrow_body, match[9].str());

    const auto left_node{ add_node(match[1].str()) };
    const auto right_node{ add_node(match[6].str()) };

    const auto edge{ make_shared<UcEdge>(ch->next_edge_id(), match[8].str(), tp) };
    edge->beg = left_node;
    edge->end = right_node;

    left_node->outs.push_back(edge);
    right_node->inns.push_back(edge);
    ch->id_edge[edge->id] = edge;

    return true;
}



