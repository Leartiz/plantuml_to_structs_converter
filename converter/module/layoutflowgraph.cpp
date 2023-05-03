#include <regex>

#include "layoutflowgraph.h"

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

LwEdge::LwEdge(std::string id, std::string name) {
    this->name = std::move(name);
    this->id = std::move(id);
}

// -----------------------------------------------------------------------

namespace {

ConstructHelper* ch{ nullptr };

bool try_image_state_short_node(const std::string& line) {
    smatch match;
    static const regex rx{ "^\\s*state\\s+\\\"\\s*(.+?)\\s*\\n<img\\s*:\\s*(.+?)\\s*>\\s*\\\"\\s+as\\s+(\\w+)\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }
}

bool try_image_state_whole_node(const std::string& line) {

}

// *** json

}

// -----------------------------------------------------------------------

void LayoutFlowGraph::read_puml(std::istream& in) {
    ch = m_ch.get();
    Graph::read_puml(in);
    ch = nullptr;
}

void LayoutFlowGraph::write_json(std::ostream&) {
    return;
}

// -----------------------------------------------------------------------

bool LayoutFlowGraph::try_node(const std::string& line, std::istream&) {
    return try_image_state_short_node(line) || try_image_state_whole_node(line);
}

bool LayoutFlowGraph::try_connection(const std::string&, std::istream&) {

}

bool LayoutFlowGraph::try_grouping(const std::string&, std::istream&) {

}
