#include <regex>
#include <iostream>
#include <stdexcept>

#include "robustnessgraph.h"
#include "constructhelper.h"

#include "str_utils.h"
#include "json_utils.h"

using namespace std;
using namespace nlohmann;

using RobNode = RobustnessGraph::RobNode;
using RobEdge = RobustnessGraph::RobEdge;

// -----------------------------------------------------------------------

RobNode::RobNode(string id, string name, Type tp) {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

RobEdge::RobEdge(std::string id, std::string name) {
    this->name = std::move(name);
    this->id = std::move(id);
}

// -----------------------------------------------------------------------

namespace {

ConstructHelper* ch{ nullptr };

// *** json and not only

string node_type_to_str(const RobNode::Type tp) {
    switch (tp) {
    case RobNode::Actor: return "actor";
    case RobNode::Boundary: return "boundary";
    case RobNode::Control: return "control";
    case RobNode::Entity: return "entity";
    }

    throw runtime_error{ "rob node type unknown" };
}

RobNode::Type str_to_node_type(const string& str) {
    using Type = RobNode::Type;
    if (node_type_to_str(Type::Actor) == str) return Type::Actor;
    if (node_type_to_str(Type::Boundary) == str) return Type::Boundary;
    if (node_type_to_str(Type::Control) == str) return Type::Control;
    if (node_type_to_str(Type::Entity) == str) return Type::Entity;
    throw runtime_error{ "rob node type as str unknown" };
}

json edge_to_json(RobustnessGraph::RobEdge& edge) {
    json result = json_utils::edge_to_json(edge);
    return result;
}

json node_to_json(RobustnessGraph::RobNode& node) {
    json result = json_utils::node_to_json(node);
    result["is_error"] = node.is_error;
    result["type"] = node_type_to_str(node.type);
    return result;
}

// -----------------------------------------------------------------------

bool try_whole_node(const std::string& line) {
    smatch match;
    static const regex rx{ "^\\s*(boundary|entity|actor|control)\\s+\"(.+)\"\\s+as\\s+([^\\s#]+)\\s*(#red)?\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto node_id = match[3].str();
    const auto node_name = match[2].str();
    const auto node_type = str_to_node_type(match[1].str()); // regex level check.
    const auto node{ make_shared<RobNode>(node_id, node_name, node_type) };

    if (match[4].matched) {
        node->is_error = true;
    }

    ch->id_node[node_id] = node;
    return true;
}

bool try_short_node(const std::string& line) {
    smatch match;
    static const regex rx{ "^\\s*(boundary|entity|actor|control)\\s+([^\\s#]+)\\s*(#red)?\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto node_nmid = match[2].str();
    const auto node_type = str_to_node_type(match[1].str());
    const auto node{ make_shared<RobNode>(node_nmid, node_nmid, node_type) };

    if (match[3].matched) {
        node->is_error = true;
    }

    ch->id_node[node_nmid] = node;
    return true;
}

} // <anonymous>

// -----------------------------------------------------------------------

void RobustnessGraph::read_puml(std::istream& in) {
    ch = m_ch.get();
    Graph::read_puml(in);
    ch = nullptr;
}

void RobustnessGraph::write_json(std::ostream& out) {
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
            auto uc_edge = static_pointer_cast<RobEdge>(edge);
            json_edges.push_back(edge_to_json(*uc_edge));
        }
        json_graph["edges"] = json_edges;
    }

    /* nodes */
    {
        json::array_t json_nodes;
        for (const auto& node : nodes) {
            auto uc_node = static_pointer_cast<RobNode>(node);
            json_nodes.push_back(node_to_json(*uc_node));
        }
        json_graph["nodes"] = json_nodes;
    }

    out << setw(2) << json_graph;
}

// -----------------------------------------------------------------------

bool RobustnessGraph::try_node(const std::string& line, std::istream&) {
    return try_whole_node(line) || try_short_node(line);
}

bool RobustnessGraph::try_connection(const std::string& line, std::istream&) {
    smatch match;
    static const regex rx{ "^\\s*(\\S+)\\s+((<)?([-]+([lrdu]|left|right|up|down)[-]+|[-]+)(>)?)"
                           "\\s+(\\S+)\\s*(:(.*))?$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto left_node_name = match[1].str();
    const auto rght_node_name = match[7].str();
    if (!ch->id_node.count(left_node_name) || !ch->id_node.count(rght_node_name)) {
        throw runtime_error{ "node not defined" };
    }

    const auto left_node{ static_pointer_cast<RobNode>(ch->id_node[left_node_name]) };
    const auto rght_node{ static_pointer_cast<RobNode>(ch->id_node[rght_node_name]) };

    const auto left_head_arrow{ match[3].str() };
    const auto rght_head_arrow{ match[6].str() };
    auto edge_text = str_utils::trim_space(match[9].str());
    edge_text = str_utils::un_quote(edge_text);

    // TODO: мб. сделать иначе; приводит к добавлению двух дуг, когда встречаются стрелки -- OR <-->
    if ((left_head_arrow.empty() && rght_head_arrow.empty()) ||
        (!left_head_arrow.empty() && !rght_head_arrow.empty())) {

        // -->
        {
            const auto left_to_rght_edge{ make_shared<RobEdge>(ch->next_edge_id(), edge_text) }; // -->
            left_to_rght_edge->beg = left_node;
            left_to_rght_edge->end = rght_node;

            left_node->outs.push_back(left_to_rght_edge);
            rght_node->inns.push_back(left_to_rght_edge);

            ch->id_edge[left_to_rght_edge->id] = left_to_rght_edge;
        }
        // <--
        {
            const auto rght_to_left_edge{ make_shared<RobEdge>(ch->next_edge_id(), edge_text) };
            rght_to_left_edge->beg = rght_node;
            rght_to_left_edge->end = left_node;

            rght_node->outs.push_back(rght_to_left_edge);
            left_node->inns.push_back(rght_to_left_edge);

            ch->id_edge[rght_to_left_edge->id] = rght_to_left_edge;
        }
        return true;
    }

    const auto edge{ make_shared<RobEdge>(ch->next_edge_id(), edge_text) };
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
