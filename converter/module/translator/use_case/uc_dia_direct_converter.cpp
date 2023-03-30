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
        read_node_creation(UC_node::USE_CASE);
    }
    else if (String_utils::eq_ref(word, Puml_utils::kw_actor, false)) {
        read_node_creation(UC_node::ACTOR);
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

    std::string node_id;
    if (start_ch == ':') {
        std::getline(m_csin, node_id, ':');
        if (!m_csin.good()) {

        }
    }
    else if (start_ch == '(') {
        std::getline(m_csin, node_id, ')');
        if (!m_csin.good()) {

        }
    }
    else {

    }
}

// -----------------------------------------------------------------------

void UC_dia_direct_converter::read_node_creation(UC_node::Type primary_type)
{
    std::string name, id;
    UC_node::Type type = primary_type;

    if (primary_type == UC_node::Type::ACTOR) {
        Puml_utils::UC_dia::read_actor_creation(m_csin.str(), name, id, type);
    }
    else {
        Puml_utils::UC_dia::read_use_case_creation(m_csin.str(), name, id, type);
    }

    UC_node::Builder node_b{ id };
    node_b.name(name).type(type);

    const auto node{ node_b.build_ptr() };
    m_uc_dia->add_node_bfore_adder(node);
}

}
