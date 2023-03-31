#ifndef TOKEN_TAG_H
#define TOKEN_TAG_H

namespace lenv
{

enum class Token_tag
{
    DIRECTIVE_STARTUML,
    DIRECTIVE_ENDUML,

    KW_USECASE,
    KW_AS,
    KW_SKINPARAM,

    // --- UC dia.

    USECASE_FAST_DEF, // (UseCase)
    ACTOR_FAST_DEF,   // :Actor:
    CONNECTION_NOTE,  // a -> b : note

    // --- Rob dia.

    // --- Seq dia.

    // --- Class dia.

    // --- LW dia.

};

}

#endif // TOKEN_TAG_H
