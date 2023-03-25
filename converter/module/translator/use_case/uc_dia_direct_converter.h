#ifndef UC_DIA_DIRECT_CONVERTER_H
#define UC_DIA_DIRECT_CONVERTER_H

#include <string>
#include <iostream>

#include "use_case/use_case_dia.h"

namespace lenv
{

class UC_dia_direct_converter final
{
public:


public:
    UC_dia_direct_converter(std::istream& in);
    Use_Case_dia_sp operator()();
    Use_Case_dia_sp conv();

private:
    void conv_word();
    void read_multi_line_comment();
    void read_directive(); // @startuml, @enduml
    void read_use_case();
    void read_actor();

private:
    std::istream& m_in_stream;
    Use_Case_dia_sp m_uc_dia;

private:
    std::string m_cur_line;
};

}

#endif // UC_DIA_DIRECT_CONVERTER_H
