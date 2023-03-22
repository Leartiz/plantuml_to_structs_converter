#include <sstream>

#include "err_text_creator.h"

namespace lenv
{

std::string Err_text_creator::dt(const std::string& class_name,
                                 const std::string& method_name,
                                 const std::string& what)
{
    std::ostringstream sout;
    sout << class_name << ", "
         << method_name << ", "
         << what << ";";
    return sout.str();
}

}
