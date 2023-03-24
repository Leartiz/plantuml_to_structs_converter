#include "any_error.h"

namespace lenv
{

Any_error::Any_error(const std::string& arg)
    : std::runtime_error{ arg } {}

}
