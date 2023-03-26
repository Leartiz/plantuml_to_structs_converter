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
