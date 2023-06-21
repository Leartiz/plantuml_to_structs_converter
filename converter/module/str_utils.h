#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <string>
#include <vector>

namespace str_utils
{

bool eq(char lhs, char rhs, bool sensitive = true);
bool ne(char lhs, char rhs, bool sensitive = true);

void to_upper_by_ref(std::string& str);
void to_lower_by_ref(std::string& str);

std::string to_upper(const std::string& str);
std::string to_lower(const std::string& str);

bool start_with(std::string str, std::string start, bool sensitive = true);
bool stop_with(std::string str, std::string stop, bool sensitive = true);

bool eq(std::string lhs, std::string rhs, bool sensitive = true);
bool ne(std::string lhs, std::string rhs, bool sensitive = true);
bool eq_ref(const std::string& lhs, const std::string& rhs, bool sensitive = true);
bool ne_ref(const std::string& lhs, const std::string& rhs, bool sensitive = true);

void trim_left_space_by_ref(std::string& str);
void trim_rght_space_by_ref(std::string& str);
void trim_space_by_ref(std::string& str);

void trim_left_space_by_ref(std::string& str, std::string& wsps);
void trim_rght_space_by_ref(std::string& str,  std::string& wsps);

std::string trim_left_space(std::string str);
std::string trim_rght_space(std::string str);
std::string trim_space(std::string str);

void trim_left_by_ref(std::string& str, const std::string& chs);
void trim_rght_by_ref(std::string& str, const std::string& chs);
void trim_by_ref(std::string& str, const std::string& chs);

std::string trim_left(std::string str, const std::string& chs);
std::string trim_rght(std::string str, const std::string& chs);
std::string trim(std::string str, const std::string& chs);

std::string un_quote(std::string str);
std::string wrap_quote(std::string str);

bool is_bracket_balance(const std::string& str, char br);
bool is_round_bracket_balance(const std::string& str);
bool is_curly_bracket_balance(const std::string& str);
bool is_trgle_bracket_balance(const std::string& str);
bool is_squar_bracket_balance(const std::string& str);



}

#endif // STR_UTILS_H

