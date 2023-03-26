#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

namespace lenv
{

class String_utils final
{
public:
    static bool is_keyword_letter(const char ch);
    static std::string to_lower(const std::string& str);
    static std::string to_upper(const std::string& str);

public:
    static bool start_with(std::string str, std::string start, bool sensitive = true);
    static bool stop_with(std::string str, std::string stop, bool sensitive = true);
};

}

#endif // STRING_UTILS_H
