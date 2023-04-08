#ifndef USECASEGRAPH_H
#define USECASEGRAPH_H

#include <string>

#include "graph.h"

// TODO: удалить старый преобразователь (converter)

struct SequenceGraph;
struct RobustnessGraph;

// TODO: изменить имя на Use_case_dia?
struct UseCaseGraph : Graph {

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
    bool try_actor_node(std::string&);
    bool try_usecase_node(std::string&);

    bool try_node(std::string&) override;
    bool try_connection(std::string&) override;

protected:
    bool try_grouping(std::string&, std::istream&) override;
    // TODO: добавить метод для заметок в документе
};

#endif // USECASEGRAPH_H
