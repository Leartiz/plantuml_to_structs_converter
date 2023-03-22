#ifndef NULL_NODE_H
#define NULL_NODE_H

#include "error.h"

namespace lenv
{

class Null_node : public Error
{
public:
    Null_node(const std::string& arg);
};

}

#endif // NULL_NODE_H
