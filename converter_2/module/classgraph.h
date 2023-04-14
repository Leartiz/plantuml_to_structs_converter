#ifndef CLASSGRAPH_H
#define CLASSGRAPH_H

#include <vector>

#include "graph.h"

struct ClassGraph : Graph {

    struct ClassNode : public Node {
        // TODO: separate data and funcs?
        struct Member final {
            enum Mark : uint32_t {
                Public = '+', Private = '-', Protected = '#',
            };

            static Member from_str(std::string&);

            Mark mark{ Public };
            std::string name, type; // or return type!
            std::vector<std::string> param_types;
        };

        enum Type : uint32_t {
            Class, Enum, Interface,
        };

        ClassNode() = default;
        ClassNode(std::string id, std::string name, Type);

        Type type{ Class };
        std::vector<Member> datas, funcs;
    };

    struct ClassEdge : public Edge {
        enum Type : uint32_t {
            Dependency, Association,
            Aggregation, Composition,
            Implementation, Generalization,
        };

        ClassEdge() = default;
        ClassEdge(std::string id, std::string name, Type);

        Type type{ Association };
    };

public:
    void read_puml(std::istream&) override;
    void write_json(std::ostream&) override;

protected:
    bool try_node(std::string&) override;
    bool try_connection(std::string&) override;
};

#endif // CLASSGRAPH_H
