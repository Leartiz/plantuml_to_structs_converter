#include "error.h"

namespace lenv
{

Error::Error(const std::string& arg)
    : std::runtime_error{ arg } {}

}
