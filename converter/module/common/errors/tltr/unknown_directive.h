#ifndef UNKNOWN_DIRECTIVE_H
#define UNKNOWN_DIRECTIVE_H

#include "errors/tltr/tltr_error.h"

namespace lenv
{

class Unknown_directive : public Tltr_error
{
public:
    Unknown_directive(const std::string& arg);
};

}

#endif // UNKNOWN_DIRECTIVE_H
