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
    virtual void read(istream&) = 0;
    virtual void write(ostream&) = 0;

public:
    vector<shared_ptr<Node>> nodes;
    vector<shared_ptr<Edge>> edges;
};

#endif // GRAPH_H
