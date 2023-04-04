#include "regex_translator.h"

namespace lenv
{

Project_sp Regex_translator::convert_all(const std::string& project_root_path)
{
    return Translator::convert_all(project_root_path);
}

Layout_Flow_dia_sp Regex_translator::convert_lw_dia(std::istream& in)
{
    return nullptr;
}

Use_Case_dia_sp Regex_translator::convert_uc_dia(std::istream& in)
{

}

Robustness_dia_sp Regex_translator::convert_rob_dia(std::istream& in)
{
    return nullptr;
}

Sequence_dia_sp Regex_translator::convert_seq_dia(std::istream& in)
{
    return nullptr;
}

Class_dia_sp Regex_translator::convert_class_dia(std::istream& in)
{
    return nullptr;
}

}
