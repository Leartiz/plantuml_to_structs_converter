#ifndef REPEATING_EDGE_H
#define REPEATING_EDGE_H

#include "error.h"

namespace lenv
{

class Repeating_edge : public Error
{
public:
    Repeating_edge(const std::string& arg);
};

}

#endif // REPEATING_EDGE_H
