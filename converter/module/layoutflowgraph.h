#ifndef LAYOUTFLOWGRAPH_H
#define LAYOUTFLOWGRAPH_H

#include "graph.h"

struct LayoutFlowGraph final : Graph {

    struct LwNode : Node {
        enum Type : uint32_t {
            Initial, Ordinary, Final
        };

        LwNode() = default;
        LwNode(std::string id, std::string name, Type);

        Type type{ Ordinary };
        bool image_source;
        std::vector<std::string> desc;
    };

    struct LwEdge : Edge {
        LwEdge() = default;
        LwEdge(std::string id, std::string name);
    };

public:
    void read_puml(std::istream&) override;
    void write_json(std::ostream&) override;

protected:
    bool try_node(const std::string&, std::istream&) override;
    bool try_connection(const std::string&, std::istream&) override;
    bool try_grouping(const std::string&, std::istream&) override;
};

#endif // LAYOUTFLOWGRAPH_H
