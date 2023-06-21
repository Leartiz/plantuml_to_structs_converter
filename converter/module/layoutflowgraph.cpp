#include <regex>

#include "layoutflowgraph.h"

#include "constructhelper.h"
#include "grapherror.h"

#include "json_utils.h"
#include "str_utils.h"

using namespace std;
using namespace nlohmann;

using LwNode = LayoutFlowGraph::LwNode;
using LwEdge = LayoutFlowGraph::LwEdge;

// -----------------------------------------------------------------------

LwNode::LwNode(string id, std::string name, Type tp) {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

LwNode::LwNode(const std::string& nmid, Type tp) {
    this->name = nmid;
    this->id = nmid;
    this->type = tp;
}

LwEdge::LwEdge(std::string id, std::string name) {
    this->name = std::move(name);
    this->id = std::move(id);
}

// -----------------------------------------------------------------------

namespace {

shared_ptr<LwNode> create_node_if_need(ConstructHelper::Sp ch, string nmid,
                                       LwNode::Type prit_tp = LwNode::Ordinary) {
    if (prit_tp == LwNode::Starting) {
        static_cast<void>(nmid);
        const auto& sid = LwNode::starting_id; /* priv. case */
        if (!ch->id_node.count(sid))
            ch->id_node[sid] = make_shared<LwNode>(sid, prit_tp);
        return static_pointer_cast<LwNode>(ch->id_node[sid]);
    }
    else if (prit_tp == LwNode::Ending) {
        static_cast<void>(nmid);
        const auto& eid = LwNode::ending_id;
        if (!ch->id_node.count(eid))
            ch->id_node[eid] = make_shared<LwNode>(eid, prit_tp);
        return static_pointer_cast<LwNode>(ch->id_node[eid]);
    }

    // ***

    if (!ch->id_node.count(nmid))
        ch->id_node[nmid] = make_shared<LwNode>(nmid);
    return static_pointer_cast<LwNode>(ch->id_node[nmid]);
}

// ***

bool try_empty_state_short_node(ConstructHelper::Sp ch, const std::string& line) {
    smatch match;
    //                                    #1
    static const regex rx{ R"(^\s*state\s+(\w+)\s*$)" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto node_nmid = match[1].str();
    const auto node = make_shared<LwNode>(match[1].str());
    ch->id_node[node_nmid] = node;
    return true;
}

bool try_empty_state_whole_node(ConstructHelper::Sp ch, const std::string& line) {
    smatch match;
    //                                      #1                #2
    static const regex rx{ R"(^\s*state\s+\"([^<>]+)\"\s+as\s+(\w+)\s*$)" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto node_id{ match[2].str() };
    const auto node_name{ match[1].str() };
    const auto node = make_shared<LwNode>(node_id, node_name);

    ch->id_node[node_id] = node;
    return true;
}

// ***

bool try_image_state_short_node(ConstructHelper::Sp ch, const std::string& line) {
    smatch match;
    //                                                    #1                    #2
    static const regex rx{ R"(^\s*state\s+\"\s*<img\s*:\s*(.+?)\s*>\s*\"\s+as\s+(\w+)\s*$)" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto node_nmid = match[2].str();
    const auto node = make_shared<LwNode>(node_nmid);
    node->img_ref = match[1].str();

    // TODO: можно на этом этапе сделать проверку на наличие файла. Ее также делает PlantUML
    ch->id_node[node_nmid] = node;
    return true;
}

bool try_image_state_whole_node(ConstructHelper::Sp ch, const std::string& line) {
    smatch match;
    //                                         #1                         #2                    #3
    static const regex rx{ R"(^\s*state\s+\"\s*([^<>"]+?)\s*\\n<img\s*:\s*(.+?)\s*>\s*\"\s+as\s+(\w+)\s*$)" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto node_id{ match[3].str() };
    const auto node_name{ match[1].str() };
    const auto node = make_shared<LwNode>(node_id, node_name);

    node->img_ref = match[2].str();

    ch->id_node[node_id] = node;
    return true;
}

// *** json

string node_type_to_str(const LwNode::Type tp) {
    switch (tp) {
    case LwNode::Starting: return "starting";
    case LwNode::Ending:   return "ending";
    case LwNode::Ordinary: return "ordinary";
    }

    throw runtime_error{ "node type unknown" };
}

// ***

json edge_to_json(LwEdge& edge) {
    return json_utils::edge_to_whole_json(edge);
}

json node_to_json(LwNode& node) {
    json result = json_utils::node_to_whole_json(node);
    result["type"] = node_type_to_str(node.type);
    result["img_ref"] = node.img_ref;
    result["desc_lines"] = node.desc_lines;
    return result;
}

// -----------------------------------------------------------------------

bool try_desc_line_to_state(ConstructHelper::Sp ch, const std::string& line) {
    smatch match;
    //                            #1      #2 #3
    static const regex rx{ R"(^\s*(\w+)\s*(:(.*))$)" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto node = create_node_if_need(ch, match[1].str());
    auto desc_line = str_utils::trim_space(match[3].str());
    node->desc_lines.push_back(std::move(desc_line));

    return true;
}

bool try_hide_empty_description(const std::string& line) {
    static const regex rx{ R"(^\s*hide\s+empty\s+description\s*$)" };
    return regex_match(line, rx);
}

} // <anonymous>

// -----------------------------------------------------------------------

void LayoutFlowGraph::read_puml(std::istream& in) {
    Graph::read_puml(in);
}

void LayoutFlowGraph::write_json(std::ostream& out) {
    json json_graph;
    json_graph["id"] = "layout_flow_graph";

    /* edges */
    {
        json::array_t json_edges; json_edges.reserve(edges.size());
        std::transform(begin(edges), end(edges), std::back_inserter(json_edges),
                       [](const std::shared_ptr<Edge>& one) -> json {
            return edge_to_json(*static_pointer_cast<LwEdge>(one));
        });
        json_graph["edges"] = std::move(json_edges);
    }

    /* nodes */
    {
        json::array_t json_nodes; json_nodes.reserve(nodes.size());
        std::transform(begin(nodes), end(nodes), std::back_inserter(json_nodes),
                       [](const std::shared_ptr<Node>& one) -> json {
            return node_to_json(*static_pointer_cast<LwNode>(one));
        });
        json_graph["nodes"] = std::move(json_nodes);
    }

    out << setw(2) << json_graph;
}

// -----------------------------------------------------------------------

bool LayoutFlowGraph::try_any(const std::string& line, std::istream& in) {
    return Graph::try_any(line, in) ||
            try_desc_line_to_state(m_ch, line) ||
            try_hide_empty_description(line);
}

bool LayoutFlowGraph::try_node(const std::string& line, std::istream&) {
    return
            try_image_state_whole_node(m_ch, line) ||
            try_image_state_short_node(m_ch, line) ||
            try_empty_state_whole_node(m_ch, line) ||
            try_empty_state_short_node(m_ch, line);
}

bool LayoutFlowGraph::try_connection(const std::string& line, std::istream&) {
    smatch match;
    //                            #1 | exs: [*], MainWindow or ...
    static const regex rx{ R"(^\s*((\[\*\])|([^\[\*\]\s]+))\s+)"
                           // #4 #5 #6             #7                        #8
                           R"(((<?)([-]+([lrdu]|left|right|up|down)[-]+|[-]+)(>?))\s+)"
                           // #9 #10 #11                  #12 #13
                           R"(((\[\*\])|([^\[\*\]\s]+))\s*(:(.*))?\s*$)" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto left_head_arrow{ match[5].str() };
    const auto rght_head_arrow{ match[8].str() };

    if (!left_head_arrow.empty() && !rght_head_arrow.empty()) {
        throw GraphError(m_ch->line_number, "double-sided arrow");
    }
    if (left_head_arrow.empty() && rght_head_arrow.empty()) {
        throw GraphError(m_ch->line_number, "undirected arrow");
    }

    // ***

    const bool is_left_to_rght{ !rght_head_arrow.empty() };
    shared_ptr<LwNode> left_node{ nullptr }, rght_node{ nullptr };
    const string left_node_nmid = match[1].str();
    const string rght_node_nmid = match[9].str();

    const auto edge_note = str_utils::trim_space(match[13].str());
    const auto edge{ make_shared<LwEdge>(m_ch->next_edge_id(), edge_note) };

    // left --> rght
    if (is_left_to_rght) {
        // left | beg
        {
            const auto prt = left_node_nmid == "[*]" ? LwNode::Starting : LwNode::Ordinary;
            left_node = create_node_if_need(m_ch, left_node_nmid, prt);
        }

        // rght | end
        {
            const auto prt = rght_node_nmid == "[*]" ? LwNode::Ending : LwNode::Ordinary;
            rght_node = create_node_if_need(m_ch, rght_node_nmid, prt);
        }

        // ***

        edge->beg = left_node;
        edge->end = rght_node;

        left_node->outs.push_back(edge);
        rght_node->inns.push_back(edge);
    }
    // left <-- rght
    else {
        // left | end
        {
            const auto prt = left_node_nmid == "[*]" ? LwNode::Ending : LwNode::Ordinary;
            left_node = create_node_if_need(m_ch, left_node_nmid, prt);
        }

        // rght | beg
        {
            const auto prt = rght_node_nmid == "[*]" ? LwNode::Starting : LwNode::Ordinary;
            rght_node = create_node_if_need(m_ch, rght_node_nmid, prt);
        }

        // ***

        edge->beg = rght_node;
        edge->end = left_node;

        left_node->inns.push_back(edge);
        rght_node->outs.push_back(edge);
    }
    m_ch->id_edge[edge->id] = edge; /* prolongs life */
    return true;
}
