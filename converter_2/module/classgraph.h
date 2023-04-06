#ifndef CLASSGRAPH_H
#define CLASSGRAPH_H

#include "graph.h"

struct ClassGraph : Graph {
    struct Member {
        enum Mark : unsigned char {
            Public = '+',
            Private = '-',
            Protected = '#',
        };

        static Member from_str(std::string&);

        Mark mark{ Public };
        std::string name;
    };

    struct ClassEdge;
    struct ClassNode : public Node {
        std::vector<std::weak_ptr<ClassEdge>> outs, inns;
        std::vector<Member> datas, funcs;
    };

    struct ClassEdge : public Edge {
        enum Type {
            Dependency,
            Association,
            Aggregation,
            Composition,
            Implementation,
            Generalization,
        };

        Type type{ Association };
        std::weak_ptr<ClassNode> beg, end;
    };

public:
    void read_puml(std::istream&) override;
    void write_json(std::ostream&) override;
};

#endif // CLASSGRAPH_H
