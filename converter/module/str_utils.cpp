#include <stack>
#include <cctype>
#include <locale>
#include <algorithm> // std::transform, std::find_if
#include <stdexcept>

#include "str_utils.h"

namespace str_utils
{

bool eq(char lhs, char rhs, bool sensitive)
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

bool ne(char lhs, char rhs, bool sensitive)
{
    return !eq(lhs, rhs, sensitive);
}

void to_upper_by_ref(std::string& str)
{
    // TODO: разобраться с std::locale!

    std::transform(str.cbegin(), str.cend(), str.begin(),
                   [](unsigned char ch) -> unsigned char {
        return std::toupper(ch);
    });
}

void to_lower_by_ref(std::string& str)
{
    std::transform(str.cbegin(), str.cend(), str.begin(),
                   [](unsigned char ch) -> unsigned char {
        return std::tolower(ch);
    });
}

std::string to_upper(const std::string& str)
{
    std::string result{ str };
    to_upper_by_ref(result);
    return result;
}

std::string to_lower(const std::string& str)
{
    std::string result{ str };
    to_lower_by_ref(result);
    return result;
}

// -----------------------------------------------------------------------

bool start_with(std::string str, std::string start, bool sensitive)
{
    if (!sensitive) {
        to_lower_by_ref(str);
        to_lower_by_ref(str);
    }

    return !str.find(start);
}

bool stop_with(std::string str, std::string stop, bool sensitive)
{
    if (!sensitive) {
        to_lower_by_ref(str);
        to_lower_by_ref(stop);
    }

    return str.rfind(stop) == (str.length() - stop.length());
}

// -----------------------------------------------------------------------

bool eq(std::string lhs, std::string rhs, bool sensitive)
{
    if (!sensitive) {
        to_lower_by_ref(lhs);
        to_lower_by_ref(rhs);
    }

    return lhs == rhs;
}

bool ne(std::string lhs, std::string rhs, bool sensitive)
{
    return !eq(lhs, rhs, sensitive);
}

bool eq_ref(const std::string& lhs, const std::string& rhs, bool sensitive)
{
    if (lhs.size() != rhs.size()) return false;
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (!eq(lhs[i], rhs[i], sensitive)) {
            return false;
        }
    }
    return true;
}

bool ne_ref(const std::string& lhs, const std::string& rhs, bool sensitive)
{
    return !eq_ref(lhs, rhs, sensitive);
}

// -----------------------------------------------------------------------

void trim_left_space_by_ref(std::string& str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(),
                                        [](unsigned char ch) -> bool {
        return !std::isspace(ch);
    }));
}

void trim_rght_space_by_ref(std::string& str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(),
                           [&](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
}

void trim_space_by_ref(std::string& str)
{
    trim_left_space_by_ref(str);
    trim_rght_space_by_ref(str);
}

void trim_left_space_by_ref(std::string& str, std::string& wsps)
{
    wsps.clear();
    auto nosp_it = std::find_if(str.begin(), str.end(),
                                [](unsigned char ch) -> bool {
        return !std::isspace(ch);
    });

    std::copy(str.begin(), nosp_it, std::back_inserter(wsps));
    str.erase(str.begin(), nosp_it);
}

void trim_rght_space_by_ref(std::string& str,  std::string& wsps)
{
    wsps.clear();
    auto nosp_it = std::find_if(str.rbegin(), str.rend(),
                                [&](unsigned char ch) {
        return !std::isspace(ch);
    }).base();

    std::copy(nosp_it, str.end(), std::back_inserter(wsps));
    str.erase(nosp_it, str.end());
}

std::string trim_left_space(std::string str)
{
    trim_left_space_by_ref(str);
    return str;
}

std::string trim_rght_space(std::string str)
{
    trim_rght_space_by_ref(str);
    return str;
}

std::string trim_space(std::string str)
{
    trim_space_by_ref(str);
    return str;
}

// -----------------------------------------------------------------------

void trim_left_by_ref(std::string& str, const std::string& chs)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(),
                                        [&](unsigned char ch) {
        return chs.find(ch) == std::string::npos;
    }));
}

void trim_rght_by_ref(std::string& str, const std::string& chs)
{
    str.erase(std::find_if(str.rbegin(), str.rend(),
                           [&](unsigned char ch) {
        return chs.find(ch) == std::string::npos;
    }).base(), str.end());
}

void trim_by_ref(std::string& str, const std::string& chs)
{
    trim_left_by_ref(str, chs);
    trim_rght_by_ref(str, chs);
}

std::string trim_left(std::string str, const std::string& chs)
{
    trim_left_by_ref(str, chs);
    return str;
}

std::string trim_rght(std::string str, const std::string& chs)
{
    trim_rght_by_ref(str, chs);
    return str;
}

std::string trim(std::string str, const std::string& chs)
{
    trim_by_ref(str, chs);
    return str;
}

// -----------------------------------------------------------------------

std::string un_quote(std::string str)
{
    if (str.size() < 2) return str;
    if (str.front() == '\"' && str.back() == '\"') {
        trim_by_ref(str, "\"");
    }
    return str;
}

std::string wrap_quote(std::string str)
{
    return "\"" + str + "\"";
}

// -----------------------------------------------------------------------

bool is_bracket_balance(const std::string& str, char open_br)
{
    char clos_br(0);
    switch (open_br) {
    case '(': clos_br = ')'; break;
    case '{': clos_br = '}'; break;
    case '<': clos_br = '>'; break;
    case '[': clos_br = ']'; break;
    default:
        throw std::runtime_error("bracket unknown");
    }

    std::stack<char> brs;
    for (auto it = str.begin(); it != str.end(); ++it) {
        if (*it == open_br) {
            brs.push(*it);
        }
        else if (*it == clos_br) {
            if (brs.empty())
                return false;
            brs.pop();
        }
    }
    return brs.empty();
}

bool is_round_bracket_balance(const std::string& str)
{
    return is_bracket_balance(str, '(');
}

bool is_curly_bracket_balance(const std::string& str)
{
    return is_bracket_balance(str, '{');
}

bool is_trgle_bracket_balance(const std::string& str)
{
    return is_bracket_balance(str, '<');
}

bool is_squar_bracket_balance(const std::string& str)
{
    return is_bracket_balance(str, '[');
}

}
