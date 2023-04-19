#ifndef SEQUENCEGRAPH_H
#define SEQUENCEGRAPH_H

#include <string>

#include "graph.h"

struct SequenceGraph final : Graph {
    struct SeqOpd;
    struct SeqFrag {
        std::string name;
        std::vector<std::shared_ptr<SeqOpd>> opds;
    };

    struct SeqOpd {
        std::string condition;
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
        SeqEdge(std::string id, std::string name);

        Type type;
        std::weak_ptr<SeqOpd> opd;
    };

public:
    void read_puml(std::istream&) override;
    void write_json(std::ostream&) override;

protected:
    bool try_node(std::string&, std::istream&) override;
    bool try_connection(std::string&, std::istream&) override;

private:
    bool try_whole_node(std::string&);
    bool try_short_node(std::string&);
};

#endif // SEQUENCEGRAPH_H

// TODO: сделать описание через БНФ
