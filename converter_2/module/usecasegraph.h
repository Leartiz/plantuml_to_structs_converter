#ifndef USECASEGRAPH_H
#define USECASEGRAPH_H

#include <string>

#include "graph.h"

struct SequenceGraph;
struct RobustnessGraph;

// TODO: изменить имя на Use_case_dia?
struct UseCaseGraph final : Graph {

    struct UcNode : public Node {
        enum Type : uint32_t {
            Actor, Usecase, // Usecase - тип узла.
        };

        UcNode() = default;
        UcNode(std::string id, std::string name, Type);

        Type type{ Actor };
        std::shared_ptr<SequenceGraph> seq_graph;
        std::shared_ptr<RobustnessGraph> rob_graph;
    };

    struct UcEdge : public Edge {
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
