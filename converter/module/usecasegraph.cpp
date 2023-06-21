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

#include "json_utils.h"
#include "str_utils.h"

using namespace std;
using namespace nlohmann;

using UcNode = UseCaseGraph::UcNode;
using UcEdge = UseCaseGraph::UcEdge;

// -----------------------------------------------------------------------

UcNode::UcNode(string id, string name, Type tp)  {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

UcEdge::UcEdge(string id, string name, Type tp) {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

// -----------------------------------------------------------------------

namespace {

UcEdge::Type edge_type_from_arrow_part(ConstructHelper::Sp ch,
                                       const string& head,
                                       const string& body,
                                       const string& note) {
    const auto it{ find(begin(body), end(body), '.') };
    const auto detected_dot{ it != end(body) ? true : false };

    if ((head == "<" || head == ">") && !detected_dot && note.empty())
        return UcEdge::Association;
    if ((head == "<|" || head == "|>") && !detected_dot && note.empty())
        return UcEdge::Generalization;

    if ((head == "<" || head == ">") && detected_dot && note == "extend")
        return UcEdge::Extend;
    if ((head == "<" || head == ">") && detected_dot && note == "include")
        return UcEdge::Include;

    throw GraphError(ch->line_number, "unknown edge type");
}

shared_ptr<UcNode> create_node_if_need(ConstructHelper::Sp ch, string str) {
    auto node_type = UcNode::Actor;
    if (str.front() == '(' && str.back() == ')') {
        str_utils::trim_by_ref(str, "()");
        node_type = UcNode::Usecase;
    }
    else {
        str_utils::trim_by_ref(str, ":");
    }

    // *** create

    shared_ptr<UcNode> res_node;
    if (!ch->id_node.count(str)) {
        res_node = make_shared<UcNode>(str, str, node_type);
        ch->id_node[str] = res_node;
    }
    else {
        res_node = static_pointer_cast<UcNode>(
                    ch->id_node[str]);
    }
    return res_node;
}

// *** json

string node_type_to_str(const UcNode::Type tp) {
    switch (tp) {
    case UcNode::Actor: return "actor";
    case UcNode::Usecase: return "usecase";
    }

    throw runtime_error{ "node type unknown" };
}

string edge_type_to_str(const UcEdge::Type tp) {
    switch (tp) {
    case UcEdge::Association: return "association";
    case UcEdge::Generalization: return "generalization";
    case UcEdge::Include: return "include";
    case UcEdge::Extend: return "extend";
    }

    throw runtime_error{ "edge type unknown" };
}

json edge_to_json(UcEdge& edge) {
    json result = json_utils::edge_to_whole_json(edge);
    result["type"] = edge_type_to_str(edge.type);
    return result;
}

json node_to_json(UcNode& node) {
    json result = json_utils::node_to_whole_json(node);
    result["type"] = node_type_to_str(node.type);

    result["rob_dia"] = node.id;
    result["seq_dia"] = node.id;

    if (node.rob_graph.expired())
        result["rob_dia"] = nullptr;
    if (node.seq_graph.expired())
        result["seq_dia"] = nullptr;
    return result;
}

// -----------------------------------------------------------------------

bool try_whole_actor_node(ConstructHelper::Sp ch, const std::string& line) {
    smatch match;

    // TODO: вынести все рег. выражения в отдельный файл
    static const regex rx{ "^\\s*actor\\s+(:(.+):|\\\"(.+)\\\")\\s+as\\s+(\\S+)\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto node_id{ match[4].str() };
    const string node_name{ match[2].str().empty() ? match[3].str() : match[2].str()};
    const auto node = make_shared<UcNode>(node_id, node_name, UcNode::Actor);

    ch->id_node[node_id] = node;
    return true;
}

bool try_short_actor_node(ConstructHelper::Sp ch, const std::string& line) {
    smatch match;
    static const regex rx{ "^\\s*:(.+):\\s+as\\s+(\\S+)\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto node = make_shared<UcNode>(match[2].str(), match[1].str(), UcNode::Actor);
    ch->id_node[node->id] = node;
    return true;
}

// -----------------------------------------------------------------------

bool try_whole_usecase_node(ConstructHelper::Sp ch, const std::string& line) {
    smatch match;
    static const regex rx{ "^\\s*usecase\\s+(\\((.+)\\)|\\\"(.+)\\\")\\s+as\\s+(\\S+)\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const string node_id{ match[4].str() };
    const string node_name{ match[2].str().empty() ? match[3].str() : match[2].str()};
    const auto node{ make_shared<UcNode>(node_id, node_name, UcNode::Usecase) };

    ch->id_node[node->id] = node;
    return true;
}

bool try_short_usecase_node(ConstructHelper::Sp ch, const std::string& line) {
    smatch match;
    static const regex rx{ "^\\s*\\((.+)\\)\\s+as\\s+(\\S+)\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto node = make_shared<UcNode>(match[2].str(), match[1].str(), UcNode::Usecase);
    ch->id_node[node->id] = node;
    return true;
}

} // <anonymous>

// -----------------------------------------------------------------------

void UseCaseGraph::read_puml(istream& in) {
    Graph::read_puml(in);
}

void UseCaseGraph::write_json(ostream& out) {
    json json_graph;
    json_graph["id"] = "use_case_dia";

    /* edges */
    {
        json::array_t json_edges; json_edges.reserve(edges.size());
        std::transform(begin(edges), end(edges), std::back_inserter(json_edges),
                       [](const std::shared_ptr<Edge>& one) -> json {
            return edge_to_json(*static_pointer_cast<UcEdge>(one));
        });
        json_graph["edges"] = std::move(json_edges);
    }

    /* nodes */
    {
        json::array_t json_nodes; json_nodes.reserve(nodes.size());
        std::transform(begin(nodes), end(nodes), std::back_inserter(json_nodes),
                       [](const std::shared_ptr<Node>& one) -> json {
            return node_to_json(*static_pointer_cast<UcNode>(one));
        });
        json_graph["nodes"] = std::move(json_nodes);
    }

    out << setw(2) << json_graph;
}

// -----------------------------------------------------------------------

bool UseCaseGraph::try_node(const std::string& line, std::istream&) {
    return try_whole_actor_node(m_ch, line) || try_whole_usecase_node(m_ch, line) ||
            try_short_actor_node(m_ch, line) || try_short_usecase_node(m_ch, line);
}

bool UseCaseGraph::try_connection(const string& line, std::istream&) {
    smatch match;
    static const regex rx{ "^\\s*(:.+:|\\(.+\\)|[^\\s\\:\\(\\)]+)\\s+"
                           "((<|<\\|)?([-\\.]+([lrdu]|left|right|up|down)[-\\.]+|[-\\.]+)(\\|>|>)?)\\s+"
                           "(:.+:|\\(.+\\)|[^\\s\\:\\(\\)]+)\\s*(:\\s*(<<(include|extend)>>))?\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto left_head_arrow{ match[3].str() };
    const auto rght_head_arrow{ match[6].str() };

    if (!left_head_arrow.empty() && !rght_head_arrow.empty()) {
        throw GraphError(m_ch->line_number, "double-sided arrow");
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

    const auto arrow_body{ match[4].str() };
    const auto etype{ edge_type_from_arrow_part(m_ch, arrow_head, arrow_body, match[10].str()) };

    const auto left_node{ create_node_if_need(m_ch, match[1].str()) };
    const auto rght_node{ create_node_if_need(m_ch, match[7].str()) };

    const auto edge{ make_shared<UcEdge>(m_ch->next_edge_id(), match[9].str(), etype) };
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
    m_ch->id_edge[edge->id] = edge;
    return true;
}

// -----------------------------------------------------------------------

bool UseCaseGraph::try_grouping(const string& line, istream& in) {
    if (!try_beg_grouping(line)) {
        return false;
    }

    while (in) {
        const auto next_line{ read_line(in) };
        if (try_end_curly_brace(next_line)) {
            return true;
        }

        if (
                !try_node(next_line, in) &&
                !try_connection(next_line, in) &&
                !try_whitespaces(next_line) &&

                !try_note(next_line, in) &&
                !try_comment(next_line, in) &&

                !try_grouping(next_line, in)) {
            throw GraphError(m_ch->line_number, "unknown line");
        }
    }
    throw GraphError(m_ch->line_number, "group is not closed");
}
