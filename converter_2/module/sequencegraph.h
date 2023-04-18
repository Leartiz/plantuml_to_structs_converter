#ifndef SEQUENCEGRAPH_H
#define SEQUENCEGRAPH_H

#include <string>

#include "graph.h"

struct SequenceGraph : Graph {
    struct SeqNode : public Node {
        enum Type : uint32_t {
            Actor, Boundary, Control, Entity,
        };

        bool is_error{ false };
        Type type{ Actor };
    };
};

#endif // SEQUENCEGRAPH_H
