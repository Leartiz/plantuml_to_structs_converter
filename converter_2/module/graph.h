#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <memory>
#include <sstream>

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
    virtual bool try_node(std::string&) = 0;
    virtual bool try_connection(std::string&) = 0;
    virtual bool try_grouping(std::string&, std::istream&) = 0;
};

#endif // GRAPH_H

