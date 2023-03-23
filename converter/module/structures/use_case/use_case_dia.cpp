#include <algorithm>

#include "uc_node.h"
#include "uc_edge.h"
#include "use_case_dia.h"

namespace lenv
{

const std::string Use_Case_dia::id{ "use_case_dia" };

const std::string Use_Case_dia::Field::id{ "id" };
const std::string Use_Case_dia::Field::nodes{ "nodes" };
const std::string Use_Case_dia::Field::edges{ "edges" };

// -----------------------------------------------------------------------

nlohmann::json Use_Case_dia::to_whole_json() const
{
    nlohmann::json result;
    result[Field::id] = Use_Case_dia::id;
    result[Field::nodes] = nodes_to_whole_json();
    result[Field::edges] = edges_to_whole_json();
    return result;
}

nlohmann::json Use_Case_dia::to_short_json() const
{
    nlohmann::json result;
    result[Field::id] = Use_Case_dia::id;

    {
        nlohmann::json::array_t nodes;
        std::for_each(std::begin(m_nodes), std::end(m_nodes),
                      [&nodes](const std::pair<std::string, UC_node_sp>& p) {
            nodes.push_back(p.second->to_short_json());
        });
        result[Field::nodes] = nodes;
    }

    /* repeated code? */
    {
        nlohmann::json::array_t edges;
        std::for_each(std::begin(m_edges), std::end(m_edges),
                      [&edges](const std::pair<std::string, UC_edge_sp>& p) {
            edges.push_back(p.second->to_short_json());
        });
        result[Field::edges] = edges;
    }

    return result;
}

nlohmann::json::array_t Use_Case_dia::nodes_to_whole_json() const
{
    nlohmann::json::array_t res;
    std::for_each(std::begin(m_nodes), std::end(m_nodes),
                  [&res](const std::pair<std::string, UC_node_sp>& p) {
        res.push_back(p.second->to_whole_json());
    });
    return res;
}

/* repeated code? can be done as in edge! */
nlohmann::json::array_t Use_Case_dia::edges_to_whole_json() const
{
    nlohmann::json::array_t res;
    std::for_each(std::begin(m_edges), std::end(m_edges),
                  [&res](const std::pair<std::string, UC_edge_sp>& p) {
        res.push_back(p.second->to_whole_json());
    });
    return res;
}

// -----------------------------------------------------------------------

}
