#ifndef CONSTRUCTHELPER_H
#define CONSTRUCTHELPER_H

#include <map>
#include <stack>
#include <string>
#include <memory>

#include "graph.h"
#include "sequencegraph.h"

struct ConstructHelper final
{
    void reset();

public:
    std::string next_edge_id() const;
    std::string last_edge_id() const;

    std::string next_opd_id();
    size_t next_order_number(); // to stamp!
    size_t next_onum();

public:
    std::shared_ptr<SequenceGraph::SeqOpd> current_opd() const;
    std::vector<std::shared_ptr<Graph::Node>> to_nodes() const;
    std::vector<std::shared_ptr<Graph::Edge>> to_edges() const;

public:
    std::stack<std::shared_ptr<SequenceGraph::SeqOpd>> nested_opds;
    std::map<std::string, std::shared_ptr<Graph::Node>> id_node;
    std::map<std::string, std::shared_ptr<Graph::Edge>> id_edge;

public:
    size_t line_number{ 0 };
    size_t operand_number{ 0 };
    size_t order_number{ 0 };
};

#endif // CONSTRUCTHELPER_H
