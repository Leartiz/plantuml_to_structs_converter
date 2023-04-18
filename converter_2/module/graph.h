#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <memory>
#include <sstream>

struct ConstructHelper;
struct Graph {
    struct Unit {
        std::string id, name;
        virtual ~Unit() = default;
    };

    struct Edge;
    struct Node : Unit {
        std::vector<std::weak_ptr<Edge>> outs, inns;
    };

    struct Edge : Unit {
        std::weak_ptr<Node> beg, end;
    };

public:
    Graph();
    virtual ~Graph() = default;

    // TODO: в один метод - почти одинаковая логика для всех диаграмм (графов)
    virtual void read_puml(std::istream&) = 0;
    virtual void write_json(std::ostream&) = 0;

public:
    /* readonly */
    std::vector<std::shared_ptr<Node>> nodes; // &
    std::vector<std::shared_ptr<Edge>> edges;

protected:
    bool try_whitespaces(std::string&);
    bool try_directive(std::string&);
    bool try_skinparam(std::string&);
    bool try_direction(std::string&);

protected:
    bool try_beg_grouping(std::string&);
    bool try_end_curly_brace(std::string&);

protected:
    bool try_one_note(std::string&);
    bool try_beg_multi_note(std::string&);
    bool try_beg_note_with_id(std::string&);
    bool try_end_multi_note(std::string&);
    bool try_note(std::string&, std::istream&);

protected:
    bool try_one_comment(std::string&);
    bool try_multi_comment(std::string&, std::istream&);

protected:
    virtual bool try_node(std::string&, std::istream&) = 0;
    virtual bool try_connection(std::string&, std::istream&) = 0;
    virtual bool try_grouping(std::string&, std::istream&) = 0;

protected:
    const std::shared_ptr<ConstructHelper> m_ch;
};

#endif // GRAPH_H

