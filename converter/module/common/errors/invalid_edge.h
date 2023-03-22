#ifndef INVALID_EDGE_H
#define INVALID_EDGE_H

#include "error.h"

namespace lenv
{

class Invalid_edge : public Error
{
public:
    Invalid_edge(const std::string& arg);
};

}

#endif // INVALID_EDGE_H
