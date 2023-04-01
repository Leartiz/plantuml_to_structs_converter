#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

namespace lenv
{

/*
    ABCDEFGHIJKLMNOPQRSTUVWXYZ ->
    -> abcdefghijklmnopqrstuvwxyz

    Can be changed for references.
*/
class str_utils final
{
public:
    static bool is_eng_letter(const char ch);
    static bool is_brs_letter(const char ch);

public:
    static bool eq(char lhs, char rhs, bool sensitive = true);
    static bool ne(char lhs, char rhs, bool sensitive = true);

    static void to_upper_by_ref(std::string& str);
    static void to_lower_by_ref(std::string& str);

    static std::string to_upper(const std::string& str);
    static std::string to_lower(const std::string& str);

public:
    static bool start_with(std::string str, std::string start, bool sensitive = true);
    static bool stop_with(std::string str, std::string stop, bool sensitive = true);

public:
    static bool eq(std::string lhs, std::string rhs, bool sensitive = true);
    static bool ne(std::string lhs, std::string rhs, bool sensitive = true);
    static bool eq_ref(const std::string& lhs, const std::string& rhs, bool sensitive = true);
    static bool ne_ref(const std::string& lhs, const std::string& rhs, bool sensitive = true);

public:
    static void trim_left_space_by_ref(std::string& str);
    static void trim_rght_space_by_ref(std::string& str);
    static void trim_space_by_ref(std::string& str);

    static void trim_left_space_by_ref(std::string& str, std::string& wsps);
    static void trim_rght_space_by_ref(std::string& str,  std::string& wsps);

    static std::string trim_left_space(std::string str);
    static std::string trim_rght_space(std::string str);
    static std::string trim_space(std::string str);

public:
    static void trim_left_by_ref(std::string& str, const std::string& chs);
    static void trim_rght_by_ref(std::string& str, const std::string& chs);
    static void trim_by_ref(std::string& str, const std::string& chs);

    static std::string trim_left(std::string str, const std::string& chs);
    static std::string trim_rght(std::string str, const std::string& chs);
    static std::string trim(std::string str, const std::string& chs);

public:
    static std::string un_quote(std::string str);
    static std::string wrap_quote(std::string str);
};

}

#endif // STRING_UTILS_H

