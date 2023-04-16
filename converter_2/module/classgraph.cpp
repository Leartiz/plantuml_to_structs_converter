#include <map>
#include <regex>
#include <iostream>
#include <stdexcept>

#include "classgraph.h"

#include "constructhelper.h"
#include "grapherror.h"

#include "json_utils.h"
#include "nlohmann/json.hpp"

using namespace std;
using namespace nlohmann;

using Member = ClassGraph::ClassNode::Member;

ClassGraph::ClassNode::ClassNode(std::string id, std::string name, Type tp) {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

ClassGraph::ClassEdge::ClassEdge(std::string id, std::string name, Type tp) {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

// -----------------------------------------------------------------------


namespace {

ConstructHelper* ch{ nullptr };

// *** json

string node_type_to_str(ClassGraph::ClassNode::Type tp) {
    switch (tp) {
    case ClassGraph::ClassNode::Class: return "class";
    case ClassGraph::ClassNode::Interface: return "interface";
    case ClassGraph::ClassNode::Enum: return "enum";
    }

    throw runtime_error{ "node type unknown" };
}

ClassGraph::ClassNode::Type str_to_node_type(const string& str) {
    using Type = ClassGraph::ClassNode::Type;
    if (node_type_to_str(Type::Class) == str) return Type::Class;
    if (node_type_to_str(Type::Interface) == str) return Type::Interface;
    if (node_type_to_str(Type::Enum) == str) return Type::Enum;
    throw runtime_error{ "node type as str unknown" };
}

string edge_type_to_str(ClassGraph::ClassEdge::Type tp) {
    switch (tp) {
    case ClassGraph::ClassEdge::Association: return "association";
    case ClassGraph::ClassEdge::Dependency: return "dependency";
    case ClassGraph::ClassEdge::Aggregation: return "aggregation";
    case ClassGraph::ClassEdge::Composition: return "composition";
    case ClassGraph::ClassEdge::Implementation: return "implementation";
    case ClassGraph::ClassEdge::Generalization: return "generalization";
    }

    throw runtime_error{ "edge type unknown" };
}

ClassGraph::ClassEdge::Type str_to_edge_type(const string& str) {
    using Type = ClassGraph::ClassEdge::Type;
    if (edge_type_to_str(Type::Dependency) == str) return Type::Dependency;
    if (edge_type_to_str(Type::Association) == str) return Type::Association;
    if (edge_type_to_str(Type::Aggregation) == str) return Type::Aggregation;
    if (edge_type_to_str(Type::Composition) == str) return Type::Composition;
    if (edge_type_to_str(Type::Implementation) == str) return Type::Implementation;
    if (edge_type_to_str(Type::Generalization) == str) return Type::Generalization;
    throw runtime_error{ "edge type as str unknown" };
}

json member_data_to_json(Member data) {

}

json member_func_to_json(Member data) {

}

json edge_to_json(ClassGraph::ClassEdge& edge) {
    json result = json_utils::edge_to_json(edge);
    result["type"] = edge_type_to_str(edge.type);
    return result;
}

json node_to_json(ClassGraph::ClassNode& node) {

}

}

// -----------------------------------------------------------------------

void ClassGraph::read_puml(std::istream& in) {
    ch = m_ch.get();
    Graph::read_puml(in);
    ch = nullptr;
}

void ClassGraph::write_json(std::ostream&) {
    json json_graph;
    json_graph["id"] = "class_dia";

    /* edges */
    {

    }

    /* nodes */
    {

    }
}

// -----------------------------------------------------------------------

bool ClassGraph::try_node(std::string& line, std::istream& in) {
    using ClassNode = ClassGraph::ClassNode;
    using Type = ClassGraph::ClassNode::Type;

    smatch match;
    static const regex rx{ "^\\s*(class|enum|interface)\\s+(\\S+)\\s*{\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false; /* not scary */
    }

    const auto node_nmid = match[2].str();
    const auto node_type = str_to_node_type(match[1].str());

    if (!ch->id_node.count(node_nmid)) {
        auto res_node{ make_shared<ClassNode>(node_nmid, node_nmid, node_type) };
        ch->id_node[node_nmid] = res_node;
    }

    if (node_type == Type::Class)
        try_class_body(node_nmid, in);
    else if (node_type == Type::Interface)
        try_interface_body(node_nmid, in);
    else try_enum_body(node_nmid, in);

    return true;
}

bool ClassGraph::try_connection(std::string& line, std::istream&) {

}

bool ClassGraph::try_grouping(std::string&, std::istream&) {
    return false; // groups are prohibited in this diagram!
}

// -----------------------------------------------------------------------

void ClassGraph::try_class_body(const std::string& nmid, std::istream& in) {
    auto node{ static_pointer_cast<ClassNode>(ch->id_node[nmid]) };
    if (node->type != ClassGraph::ClassNode::Class) {
        throw GraphError(ch->line_number, "node exists with different type");
    }

    while (in) {
        string line;
        getline(in, line);
        ch->line_number++;

        if (try_end_curly_brace(line)) {
            return;
        }

        if (
                !try_class_member(line) &&
                !try_whitespaces(line)) {
            throw GraphError(ch->line_number, "unknown line");
        }
    }

    throw GraphError(ch->line_number, "class body is not closed");
}

void ClassGraph::try_enum_body(const std::string& nmid, std::istream& in) {

}

void ClassGraph::try_interface_body(const std::string& nmid, std::istream& in) {

}

bool ClassGraph::try_class_member(const std::string& line) {
    smatch match;
    static const regex rx{ "^\\s*([+#-]?)(\\w+)\\s*(\\(([\\w<>]+,?\\s*)*\\))?\\s*:\\s*([\\w<>]+)\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    Member member;

}
