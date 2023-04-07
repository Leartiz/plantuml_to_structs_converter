#ifndef USECASEGRAPH_H
#define USECASEGRAPH_H

#include <string>

#include "graph.h"

// TODO: блоки ректангле и еще что то
// TODO: изменять напрвления
// TODO: удалить тарый конвертер!

// TODO: изменить имя на Use_case_dia?
struct UseCaseGraph : Graph {

    struct UcEdge;
    struct UcNode : public Node {
        enum Type {
            UseCase, Actor,
        };

        UcNode() = default;
        UcNode(std::string id, std::string name, Type);

        Type type{ Actor };
    };

    struct UcEdge : public Edge {
        enum Type {
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
    bool try_grouping(std::string&);
};

#endif // USECASEGRAPH_H
