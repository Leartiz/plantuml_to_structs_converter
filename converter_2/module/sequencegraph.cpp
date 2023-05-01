#include <regex>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "sequencegraph.h"
#include "constructhelper.h"
#include "grapherror.h"

#include "str_utils.h"
#include "json_utils.h"

using namespace std;
using namespace nlohmann;

using Stamp = SequenceGraph::Stamp;

using SeqNode = SequenceGraph::SeqNode;
using SeqEdge = SequenceGraph::SeqEdge;

using SeqGroup = SequenceGraph::SeqGroup;
using SeqFrag = SequenceGraph::SeqFrag;
using SeqOpd = SequenceGraph::SeqOpd;
using SeqRef = SequenceGraph::SeqRef;

// -----------------------------------------------------------------------

SeqGroup::SeqGroup(uint32_t onum, std::string id,
                   std::shared_ptr<SeqOpd> root) {
    this->order_number = onum;
    this->root_opd = root;

    this->id = std::move(id);
}

SeqRef::SeqRef(uint32_t onum, std::string id, std::string text,
               std::shared_ptr<SeqOpd> root)
    : SeqGroup{ onum, id, root }, text{ std::move(text) } {}

SeqFrag::SeqFrag(uint32_t onum, string id, Type tp,
                 std::shared_ptr<SeqOpd> root)
    : SeqGroup{ onum, id, root }, type{ tp } {}

SeqOpd::SeqOpd(uint32_t onum, std::string id, std::string condition) {
    this->order_number = onum;
    this->condition = std::move(condition);
    this->id = std::move(id);
}

// -----------------------------------------------------------------------

