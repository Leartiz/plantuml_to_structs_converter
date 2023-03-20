#ifndef UC_NODE_H
#define UC_NODE_H

#include <string>
#include <vector>

#include "uc_ptrs.h" // has forward declaration.
#include "robustness/rob_ptrs.h"
#include "sequence/seq_ptrs.h"

#include "common/ijson.h"

namespace lenv // learning environment!
{
// namespace use_case

class UC_node final : public IJson
{
    friend class Use_Case_dia;

public:
    struct Field final
    {
        static const std::string id;
        static const std::string name;
        static const std::string type;
        static const std::string inn_edges;
        static const std::string out_edges;
    };

public:
    enum Type : uint32_t
    {
        USE_CASE,
        ACTOR
    };

public:
    UC_node(const Type type, const std::string& name);
    explicit UC_node(const std::string& name);

public:
    Type type() const;
    const std::string& name() const;

    const std::vector<UC_edge_wp>& inn_edges() const;
    const std::vector<UC_edge_wp>& out_edges() const;

public:
    bool equal_by_name(const UC_node& rhs) const;
    friend bool operator==(const UC_node& lhs, const UC_node& rhs);
    friend bool operator!=(const UC_node& lhs, const UC_node& rhs);

    // IJson interface
public:
    nlohmann::json to_whole_json() const; // throw exception
    nlohmann::json to_short_json() const;

private:
    static nlohmann::json::array_t edges_to_json(const UC_edge_wps& edges);

private:
    std::string m_id;
    std::string m_name; // unique
    Type m_type{ USE_CASE };

    std::vector<UC_edge_wp> m_inn_edges;
    std::vector<UC_edge_wp> m_out_edges;

private:
    Robustness_dia_sp m_rob_dia;
    Sequence_dia_sp m_seq_dia;
};

} // lenv

#endif // UC_NODE_H

