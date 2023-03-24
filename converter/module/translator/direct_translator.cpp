#include <string>

#include "direct_translator.h"

namespace lenv
{

Use_Case_dia_sp Direct_translator::convert_uc_dia(std::istream& in)
{
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << std::endl;
    }
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
