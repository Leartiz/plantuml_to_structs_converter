#include <utility> // std::move

#include "uc_edge.h"
#include "uc_node.h"

#include "errors/null_node.h"
#include "errors/invalid_edge.h"
#include "errors/err_text_creator.h"

namespace lenv
{

// -----------------------------------------------------------------------

const std::string UC_edge::Field::id{ "id" };
const std::string UC_edge::Field::type{ "type" };
const std::string UC_edge::Field::beg{ "beg" };
const std::string UC_edge::Field::end{ "end" };

// -----------------------------------------------------------------------

UC_edge::Builder::Builder(std::string id) noexcept
{
    m_edge_impl.id = std::move(id);
}

UC_edge::Builder& UC_edge::Builder::type(const Type type)
{
    m_edge_impl.type = type;
    return *this;
}

UC_edge::Builder& UC_edge::Builder::beg(UC_node_sp beg)
{
    if (!beg) throw Null_node{
        Err_text_creator::dt("UC_edge::Builder", "beg",
                             "beg is null")
    };

    /*
    can be immediately placed in a node,
    but edge has not yet been created.
    */

    m_edge_impl.beg = beg;
    return *this;
}

UC_edge::Builder& UC_edge::Builder::end(UC_node_sp end)
{
    if (!end) throw Null_node{
        Err_text_creator::dt("UC_edge::Builder", "beg",
                             "end is null")
    };

    m_edge_impl.end = end;
    return *this;
}

/* next to be placed in inn_edges/out_edges */
UC_edge_sp UC_edge::Builder::build_ptr() const
{
    return UC_edge_sp{ std::make_shared<UC_edge>(build_cpy()) };
}

UC_edge UC_edge::Builder::build_cpy() const
{
    const UC_edge edge{ m_edge_impl.id, m_edge_impl.type,
                m_edge_impl.beg.lock(),
                m_edge_impl.end.lock() };
    if (!edge.is_valid()) throw Invalid_edge{
        Err_text_creator::dt("UC_edge::Builder", "build_cpy",
                             "beg and end not specified")
    };
    return edge;
}

// -----------------------------------------------------------------------

UC_edge::UC_edge(std::string id, const Type type,
                 UC_node_sp beg, UC_node_sp end) noexcept
    : m_impl{ .id = std::move(id), .type = type,
              .beg = beg, .end = end } {}

const std::string& UC_edge::id() const
{
    return m_impl.id;
}

UC_edge::Type UC_edge::type() const
{
    return m_impl.type;
}

UC_node_sp UC_edge::beg() const
{
    /* without check? */
    if (m_impl.beg.expired()) throw Null_node{
        Err_text_creator::dt("UC_edge", "beg",
                             "beg expired")
    };
    return m_impl.beg.lock();
}

UC_node_sp UC_edge::end() const
{
    if (m_impl.end.expired()) throw Null_node{
        Err_text_creator::dt("UC_edge", "beg",
                             "end expired")
    };
    return m_impl.end.lock();
}

// -----------------------------------------------------------------------

nlohmann::json UC_edge::to_whole_json() const
{
    if (m_impl.beg.expired() || m_impl.end.expired()) {
        /* lvalue, like waiting somewhere */
        throw Null_node{ Err_text_creator::dt("UC_edge", "to_whole_json",
                                              "beg or end expired") };
    }

    nlohmann::json result;
    result[Field::id] = m_impl.id;
    result[Field::type] = static_cast<uint32_t>(m_impl.type);
    result[Field::beg] = m_impl.beg.lock()->to_short_json();
    result[Field::end] = m_impl.end.lock()->to_short_json();

    return result;
}

nlohmann::json UC_edge::to_short_json() const
{
    return { { Field::id, m_impl.id } };
}

// -----------------------------------------------------------------------

bool UC_edge::is_valid() const
{
    return !m_impl.beg.expired() && !m_impl.end.expired();
}

}
