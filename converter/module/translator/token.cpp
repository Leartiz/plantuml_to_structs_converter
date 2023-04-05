#include <utility> // std::move

#include "token.h"

namespace lenv
{

bool Token::is_directive(const Tag tag)
{
    return tag == DIRECTIVE_STARTUML ||
            tag == DIRECTIVE_ENDUML;
}

bool Token::is_whitespace(const Tag tag)
{
    return tag == WHITESPACE;
}

bool Token::is_node(const Tag tag)
{
    return
            tag == IDENTIFIER ||
            tag == ACTOR_FAST_USE ||
            tag == USECASE_FAST_USE ||
            tag == ONE_STRING;
}

// -----------------------------------------------------------------------

Token::Token(std::string val, Token::Tag tag)
    : m_value{ std::move(val) }, m_tag{ tag } {}

Token::Token(const char val, Token::Tag tag)
    : m_value(1, val), m_tag{ tag } {}

const std::string& Token::value() const
{
    return m_value;
}

Token::Tag Token::tag() const
{
    return m_tag;
}

// -----------------------------------------------------------------------

bool operator==(const Token& lhs, const Token& rhs)
{
    return lhs.tag() == rhs.tag() && lhs.value() == rhs.value();
}

bool operator!=(const Token& lhs, const Token& rhs)
{
    return !operator==(lhs, rhs);
}

}