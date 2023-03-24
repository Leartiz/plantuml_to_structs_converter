#ifndef INVALID_NODE_H
#define INVALID_NODE_H

#include "bldr_error.h"

namespace lenv
{

class Invalid_node : public Bldr_error
{
public:
    Invalid_node(const std::string& arg);
};

}

#endif // INVALID_NODE_H
