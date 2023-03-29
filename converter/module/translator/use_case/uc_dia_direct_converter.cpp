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

    auto cur_line = std::string{};
    while (!m_completed && std::getline(m_in_stream, cur_line, '\n')) {
        String_utils::trim_space_by_ref(cur_line);
        m_csin.str(cur_line);
        m_csin.clear(); // to good.

        char start_ch{ 0 };
        m_csin >> start_ch;
        m_csin.unget();

        if (start_ch == '@') {
            read_directive();
        }
        else if (start_ch == '(' || start_ch == ':') {
            read_connection();
        }
        else if (start_ch == '/') {
            read_multi_line_comment();
        }
        /* skip line */
        else if (start_ch == '\'') {}
        else if (cur_line.empty()) {}
        else {
            conv_word();
        }
    }

    return m_uc_dia;
}

void UC_dia_direct_converter::conv_word()
{
    std::string word;
    m_csin >> word; // new position!

    if (String_utils::eq_ref(word, Puml_utils::kw_usecase, false)) {
        read_use_case();
    }
    else if (String_utils::eq_ref(word, Puml_utils::kw_actor, false)) {
        read_actor();
    }
    /* extra */
    else if (String_utils::eq(word, Puml_utils::kw_skinparam, false)) {
        read_skinparam();
    }
    else if (String_utils::eq(word, Puml_utils::kw_note, false)) {
        read_note();
    }
    else if (String_utils::eq(word, Puml_utils::kw_left, false)) {}
    else if (String_utils::eq(word, Puml_utils::kw_top, false)) {}
    else if (Puml_utils::is_keyword(word)) {
        throw int{  1 };
        // TODO: все остальное запрет!
    }
    else {
        read_connection();
    }
}

// -----------------------------------------------------------------------

void UC_dia_direct_converter::read_multi_line_comment()
{

}

void UC_dia_direct_converter::read_skinparam()
{

}

void UC_dia_direct_converter::read_note()
{

}

void UC_dia_direct_converter::read_directive()
{
    std::string dia_id; /* eq Use_Case_dia::id; */
    const std::string cur_line{ m_csin.str() };
    if (String_utils::start_with(cur_line, Puml_utils::startuml)) {
        if (!Puml_utils::read_startuml_directive(cur_line, dia_id)) {
            throw int{}; // TODO:
        }
        if (!dia_id.empty() && dia_id != Use_Case_dia::id) {
            throw int{};
        }

        m_uc_dia->reset_all(); // multiple @startuml allowed!
    }
    else if (String_utils::start_with(cur_line, Puml_utils::enduml)) {
        if (!Puml_utils::read_enduml_directive(cur_line)) {
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
    char start_ch{ 0 };
    m_csin >> start_ch;
    m_csin.unget();

    if (start_ch == ':') {

    }
    else if (start_ch == '(') {

    }
    else {

    }
}

// -----------------------------------------------------------------------

void UC_dia_direct_converter::read_use_case()
{
    UC_node::Type id_type{ UC_node::Type::USE_CASE };
    UC_node::Type name_type{ UC_node::Type::USE_CASE };

    char start_name_ch{ 0 };
    m_csin >> start_name_ch;

    std::string name;
    if (start_name_ch == ':') {
        std::getline(m_csin, name, ':');
        name_type = UC_node::Type::ACTOR;
    }
    else if (start_name_ch == '(') {
        std::getline(m_csin, name, ')');
    }
    else if (start_name_ch == '\"') {
        std::getline(m_csin, name, '\"');
    }
    else {
        m_csin.unget();
        m_csin >> name;
    }

    std::string id{ name };
    std::string as; m_csin >> as;
    if (String_utils::eq_ref(as, Puml_utils::kw_as, false)) {
        char start_id_ch{ 0 };
        m_csin >> start_id_ch;

        if (start_id_ch == ':') {
            std::getline(m_csin, id, ':');
            name_type = UC_node::Type::ACTOR;
        }
        else if (start_id_ch == '(') {
            std::getline(m_csin, id, ')');
        }
        else if (start_id_ch == '\"') {
            std::getline(m_csin, id, '\"');
        }
        else {
            m_csin.unget();
            m_csin >> id;
        }
    }

    UC_node::Builder node_b{ id };
    node_b.type(name_type).name(name);

    const auto node = node_b.build_ptr();
    m_uc_dia->add_node_bfore_adder(node);
}

void UC_dia_direct_converter::read_actor()
{
    std::string name;
    std::string as;
    std::string id;
    m_csin >> name;
    m_csin >> as;
    m_csin >> id;

    UC_node::Builder node_b{ id };
    auto node = node_b.type( UC_node::Type::ACTOR )
            .name(String_utils::un_quote(name)).build_ptr();

    m_uc_dia->add_node_bfore_adder(node);
}

}
