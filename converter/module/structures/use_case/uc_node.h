#ifndef UC_NODE_H
#define UC_NODE_H

#include <string>
#include <vector>
#include <memory>
#include <utility>

#include "uc_ptrs.h" // has forward declaration.
#include "robustness/rob_ptrs.h"
#include "sequence/seq_ptrs.h"

namespace lenv // learning environment!
{
// namespace use_case

class UC_node final
{
    friend class Use_Case_dia;

public:
    enum Type
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

private:
    Type m_type{ USE_CASE };
    std::string m_name; // unique

    std::vector<UC_edge_wp> m_inn_edges;
    std::vector<UC_edge_wp> m_out_edges;

private:
    Robustness_dia_sp m_rob_dia;
    Sequence_dia_sp m_seq_dia;
};

} // lenv

#endif // UC_NODE_H

