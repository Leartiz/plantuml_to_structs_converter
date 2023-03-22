#ifndef NULL_EDGE_H
#define NULL_EDGE_H

#include "error.h"

namespace lenv
{

class Null_edge : public Error
{
public:
    Null_edge(const std::string& arg);
};

}

#endif // NULL_EDGE_H
