#ifndef NODE_AFTER_ADDER_H
#define NODE_AFTER_ADDER_H

#include "bldr_error.h"

namespace lenv
{

/* rare */
class Node_after_adder : public Bldr_error
{
public:
    Node_after_adder(const std::string& arg);
};

}

#endif // NODE_AFTER_ADDER_H
