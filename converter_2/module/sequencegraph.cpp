#include <regex>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "sequencegraph.h"
#include "constructhelper.h"
#include "grapherror.h"

#include "str_utils.h"
#include "json_utils.h"
#include "nlohmann/json.hpp"

using namespace std;
using namespace nlohmann;

using SeqNode = SequenceGraph::SeqNode;
using SeqEdge = SequenceGraph::SeqEdge;
using SeqFrag = SequenceGraph::SeqFrag;
using SeqOpd = SequenceGraph::SeqOpd;

// -----------------------------------------------------------------------

SequenceGraph::SeqNode::SeqNode(string id, string name, Type tp) {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

SequenceGraph::SeqEdge::SeqEdge(string id, string name, Type tp) {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

// -----------------------------------------------------------------------

namespace {

ConstructHelper* ch{ nullptr };

SeqEdge::Type edge_type_from_arrow_part(const string& body) {
    const auto part_count{ count(begin(body), end(body), '-') };

    if (part_count == 1)
        return SeqEdge::Sync;
    return SeqEdge::Reply;
}


// *** json and not only

string edge_type_to_str(const SeqEdge::Type tp) {
    switch (tp) {
    case SeqEdge::Sync: return "sync";
    case SeqEdge::Reply: return "reply";
    }

    throw runtime_error{ "seq edge type unknown" };
}

string node_type_to_str(const SeqNode::Type tp) {
    switch (tp) {
    case SeqNode::Actor: return "actor";
    case SeqNode::Boundary: return "boundary";
    case SeqNode::Control: return "control";
    case SeqNode::Entity: return "entity";
    }

    throw runtime_error{ "seq node type unknown" };
}

SeqNode::Type str_to_node_type(const string& str) {
    using Type = SeqNode::Type;
    if (node_type_to_str(Type::Actor) == str) return Type::Actor;
    if (node_type_to_str(Type::Boundary) == str) return Type::Boundary;
    if (node_type_to_str(Type::Control) == str) return Type::Control;
    if (node_type_to_str(Type::Entity) == str) return Type::Entity;
    throw runtime_error{ "seq node type as str unknown" };
}

json edge_to_json(SeqEdge& edge) {
    json result = json_utils::edge_to_json(edge);
    result["type"] = edge_type_to_str(edge.type);

    result["opd"] = nullptr;
    // TODO:
    return result;
}

json node_to_json(SeqNode& node) {
    json result = json_utils::node_to_json(node);
    result["is_error"] = node.is_error;
    result["type"] = node_type_to_str(node.type);
    return result;
}

// -----------------------------------------------------------------------

bool try_whole_node(const std::string& line) {
    smatch match;
    if (!regex_match(line, match, regex("^\\s*(boundary|entity|actor|control)\\s+\"(.+)\"\\s+as\\s+([^\\s#]+)\\s*(#red)?\\s*$"))) {
        return false;
    }

    const auto node_id = match[3].str();
    const auto node_name = match[2].str();
    const auto node_type = str_to_node_type(match[1].str());
    const auto node{ make_shared<SeqNode>(node_id, node_name, node_type) };

    if (match[4].matched) {
        node->is_error = true;
    }

    ch->id_node[node_id] = node;
    return true;
}

bool try_short_node(const std::string& line) {
    smatch match;
    if (!regex_match(line, match, regex("^\\s*(boundary|entity|actor|control)\\s+([^\\s#]+)\\s*(#red)?\\s*$"))) {
        return false;
    }

    const auto node_nmid = match[2].str();
    const auto node_type = str_to_node_type(match[1].str());
    const auto node{ make_shared<SeqNode>(node_nmid, node_nmid, node_type) };

    if (match[3].matched) {
        node->is_error = true;
    }

    ch->id_node[node_nmid] = node;
    return true;
}

} // <anonymous>

// -----------------------------------------------------------------------

void SequenceGraph::read_puml(std::istream& in) {
    ch = m_ch.get();
    Graph::read_puml(in);
    ch = nullptr;
}

void SequenceGraph::write_json(std::ostream& out) {
    using namespace nlohmann;

    json json_graph;
    json_graph["id"] = nullptr;
    if (!uc_node.expired()) {
        auto uc_node_sp = uc_node.lock();
        json_graph["id"] = uc_node_sp->id;
    }

    /* edges */
    {
        json::array_t json_edges;
        for (const auto& edge : edges) {
            auto uc_edge = static_pointer_cast<SeqEdge>(edge);
            json_edges.push_back(edge_to_json(*uc_edge));
        }
        json_graph["edges"] = json_edges;
    }

    /* nodes */
    {
        json::array_t json_nodes;
        for (const auto& node : nodes) {
            auto uc_node = static_pointer_cast<SeqNode>(node);
            json_nodes.push_back(node_to_json(*uc_node));
        }
        json_graph["nodes"] = json_nodes;
    }

    out << setw(2) << json_graph;
}

// -----------------------------------------------------------------------

bool SequenceGraph::try_any(const std::string& line, std::istream& in) {
    return Graph::try_any(line, in);
}

bool SequenceGraph::try_node(const std::string& line, std::istream&) {
    return try_whole_node(line) || try_short_node(line);
}

bool SequenceGraph::try_connection(const std::string& line, std::istream&) {
    smatch match;
    if (!regex_match(line, match, regex("^\\s*(\\S+)\\s+((<)?([-]+)(>)?)\\s+(\\S+)\\s*(:(.*))?\\s*$"))) {
        return false;
    }

    const auto left_node_name = match[1].str();
    const auto rght_node_name = match[6].str();
    if (!ch->id_node.count(left_node_name) || !ch->id_node.count(rght_node_name)) {
        throw runtime_error{ "node not defined" };
    }

    const auto left_node{ static_pointer_cast<SeqNode>(ch->id_node[left_node_name]) };
    const auto rght_node{ static_pointer_cast<SeqNode>(ch->id_node[rght_node_name]) };

    const auto left_head_arrow{ match[3].str() };
    const auto rght_head_arrow{ match[5].str() };
    if (
            (!left_head_arrow.empty() && !rght_head_arrow.empty()) ||
            (left_head_arrow.empty() && rght_head_arrow.empty())) {
        throw GraphError(ch->line_number, "double-sided arrow");
    }

    const auto edge_type = edge_type_from_arrow_part(match[4].str());
    auto edge_text = str_utils::trim_space(match[8].str());
    edge_text = str_utils::un_quote(edge_text);

    const auto edge{ make_shared<SeqEdge>(ch->next_edge_id(), edge_text, edge_type) };
    // -->
    if (left_head_arrow.empty()) {
        edge->beg = left_node;
        edge->end = rght_node;

        left_node->outs.push_back(edge);
        rght_node->inns.push_back(edge);
    }
    // <--
    else {
        edge->end = left_node;
        edge->beg = rght_node;

        left_node->inns.push_back(edge);
        rght_node->outs.push_back(edge);
    }
    ch->id_edge[edge->id] = edge;
    return true;
}
