#ifndef NULL_EDGE_H
#define NULL_EDGE_H

#include "bldr_error.h"

namespace lenv
{

class Null_edge : public Bldr_error
{
public:
    Null_edge(const std::string& arg);
};

}

#endif // NULL_EDGE_H
