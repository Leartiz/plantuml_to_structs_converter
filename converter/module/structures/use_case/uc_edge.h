#ifndef UC_EDGE_H
#define UC_EDGE_H

#include <memory>

#include "uc_ptrs.h"

#include "common/ijson.h"

namespace lenv
{

class UC_edge final : public IJson
{
    friend class Use_Case_dia;

    struct Inner final
    {

    };

public:
    /* metadata */
    struct Field final
    {
        static const std::string id;
        static const std::string type;
        static const std::string beg;
        static const std::string end;
    };

public:
    enum Type : uint32_t
    {
        ASSOCIATION,
        GENERALIZATION,
        INCLUDE,
        EXTEND,
    };

public:
    class Builder final
    {
    public:

    private:
        Inner m_inn;
    };

public:
    UC_edge(const std::string& id, const Type type) noexcept;
    UC_edge(const std::string& id, const Type type,
            UC_node_sp beg, UC_node_sp end);
    Type type() const;

    // IJson interface
public:
    nlohmann::json to_whole_json() const;
    nlohmann::json to_short_json() const;

private:
    std::string m_id; // surrogate
    Type m_type{ ASSOCIATION };
    UC_node_wp m_beg;
    UC_node_wp m_end;
};

}

#endif // UC_EDGE_H
