#ifndef NULL_EDGE_H
#define NULL_EDGE_H

#include <string>
#include <memory>

#include "error.h"

namespace lenv
{

class Null_edge : public Error
{
public:
    Null_edge();
};

}

#endif // NULL_EDGE_H
