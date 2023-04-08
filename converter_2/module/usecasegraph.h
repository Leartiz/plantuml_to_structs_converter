#ifndef USECASEGRAPH_H
#define USECASEGRAPH_H

#include <string>

#include "graph.h"

// TODO: блоки ректангле и еще что то
// TODO: изменять напрвления
// TODO: удалить тарый конвертер!

struct SequenceGraph;
struct RobustnessGraph;

// TODO: изменить имя на Use_case_dia?
struct UseCaseGraph : Graph {

    struct UcEdge;
    struct UcNode : public Node {
        enum Type : uint32_t {
            UseCase, Actor,
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
    bool try_actor_node(std::string&);
    bool try_usecase_node(std::string&);
    bool try_connection(std::string&);

protected:
    bool try_grouping(std::string&, std::istream&);
};

#endif // USECASEGRAPH_H
