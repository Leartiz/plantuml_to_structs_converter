#ifndef PUML_UTILS_H
#define PUML_UTILS_H

#include <string>

#include "use_case/uc_node.h"
#include "use_case/uc_edge.h"

namespace lenv
{

/* correctly parse by tokens or just return a DTO! */
class wsd_utils final
{
public:
    static const std::string startuml;
    static const std::string enduml;

    static const std::string kw_startuml;
    static const std::string kw_enduml;

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
    static const std::vector<std::string> arrow_heads;
    static const std::vector<char> arrow_body;

    /* Such TDD */
public:
    static bool is_keyword(const std::string& str);

    static bool read_startuml_directive(const std::string& line, std::string& out_name);
    static bool read_enduml_directive(const std::string& line);

public:
    class UC_dia final
    {
    public:
        static bool read_use_case_creation(const std::string& line,
                                           std::string& out_name,
                                           std::string& out_id,
                                           UC_node::Type& out_type);
        static bool read_actor_creation(const std::string& line,
                                        std::string& out_name,
                                        std::string& out_id,
                                        UC_node::Type& out_type);
    public:
        static bool read_connection_creation(const std::string& line,
                                             std::string& out_beg_str,
                                             std::string& out_end_str,
                                             UC_edge::Type& out_type);
        static bool arrow_to_type(const std::string& arrow,
                                  UC_edge::Type& out_type);
    };

public:
    class Rob_dia final
    {
    };

public:
    // ...
};

}

#endif // PUML_UTILS_H

