#include "constructhelper.h"

using namespace std;

void ConstructHelper::reset() {
    nested_opds = {};
    id_node.clear();
    id_edge.clear();

    // ***

    line_number = 0;
    operand_number = 0;
    order_number = 0;
}

// -----------------------------------------------------------------------

string ConstructHelper::next_edge_id() const {
    return "edge_" + to_string(id_edge.size() + 1);
}

string ConstructHelper::last_edge_id() const {
    return "edge_" + to_string(id_edge.size() + 0);
}

string ConstructHelper::next_opd_id() {
    return "opd_" + to_string(operand_number++ + 1);
}

size_t ConstructHelper::next_order_number() {
    return order_number++ + 1;
}

size_t ConstructHelper::next_onum() {
    return next_order_number();
}

// -----------------------------------------------------------------------

std::shared_ptr<SequenceGraph::SeqOpd> ConstructHelper::current_opd() const {
    if (nested_opds.empty())
        return nullptr;
    return nested_opds.top();
}

vector<shared_ptr<Graph::Node>> ConstructHelper::to_nodes() const {
    vector<shared_ptr<Graph::Node>> res;
    for (const auto& one : id_node)
        res.push_back(one.second);
    return res;
}

vector<shared_ptr<Graph::Edge>> ConstructHelper::to_edges() const {
    vector<shared_ptr<Graph::Edge>> res;
    for (const auto& one : id_edge)
        res.push_back(one.second);
    return res;
}
