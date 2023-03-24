#ifndef ANY_ERROR_H
#define ANY_ERROR_H

#include <string>
#include <stdexcept> // std::runtime_error

namespace lenv
{

class Any_error : public std::runtime_error
{
public:
    Any_error(const std::string& arg);
};

}

#endif // ANY_ERROR_H
