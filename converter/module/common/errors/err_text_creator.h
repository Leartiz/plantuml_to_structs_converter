#ifndef ERR_TEXT_CREATOR_H
#define ERR_TEXT_CREATOR_H

#include <string>

namespace lenv
{

class Err_text_creator final
{
public:
    static std::string dt(const std::string& class_name,
                          const std::string& method_name,
                          const std::string& what);
};

}

#endif // ERR_TEXT_CREATOR_H
