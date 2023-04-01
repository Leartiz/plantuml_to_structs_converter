#include <algorithm>
#include <sstream>
#include <utility> // std::move
#include <cctype>

#include "lex_analyzer.h"
#include "utils/puml_utils.h"
#include "utils/string_utils.h"

#include "errors/tltr/lexer_error.h"
#include "errors/err_text_creator.h"

namespace lenv
{

std::vector<Token> tokenize(std::istream& in)
{
    std::vector<Token> res;
    Lex_analyzer lxar{ in };
    while (lxar.scan_some()) {
        while (lxar.available()) {
            res.push_back(lxar.take());
        }
    }
    return res;
}

std::vector<Token> tokenize(const std::string& str)
{
    std::istringstream sin{ str };
    return tokenize(sin);
}

// -----------------------------------------------------------------------

const std::vector<std::string> Lex_analyzer::directives{

};
const std::vector<std::string> Lex_analyzer::keywords{

};

// -----------------------------------------------------------------------

Lex_analyzer::Lex_analyzer(std::istream& in)
    : m_in_stream{ in } {}

bool Lex_analyzer::empty() const
{
    return m_tokns.empty();
}

bool Lex_analyzer::available() const
{
    return !empty();
}

bool Lex_analyzer::ready() const
{
    return m_in_stream.good();
}

void Lex_analyzer::unget(Token token)
{
    m_tokns.push_front(std::move(token));
}

Token Lex_analyzer::actual()
{
    if (!empty() || !scan_some()) {
        return m_tokns.front();
    }
    throw int{};
}

Token Lex_analyzer::take()
{
    if (!empty() || !scan_some()) {
        auto one{ m_tokns.front() };
        m_tokns.pop_front();
        return one;
    }
    throw int{};
}

// -----------------------------------------------------------------------

bool Lex_analyzer::scan_some()
{
    while (can_any_char()) {
        const auto uch{ peek_cur_char() };
        if (is_line_end(uch)) break;

        if (!empty()) {
            const auto one{ actual() };
            if (Token::is_directive(one.tag())) {
                scan_directive_value();
                continue;
            }
        }

        if (uch == '@') {
            scan_any_directive();
        }
        else if (uch == ':') {
            scan_actor_fast_apply();
        }
        else if (uch == '(') {
            scan_use_case_fast_apply();
        }
        else if (is_letter(uch)) {
            scan_kw_or_id();
        }
        else if (is_part_arrow(uch)) {
            scan_arrow();
        }
        else if (is_whitespace(uch)) {
            try_scan_whitespace();
            /* not important? */
        }

        else {
            scan_id();
        }
    }

    if (ready()) {
        push(Token{ '\n', Token::LINE_END });
        discard_any_char();
        m_line_number++;
    }
    return is_pushed();
}

// -----------------------------------------------------------------------

uint8_t Lex_analyzer::take_cur_char()
{
    /* can lead to: eof == 1 */
    const auto ch{ m_in_stream.get() };
    return static_cast<uint8_t>(ch);
}

uint8_t Lex_analyzer::take_low_char()
{
    return std::tolower(take_cur_char());
}

uint8_t Lex_analyzer::peek_cur_char()
{
    const auto ch{ m_in_stream.peek() };
    return static_cast<uint8_t>(ch);
}

uint8_t Lex_analyzer::peek_low_char()
{
    return std::tolower(peek_cur_char());
}

void Lex_analyzer::discard_any_char()
{
    static_cast<void>(take_cur_char());
}

void Lex_analyzer::unget_any_char()
{
    m_in_stream.unget();
}

bool Lex_analyzer::can_any_char()
{
    /* also set to: eof == 1 */
    const auto ch{ m_in_stream.peek() };
    return ch != std::char_traits<char>::eof();
}

// -----------------------------------------------------------------------

void Lex_analyzer::try_scan_whitespace()
{
    std::string actual;
    read_whitespace(actual);
    if (actual.empty()) return;
    push(Token{ actual, Token::WHITESPACE });
}

void Lex_analyzer::scan_any_directive()
{
    discard_any_char();
    if (can_any_char()) {
        const uint8_t low_uch{ peek_low_char() };

        std::string actual;
        if (low_uch == 's') {
            if (read_overlay("startuml", actual)) {
                push(Token{ '@' + actual, Token::DIRECTIVE_STARTUML });
                return; // OK!
            }
        }
        else if (low_uch == 'e') {
            if (read_overlay("enduml", actual)) {
                push(Token{ '@' + actual, Token::DIRECTIVE_ENDUML });
                return;
            }
        }
        // ...
    };

    throw Lexer_error{
        Err_text_creator::dt("Lex_analyzer", "scan_any_directive",
                             "scan failed")
    };
}

void Lex_analyzer::scan_directive_value()
{
    try_scan_whitespace();

    auto actual = std::string{};
    static_cast<void>(read_to('\n', actual));

    if (!actual.empty()) {
        auto wsps = std::string{};
        str_utils::trim_rght_space_by_ref(actual, wsps);
        push(Token{ actual, Token::DIRECTIVE_VALUE });

        if (wsps.empty()) return;
        push(Token{ wsps, Token::WHITESPACE }); // only here!
    }
}

void Lex_analyzer::scan_use_case_fast_apply()
{
    discard_any_char(); // '('
    auto actual = std::string{};
    if (read_to_line_end_or_to(')', actual)) {
        push(Token{ actual, Token::USECASE_FAST_USE });
        discard_any_char(); // ')'
        return;
    }

    throw Lexer_error{
        Err_text_creator::dt("Lex_analyzer", "scan_use_case_fast_apply",
                             "scan failed")
    };
}

void Lex_analyzer::scan_actor_fast_apply()
{
    discard_any_char(); // ':'
    auto actual = std::string{};
    if (read_to_line_end_or_to(':', actual)) {
        push(Token{ actual, Token::ACTOR_FAST_USE });
        discard_any_char(); // ':'
        return;
    }

    throw Lexer_error{
        Err_text_creator::dt("Lex_analyzer", "scan_actor_fast_apply",
                             "scan failed")
    };
}

void Lex_analyzer::scan_arrow()
{
    auto actual = std::string{};
    while (can_any_char() && is_part_arrow(peek_low_char())) {
        actual.push_back(take_cur_char());
    }

    if (ready()) {
        push(Token{ actual, Token::ARROW });
    }
}

void Lex_analyzer::scan_kw_or_id()
{
    auto actual = std::string{};
    read_to_separator(actual);

    if (str_utils::eq_ref(actual, wsd_utils::kw_as, false)) {
        push(Token{ actual, Token::KW_AS });
    }
    else if (str_utils::eq_ref(actual, wsd_utils::kw_usecase, false)) {
        push(Token{ actual, Token::KW_USECASE });
    }
    else if (str_utils::eq_ref(actual, wsd_utils::kw_actor, false)) {
        push(Token{ actual, Token::KW_ACTOR });
    }
    // ...
    else {
        push(Token{ actual, Token::IDENTIFIER });
    }
}

void Lex_analyzer::scan_id()
{
    auto actual = std::string{};
    (void)read_to_separator(actual);
    push(Token{ actual, Token::IDENTIFIER });
}

// -----------------------------------------------------------------------

bool Lex_analyzer::read_overlay(const std::string& expected, std::string& actual)
{
    actual.clear();
    for (size_t i = 0; i < expected.size() && can_any_char(); ++i) {
        const uint8_t uch = take_cur_char();
        const uint8_t low_uch = std::tolower(uch);

        actual.push_back(uch);
        if (expected[i] != low_uch) {
            return false;
        }
    }
    return ready();
}

bool Lex_analyzer::read_to_line_end_or_to(const char end, std::string& actual)
{
    actual.clear();
    while (can_any_char() && peek_low_char() != end) {
        if (is_line_end(peek_low_char())) {
            return false;
        }

        actual.push_back(take_cur_char());
    }
    return ready();
}

bool Lex_analyzer::read_to(const char end, std::string& actual)
{
    actual.clear();
    while (can_any_char() && peek_low_char() != end) {
        actual.push_back(take_cur_char());
    }
    return ready();
}

bool Lex_analyzer::read_to_separator(std::string& actual)
{
    actual.clear();
    while (can_any_char() && !is_separator(peek_low_char())) {
        actual.push_back(take_cur_char());
    }
    return ready();
}

void Lex_analyzer::read_whitespace(std::string& actual)
{
    actual.clear();
    while (can_any_char() && is_whitespace(peek_low_char())) {
        actual.push_back(take_cur_char());
    }
}

// -----------------------------------------------------------------------

bool Lex_analyzer::is_letter(const uint8_t uch) const
{
    return str_utils::is_eng_letter(uch);
}

bool Lex_analyzer::is_separator(const uint8_t uch) const
{
    return std::isspace(uch) || uch == ':' ||
            str_utils::is_brs_letter(uch) || is_part_arrow(uch); // ??
}

bool Lex_analyzer::is_part_arrow(const uint8_t uch) const
{
    return uch == '.' || uch == '-' || uch == '<' || uch == '>';
}

bool Lex_analyzer::is_whitespace(const uint8_t uch) const
{
    return std::isspace(uch) && uch != '\n';
}

bool Lex_analyzer::is_line_end(const uint8_t uch) const
{
    return uch == '\n';
}

// -----------------------------------------------------------------------

void Lex_analyzer::push(Token token)
{
    m_tokns.push_back(std::move(token));
    m_is_pushed = true;
}

bool Lex_analyzer::is_pushed()
{
    const auto cpy{ m_is_pushed };
    m_is_pushed = false;
    return cpy;
}

}
