#ifndef DIRECT_TRANSLATOR_H
#define DIRECT_TRANSLATOR_H

#include "translator.h"

namespace lenv
{

class Direct_translator final : public Translator
{
public:
    Use_Case_dia_sp convert_uc_dia(std::istream& in) override;
    Robustness_dia_sp convert_rob_dia(std::istream& in) override;
    Sequence_dia_sp convert_seq_dia(std::istream& in) override;
    Class_dia_sp convert_class_dia(std::istream& in) override;
};

}

#endif // DIRECT_TRANSLATOR_H