SequenceGraph::SeqNode::SeqNode(string id, string name, Type tp) {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

SequenceGraph::SeqEdge::SeqEdge(uint32_t onum, string id, string name, Type tp) {
    this->order_number = onum;
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

vector<string> str_to_node_names(const string& str) {
    vector<string> result;
    istringstream sin{ str };
    while (!sin.eof()) {
        string node_mame;
        getline(sin, node_mame, ',');

        str_utils::trim_space_by_ref(node_mame);
        if (!node_mame.empty()) {
            result.push_back(std::move(node_mame));
            continue;
        }

        throw GraphError{ ch->line_number, "invalid node name" };
    }
    return result;
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

// ***

string frag_type_to_str(const SeqFrag::Type tp) {
    switch (tp) {
    case SeqFrag::Opt: return "opt";
    case SeqFrag::Alt: return "alt";
    case SeqFrag::Loop: return "loop";
    }

    throw runtime_error{ "seq fragment type unknown" };
}

SeqFrag::Type str_to_frag_type(const string& str) {
    using Type = SeqFrag::Type;
    if (frag_type_to_str(Type::Loop) == str) return Type::Loop;
    if (frag_type_to_str(Type::Alt) == str) return Type::Alt;
    if (frag_type_to_str(Type::Opt) == str) return Type::Opt;
    throw runtime_error{ "seq fragment type as str unknown" };
}

// ***

json opd_to_json(SeqOpd& opd) {
    json result;
    result["id"] = opd.id;
    result["condition"] = opd.condition;
    result["order_number"] = opd.order_number;

    if (opd.frag.expired()) {
        throw runtime_error{ "lost seq operand" };
    }

    result["frag"] = opd.frag.lock()->id;
    return result;
}

json ref_to_json(SeqRef& rf) {
    json result;
    result["id"] = rf.id;
    result["text"] = rf.text;
    result["order_number"] = rf.order_number;

    // may not exist!
    result["root_opd"] = nullptr;
    if (!rf.root_opd.expired()) {
        result["root_opd"] = rf.root_opd.lock()->id;
    }

    // ***

    json::array_t json_nodes;
    std::for_each(begin(rf.nodes), end(rf.nodes),
                  [&json_nodes](const weak_ptr<SeqNode>& node_wp) -> void {
        if (!node_wp.expired()) {
            const auto node_sp = node_wp.lock();
            json_nodes.push_back(json_utils::node_to_short_json(*node_sp));
            return;
        }
        throw runtime_error{ "node expired" };
    });

    result["nodes"] = json_nodes;
    return result;
}

json frag_to_json(SeqFrag& frag) {
    json result;
    result["id"] = frag.id;
    result["type"] = frag_type_to_str(frag.type);
    result["order_number"] = frag.order_number;

    // may not exist!
    result["root_opd"] = nullptr;
    if (!frag.root_opd.expired()) {
        result["root_opd"] = frag.root_opd.lock()->id;
    }

    // ***

    json::array_t json_opds;
    std::for_each(begin(frag.opds), end(frag.opds),
                  [&json_opds](const shared_ptr<SeqOpd>& opd_sp) -> void {
        json_opds.push_back(opd_to_json(*opd_sp));
    });

    result["opds"] = json_opds;
    return result;
}

// ***

json stamp_to_json(Stamp& stamp) {
    json result;
    string stamp_id;
    if (const auto edge = dynamic_cast<SeqEdge*>(&stamp); edge) stamp_id = edge->id;
    else if (const auto group = dynamic_cast<SeqGroup*>(&stamp); group) stamp_id = group->id;
    else if (const auto opd = dynamic_cast<SeqOpd*>(&stamp); opd) stamp_id = opd->id;
    else throw runtime_error{ "stamp has no id" };

    result["order_number"] = stamp.order_number;
    result["id"] = stamp_id;
    return result;
}

// ***

json edge_to_json(SeqEdge& edge) {
    json result = json_utils::edge_to_whole_json(edge);
    result["type"] = edge_type_to_str(edge.type);
    result["order_number"] = edge.order_number;

    // may not exist!
    result["opd"] = nullptr;
    if (!edge.opd.expired())
        result["opd"] = edge.opd.lock()->id;
    return result;
}

json node_to_json(SeqNode& node) {
    json result = json_utils::node_to_whole_json(node);
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
    static const regex rx{ "^\\s*(boundary|entity|actor|control)\\s+([^\\s#]+)\\s*(#red)?\\s*$" };
    if (!regex_match(line, match, rx)) {
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

// -----------------------------------------------------------------------

bool try_three_dots(const std::string& line) {
    static const regex rx{ "^\\s*[.]{3}\\s*$" };
    return regex_match(line, rx);
}

bool try_operand_else(const std::string& line, shared_ptr<SeqFrag> frag) {
    smatch match;
    static const regex rx{ "^\\s*else\\s+(.*)$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto opd_cond = str_utils::trim_space(match[1].str());
    auto opd = make_shared<SeqOpd>(ch->next_order_number(), ch->next_opd_id(), opd_cond);
    frag->opds.push_back(opd);
    opd->frag = frag;
    return true;
}

bool try_operand_end(const std::string& line) {
    static const regex rx{ "^\\s*end\\s*$" };
    return regex_match(line, rx);
}

bool try_end_ref(const std::string& line) {
    static const regex rx{ "^\\s*end\\s+ref\\s*$" };
    return regex_match(line, rx);
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

    /* groups */
    {
        /* frags */
        {
            json::array_t json_frags;
            for (const auto& frag : frags)
                json_frags.push_back(frag_to_json(*frag));
            json_graph["frags"] = json_frags;
        }
        /* refs */
        {
            json::array_t json_refs;
            for (const auto& rf : refs)
                json_refs.push_back(ref_to_json(*rf));
            json_graph["refs"] = json_refs;
        }
    }

    /* stamps */
    {
        {
            json::array_t json_stamps;
            for (const auto& stamp : stamps)
                json_stamps.push_back(stamp_to_json(*stamp));
            json_graph["stamps"] = json_stamps;
        }
    }

    out << setw(2) << json_graph;
}

// -----------------------------------------------------------------------

bool SequenceGraph::try_node(const std::string& line, std::istream&) {
    return try_whole_node(line) || try_short_node(line);
}

bool SequenceGraph::try_connection(const std::string& line, std::istream&) {
    smatch match;
    static const regex rx{ "^\\s*(\\S+)\\s+((<)?([-]+)(>)?)\\s+(\\S+)\\s*(:(.*))?\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    // *** node
    const auto left_node_name = match[1].str();
    const auto rght_node_name = match[6].str();
    if (!ch->id_node.count(left_node_name)) throw GraphError{ ch->line_number, "left node not defined" };
    if (!ch->id_node.count(rght_node_name)) throw GraphError{ ch->line_number, "right node not defined" };

    const auto left_node{ static_pointer_cast<SeqNode>(ch->id_node[left_node_name]) };
    const auto rght_node{ static_pointer_cast<SeqNode>(ch->id_node[rght_node_name]) };

    // *** edge
    const auto left_head_arrow{ match[3].str() };
    const auto rght_head_arrow{ match[5].str() };
    if (
            (!left_head_arrow.empty() && !rght_head_arrow.empty()) ||
            (left_head_arrow.empty() && rght_head_arrow.empty())) {
        throw GraphError{ ch->line_number, "double-sided arrow" };
    }

    const auto edge_type = edge_type_from_arrow_part(match[4].str());
    auto edge_text = str_utils::trim_space(match[8].str());
    edge_text = str_utils::un_quote(edge_text);

    const auto edge{ make_shared<SeqEdge>(ch->next_order_number(), ch->next_edge_id(), edge_text, edge_type) };
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
    stamps.push_back(edge);
    return true;
}

// -----------------------------------------------------------------------

bool SequenceGraph::try_grouping(const std::string& line, std::istream& in) {
    return try_fragment(line, in) || try_ref_over(line, in);
}

bool SequenceGraph::try_fragment(const std::string& line, std::istream& in) {
    smatch match;
    static const regex rx{ "^\\s*(alt|opt|loop)\\s+(.*)$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto frag_id = "frag_" + to_string(frags.size() + 1);
    const auto frag_type = str_to_frag_type(match[1].str()); // regex level check.
    const auto opd_cond = str_utils::trim_space(match[2].str());

    auto frag = make_shared<SeqFrag>(ch->next_order_number(), frag_id, frag_type, ch->current_opd());
    auto opd = make_shared<SeqOpd>(ch->next_order_number(), ch->next_opd_id(), opd_cond);
    frags.push_back(frag);
    stamps.push_back(frag);

    ch->nested_opds.push(opd);
    frag->opds.push_back(opd);
    stamps.push_back(opd);
    opd->frag = frag; // enable_shared_from_this!

    while (in) {
        const auto line{ read_line(in) };
        if (try_operand_end(line)) {
            ch->nested_opds.pop();
            return true;
        }

        if (try_operand_else(line, frag)) {
            opd = frag->opds.back();
            ch->nested_opds.top() = opd;
            stamps.push_back(opd);
            continue;
        }

        if (try_connection(line, in)) {
            auto edge = ch->id_edge[ch->last_edge_id()];
            auto seq_edge = static_pointer_cast<SeqEdge>(edge);
            seq_edge->opd = ch->current_opd();
            continue;
        }

        // TODO: добавить возможность объявления новых узлов
        if (!try_whitespaces(line) && !try_three_dots(line) && !try_grouping(line, in)) {
            throw GraphError(ch->line_number, "unknown line");
        }
    }
    throw GraphError(ch->line_number, "fragment is not closed");
}

bool SequenceGraph::try_ref_over(const std::string& line, std::istream& in) {
    smatch match;
    static const regex rx{ "^\\s*ref\\s+over\\s+(.*)" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto node_names{ str_to_node_names(match[1].str()) };
    for (const auto& one : node_names) {
        if (!ch->id_node.count(one)) {
            throw GraphError(ch->line_number, "unknown node name");
        }
    }

    string text;
    while (!in.eof()) {
        const auto line{ read_line(in) };
        if (try_end_ref(line)) {
            const auto ref_id = "ref_" + to_string(refs.size() + 1);
            auto ref = make_shared<SeqRef>(ch->next_order_number(), ref_id, text, ch->current_opd());
            stamps.push_back(ref);
            refs.push_back(ref);
            return true;
        }
        else {
            text.append(line);
        }
    }
    throw GraphError(ch->line_number, "ref over is not closed");
}
