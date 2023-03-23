#ifndef REPEATING_NODE_H
#define REPEATING_NODE_H

#include "error.h"

namespace lenv
{

class Repeating_node : public Error
{
public:
    Repeating_node(const std::string& arg);
};

}

#endif // REPEATING_NODE_H
