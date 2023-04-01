#include <utility>

#include "arrow.h"

namespace lenv
{

Arrow::Arrow(std::string str)
{

}

Arrow from_str(const std::string& str)
{

}

std::string Arrow::raw() const
{
    return m_lhead + m_body + m_rhead;
}

const std::string& Arrow::left_head() const
{
    return m_lhead;
}

const std::string& Arrow::rght_head() const
{
    return m_rhead;
}

const std::string& Arrow::body() const
{
    return m_body;
}

}
