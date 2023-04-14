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

json member_data_to_json(Member data) {

}

json member_func_to_json(Member data) {

}

}

// -----------------------------------------------------------------------

void ClassGraph::read_puml(std::istream& in) {
    ch = m_ch.get();
    Graph::read_puml(in);
    ch = nullptr;
}

void ClassGraph::write_json(std::ostream&) {

}

bool ClassGraph::try_node(std::string&) {

}

bool ClassGraph::try_connection(std::string&) {

}
