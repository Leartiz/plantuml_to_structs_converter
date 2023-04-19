#ifndef CLASSGRAPH_H
#define CLASSGRAPH_H

#include <vector>

#include "graph.h"

struct ClassGraph final : Graph {

    struct ClassNode : public Node {
        // TODO: separate data and funcs?
        struct Member final {
            enum Mark : uint32_t {
                Public = '+', Private = '-', Protected = '#',
            };

            Member(Mark, std::string name, std::string type);

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
        std::vector<std::string> enum_values;
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
    bool try_node(std::string&, std::istream&) override;
    bool try_connection(std::string&, std::istream&) override;

protected:
    void try_interface_body(const std::string&, std::istream&);
    void try_class_body(const std::string&, std::istream&);
    void try_enum_body(const std::string&, std::istream&);

private:
    // TODO: или вообще убрать в анонимный namespace?
    bool try_interface_member_func(std::shared_ptr<ClassNode>, const std::string&);
    bool try_class_member(std::shared_ptr<ClassNode>, const std::string&);
    bool try_enum_value(std::shared_ptr<ClassNode>, const std::string&);

protected:
    bool try_hide_empty_members(const std::string&);
};

#endif // CLASSGRAPH_H
