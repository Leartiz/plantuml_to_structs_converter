#include "robustnessgraph.h"

#include "constructhelper.h"
#include "grapherror.h"

#include "nlohmann/json.hpp"

using namespace std;
using namespace nlohmann;

RobustnessGraph::RobNode::RobNode(string id, string name, Type tp) {
    this->name = std::move(name);
    this->id = std::move(id);
    this->type = tp;
}

RobustnessGraph::RobEdge::RobEdge(std::string id, std::string name) {
    this->name = std::move(name);
    this->id = std::move(id);
}

// -----------------------------------------------------------------------

namespace {

const auto ch{ make_shared<ConstructHelper>() };

}

// -----------------------------------------------------------------------

void RobustnessGraph::read_puml(std::istream& in) {
    ch->reset();
    while (in) {
        string line;
        getline(in, line);
        ch->line_number++;

        if (
                !try_node(line) &&
                !try_connection(line) &&
                !try_whitespaces(line) &&
                !try_grouping(line, in) &&

                !try_directive(line) &&
                !try_skinparam(line) &&
                !try_direction(line)) {
            throw GraphError(ch->line_number, "unknown line");
        }
    }

    nodes = ch->to_nodes();
    edges = ch->to_edges();
}

void RobustnessGraph::write_json(std::ostream&) {

}

bool RobustnessGraph::try_node(std::string&) {

}

bool RobustnessGraph::try_connection(std::string&) {

}

bool RobustnessGraph::try_grouping(std::string&, std::istream&) {
    return false;
}
