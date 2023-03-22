#ifndef UNSUITABLE_EDGE_H
#define UNSUITABLE_EDGE_H

#include "error.h"

namespace lenv
{

class Unsuitable_edge : public Error
{
public:
    Unsuitable_edge(const std::string& arg);
};

}

#endif // UNSUITABLE_EDGE_H
