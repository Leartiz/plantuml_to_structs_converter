#include <sstream>

#include "puml_utils.h"
#include "string_utils.h"

namespace lenv
{

const std::string Puml_utils::startuml{ "@startuml" };
const std::string Puml_utils::enduml{ "@enduml" };

const std::string Puml_utils::start_mcomment{ "\'" };
const std::string Puml_utils::start_ocomment{ "'" };

const std::string Puml_utils::kw_actor{ "actor" };
const std::string Puml_utils::kw_usecase{ "usecase" };
const std::string Puml_utils::kw_as{ "as" };

const std::string Puml_utils::kw_package{ "package" };
const std::string Puml_utils::kw_rectangle{ "rectangle" };

const std::string Puml_utils::kw_left{ "left" };
const std::string Puml_utils::kw_right{ "right" };
const std::string Puml_utils::kw_top{ "top" };
const std::string Puml_utils::kw_bottom{ "bottom" };

const std::string Puml_utils::kw_to{ "to" };
const std::string Puml_utils::kw_direction{ "direction" };

const std::string Puml_utils::kw_skinparam{ "skinparam" };
const std::string Puml_utils::kw_note{ "note" };
const std::string Puml_utils::kw_of{ "of" };
const std::string Puml_utils::kw_end{ "end" };

// -----------------------------------------------------------------------

bool Puml_utils::is_keyword(const std::string& str)
{
    return false;
}

bool Puml_utils::read_startuml_directive(const std::string& line,
                                         std::string& out_name)
{
    std::istringstream sin{ line };
    std::string directive; sin >> directive;
    if (!String_utils::eq_ref(directive, startuml, false))
        return false;

    std::getline(sin, out_name, '\n');
    String_utils::trim_space_by_ref(out_name);
    return true;
}

bool Puml_utils::read_enduml_directive(const std::string& line)
{
    std::string directive{ String_utils::trim_space(line) };
    return String_utils::eq_ref(directive, enduml, false);
}

// -----------------------------------------------------------------------

bool Puml_utils::read_use_case_creation(const std::string& line,
                                        std::string& out_name,
                                        std::string& out_id,
                                        UC_node::Type& out_type)
{
    std::istringstream sin{ line };
    std::string keyword;
    sin >> keyword;

    if (!String_utils::eq_ref(keyword, kw_usecase, false)) {
        return false;
    }

    char start_lstr_ch{ 0 };
    sin >> start_lstr_ch;

    std::string lstr;
    UC_node::Type ltype{ UC_node::USE_CASE };
    if (start_lstr_ch == ':') {
        std::getline(sin, lstr, ':'); // TODO:
        ltype = UC_node::ACTOR;
    }
    else if (start_lstr_ch == '(') {
        std::getline(sin, lstr, ')');

    }
    else if (start_lstr_ch == '\"') {
        std::getline(sin, lstr, '\"');
    }
    else {
        sin.unget();
        sin >> lstr;
    }

    keyword.clear();
    sin >> keyword;

    if (keyword.empty()) {
        out_id = lstr;
        out_name = lstr;
        out_type = ltype;
        return true;
    }

    if (!String_utils::eq_ref(keyword, kw_as, false)) {
        return false;
    }

    char start_rstr_ch{ 0 };
    sin >> start_rstr_ch;

    std::string rstr;
    UC_node::Type rtype{ UC_node::USE_CASE };
    if (start_rstr_ch == ':') {
        std::getline(sin, rstr, ':');
        rtype = UC_node::ACTOR;

        out_id = rstr;
        out_name = lstr;
        out_type = rtype;

        if (start_lstr_ch == '(') {
            out_type = ltype;
        }
    }
    else if (start_rstr_ch == '(') {
        std::getline(sin, rstr, ')');

        out_id = rstr;
        out_name = lstr;
        out_type = rtype;
    }
    else if (start_rstr_ch == '\"') {
        std::getline(sin, rstr, '\"');

        out_id = lstr;
        out_name = rstr;
        out_type = ltype;
    }
    else {
        sin.unget();
        sin >> rstr;

        out_id = rstr;
        out_name = lstr;
        out_type = rtype;
    }

    std::string rest;
    sin >> rest;
    if (!rest.empty()) {
        out_id.clear();
        out_name.clear();
        return false;
    }
    return true;
}

bool Puml_utils::read_actor_creation(const std::string& line,
                                     std::string& out_name,
                                     std::string& out_id,
                                     UC_node::Type& out_type)
{

}

}
