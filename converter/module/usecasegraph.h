#ifndef USECASEGRAPH_H
#define USECASEGRAPH_H

#include <string>

#include "graph.h"

struct SequenceGraph;
struct RobustnessGraph;

struct UseCaseGraph final : Graph {

    struct UcNode : Node {
        enum Type : uint32_t {
            Actor, Usecase, // Usecase - тип узла.
        };

        UcNode() = default;
        UcNode(std::string id, std::string name, Type);

        Type type{ Actor };
        std::weak_ptr<RobustnessGraph> rob_graph;
        std::weak_ptr<SequenceGraph> seq_graph;
    };

    struct UcEdge : Edge {
        enum Type : uint32_t {
            Association,
            Generalization,
            Include,
            Extend,
        };

        UcEdge() = default;
        UcEdge(std::string id, std::string name, Type);

        Type type{ Association };
    };

public:
    void read_puml(std::istream&) override;
    void write_json(std::ostream&) override;

protected:
    bool try_node(const std::string&, std::istream&) override;
    bool try_connection(const std::string&, std::istream&) override;
    bool try_grouping(const std::string&, std::istream&) override;
};

#endif // USECASEGRAPH_H
