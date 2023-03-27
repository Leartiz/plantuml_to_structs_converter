#include <string>
#include <sstream>

#include "use_case/use_case_dia.h"
#include "uc_dia_direct_converter.h"

#include "utils/string_utils.h"
#include "utils/puml_utils.h"

#include "errors/err_text_creator.h"
#include "errors/tltr/unknown_directive.h"

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

    while (!m_completed && std::getline(m_in_stream, m_cur_line)) {
        std::istringstream sin{ m_cur_line };
        char start_ch{ 0 }; sin >> start_ch;

        if (start_ch == '@') {
            read_directive();
        }
        else if (start_ch == '(') {
            read_connection();
        }
        else if (start_ch == ':') {
            read_connection();
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
        else if (m_cur_line.empty()) {
        }
        else {
            conv_word();
        }
    }

    return m_uc_dia;
}

void UC_dia_direct_converter::conv_word()
{
    auto word = std::string{};
    std::istringstream sin{ m_cur_line };
    sin >> word;

    if (word == Puml_utils::kw_usecase) {
        m_cur_line = m_cur_line.substr(Puml_utils::kw_usecase.size());
        read_whole_use_case();
    }
    else if (word == Puml_utils::kw_actor) {
        m_cur_line = m_cur_line.substr(Puml_utils::kw_actor.size());
        read_whole_actor();
    }
    else {
        read_connection();
    }
}

// -----------------------------------------------------------------------

void UC_dia_direct_converter::read_multi_line_comment()
{

}

void UC_dia_direct_converter::read_directive()
{
    std::string name_dia; /* eq Use_Case_dia::id; */
    if (String_utils::start_with(m_cur_line, Puml_utils::startuml)) {
        if (!Puml_utils::read_startuml_directive(m_cur_line, name_dia)) {
            throw int{}; // TODO:
        }
        if (!name_dia.empty() && name_dia != Use_Case_dia::id) {
            throw int{};
        }

        m_uc_dia->reset_all(); // multiple @startuml allowed!
    }
    else if (String_utils::start_with(m_cur_line, Puml_utils::enduml)) {
        if (!Puml_utils::read_enduml_directive(m_cur_line)) {
            throw int{};
        }
        m_completed = true;
    }
    else {
        throw Unknown_directive{
            Err_text_creator::dt("UC_dia_direct_converter", "read_directive",
                                 "")
        };
    }
}

void UC_dia_direct_converter::read_connection()
{

}

// -----------------------------------------------------------------------

void UC_dia_direct_converter::read_whole_use_case()
{
    std::istringstream sin{ m_cur_line };
    std::string name;
    std::string as;
    std::string id;
    sin >> name;
    sin >> as;
    sin >> id;

    UC_node::Builder node_b{ id };
    auto node = node_b.type( UC_node::Type::USE_CASE )
            .name(String_utils::un_quote(name)).build_ptr();

    m_uc_dia->add_node_bfore_adder(node);
}

void UC_dia_direct_converter::read_short_use_case()
{

}

void UC_dia_direct_converter::read_whole_actor()
{
    std::istringstream sin{ m_cur_line };
    std::string name;
    std::string as;
    std::string id;
    sin >> name;
    sin >> as;
    sin >> id;

    UC_node::Builder node_b{ id };
    auto node = node_b.type( UC_node::Type::ACTOR )
            .name(String_utils::un_quote(name)).build_ptr();

    m_uc_dia->add_node_bfore_adder(node);
}

void UC_dia_direct_converter::read_short_actor()
{

}

}
