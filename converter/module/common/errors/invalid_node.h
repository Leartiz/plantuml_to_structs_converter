#ifndef INVALID_NODE_H
#define INVALID_NODE_H

#include "error.h"

namespace lenv
{

class Invalid_node : public Error
{
public:
    Invalid_node(const std::string& arg);
};

}

#endif // INVALID_NODE_H
