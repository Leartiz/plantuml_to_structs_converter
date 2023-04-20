#ifndef SEQUENCEGRAPH_H
#define SEQUENCEGRAPH_H

#include <string>

#include "graph.h"
#include "usecasegraph.h"

struct SequenceGraph final : Graph {
    struct SeqOpd;
    struct SeqFrag {
        enum Type : uint32_t {
            Ref, Loop, Alt, Opt
        };

        std::string id;
        Type type{ Type::Ref };
        std::vector<std::shared_ptr<SeqOpd>> opds;
    };

    struct SeqOpd {
        std::string id, condition;
        std::weak_ptr<SeqFrag> frag;
    };

    struct SeqNode : public Node {
        enum Type : uint32_t {
            Actor, Boundary, Control, Entity,
        };

        SeqNode() = default;
        SeqNode(std::string id, std::string name, Type);

        bool is_error{ false };
        Type type{ Actor };
    };

    struct SeqEdge : public Edge {
        enum Type : uint32_t {
            Sync, Reply,
        };

        SeqEdge() = default;
        SeqEdge(std::string id, std::string name, Type);

        Type type;
        std::weak_ptr<SeqOpd> opd;
    };

    std::weak_ptr<UseCaseGraph::UcNode> uc_node;
    std::vector<std::shared_ptr<SeqFrag>> frags;

public:
    void read_puml(std::istream&) override;
    void write_json(std::ostream&) override;

protected:
    bool try_any(const std::string&, std::istream&) override;
    bool try_node(const std::string&, std::istream&) override;
    bool try_connection(const std::string&, std::istream&) override;
};

#endif // SEQUENCEGRAPH_H

// TODO: сделать описание через БНФ
// TODO: сделать эксперимент  Json массив объектов!
