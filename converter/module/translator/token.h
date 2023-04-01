#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace lenv
{

class Token final
{
public:
    enum Tag
    {
        DIRECTIVE_STARTUML,
        DIRECTIVE_ENDUML,
        DIRECTIVE_VALUE, // no directive!

        EMBEDDED_STYLE, // #...

        KW_SKINPARAM,
        KW_RECTANGLE,
        KW_PACKAGE,

        KW_USECASE,
        KW_ACTOR,
        KW_AS,
        KW_TO,

        KW_LEFT,
        KW_RIGHT,
        KW_UP,
        KW_DOWN,
        KW_DIRECTION,
        KW_NOTE,
        KW_OF,
        KW_END,

        WHITESPACE,
        LINE_END,
        OPN_CURLY_BR,
        CLS_CURLY_BR,
        COLON,

        STRING,
        IDENTIFIER,
        ARROW,

        ONE_LINE_COMMENT,
        MULTI_LINE_COMMENT,

        ONE_LINE_NOTE,
        MULTI_LINE_NOTE,

        // --- UC dia.

        USECASE_FAST_USE, // (UseCase)
        ACTOR_FAST_USE,   // :Actor:

        // --- Rob dia.

        // --- Seq dia.

        // --- Class dia.

        // --- LW dia.

    };

    static bool is_directive(const Tag tag);

public:
    Token(std::string val, Token::Tag tag);
    Token(const char val, Token::Tag tag);

public:
    const std::string& value() const;
    Tag tag() const;

private:
    std::string m_value;
    Tag m_tag;
};

bool operator==(const Token& lhs, const Token& rhs);
bool operator!=(const Token& lhs, const Token& rhs);

}

#endif // TOKEN_H
