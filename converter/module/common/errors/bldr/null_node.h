#ifndef NULL_NODE_H
#define NULL_NODE_H

#include "bldr_error.h"

namespace lenv
{

class Null_node : public Bldr_error
{
public:
    Null_node(const std::string& arg);
};

}

#endif // NULL_NODE_H
