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

// TODO: и тесты.

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
                !try_grouping(line) &&

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

    // TODO:

    out << setw(2) << json_graph;
}

// -----------------------------------------------------------------------

bool UseCaseGraph::try_actor_node(string& line) {
    std::smatch match;
    if (!regex_match(line, match, regex("^\\s*actor\\s+(:(.+):|\"(.+)\")\\s+as\\s+(\\S+)\\s*$"))) {
        return false;
    }

    const auto node_id{ match[4].str() };
    const auto node = make_shared<UcNode>(node_id, match[2].str(), UcNode::Actor);

    ch->id_node[node_id] = node;
    return true;
}

bool UseCaseGraph::try_usecase_node(string& line) {
    std::smatch match;
    if (!regex_match(line, match, regex("^\\s*usecase\\s+(\\((.+)\\)|\\\"(.+)\\\")\\s+as\\s+(\\S+)\\s*$"))) {
        return false;
    }

    const auto node_id{ match[4].str() };
    const auto node{ make_shared<UcNode>(node_id, match[2].str(), UcNode::UseCase) };

    ch->id_node[node->id] = node;
    return true;
}

// -----------------------------------------------------------------------

bool UseCaseGraph::try_connection(string& line) {
    std::smatch match;

    if (!regex_match(line, match, regex("^\\s*(\\S+)\\s+((<|<\\|)?([-\\.]+([lrdu]|left|right|up|down)[-\\.]+|[-\\.]+)(\\|>|>)?)\\s+(\\S+)\\s*(:\\s*(<<(include|extend)>>))?\\s*$"))) {
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

bool UseCaseGraph::try_grouping(std::string& line) {
    // TODO: автомат или глобальный поток для чтения.
}
