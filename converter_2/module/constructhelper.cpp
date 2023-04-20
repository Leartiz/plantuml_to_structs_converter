#include "constructhelper.h"

using namespace std;

void ConstructHelper::reset() {
    id_node.clear();
    id_edge.clear();
    line_number = 0;
    opd_number = 0;
}

string ConstructHelper::next_edge_id() {
    return to_string(id_edge.size() + 1);
}

string ConstructHelper::last_edge_id() {
    return to_string(id_edge.size());
}

string ConstructHelper::next_opd_id() {
    return to_string((opd_number++) + 1);
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
