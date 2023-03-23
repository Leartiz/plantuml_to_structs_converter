#ifndef UC_NODE_H
#define UC_NODE_H

#include <string>
#include <vector>
#include <unordered_map>

#include "uc_ptrs.h" // has forward declaration.
#include "robustness/rob_ptrs.h"
#include "sequence/seq_ptrs.h"

#include "common/ijson.h"
#include "common/ivalid.h"

namespace lenv // learning environment!
{
// namespace use_case

class UC_node final : public IJson, public IValid
{
    friend class lenv::Use_Case_dia; // extra!

public:
    enum Type : uint32_t
    {
        USE_CASE,
        ACTOR
    };

public:
    struct Field final
    {
        static const std::string id;
        static const std::string name;
        static const std::string type;
        static const std::string inn_edges;
        static const std::string out_edges;
        static const std::string rob_dia;
        static const std::string seq_dia;
    };

private:
    struct Impl final /* POD? */
    {
        std::string id; // unique
        std::string name;
        Type type{ USE_CASE };

        /* filled out separately */
        /* replace to: map or unordered_map? */
        std::vector<UC_edge_wp> inn_edges;
        std::vector<UC_edge_wp> out_edges;

        /* always exist */
        Robustness_dia_sp rob_dia;
        Sequence_dia_sp seq_dia;
    };

public:
    class Builder final
    {
    public:
        Builder(UC_node_sp node); // clone based!
        Builder(std::string id) noexcept;
        Builder& name(std::string name);
        Builder& type(const Type type);
        Builder& rob_dia(Robustness_dia_sp rob_dia);
        Builder& seq_dia(Sequence_dia_sp seq_dia);

        UC_node_sp build_ptr() const;
        UC_node build_cpy() const; // do move
    private:
        Impl m_node_impl;
    };

public:
    class Adder final
    {
    public:
        Adder(UC_node_sp node);
        Adder& add_inn_edge(UC_edge_sp edge);
        Adder& add_out_edge(UC_edge_sp edge);
    private:
        UC_node_sp m_node;
    };

public:
    UC_node() = delete;

public:
    const std::string& id() const;
    const std::string& name() const;
    Type type() const;

    std::vector<UC_edge_sp> inn_edges() const;
    std::vector<UC_edge_sp> out_edges() const;

    bool contains_inn_edge(const std::string& id) const;
    bool contains_out_edge(const std::string& id) const;

public:
    bool equal_by_id(const UC_node& rhs) const;
    friend bool operator==(const UC_node& lhs, const UC_node& rhs);
    friend bool operator!=(const UC_node& lhs, const UC_node& rhs);

    // IJson interface
public:
    nlohmann::json to_whole_json() const override; // throw exception
    nlohmann::json to_short_json() const override;

    // IValid interface
public:
    bool is_valid() const override;

private:
    UC_node(Impl impl);
    static UC_edge_sps edge_wps_to_sps(const UC_edge_wps& edges);
    static bool contains_edge(const UC_edge_wps& edges, const std::string& id);
    static nlohmann::json::array_t edges_to_json(const UC_edge_wps& edges);
    static bool is_valid(const UC_edge_wps& edges);

private:
    Impl m_impl;
};

} // lenv

#endif // UC_NODE_H

