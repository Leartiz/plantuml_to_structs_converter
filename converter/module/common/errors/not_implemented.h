#ifndef NOT_IMPLEMENTED_H
#define NOT_IMPLEMENTED_H

#include "any_error.h"

namespace lenv
{

class Not_implemented : public Any_error
{
public:
    Not_implemented(const std::string& arg);
};

}

#endif // NOT_IMPLEMENTED_H
