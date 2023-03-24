#ifndef REPEATING_NODE_H
#define REPEATING_NODE_H

#include "bldr_error.h"

namespace lenv
{

class Repeating_node : public Bldr_error
{
public:
    Repeating_node(const std::string& arg);
};

}

#endif // REPEATING_NODE_H
