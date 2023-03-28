#ifndef UNKNOWN_NODE_TYPE_H
#define UNKNOWN_NODE_TYPE_H

#include "bldr_error.h"

namespace lenv
{

class Unknown_node_type : public Bldr_error
{
public:
    Unknown_node_type(const std::string& arg);
};

}

#endif // UNKNOWN_NODE_TYPE_H
