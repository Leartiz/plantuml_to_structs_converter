#ifndef PUML_UTILS_H
#define PUML_UTILS_H

#include <string>

namespace lenv
{

class Puml_utils final
{
public:
    static const std::string startuml;
    static const std::string enduml;

    static const std::string start_mcomment;
    static const std::string start_ocomment;

public:
    static const std::string kw_usecase;
    static const std::string kw_actor;

public:

    static bool read_startuml_directive(const std::string& line, std::string& out_name);
    static bool read_enduml_directive(const std::string& line);
};

}

#endif // PUML_UTILS_H
