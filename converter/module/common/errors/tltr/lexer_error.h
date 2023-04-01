#ifndef LEXER_ERROR_H
#define LEXER_ERROR_H

#include "tltr_error.h"

namespace lenv
{

class Lexer_error : public Tltr_error
{
public:
    Lexer_error(const std::string& arg);
};

}

#endif // LEXER_ERROR_H
