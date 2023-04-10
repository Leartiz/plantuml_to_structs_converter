#ifndef CLASSGRAPH_H
#define CLASSGRAPH_H

#include <vector>

#include "graph.h"

struct ClassGraph : Graph {
    struct Member {
        enum Mark : uint32_t {
            Public = '+', Private = '-', Protected = '#',
        };

        static Member from_str(std::string&);

        Mark mark{ Public };
        std::string name, type;
        std::vector<std::string> args;

    };

    struct ClassEdge;
    struct ClassNode : public Node {
        enum Type : uint32_t {
            Class, Enum, Interface,
        };

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
    };

public:
    void read_puml(std::istream&) override;
    void write_json(std::ostream&) override;
};

#endif // CLASSGRAPH_H
