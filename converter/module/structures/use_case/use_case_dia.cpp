#include <algorithm> // std::for_each, std::sort, std::find_if

#include "uc_node.h"
#include "uc_edge.h"
#include "use_case_dia.h"

#include "errors/err_text_creator.h"
#include "errors/bldr/not_found_node.h"
#include "errors/bldr/repeating_edge.h"
#include "errors/bldr/repeating_node.h"
#include "errors/bldr/node_after_adder.h"
#include "errors/bldr/null_node.h"
#include "errors/bldr/null_edge.h"

namespace lenv
{

// -----------------------------------------------------------------------

const std::string Use_Case_dia::id{ "use_case_dia" }; // permanent!

const std::string Use_Case_dia::Field::id{ "id" };
const std::string Use_Case_dia::Field::nodes{ "nodes" };
const std::string Use_Case_dia::Field::edges{ "edges" };

// -----------------------------------------------------------------------

Use_Case_dia_sp Use_Case_dia::Builder::build_ptr() const
{
    return std::shared_ptr<Use_Case_dia>{
        std::make_shared<Use_Case_dia>(build_cpy())
    };
}

Use_Case_dia Use_Case_dia::Builder::build_cpy() const
{
    return Use_Case_dia{};
}

// -----------------------------------------------------------------------

Use_Case_dia::Use_Case_dia() noexcept {}

void Use_Case_dia::add_node_bfore_adder(UC_node_sp node)
{
    if (!node) throw Null_node{
        Err_text_creator::dt("Use_Case_dia", "add_node_bfore_adder",
                             "node is null")
    };
    if (node->has_edges()) throw Node_after_adder{
        Err_text_creator::dt("Use_Case_dia", "add_node_bfore_adder",
                             "modified node")
    };

    if (contains_edge_with_id(node->id())) throw Repeating_node{
        Err_text_creator::dt("Use_Case_dia", "add_node_bfore_adder",
                             "node already added")
    };

    m_nodes.push_back(node);
}

void Use_Case_dia::add_edge(UC_edge_sp edge,
                            const std::string& beg_node_id,
                            const std::string& end_node_id)
{
    if (!edge) throw Null_edge{
        Err_text_creator::dt("Use_Case_dia", "add_edge", "edge is null")
    };
    if (contains_edge_with_id(edge->id())) throw Repeating_edge {
        Err_text_creator::dt("Use_Case_dia", "add_edge",
                             "edge already added")
    };

    if (!contains_node_with_id(beg_node_id)) throw Not_found_node{
        Err_text_creator::dt("Use_Case_dia", "add_edge",
                             "begin node not found")
    };
    if (!contains_node_with_id(end_node_id)) throw Not_found_node{
        Err_text_creator::dt("Use_Case_dia", "add_edge",
                             "end node not found")
    };

    {
        UC_node::Adder beg_node_adr{ node_by_id(beg_node_id) };
        beg_node_adr.add_out_edge(edge);
    }
    {
        UC_node::Adder end_node_adr{ node_by_id(end_node_id) };
        end_node_adr.add_inn_edge(edge);
    }

    m_edges.push_back(edge);
}

// -----------------------------------------------------------------------

const UC_node_sps& Use_Case_dia::nodes() const
{
    return m_nodes;
}

const UC_edge_sps& Use_Case_dia::edges() const
{
    return m_edges;
}

UC_node_sps Use_Case_dia::sorted_nodes() const
{
    UC_node_sps nodes_copy{ m_nodes };
    std::sort(std::begin(nodes_copy), std::end(nodes_copy),
              [](UC_node_sp lhs, UC_node_sp rhs) -> bool {
        return lhs->id() < rhs->id();
    });
    return nodes_copy;
}

UC_edge_sps Use_Case_dia::sorted_edges() const
{
    UC_edge_sps edges_copy{ m_edges };
    std::sort(std::begin(edges_copy), std::end(edges_copy),
              [](UC_edge_sp lhs, UC_edge_sp rhs) -> bool {
        return lhs->id() < rhs->id();
    });
    return edges_copy;
}

// -----------------------------------------------------------------------

bool Use_Case_dia::contains_node_with_id(const std::string& id) const
{
    return node_by_id(id) != nullptr;
}

bool Use_Case_dia::contains_edge_with_id(const std::string& id) const
{
    return edge_by_id(id) != nullptr;
}

bool Use_Case_dia::contains_node_with_name(const std::string& name) const
{
    return node_by_name(name) != nullptr;
}

bool Use_Case_dia::contains_node_with_type(const UC_node::Type type) const
{
    return node_by_type(type) != nullptr;
}

bool Use_Case_dia::contains_edge_with_type(const UC_edge::Type type) const
{
    return edge_by_type(type) != nullptr;
}

// -----------------------------------------------------------------------

UC_node_sp Use_Case_dia::node_by_id(const std::string& id) const
{
    auto found_it = std::find_if(std::begin(m_nodes), std::end(m_nodes),
                 [&id](const UC_node_sp node_sp) -> bool {
        return node_sp->id() == id;
    });

    if (found_it != m_nodes.end())
        return *found_it;
    return nullptr;
}

UC_edge_sp Use_Case_dia::edge_by_id(const std::string& id) const
{
    auto found_it = std::find_if(std::begin(m_edges), std::end(m_edges),
                 [&id](const UC_edge_sp edge_sp) -> bool {
        return edge_sp->id() == id;
    });

    if (found_it != m_edges.end())
        return *found_it;
    return nullptr;
}

UC_node_sp Use_Case_dia::node_by_name(const std::string& name) const
{
    auto found_it = std::find_if(std::begin(m_nodes), std::end(m_nodes),
                 [&name](const UC_node_sp node_sp) -> bool {
        return node_sp->name() == name;
    });

    if (found_it != m_nodes.end())
        return *found_it;
    return nullptr;
}

UC_node_sp Use_Case_dia::node_by_type(const UC_node::Type type) const
{
    auto found_it = std::find_if(std::begin(m_nodes), std::end(m_nodes),
                 [&type](const UC_node_sp node_sp) -> bool {
        return node_sp->type() == type;
    });

    if (found_it != m_nodes.end())
        return *found_it;
    return nullptr;
}

UC_edge_sp Use_Case_dia::edge_by_type(const UC_edge::Type type) const
{
    auto found_it = std::find_if(std::begin(m_edges), std::end(m_edges),
                 [&type](const UC_edge_sp edge_sp) -> bool {
        return edge_sp->type() == type;
    });

    if (found_it != m_edges.end())
        return *found_it;
    return nullptr;
}

// -----------------------------------------------------------------------

nlohmann::json Use_Case_dia::to_whole_json() const
{
    nlohmann::json result;
    result[Field::id] = Use_Case_dia::id;
    result[Field::nodes] = nodes_to_whole_json(); // move or copy?
    result[Field::edges] = edges_to_whole_json();
    return result;
}

nlohmann::json Use_Case_dia::to_short_json() const
{
    nlohmann::json result;
    result[Field::id] = Use_Case_dia::id;

    const UC_node_sps nodes_copy{ sorted_nodes() };
    const UC_edge_sps edges_copy{ sorted_edges() };

    {
        nlohmann::json::array_t nodes;
        std::for_each(std::begin(nodes_copy), std::end(nodes_copy),
                      [&nodes](const UC_node_sp p) -> void {
            nodes.push_back(p->to_short_json());
        });
        result[Field::nodes] = std::move(nodes); // exactly move!
    }
    {
        nlohmann::json::array_t edges;
        std::for_each(std::begin(edges_copy), std::end(edges_copy),
                      [&edges](const UC_edge_sp p) {
            edges.push_back(p->to_short_json());
        });
        result[Field::edges] = std::move(edges);
    }

    return result;
}

nlohmann::json::array_t Use_Case_dia::nodes_to_whole_json() const
{
    nlohmann::json::array_t res;
    const UC_node_sps nodes_copy{ sorted_nodes() };
    std::for_each(std::begin(nodes_copy), std::end(nodes_copy),
                  [&res](const UC_node_sp node_sp) -> void {
        res.push_back(node_sp->to_whole_json());
    });
    return res;
}

nlohmann::json::array_t Use_Case_dia::edges_to_whole_json() const
{
    nlohmann::json::array_t res;
    const UC_edge_sps edges_copy{ sorted_edges() };
    std::for_each(std::begin(edges_copy), std::end(edges_copy),
                  [&res](const UC_edge_sp edge_sp) -> void {
        res.push_back(edge_sp->to_whole_json());
    });
    return res;
}

// -----------------------------------------------------------------------

bool Use_Case_dia::is_valid() const
{

}

}
