#include <string>
#include <sstream>

#include "uc_dia_direct_converter.h"

namespace lenv
{

UC_dia_direct_converter::UC_dia_direct_converter(std::istream& in)
    : m_in_stream{ in } {}

Use_Case_dia_sp UC_dia_direct_converter::operator()()
{
    return conv();
}

// -----------------------------------------------------------------------

Use_Case_dia_sp UC_dia_direct_converter::conv()
{
    Use_Case_dia::Builder uc_dia_b;
    m_uc_dia = uc_dia_b.build_ptr();

    while (std::getline(m_in_stream, m_cur_line)) {
        std::istringstream sin{ m_cur_line };
        char start_ch{ 0 }; sin >> start_ch;

        if (start_ch == '@') {
            read_directive();
        }
        else if (start_ch == '(') {
            read_use_case();
        }
        else if (start_ch == ':') {
            read_actor();
        }
        else if (start_ch == '\'') {
            // one line comment
        }
        else if (start_ch == '/') {
            read_multi_line_comment();
        }
        else if (start_ch == '}') {
            // end of rectangle/package/...
        }
        else {
            conv_word();
        }
    }

    return m_uc_dia;
}

void UC_dia_direct_converter::conv_word()
{

}

// -----------------------------------------------------------------------

void UC_dia_direct_converter::read_multi_line_comment()
{

}

void UC_dia_direct_converter::read_directive()
{
    std::istringstream sin{ m_cur_line };
    std::string directive; sin >> directive;

    if (directive == "@startuml") {

    }


    std::cout << directive << std::endl;
}

void UC_dia_direct_converter::read_use_case()
{

}

void UC_dia_direct_converter::read_actor()
{

}

}
