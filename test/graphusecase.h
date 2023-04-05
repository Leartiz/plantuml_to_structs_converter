#ifndef GRAPHUSECASE_H
#define GRAPHUSECASE_H

#include <string>

#include "graph.h"

struct GraphUseCase : Graph {

    struct EdgeUseCase;
    struct NodeUseCase : public Node {
        NodeUseCase() = default;
        NodeUseCase(string id, string name);

        enum Type {
            UseCase, Actor,
        } type;

        vector<weak_ptr<EdgeUseCase>> outs, inns;
    };

    struct EdgeUseCase : public Edge {
        enum Type {
            Association,
            Generalization,
            Include,
            Extend,
        } type;

        weak_ptr<NodeUseCase> beg, end;
    };

    void read_puml(istream&) override;
    void write_json(ostream&) override;

private:
    bool try_actor_node(string&);
    bool try_usecase_node(string&);
    bool try_connection(string&);
};

#endif // GRAPHUSECASE_H
