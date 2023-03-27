#ifndef COMPLETE_TRANSLATOR_H
#define COMPLETE_TRANSLATOR_H

#include "translator.h"

namespace lenv
{

/* according to the rules of the translator? */
class Complete_translator final : public Translator
{
public:
    Layout_Flow_dia_sp convert_lw_dia(std::istream& in) override;
    Use_Case_dia_sp convert_uc_dia(std::istream& in) override;
    Robustness_dia_sp convert_rob_dia(std::istream& in) override;
    Sequence_dia_sp convert_seq_dia(std::istream& in) override;
    Class_dia_sp convert_class_dia(std::istream& in) override;
};

}

#endif // COMPLETE_TRANSLATOR_H
