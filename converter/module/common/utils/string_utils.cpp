#include <cctype>
#include <locale>
#include <algorithm> // std::transform, std::find_if

#include "string_utils.h"

namespace lenv
{

bool String_utils::is_keyword_letter(const char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool String_utils::eq(char lhs, char rhs, bool sensitive)
{
    /*
       The behavior of std::tolower is undefined
       if the argument's value is neither
       representable as unsigned char.
    */

    if (!sensitive) {
        lhs = static_cast<char>(std::tolower(static_cast<unsigned char>(lhs)));
        rhs = static_cast<char>(std::tolower(static_cast<unsigned char>(rhs)));
    }
    return lhs == rhs;
}

bool String_utils::ne(char lhs, char rhs, bool sensitive)
{
    return !eq(lhs, rhs, sensitive);
}

void String_utils::to_upper_by_ref(std::string& str)
{
    // TODO: study std::locale!

    std::transform(str.cbegin(), str.cend(), str.begin(),
                   [](unsigned char ch) -> unsigned char {
        return std::toupper(ch);
    });
}

void String_utils::to_lower_by_ref(std::string& str)
{
    std::transform(str.cbegin(), str.cend(), str.begin(),
                   [](unsigned char ch) -> unsigned char {
        return std::tolower(ch);
    });
}

std::string String_utils::to_upper(const std::string& str)
{
    /* can it mess up emojis and characters larger than 1 byte? */

    std::string result{ str };
    to_upper_by_ref(result);
    return result;
}

std::string String_utils::to_lower(const std::string& str)
{
    std::string result{ str };
    to_lower_by_ref(result);
    return result;
}

// -----------------------------------------------------------------------

bool String_utils::start_with(std::string str, std::string start, bool sensitive)
{
    if (!sensitive) {
        to_lower_by_ref(str);
        to_lower_by_ref(str);
    }

    return !str.find(start);
}

bool String_utils::stop_with(std::string str, std::string stop, bool sensitive)
{
    if (!sensitive) {
        to_lower_by_ref(str);
        to_lower_by_ref(stop);
    }

    return str.rfind(stop) == (str.length() - stop.length());
}

// -----------------------------------------------------------------------

bool String_utils::eq(std::string lhs, std::string rhs, bool sensitive)
{
    if (!sensitive) {
        to_lower_by_ref(lhs);
        to_lower_by_ref(rhs);
    }

    return lhs == rhs;
}

bool String_utils::ne(std::string lhs, std::string rhs, bool sensitive)
{
    return !eq(lhs, rhs, sensitive);
}

bool String_utils::eq_ref(const std::string& lhs, const std::string& rhs, bool sensitive)
{
    if (lhs.size() != rhs.size()) return false;
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (!eq(lhs[i], rhs[i], sensitive)) {
            return false;
        }
    }
    return true;
}

bool String_utils::ne_ref(const std::string& lhs, const std::string& rhs, bool sensitive)
{
    return !eq_ref(lhs, rhs, sensitive);
}

// -----------------------------------------------------------------------

void String_utils::trim_left_space_by_ref(std::string& str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(),
                                        [](unsigned char ch) -> bool{
        return !std::isspace(ch);
    }));
}

void String_utils::trim_rght_space_by_ref(std::string& str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(),
                           [&](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
}

void String_utils::trim_space_by_ref(std::string& str)
{
    trim_left_space_by_ref(str);
    trim_rght_space_by_ref(str);
}

std::string String_utils::trim_left_space(std::string str)
{
    trim_left_space_by_ref(str);
    return str;
}

std::string String_utils::trim_rght_space(std::string str)
{
    trim_rght_space_by_ref(str);
    return str;
}

std::string String_utils::trim_space(std::string str)
{
    trim_space_by_ref(str);
    return str;
}

// -----------------------------------------------------------------------

void String_utils::trim_left_by_ref(std::string& str, const std::string& chs)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(),
                                        [&](unsigned char ch) {
        return chs.find(ch) == std::string::npos;
    }));
}

void String_utils::trim_rght_by_ref(std::string& str, const std::string& chs)
{
    str.erase(std::find_if(str.rbegin(), str.rend(),
                           [&](unsigned char ch) {
        return chs.find(ch) == std::string::npos;
    }).base(), str.end());
}

void String_utils::trim_by_ref(std::string& str, const std::string& chs)
{
    trim_left_by_ref(str, chs);
    trim_rght_by_ref(str, chs);
}

std::string String_utils::trim_left(std::string str, const std::string& chs)
{
    trim_left_by_ref(str, chs);
    return str;
}

std::string String_utils::trim_rght(std::string str, const std::string& chs)
{
    trim_rght_by_ref(str, chs);
    return str;
}

std::string String_utils::trim(std::string str, const std::string& chs)
{
    trim_by_ref(str, chs);
    return str;
}

// -----------------------------------------------------------------------

std::string String_utils::un_quote(std::string str)
{
    trim_by_ref(str, "\"");
    return str;
}

std::string String_utils::wrap_quote(std::string str)
{
    return "\"" + str + "\"";
}

}
