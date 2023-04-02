#include <sstream>
#include <algorithm>

#include "wsd_utils.h"
#include "str_utils.h"

namespace lenv
{

const std::string wsd_utils::startuml{ "@startuml" };
const std::string wsd_utils::enduml{ "@enduml" };

const std::string wsd_utils::kw_startuml{ "startuml" };
const std::string wsd_utils::kw_enduml{ "enduml" };

const std::string wsd_utils::start_mcomment{ "\'" };
const std::string wsd_utils::start_ocomment{ "'" };

const std::string wsd_utils::kw_actor{ "actor" };
const std::string wsd_utils::kw_usecase{ "usecase" };
const std::string wsd_utils::kw_as{ "as" };

const std::string wsd_utils::kw_package{ "package" };
const std::string wsd_utils::kw_rectangle{ "rectangle" };

const std::string wsd_utils::kw_left{ "left" };
const std::string wsd_utils::kw_right{ "right" };
const std::string wsd_utils::kw_top{ "top" };
const std::string wsd_utils::kw_up{ "up" };
const std::string wsd_utils::kw_bottom{ "bottom" };

const std::string wsd_utils::kw_to{ "to" };
const std::string wsd_utils::kw_direction{ "direction" };

const std::string wsd_utils::kw_skinparam{ "skinparam" };
const std::string wsd_utils::kw_note{ "note" };
const std::string wsd_utils::kw_of{ "of" };
const std::string wsd_utils::kw_end{ "end" };

// -----------------------------------------------------------------------

const std::vector<std::string> wsd_utils::arrow_heads {
    "<|", "|>",
    "*", "o",
    "<", ">",
    "#", "x",
    "}", "{",
    "+", "^",
    "()", "(", ")",
};

const std::vector<char> wsd_utils::arrow_body {
    '.', '-',
};

// -----------------------------------------------------------------------

bool wsd_utils::is_keyword(const std::string& str)
{
    return false;
}

bool wsd_utils::read_startuml_directive(const std::string& line,
                                         std::string& out_name)
{
    std::istringstream sin{ line };
    std::string directive; sin >> directive;
    if (!str_utils::eq_ref(directive, startuml, false))
        return false;

    std::getline(sin, out_name, '\n');
    str_utils::trim_space_by_ref(out_name);
    return true;
}

bool wsd_utils::read_enduml_directive(const std::string& line)
{
    std::string directive{ str_utils::trim_space(line) };
    return str_utils::eq_ref(directive, enduml, false);
}

// -----------------------------------------------------------------------

bool wsd_utils::UC_dia::read_use_case_creation(const std::string& line,
                                                std::string& out_name,
                                                std::string& out_id,
                                                UC_node::Type& out_type)
{
    std::istringstream sin{ line };
    std::string keyword;
    sin >> keyword;

    if (!str_utils::eq_ref(keyword, kw_usecase, false)) {
        return false;
    }

    char start_lstr_ch{ 0 };
    sin >> start_lstr_ch;

    std::string lstr;
    UC_node::Type ltype{ UC_node::USE_CASE };
    if (start_lstr_ch == ':') {
        if (!std::getline(sin, lstr, ':').good()) {
            return false;
        }
        ltype = UC_node::ACTOR;
    }
    else if (start_lstr_ch == '(') {
        if (!std::getline(sin, lstr, ')').good()) {
            return false;
        }
    }
    else if (start_lstr_ch == '\"') {
        if (!std::getline(sin, lstr, '\"').good()) {
            return false;
        }
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

    if (!str_utils::eq_ref(keyword, kw_as, false)) {
        return false;
    }

    char start_rstr_ch{ 0 };
    sin >> start_rstr_ch;

    std::string rstr;
    UC_node::Type rtype{ UC_node::USE_CASE };
    if (start_rstr_ch == ':') {
        if (!std::getline(sin, rstr, ':').good()) {
            return false;
        }
        rtype = UC_node::ACTOR;

        out_id = rstr;
        out_name = lstr;
        out_type = rtype;

        if (start_lstr_ch == '(') {
            out_type = ltype;
        }
    }
    else if (start_rstr_ch == '(') {
        if (!std::getline(sin, rstr, ')').good()) {
            return false;
        }

        out_id = rstr;
        out_name = lstr;
        out_type = rtype;
    }
    else if (start_rstr_ch == '\"') {
        if (!std::getline(sin, rstr, '\"').good()) {
            return false;
        }

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

bool wsd_utils::UC_dia::read_actor_creation(const std::string& line,
                                             std::string& out_name,
                                             std::string& out_id,
                                             UC_node::Type& out_type)
{
    std::istringstream sin{ line };
    std::string keyword;
    sin >> keyword;

    if (!str_utils::eq_ref(keyword, kw_actor, false)) {
        return false;
    }

    char start_lstr_ch{ 0 };
    sin >> start_lstr_ch;

    std::string lstr;
    UC_node::Type ltype{ UC_node::ACTOR };
    if (start_lstr_ch == ':') {
        if (!std::getline(sin, lstr, ':').good()) {
            return false;
        }
    }
    else if (start_lstr_ch == '(') {
        if (!std::getline(sin, lstr, ')').good()) {
            return false;
        }
        ltype = UC_node::USE_CASE;
    }
    else if (start_lstr_ch == '\"') {
        if (!std::getline(sin, lstr, '\"').good()) {
            return false;
        }
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

    if (!str_utils::eq_ref(keyword, kw_as, false)) {
        return false;
    }

    char start_rstr_ch{ 0 };
    sin >> start_rstr_ch;

    std::string rstr;
    UC_node::Type rtype{ UC_node::ACTOR };
    if (start_rstr_ch == ':') {
        if (!std::getline(sin, rstr, ':').good()) {
            return false;
        }

        out_id = rstr;
        out_name = lstr;
        out_type = rtype;

        if (start_lstr_ch == '(') {
            out_type = ltype;
        }
    }
    else if (start_rstr_ch == '(') {
        if (!std::getline(sin, rstr, ')').good()) {
            return false;
        }
        rtype = UC_node::USE_CASE;

        out_id = rstr;
        out_name = lstr;
        out_type = rtype;
    }
    else if (start_rstr_ch == '\"') {
        if (!std::getline(sin, rstr, '\"').good()) {
            return false;
        }

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

bool wsd_utils::UC_dia::read_connection_creation(const std::string& line,
                                                  std::string& out_beg_str,
                                                  std::string& out_end_str,
                                                  UC_edge::Type& out_type)
{
    std::istringstream sin{ line };
    char start_str_ch{ 0 };
    sin >> start_str_ch;

    std::string lstr;
    if (start_str_ch == ':') {
        if (!std::getline(sin, lstr, ':').good()) {
            return false;
        }
        lstr = ':' + lstr + ':';
    }
    else if (start_str_ch == '(') {
        if (!std::getline(sin, lstr, ')').good()) {
            return false;
        }
        lstr = '(' + lstr + ')';
    }
    else {
        sin.unget();
        sin >> lstr;
    }

    std::string arrw;
    sin >> arrw;

    UC_edge::Type type;
    if (!arrow_to_type(arrw, type)) {
        return false;
    }
    bool is_ltr{ true };
    if (arrw[0] != '-' && arrw[0] != '.') {
        is_ltr = false;
    }

    start_str_ch = 0;
    sin >> start_str_ch;

    std::string rstr;
    if (start_str_ch == ':') {
        if (!std::getline(sin, rstr, ':').good()) {
            return false;
        }
        rstr = ':' + rstr + ':';
    }
    else if (start_str_ch == '(') {
        if (!std::getline(sin, rstr, ')').good()) {
            return false;
        }
        rstr = '(' + rstr + ')';
    }
    else {
        sin.unget();
        sin >> rstr;
    }

    if (!is_ltr) {
        std::swap(lstr, rstr);
    }

    out_beg_str = lstr;
    out_end_str = rstr;
    out_type = type;

    return true;
}

bool wsd_utils::UC_dia::arrow_to_type(const std::string& arrow,
                                       UC_edge::Type& out_type)
{
    if (arrow.empty()) {
        return false;
    }

    std::string lhead(1, arrow[0]);

}

}
