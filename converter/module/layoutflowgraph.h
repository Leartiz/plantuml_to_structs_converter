#ifndef LAYOUTFLOWGRAPH_H
#define LAYOUTFLOWGRAPH_H

#include "graph.h"

// TODO: неудачное название. Наверно лучше Screen или...
struct LayoutFlowGraph final : Graph {

    struct LwNode : Node {
        enum Type : uint32_t {
            Starting, Ending,
            Ordinary, // --> state
        };

        // TODO: можно вообще объединить в один узел
        static const std::string starting_id;
        static const std::string ending_id;

        LwNode() = default;
        explicit LwNode(const std::string& nmid, Type = Ordinary);
        LwNode(std::string id, std::string name, Type = Ordinary);

        Type type{ Ordinary };

        // TODO: можно "жестко" проверить что имя = идентификатор. Но лучше гибко, через файл конфигурации
        std::string img_ref; // may differ!
        // not used for analysis?
        std::vector<std::string> desc_lines;
    };

    struct LwEdge : Edge {
        LwEdge() = default;
        LwEdge(std::string id, std::string name);
    };

public:
    void read_puml(std::istream&) override;
    void write_json(std::ostream&) override;

protected:
    bool try_any(const std::string&, std::istream&) override;
    bool try_node(const std::string&, std::istream&) override;
    bool try_connection(const std::string&, std::istream&) override;
};

#endif // LAYOUTFLOWGRAPH_H
