#ifndef NOT_FOUND_NODE_H
#define NOT_FOUND_NODE_H

#include "bldr_error.h"

namespace lenv
{

class Not_found_node : public Bldr_error
{
public:
    Not_found_node(const std::string& arg);
};

}

#endif // NOT_FOUND_NODE_H
