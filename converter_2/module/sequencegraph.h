#ifndef SEQUENCEGRAPH_H
#define SEQUENCEGRAPH_H

#include <string>

#include "graph.h"
#include "usecasegraph.h"

struct SequenceGraph final : Graph {

    struct SeqEdge;
    struct SeqNode;
    struct SeqOpd;

    /* on the timeline */
    struct Stamp {
        uint32_t order_number{ 0 };
        virtual ~Stamp() = default;
    };

public:
    struct SeqGroup : Stamp {
        SeqGroup(uint32_t onum, std::string id,
                 std::shared_ptr<SeqOpd> = {});

        std::string id;
        std::weak_ptr<SeqOpd> root_opd;
        virtual ~SeqGroup() = default;
    };

    struct SeqRef : SeqGroup {
        SeqRef(uint32_t onum, std::string id, std::string text,
               std::shared_ptr<SeqOpd> = {});

        std::string text;
        std::vector<std::weak_ptr<SeqNode>> nodes;
    };

    struct SeqFrag : SeqGroup {
        enum Type : uint32_t {
            Opt, Alt, Loop
        };

        SeqFrag(uint32_t onum, std::string id, Type,
                std::shared_ptr<SeqOpd> = {});

        Type type{ Type::Opt };
        std::vector<std::shared_ptr<SeqOpd>> opds;
    };

    struct SeqOpd : Stamp {
        SeqOpd(uint32_t onum, std::string id, std::string condition);

        std::string id, condition;
        std::weak_ptr<SeqFrag> frag;
    };

public:
    struct SeqNode : Node {
        enum Type : uint32_t {
            Actor, Boundary, Control, Entity,
        };

        SeqNode() = default;
        SeqNode(std::string id, std::string name, Type);

        bool is_error{ false };
        Type type{ Actor };
    };

    struct SeqEdge : Edge, Stamp {
        enum Type : uint32_t {
            Sync, Reply,
        };

        SeqEdge() = default;
        SeqEdge(uint32_t onum, std::string id, std::string name, Type);

        Type type{ Sync };
        std::weak_ptr<SeqOpd> opd;
    };

public:
    std::weak_ptr<UseCaseGraph::UcNode> uc_node;
    std::vector<std::shared_ptr<SeqFrag>> frags;
    std::vector<std::shared_ptr<SeqRef>> refs;
    std::vector<std::shared_ptr<Stamp>> stamps;

public:
    void read_puml(std::istream&) override;
    void write_json(std::ostream&) override;

protected:
    bool try_node(const std::string&, std::istream&) override;
    bool try_connection(const std::string&, std::istream&) override;
    bool try_grouping(const std::string&, std::istream&) override;

private:
    bool try_fragment(const std::string&, std::istream&);
    bool try_ref_over(const std::string&, std::istream&);
};

#endif // SEQUENCEGRAPH_H

