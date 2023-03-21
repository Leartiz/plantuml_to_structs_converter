#include "uc_edge.h"
#include "uc_node.h"

namespace lenv
{

const std::string UC_edge::Field::id{ "id" };
const std::string UC_edge::Field::type{ "type" };
const std::string UC_edge::Field::beg{ "beg" };
const std::string UC_edge::Field::end{ "end" };

// -----------------------------------------------------------------------

UC_edge::UC_edge(const std::string& id, const Type type) noexcept
    : m_id{ id }, m_type{ type }
{}

UC_edge::UC_edge(const std::string& id, const Type type,
                 UC_node_sp beg, UC_node_sp end)
    : m_id{ id }, m_type{ type }
    , m_beg{ beg }, m_end{ end }
{}

UC_edge::Type UC_edge::type() const
{
    return m_type;
}

// -----------------------------------------------------------------------

nlohmann::json UC_edge::to_whole_json() const
{
    nlohmann::json result;
    result[Field::id] = m_id;
    result[Field::type] = static_cast<uint32_t>(m_type);

    const auto beg_node_sp{ m_beg.lock() };
    const auto end_node_sp{ m_beg.lock() };

    if (!beg_node_sp || !end_node_sp) {
        throw int{10};
    }

    result[Field::beg] = beg_node_sp->to_short_json();
    result[Field::end] = end_node_sp->to_short_json();

    return result;
}

nlohmann::json UC_edge::to_short_json() const
{
    return { { Field::id, m_id } };
}

}
