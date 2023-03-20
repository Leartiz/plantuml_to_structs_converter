#include <utility> // std::move

#include "uc_node.h"
#include "uc_edge.h"

#include "nlohmann/json.hpp"

namespace lenv
{

const std::string UC_node::Field::id{ "id" };
const std::string UC_node::Field::name{ "name" };
const std::string UC_node::Field::type{ "type" };
const std::string UC_node::Field::inn_edges{ "inn_edges" };
const std::string UC_node::Field::out_edges{ "out_edges" };

// -----------------------------------------------------------------------

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

// -----------------------------------------------------------------------

nlohmann::json UC_node::to_whole_json() const
{
    nlohmann::json result;
    result[Field::id] = m_id;
    result[Field::name] = m_name;
    result[Field::type] = static_cast<uint32_t>(m_type);
    result[Field::inn_edges] = edges_to_json(m_inn_edges);
    result[Field::out_edges] = edges_to_json(m_out_edges);
    return result;
}

nlohmann::json UC_node::to_short_json() const
{

}

nlohmann::json::array_t UC_node::edges_to_json(const UC_edge_wps& edges)
{
    auto result{ nlohmann::json::array() };
    std::for_each(std::begin(edges), std::end(edges),
                  [&result](const UC_edge_wp edge_wp) -> void {
        if (auto edge_sp = edge_wp.lock(); edge_sp) {
            result.push_back(edge_sp->to_short_json());
            return;
        }

        throw int{10};
    });
    return result;
}

// -----------------------------------------------------------------------

}
