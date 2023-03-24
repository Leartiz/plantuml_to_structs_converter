#ifndef BLDR_ERROR_H
#define BLDR_ERROR_H

#include "errors/any_error.h"

namespace lenv
{

class Bldr_error : public Any_error
{
public:
    Bldr_error(const std::string& arg);
};

}

#endif // BLDR_ERROR_H
