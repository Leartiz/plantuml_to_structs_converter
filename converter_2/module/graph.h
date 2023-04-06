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

    struct Node : Unit {};
    struct Edge : Unit {};

public:
    virtual ~Graph() = default;
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
};

#endif // GRAPH_H

