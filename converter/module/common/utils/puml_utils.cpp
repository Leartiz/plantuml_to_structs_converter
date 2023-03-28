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
    std::istringstream sin{ line };
    std::string directive; sin >> directive;
    return String_utils::eq_ref(directive, enduml, false);
}

bool Puml_utils::read_short_use_case(const std::string& line,
                                     std::string& out_name)
{

}

bool Puml_utils::read_short_actor(const std::string& line, std::string& out_name)
{

}

}
