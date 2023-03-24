#ifndef REPEATING_EDGE_H
#define REPEATING_EDGE_H

#include "bldr_error.h"

namespace lenv
{

class Repeating_edge : public Bldr_error
{
public:
    Repeating_edge(const std::string& arg);
};

}

#endif // REPEATING_EDGE_H
