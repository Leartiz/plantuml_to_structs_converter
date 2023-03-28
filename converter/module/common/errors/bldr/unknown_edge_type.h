#ifndef UNKNOWN_EDGE_TYPE_H
#define UNKNOWN_EDGE_TYPE_H

#include "bldr_error.h"

namespace lenv
{

class Unknown_edge_type : public Bldr_error
{
public:
    Unknown_edge_type(const std::string& arg);
};

}

#endif // UNKNOWN_EDGE_TYPE_H
