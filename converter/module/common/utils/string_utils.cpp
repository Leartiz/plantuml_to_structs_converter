#include <cctype>
#include <algorithm> // std::transform

#include "string_utils.h"

namespace lenv
{

bool String_utils::is_keyword_letter(const char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

std::string String_utils::to_upper(const std::string& str)
{
    /* can it mess up emojis and characters larger than 1 byte? */
    std::string result; result.reserve(str.length());
    std::transform(str.begin(), str.end(), std::back_inserter(result),
                   [](unsigned char ch) { return std::toupper(ch); });
    return result;
}

std::string String_utils::to_lower(const std::string& str)
{
    std::string result; result.reserve(str.length());
    std::transform(str.begin(), str.end(), std::back_inserter(result),
                   [](unsigned char ch) { return std::tolower(ch); });
    return result;
}

// -----------------------------------------------------------------------

bool String_utils::start_with(std::string str, std::string start, bool sensitive)
{
    if (!sensitive) {
        str = to_lower(str);
        start = to_lower(str);
    }

    return !str.find(start);
}

bool String_utils::stop_with(std::string str, std::string stop, bool sensitive)
{
    if (!sensitive) {
        str = to_lower(str);
        stop= to_lower(stop);
    }

    return str.rfind(stop) == (str.length() - stop.length());
}

// -----------------------------------------------------------------------

bool String_utils::eq(std::string lhs, std::string rhs, bool sensitive)
{
    if (!sensitive) {
        lhs = to_lower(lhs);
        rhs = to_lower(rhs);
    }

    return lhs == rhs;
}

bool String_utils::ne(std::string lhs, std::string rhs, bool sensitive)
{
    return !eq(lhs, rhs, sensitive);
}

}
