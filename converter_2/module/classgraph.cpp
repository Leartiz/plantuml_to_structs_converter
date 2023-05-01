#include <map>
#include <stack>
#include <regex>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "classgraph.h"

#include "constructhelper.h"
#include "grapherror.h"

#include "str_utils.h"
#include "json_utils.h"

using namespace std;
using namespace nlohmann;

using ClassNode = ClassGraph::ClassNode;
using ClassEdge = ClassGraph::ClassEdge;
using Member = ClassNode::Member;

// -----------------------------------------------------------------------

ClassNode::ClassNode(std::string id, std::string name, Type tp) {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

Member::Member(Mark mrk, std::string name, std::string type)
    : mark{ mrk }, name{ std::move(name) }, type{ std::move(type) } {}

ClassEdge::ClassEdge(std::string id, std::string name, Type tp) {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

// -----------------------------------------------------------------------

namespace {

ConstructHelper* ch{ nullptr };

ClassEdge::Type edge_type_from_arrow_part(const string& head,
                                          const string& body) {
    using Type = ClassEdge::Type;
    const auto it{ find(begin(body), end(body), '.') };
    const auto detected_dot{ it != end(body) ? true : false };

    if ((head == "<" || head == ">") && detected_dot) {
        return Type::Dependency;
    }
    else if ((head == "<" || head == ">") && !detected_dot) {
        return Type::Association;
    }

    else if (head == "o" && !detected_dot) {
        return Type::Aggregation;
    }
    else if (head == "*" && !detected_dot) {
        return Type::Composition;
    }

    else if ((head == "<|" || head == "|>") && detected_dot) {
        return Type::Implementation;
    }
    else if ((head == "<|" || head == "|>") && !detected_dot) {
        return Type::Generalization;
    }

    throw GraphError{ ch->line_number, "unknown edge type" };
}

shared_ptr<ClassNode> create_node_if_need(const string& str) {
    shared_ptr<ClassNode> res_node;
    if (!ch->id_node.count(str)) {
        res_node = make_shared<ClassNode>(str, str, ClassNode::Class);
        ch->id_node[str] = res_node;
    }
    else {
        res_node = static_pointer_cast<ClassNode>(ch->id_node[str]);
    }
    return res_node;
}

Member::Mark str_to_mark(const string str) {
    auto result{ Member::Public };
    if (str.empty()) {
        return result;
    }
    if (str.size() != 1) {
        throw runtime_error{ "mark as str too long" };
    }

    switch (str.front()) {
    case '+':
        result = Member::Public;
        break;
    case '-':
        result = Member::Private;
        break;
    case '#':
        result = Member::Protected;
        break;

    default:
        throw runtime_error{ "mark as str unknown" };
    }
    return result;
}

string mark_to_str(Member::Mark mark) {
    switch (mark) {
    case Member::Public: return "+";
    case Member::Private: return "-";
    case Member::Protected: return "#";
    }

    throw runtime_error{ "mark unknown" };
}

vector<string> str_to_param_types(const string& str) {
    vector<string> result;
    istringstream sin{ str };
    while (!sin.eof()) {
        string param_type;
        getline(sin, param_type, ',');

        str_utils::trim_space_by_ref(param_type);
        if (param_type.empty()) continue;

        if (str_utils::is_trgle_bracket_balance(param_type)) {
            result.push_back(std::move(param_type));
            continue;
        }

        throw GraphError{ ch->line_number, "invalid param type" };
    }
    return result;
}

// *** json

string node_type_to_str(ClassNode::Type tp) {
    switch (tp) {
    case ClassNode::Class: return "class";
    case ClassNode::Interface: return "interface";
    case ClassNode::Enum: return "enum";
    }

    throw runtime_error{ "node type unknown" };
}

ClassNode::Type str_to_node_type(const string& str) {
    using Type = ClassNode::Type;
    if (node_type_to_str(Type::Class) == str) return Type::Class;
    if (node_type_to_str(Type::Interface) == str) return Type::Interface;
    if (node_type_to_str(Type::Enum) == str) return Type::Enum;
    throw runtime_error{ "node type as str unknown" };
}

string edge_type_to_str(ClassEdge::Type tp) {
    switch (tp) {
    case ClassEdge::Association: return "association";
    case ClassEdge::Dependency: return "dependency";
    case ClassEdge::Aggregation: return "aggregation";
    case ClassEdge::Composition: return "composition";
    case ClassEdge::Implementation: return "implementation";
    case ClassEdge::Generalization: return "generalization";
    }

    throw runtime_error{ "edge type unknown" };
}

// ***

json member_data_to_json(const Member& data) {
    json json_data;
    json_data["mark"] = mark_to_str(data.mark);
    json_data["name"] = data.name;
    json_data["type"] = data.type;
    return json_data;
}

json member_func_to_json(const Member& func) {
    json::array_t json_pt;
    json json_func = member_data_to_json(func);
    for (const auto& one_pt : func.param_types)
        json_pt.push_back(one_pt);
    json_func["param_types"] = json_pt;
    return json_func;
}

json::array_t datas_to_json(const vector<Member>& datas) {
    json::array_t json_datas;
    for (const auto& mb : datas)
        json_datas.push_back(member_data_to_json(mb));
    return json_datas;
}

json::array_t funcs_to_json(const vector<Member>& funcs) {
    json::array_t json_funcs;
    for (const auto& mb : funcs)
        json_funcs.push_back(member_func_to_json(mb));
    return json_funcs;
}

json::array_t enum_values_to_json(const vector<string>& enum_values) {
    json::array_t json_evals;
    copy(enum_values.begin(), enum_values.end(),
         back_inserter(json_evals));
    return json_evals;
}

json edge_to_json(ClassEdge& edge) {
    json result = json_utils::edge_to_whole_json(edge);
    result["type"] = edge_type_to_str(edge.type);
    return result;
}

json node_to_json(ClassNode& node) {
    json result = json_utils::node_to_whole_json(node);
    result["type"] = node_type_to_str(node.type);

    if (node.type == ClassNode::Class) {
        result["datas"] = datas_to_json(node.datas);
        result["funcs"] = funcs_to_json(node.funcs);
    }
    else if (node.type == ClassNode::Interface) {
        result["funcs"] = funcs_to_json(node.funcs);
    }
    else {
        result["enum_values"] = enum_values_to_json(node.enum_values);
    }
    return result;
}

// -----------------------------------------------------------------------

bool try_interface_member_func(std::shared_ptr<ClassNode> node, const std::string& line) {
    smatch match;
    static const regex rx{ "^\\s*\\+?(\\w+)\\s*(\\(([\\w<>]+,?\\s*)*\\))\\s*(:\\s*([\\w<>]+))?\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto mem_name{ match[1].str() };
    const auto mem_type{ match[5].str() };

    Member member{ Member::Public, mem_name, mem_type };
    member.param_types = str_to_param_types(str_utils::trim(match[2].str(), "()"));
    node->funcs.push_back(std::move(member));
    return true;
}

bool try_class_member(std::shared_ptr<ClassNode> node, const std::string& line) {
    smatch match;
    static const regex rx{ "^\\s*([+#-])?(\\w+)\\s*(\\(([\\w<>]+(\\s*,\\s*)?\\s*)*\\))?\\s*(:\\s*([\\w<>]+))\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto mem_name{ match[2].str() };
    const auto mem_type{ match[7].str() }; // <empty> == void

    try {
        Member member{ str_to_mark(match[1].str()), mem_name, mem_type };
        if (!match[3].matched) {
            node->datas.push_back(std::move(member));
        }
        else {
            member.param_types = str_to_param_types(str_utils::trim(match[3].str(), "()"));
            node->funcs.push_back(std::move(member));
        }
    }
    catch (const runtime_error& err) {
        throw GraphError{ ch->line_number, err.what() };
    }
    return true;
}

bool try_enum_value(std::shared_ptr<ClassNode> node, const std::string& line) {
    smatch match;
    static const regex rx{ "^\\s*(\\w+)\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto eval{ match[1].str() };
    node->enum_values.push_back(eval);
    return true;
}

// -----------------------------------------------------------------------

bool try_hide_empty_members(const std::string& line) {
    static const regex rx{ "^\\s*hide\\s+empty\\s+(methods|members)\\s*$" };
    return regex_match(line, rx);
}

} // <anonymous>

// -----------------------------------------------------------------------

void ClassGraph::read_puml(std::istream& in) {
    ch = m_ch.get();
    Graph::read_puml(in);
    ch = nullptr;
}

void ClassGraph::write_json(std::ostream& out) {
    json json_graph;
    json_graph["id"] = "class_dia";

    /* edges */
    {
        // TODO: можно спрятать за полиморфизмом!
        json::array_t json_edges;
        for (const auto& edge : edges) {
            auto class_edge = static_pointer_cast<ClassEdge>(edge);
            json_edges.push_back(edge_to_json(*class_edge));
        }
        json_graph["edges"] = json_edges;
    }

    /* nodes */
    {
        json::array_t json_nodes;
        for (const auto& node : nodes) {
            auto class_node = static_pointer_cast<ClassNode>(node);
            json_nodes.push_back(node_to_json(*class_node));
        }
        json_graph["nodes"] = json_nodes;
    }

    out << setw(2) << json_graph;
}

// -----------------------------------------------------------------------

bool ClassGraph::try_any(const std::string& line, std::istream& in) {
    return Graph::try_any(line, in) || try_hide_empty_members(line);
}

bool ClassGraph::try_node(const std::string& line, std::istream& in) {
    using ClassNode = ClassGraph::ClassNode;
    using Type = ClassGraph::ClassNode::Type;

    smatch match;
    static const regex rx{ "^\\s*(class|enum|interface)\\s+(\\S+)\\s*\\{\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false; /* not scary */
    }

    const auto node_nmid = match[2].str();
    const auto node_type = str_to_node_type(match[1].str()); // regex level check.

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

bool ClassGraph::try_connection(const std::string& line, std::istream&) {
    smatch match;
    static const regex rx{ "^\\s*(\\S+)\\s+"
                           "((<|o|\\*|<\\|)?([-\\.]+([lrdu]|left|right|up|down)[-\\.]+|[-\\.]+)(\\|>|>|o|\\*)?)\\s+"
                           "(\\S+)\\s*(:(.+))?$" };
    if (!regex_match(line, match, rx)) {
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
    auto etype{ edge_type_from_arrow_part(arrow_head, arrow_body) };
    if (etype == ClassEdge::Composition || etype == ClassEdge::Aggregation) {
        is_left_to_rght = !is_left_to_rght;
    }

    const auto left_node{ create_node_if_need(match[1].str()) };
    const auto rght_node{ create_node_if_need(match[7].str()) };

    auto edge_text = str_utils::trim_space(match[9].str());
    edge_text = str_utils::un_quote(edge_text);
    const auto edge{ make_shared<ClassEdge>(ch->next_edge_id(), edge_text, etype) };
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

// TODO: много дублирующегося кода - подумать как убрать
// -----------------------------------------------------------------------

void ClassGraph::try_interface_body(const std::string& nmid, std::istream& in) {
    auto node{ static_pointer_cast<ClassNode>(ch->id_node[nmid]) };
    if (node->type != ClassNode::Interface) {
        throw GraphError{ ch->line_number, "node has no interface type" };
    }

    while (in) {
        const auto line{ read_line(in) };
        if (try_end_curly_brace(line)) {
            return;
        }

        if (
                !try_interface_member_func(node, line) &&
                !try_whitespaces(line) && !try_comment(line, in)) {
            throw GraphError(ch->line_number, "unknown line");
        }
    }

    throw GraphError{ ch->line_number, "interface body is not closed" };
}

void ClassGraph::try_class_body(const std::string& nmid, std::istream& in) {
    auto node{ static_pointer_cast<ClassNode>(ch->id_node[nmid]) };
    if (node->type != ClassNode::Class) {
        throw GraphError{ ch->line_number, "node has no class type" };
    }

    while (in) {
        const auto line{ read_line(in) };
        if (try_end_curly_brace(line)) {
            return;
        }

        if (
                !try_class_member(node, line) &&
                !try_whitespaces(line) && !try_comment(line, in)) {
            throw GraphError(ch->line_number, "unknown line");
        }
    }

    throw GraphError{ ch->line_number, "class body is not closed" };
}

void ClassGraph::try_enum_body(const std::string& nmid, std::istream& in) {
    auto node{ static_pointer_cast<ClassNode>(ch->id_node[nmid]) };
    if (node->type != ClassNode::Enum) {
        throw GraphError{ ch->line_number, "node has no enum type" };
    }

    while (in) {
        const auto line{ read_line(in) };
        if (try_end_curly_brace(line)) {
            return;
        }

        if (
                !try_enum_value(node, line) &&
                !try_whitespaces(line) && !try_comment(line, in)) {
            throw GraphError(ch->line_number, "unknown line");
        }
    }

    throw GraphError{ ch->line_number, "enum body is not closed" };
}
