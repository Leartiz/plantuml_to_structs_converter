#include "invalid_node.h"

namespace lenv
{

Invalid_node::Invalid_node(const std::string& arg)
    : Bldr_error{ arg } {}

}
