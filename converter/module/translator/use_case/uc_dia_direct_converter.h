#ifndef UC_DIA_DIRECT_CONVERTER_H
#define UC_DIA_DIRECT_CONVERTER_H

#include <string>
#include <iostream>

#include "use_case/uc_ptrs.h"

namespace lenv
{

class UC_dia_direct_converter final
{
public:
    UC_dia_direct_converter(std::istream& in);
    Use_Case_dia_sp operator()();
    Use_Case_dia_sp conv();

private:
    void conv_word();
    void read_multi_line_comment();
    void read_directive(); // @startuml, @enduml
    void read_connection();

private:
    void read_whole_use_case();
    void read_short_use_case();

    void read_whole_actor();
    void read_short_actor();

private:
    std::istream& m_in_stream;
    Use_Case_dia_sp m_uc_dia;

private:
    std::string m_cur_line;
    bool m_completed{ false };
};

}

#endif // UC_DIA_DIRECT_CONVERTER_H
