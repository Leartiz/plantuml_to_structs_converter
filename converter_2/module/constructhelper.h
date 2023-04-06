#ifndef CONSTRUCTHELPER_H
#define CONSTRUCTHELPER_H

#include <map>
#include <string>
#include <memory>

#include "graph.h"

struct ConstructHelper final
{
    void reset();
    std::string next_edge_id() const;

    std::vector<std::shared_ptr<Graph::Node>> to_nodes() const;
    std::vector<std::shared_ptr<Graph::Edge>> to_edges() const;

public:
    std::map<std::string, std::shared_ptr<Graph::Node>> id_node;
    std::map<std::string, std::shared_ptr<Graph::Edge>> id_edge;

    size_t line_number{ 0 };
};

#endif // CONSTRUCTHELPER_H
