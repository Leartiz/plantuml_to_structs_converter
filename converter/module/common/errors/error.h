#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <stdexcept> // std::runtime_error

namespace lenv
{

class Error : public std::runtime_error
{
    // exception interface
public:
    Error(const std::string& arg);
};

}

#endif // ERROR_H
