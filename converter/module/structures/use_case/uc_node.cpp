#include <utility> // std::move
#include <algorithm> // std::for_each, std::transform
#include <iostream>

#include "uc_node.h"
#include "uc_edge.h"

#include "nlohmann/json.hpp"

#include "errors/bldr/null_edge.h"
#include "errors/bldr/null_node.h"
#include "errors/bldr/unsuitable_edge.h"
#include "errors/bldr/repeating_edge.h"
#include "errors/err_text_creator.h"

namespace lenv
{

// -----------------------------------------------------------------------

const std::string UC_node::Field::id{ "id" };
const std::string UC_node::Field::name{ "name" };
const std::string UC_node::Field::type{ "type" };
const std::string UC_node::Field::inn_edges{ "inn_edges" };
const std::string UC_node::Field::out_edges{ "out_edges" };
const std::string UC_node::Field::rob_dia{ "rob_dia" };
const std::string UC_node::Field::seq_dia{ "seq_dia" };

// -----------------------------------------------------------------------

UC_node::Builder::Builder(UC_node_sp node, std::string id)
{
    if (!node) throw Null_node{
        Err_text_creator::dt("UC_node::Builder", "Builder",
                             "node to clone is null")
    };
    m_node_impl = node->m_impl;
    m_node_impl.id = std::move(id);
}

UC_node::Builder::Builder(std::string id) noexcept
{
    m_node_impl.id = std::move(id);
}

UC_node::Builder& UC_node::Builder::name(std::string name)
{
    m_node_impl.name = std::move(name);
    return *this;
}

UC_node::Builder& UC_node::Builder::type(const Type type)
{
    m_node_impl.type = type;
    return *this;
}

UC_node::Builder& UC_node::Builder::rob_dia(Robustness_dia_sp rob_dia)
{
    m_node_impl.rob_dia = rob_dia;
    return *this;
}

UC_node::Builder& UC_node::Builder::seq_dia(Sequence_dia_sp seq_dia)
{
    m_node_impl.seq_dia = seq_dia;
    return *this;
}

UC_node_sp UC_node::Builder::build_ptr() const
{
    return UC_node_sp{ std::make_shared<UC_node>(build_cpy()) };
}

UC_node UC_node::Builder::build_cpy() const
{
    return UC_node{ m_node_impl };
}

// -----------------------------------------------------------------------

UC_node::Adder::Adder(UC_node_sp node)
{
    if (!node) throw Null_node{
        Err_text_creator::dt("UC_node::Adder", "Adder",
                             "node to update is null")
    };
    m_node = node;
}

// --> node
UC_node::Adder& UC_node::Adder::add_inn_edge(UC_edge_sp edge)
{
    if (!edge) throw Null_edge{
        Err_text_creator::dt("UC_node::Adder", "add_inn_edge",
                             "edge is null")
    };
    if (!edge->end() || !edge->beg()) throw Null_node{
        Err_text_creator::dt("UC_node::Adder", "add_inn_edge",
                             "node is null")
    };

    if (edge->end()->id() != m_node->id()) throw Unsuitable_edge{
        Err_text_creator::dt("UC_node::Adder", "add_inn_edge",
                             "edge does not enter node")
    };
    if (m_node->contains_inn_edge(edge->id())) throw Repeating_edge{
        Err_text_creator::dt("UC_node::Adder", "add_inn_edge",
                             "edge already added")
    };

    /* ignore case when: edge->beg()->id() == edge->end()->id() */

    m_node->m_impl.inn_edges.push_back(edge);
    return *this;
}

// node -->
UC_node::Adder& UC_node::Adder::add_out_edge(UC_edge_sp edge)
{
    if (!edge) throw Null_edge{
        Err_text_creator::dt("UC_node::Adder", "add_out_edge",
                             "edge is null")
    };
    if (!edge->beg() || !edge->end()) throw Null_node{
        Err_text_creator::dt("UC_node::Adder", "add_out_edge",
                             "node is null")
    };

    if (edge->beg()->id() != m_node->id()) throw Unsuitable_edge{
        Err_text_creator::dt("UC_node::Adder", "add_out_edge",
                             "edge does not exit node")
    };
    if (m_node->contains_out_edge(edge->id())) throw Repeating_edge{
        Err_text_creator::dt("UC_node::Adder", "add_out_edge",
                             "edge already added")
    };

    m_node->m_impl.out_edges.push_back(edge);
    return *this;
}

// -----------------------------------------------------------------------

UC_node::UC_node(Impl impl) noexcept
    : m_impl{ std::move(impl) } {}

// -----------------------------------------------------------------------

UC_node::Type UC_node::type() const
{
    return m_impl.type;
}

const std::string& UC_node::id() const
{
    return m_impl.id;
}

const std::string& UC_node::name() const
{
    return m_impl.name;
};

std::vector<UC_edge_sp> UC_node::inn_edges() const
{    
    return edge_wps_to_sps(m_impl.inn_edges);
}

std::vector<UC_edge_sp> UC_node::out_edges() const
{
    return edge_wps_to_sps(m_impl.out_edges);
}

UC_edge_sps UC_node::edge_wps_to_sps(const UC_edge_wps& edges)
{
    std::vector<UC_edge_sp> result;
    std::transform(std::begin(edges), std::end(edges), std::back_inserter(result),
                   [](const UC_edge_wp edge_wp) -> UC_edge_sp {
        if (edge_wp.expired()) throw Null_edge{
            Err_text_creator::dt("UC_node", "edge_wps_to_sps",
                                 "inn/out edge expired")
        };

        return edge_wp.lock();
    });
    return result;
}

bool UC_node::contains_inn_edge(const std::string& id) const
{
    return contains_edge(m_impl.inn_edges, id);
}

bool UC_node::contains_out_edge(const std::string& id) const
{
    return contains_edge(m_impl.out_edges, id);
}

bool UC_node::contains_edge(const UC_edge_wps& edges, const std::string& id)
{
    for (auto it = edges.begin(); it != edges.end(); ++it) {
        if (it->expired()) throw Null_edge{
            Err_text_creator::dt("UC_node", "contains_inn_edge",
                                 "inn/out edge expired")
        };

        const UC_edge_sp edge{ it->lock() };
        if (edge->id() == id) return true;
    }
    return false;
}

bool UC_node::has_edges() const
{
    return !m_impl.inn_edges.empty() || !m_impl.out_edges.empty();
}

// -----------------------------------------------------------------------

bool UC_node::equal_by_id(const UC_node& rhs) const
{
    return m_impl.id == rhs.id();
}

bool operator==(const UC_node& lhs, const UC_node& rhs)
{
    return lhs.equal_by_id(rhs);
}

bool operator!=(const UC_node& lhs, const UC_node& rhs)
{
    return !(lhs == rhs);
}

// -----------------------------------------------------------------------

nlohmann::json UC_node::to_whole_json() const
{
    nlohmann::json result;
    result[Field::id] = m_impl.id;
    result[Field::name] = m_impl.name;
    result[Field::type] = static_cast<uint32_t>(m_impl.type);
    result[Field::inn_edges] = edges_to_json(m_impl.inn_edges);
    result[Field::out_edges] = edges_to_json(m_impl.out_edges);

    /* equivalent to precedent id */
    result[Field::rob_dia] = m_impl.id;
    result[Field::seq_dia] = m_impl.id;

    /* it's ok for now */
    if (!m_impl.rob_dia) result[Field::rob_dia] = nullptr;
    if (!m_impl.seq_dia) result[Field::seq_dia] = nullptr;

    return result;
}

nlohmann::json UC_node::to_short_json() const
{
    return { { Field::id, m_impl.id } };
}

nlohmann::json::array_t UC_node::edges_to_json(const UC_edge_wps& edges)
{
    auto result = nlohmann::json::array();
    std::for_each(std::begin(edges), std::end(edges),
                  [&result](const UC_edge_wp edge_wp) -> void {       
        if (const auto edge_sp = edge_wp.lock(); edge_sp) {
            result.push_back(edge_sp->to_short_json());
            return;
        }

        throw Null_edge{ Err_text_creator::dt("UC_node", "edges_to_json",
                                              "inn/out edge expired") };
    });
    return result;
}

// -----------------------------------------------------------------------

bool UC_node::is_valid() const
{
    return is_valid(m_impl.inn_edges) && is_valid(m_impl.out_edges);
}

bool UC_node::is_valid(const UC_edge_wps& edges)
{
    for (auto it = std::begin(edges); it != std::end(edges); ++it)
        if (it->expired()) return false;
    return true;
}

}
