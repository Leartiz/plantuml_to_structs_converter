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

public:
    static const std::string start_mcomment;
    static const std::string start_ocomment;

public:
    static const std::string kw_actor;
    static const std::string kw_usecase;
    static const std::string kw_as;

public:
    static const std::string kw_package;
    static const std::string kw_rectangle;

public:
    static const std::string kw_left;
    static const std::string kw_right;
    static const std::string kw_top;
    static const std::string kw_bottom;

public:
    static const std::string kw_to;
    static const std::string kw_direction;

public:
    static const std::string kw_skinparam;
    static const std::string kw_note;
    static const std::string kw_of;
    static const std::string kw_end;


public:
    static bool is_keyword(const std::string& str);

    static bool read_startuml_directive(const std::string& line, std::string& out_name);
    static bool read_enduml_directive(const std::string& line);

    static bool read_short_use_case(const std::string& line, std::string& out_name);
    static bool read_short_actor(const std::string& line, std::string& out_name);
};

}

#endif // PUML_UTILS_H

