#include <string>

#include "direct_translator.h"
#include "use_case/uc_dia_direct_converter.h"

namespace lenv
{

Use_Case_dia_sp Direct_translator::convert_uc_dia(std::istream& in)
{
    UC_dia_direct_converter conv{ in };
    return conv();
}

Robustness_dia_sp Direct_translator::convert_rob_dia(std::istream& in)
{
    return nullptr;
}

Sequence_dia_sp Direct_translator::convert_seq_dia(std::istream& in)
{
    return nullptr;
}

Class_dia_sp Direct_translator::convert_class_dia(std::istream& in)
{
    return nullptr;
}

}
