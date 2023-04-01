#ifndef LEX_ANALYZER_H
#define LEX_ANALYZER_H

#include <deque>
#include <vector>
#include <string>
#include <iostream>

#include "token.h"

namespace lenv
{

std::vector<Token> tokenize(std::istream& in);
std::vector<Token> tokenize(const std::string& str);

class Lex_analyzer final
{
public:
    static const std::vector<std::string> directives;
    static const std::vector<std::string> keywords;

public:
    Lex_analyzer(std::istream& in);

public:
    bool ready() const;
    void unget(Token token);
    bool scan_some();

public:
    bool empty() const;
    bool available() const;
    Token actual();
    Token take();

private:
    uint8_t take_cur_char();
    uint8_t take_low_char();
    uint8_t peek_cur_char();
    uint8_t peek_low_char();
    void discard_any_char();
    void unget_any_char();
    bool can_any_char();

private:
    void try_scan_whitespace();
    void scan_any_directive();
    void scan_directive_value();

    void scan_use_case_fast_apply();
    void scan_actor_fast_apply();
    void scan_string();
    void scan_arrow();

    void scan_kw_or_id();
    void scan_id();

private:
    bool read_overlay(const std::string& expected, std::string& actual);
    bool read_to_line_end_or_to(const char end, std::string& actual);
    bool read_to(const char end, std::string& actual);
    bool read_to_separator(std::string& actual);
    void read_whitespace(std::string& actual);

private:
    bool is_letter(const uint8_t uch) const;
    bool is_separator(const uint8_t uch) const;
    bool is_part_arrow(const uint8_t uch) const;
    bool is_whitespace(const uint8_t uch) const;
    bool is_line_end(const uint8_t uch) const;

private:
    void push(Token token);
    bool is_pushed();

private:
    std::deque<Token> m_tokns;
    bool m_is_pushed{ false };

    std::istream& m_in_stream;
    size_t m_line_number{ 1 };
};

}

#endif // LEX_ANALYZER_H



