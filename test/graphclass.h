#ifndef GRAPHCLASS_H
#define GRAPHCLASS_H

#include "graph.h"

struct GraphClass : Graph {
    struct Member {
        enum Mark : unsigned char {
            Public = '+',
            Private = '-',
            Protected = '#',
        } mark;
        string name;

        static Member from_str(string&);
    };

    struct EdgeClass;
    struct NodeClass : public Node {
        vector<weak_ptr<EdgeClass>> outs, inns;
        vector<Member> datas, funcs;
    };

    struct EdgeClass : public Edge {
        enum Type {
            Dependency,
            Association,
            Aggregation,
            Composition,
            Implementation,
            Generalization,
        } type;

        weak_ptr<NodeClass> beg, end;
    };

    void read_puml(istream&) override;
    void write_json(ostream&) override;

private:
    bool is_allowed_line(string&);
};

#endif // GRAPHCLASS_H
