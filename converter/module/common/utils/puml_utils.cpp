#include <sstream>

#include "puml_utils.h"

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

bool Puml_utils::read_startuml_directive(const std::string& line,
                                         std::string& out_name)
{
    std::istringstream sin{ line };
    std::string directive; sin >> directive;

    if (directive != startuml)
        return false;

    std::getline(sin, out_name, '\n');
    // TODO: trim
    return true;
}

bool Puml_utils::read_enduml_directive(const std::string& line)
{
    std::istringstream sin{ line };
    std::string directive; sin >> directive;

    return directive == enduml;
}

}
