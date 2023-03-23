#include "translator.h"

#include "use_case/uc_lexic_analyzer.h"

namespace lenv
{

Translator::Translator()
{

}

Use_Case_dia_sp Translator::convert_uc_dia(std::istream& in)
{
    auto lex_analyzer{ UC_lexic_analyzer(in) };


    Use_Case_dia_sp result;
    return result;
}

}
