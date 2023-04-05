#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <memory>
#include <sstream>

using namespace std;

struct Graph {    
    struct Unit {
        string id, name;
        virtual ~Unit() {}
    };

    struct Node : Unit {};
    struct Edge : Unit {};

public:
    virtual ~Graph() = default;
    virtual void read_puml(istream&) = 0;
    virtual void write_json(ostream&) = 0;

public:
    vector<shared_ptr<Node>> nodes;
    vector<shared_ptr<Edge>> edges;

protected:
    bool try_directive(string&);
    bool try_whitespaces(string&);
};

#endif // GRAPH_H
