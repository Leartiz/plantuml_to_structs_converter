#include "order_generator.h"

namespace lenv
{

std::string Order_generator::next() const
{
    ++m_counter;
    return std::to_string(m_counter);
}

}
