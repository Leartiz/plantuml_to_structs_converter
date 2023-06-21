#include <sstream>
#include <utility>

#include "grapherror.h"

namespace
{

std::string create_complete_msg(const size_t line_num,
                                const std::string& txt) {
    std::ostringstream sout;
    sout << "# " << line_num
         << " | " << txt;
    return sout.str();
}

}

GraphError::GraphError(size_t line_num, const std::string& txt)
    : std::runtime_error{ create_complete_msg(line_num, txt) }
    , m_line_num{ line_num }, m_text{ txt } {}

const std::string& GraphError::only_text() const
{
    // TODO: неэффективное хранение строки
    return m_text;
}

size_t GraphError::only_line_num() const
{
    return m_line_num;
}
