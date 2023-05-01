#include "json_utils.h"

using namespace std;
using namespace nlohmann;

namespace json_utils {

nlohmann::json node_to_short_json(Graph::Node& node) {
    return { { "id", node.id } };
}

nlohmann::json edge_to_short_json(Graph::Edge& edge) {

}

// -----------------------------------------------------------------------

nlohmann::json edge_to_whole_json(Graph::Edge& edge) {
    if (edge.beg.expired() || edge.end.expired()) {
        throw runtime_error{ "beg or end expired" };
    }

    json result;
    result["id"] = edge.id;
    result["name"] = edge.name;

    result["beg"] = node_to_short_json(*edge.beg.lock());
    result["end"] = { { "id", edge.end.lock()->id } };
    return result;
}

nlohmann::json node_to_whole_json(Graph::Node& node) {
    json result;
    result["id"] = node.id;
    result["name"] = node.name;

    result["inn_edges"] = edges_to_short_json(node.inns);
    result["out_edges"] = edges_to_short_json(node.outs);
    return result;
}

// -----------------------------------------------------------------------

// TODO: вектор слабых ссылок, можно изменить?
json edges_to_short_json(vector<weak_ptr<Graph::Edge>> edges) {
    const string id_key{ "id" };
    auto json_edges = json::array();
    for_each(begin(edges), end(edges), [&](const weak_ptr<Graph::Edge> edge_wp) {
        if (const auto edge_sp = edge_wp.lock(); edge_sp) {
            json_edges.push_back({ { id_key, edge_sp->id } });
            return;
        }
        throw runtime_error{ "inn/out edge expired" };
    });

    sort(begin(json_edges), end(json_edges), [&](const json& lhs, const json& rhs) {
        return lhs[id_key] < rhs[id_key];
    });
    return json_edges;
}

}
