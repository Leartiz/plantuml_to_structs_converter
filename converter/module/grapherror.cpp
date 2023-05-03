#include <sstream>
#include <utility>

#include "grapherror.h"

GraphError::GraphError(size_t line_num, const std::string& txt)
    : std::runtime_error{ txt }, m_line_num{ line_num } {}

std::string GraphError::complete_message() const {
    std::ostringstream sout;
    sout << "# " << m_line_num
         << " | " << what();
    return sout.str();
}
