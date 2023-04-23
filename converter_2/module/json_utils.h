#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <vector>
#include <memory>

#include "graph.h"
#include "nlohmann/json/json.hpp"

namespace json_utils {

nlohmann::json edge_to_json(Graph::Edge& edge);
nlohmann::json node_to_json(Graph::Node& node);
nlohmann::json edges_to_short_json(std::vector<std::weak_ptr<Graph::Edge>> edges);

};

#endif // JSON_UTILS_H
