#include "ordergenerator.h"

namespace lenv
{

std::string OrderGenerator::next() const
{
    ++m_counter;
    return std::to_string(m_counter);
}

}
