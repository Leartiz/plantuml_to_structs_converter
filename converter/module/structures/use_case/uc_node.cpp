#include "uc_node.h"
//#include "uc_edge.h"

namespace lenv
{

UC_node::UC_node(const Type type, const std::string& name)
    : m_type{ type }, m_name{ name }
{}

UC_node::UC_node(const std::string& name)
    : m_name{ name }
{}

// -----------------------------------------------------------------------

UC_node::Type UC_node::type() const
{
    return m_type;
}

const std::string& UC_node::name() const
{
    return m_name;
};

const std::vector<UC_edge_wp>& UC_node::inn_edges() const
{    
    return m_inn_edges;
}

const std::vector<UC_edge_wp>& UC_node::out_edges() const
{
    return m_out_edges;
}

// -----------------------------------------------------------------------

bool UC_node::equal_by_name(const UC_node& rhs) const
{
    return m_name == rhs.m_name;
}

bool operator==(const UC_node& lhs, const UC_node& rhs)
{
    return lhs.equal_by_name(rhs);
}

bool operator!=(const UC_node& lhs, const UC_node& rhs)
{
    return !(lhs == rhs);
}

}
