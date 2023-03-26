#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

namespace lenv
{

/*
In the default "C" locale, the following uppercase letters ABCDEFGHIJKLMNOPQRSTUVWXYZ
are replaced with respective lowercase letters abcdefghijklmnopqrstuvwxyz.
*/
class String_utils final
{
public:
    static bool is_keyword_letter(const char ch);
    static std::string to_upper(const std::string& str);
    static std::string to_lower(const std::string& str);

public:
    static bool start_with(std::string str, std::string start, bool sensitive = true);
    static bool stop_with(std::string str, std::string stop, bool sensitive = true);

public:
    static bool eq(std::string lhs, std::string rhs, bool sensitive = true);
    static bool ne(std::string lhs, std::string rhs, bool sensitive = true);

public:
    static std::string trim_left(std::string str);
    static std::string trim_rght(std::string str);
    static std::string trim(std::string str);

    static std::string trim_left(std::string str, const std::string& chs);
    static std::string trim_rght(std::string str, const std::string& chs);
    static std::string trim(std::string str, const std::string& chs);
};

}

#endif // STRING_UTILS_H
