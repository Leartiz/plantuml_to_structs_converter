#ifndef UNSUITABLE_EDGE_H
#define UNSUITABLE_EDGE_H

#include "bldr_error.h"

namespace lenv
{

class Unsuitable_edge : public Bldr_error
{
public:
    Unsuitable_edge(const std::string& arg);
};

}

#endif // UNSUITABLE_EDGE_H
