#ifndef ROBUSTNESSGRAPH_H
#define ROBUSTNESSGRAPH_H

#include <string>

#include "graph.h"
#include "usecasegraph.h"

struct RobustnessGraph final : Graph {

    struct RobNode : Node {
        enum Type : uint32_t {
            Actor, Boundary, Control, Entity,
        };

        RobNode() = default;
        RobNode(std::string id, std::string name, Type);

        bool is_error{ false };
        Type type{ Actor };
    };

    struct RobEdge : Edge {
        RobEdge() = default;
        RobEdge(std::string id, std::string name);
    };

    // TODO: сделать поле - отдельный строковый идентификатор?
    std::weak_ptr<UseCaseGraph::UcNode> uc_node;

public:
    void read_puml(std::istream&) override;
    void write_json(std::ostream&) override;

protected:
    bool try_node(const std::string&, std::istream&) override;
    bool try_connection(const std::string&, std::istream&) override;
};

#endif // ROBUSTNESSGRAPH_H
