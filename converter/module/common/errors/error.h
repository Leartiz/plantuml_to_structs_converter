#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <memory>

namespace lenv
{

class Error
{
public:
    enum Type : uint32_t
    {

    };

public:
    explicit Error(const std::string& what);
    virtual ~Error();

private:
    std::string m_what;
};

using Error_sp = std::shared_ptr<Error>;

}

#endif // ERROR_H
