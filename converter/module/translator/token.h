#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#include "token_tag.h"

namespace lenv
{

class Token final
{
public:

private:
    std::string m_value;
    Token_tag m_tok_tag;
};

}

#endif // TOKEN_H
