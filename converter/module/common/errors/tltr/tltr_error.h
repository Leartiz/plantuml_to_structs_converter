#ifndef TLTR_ERROR_H
#define TLTR_ERROR_H

#include "errors/any_error.h"

namespace lenv
{

class Tltr_error : public Any_error
{
public:
    Tltr_error(const std::string& arg);
};

}

#endif // TLTR_ERROR_H
