 #ifndef USE_CASE_DIA_H
#define USE_CASE_DIA_H

#include <map>
#include <vector>
#include <functional>

#include "uc_ptrs.h"
#include "uc_node.h"
#include "uc_edge.h"

#include "common/ijson.h"
#include "common/ivalid.h"

namespace lenv
{

class Use_Case_dia final : public IJson, public IValid
{
public:
    static const std::string id;
    struct Field final
    {
        static const std::string id;
        static const std::string nodes;
        static const std::string edges;
    };

private:
    struct Impl final
    {
        /* id and node/edge */
        std::map<std::string, UC_node_sp> nodes;
        std::map<std::string, UC_edge_sp> edges;
    };

public:
    class Builder final
    {
    public:
        Use_Case_dia_sp build_ptr() const;
        Use_Case_dia build_cpy() const;

    private:
        Impl m_impl;
    };

public:
    Use_Case_dia() noexcept;
    void add_node_bfore_adder(UC_node_sp node); // out/inn edge lists empty!
    void add_edge(UC_edge_sp edge,
                  const std::string& beg_node_id,
                  const std::string& end_node_id);
    void reset_all();

public:
    const UC_node_sps& nodes() const;
    const UC_edge_sps& edges() const;
    UC_node_sps sorted_nodes() const;
    UC_edge_sps sorted_edges() const;

public:
    bool contains_node_with_id(const std::string& id) const;
    bool contains_edge_with_id(const std::string& id) const;
    bool contains_node_with_name(const std::string& name) const;
    bool contains_node_with_type(const UC_node::Type type) const;
    bool contains_edge_with_type(const UC_edge::Type type) const;

public:
    UC_node_sp node_by_id(const std::string& id) const;
    UC_edge_sp edge_by_id(const std::string& id) const;
    UC_node_sp node_by_name(const std::string& name) const;
    UC_node_sp node_by_type(const UC_node::Type type) const;
    UC_edge_sp edge_by_type(const UC_edge::Type type) const;

    // IJson interface
public:
    nlohmann::json to_whole_json() const override;
    nlohmann::json to_short_json() const override;

    // IValid interface
public:
    bool is_valid() const override;

private:
    nlohmann::json::array_t nodes_to_whole_json() const;
    nlohmann::json::array_t edges_to_whole_json() const;

private:
    UC_node_sps m_nodes;
    UC_edge_sps m_edges;
};

}

#endif // USE_CASE_DIA_H

