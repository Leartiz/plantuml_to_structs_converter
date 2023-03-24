#ifndef INVALID_EDGE_H
#define INVALID_EDGE_H

#include "bldr_error.h"

namespace lenv
{

class Invalid_edge : public Bldr_error
{
public:
    Invalid_edge(const std::string& arg);
};

}

#endif // INVALID_EDGE_H
