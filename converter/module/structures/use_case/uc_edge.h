#ifndef UC_EDGE_H
#define UC_EDGE_H

#include <memory>

#include "uc_ptrs.h"

#include "common/ijson.h"
#include "common/ivalid.h"

namespace lenv
{

class UC_edge final : public IJson, public IValid
{
    friend class lenv::Use_Case_dia; // extra!

public:
    enum Type : uint32_t
    {
        ASSOCIATION,
        GENERALIZATION,
        INCLUDE,
        EXTEND,
    };

public:
    /* metadata */
    struct Field final
    {
        static const std::string id;
        static const std::string type;
        static const std::string beg;
        static const std::string end;
    };

private:
    struct Impl final
    {
        std::string id; // surrogate
        Type type{ ASSOCIATION };
        UC_node_wp beg;
        UC_node_wp end;
    };

public:
    /* don't need a factory */
    class Builder final
    {
    public:
        Builder(std::string id) noexcept;
        Builder& type(const Type type);
        Builder& beg(UC_node_sp beg);
        Builder& end(UC_node_sp end);

        UC_edge_sp build_ptr() const;
        UC_edge build_cpy() const; // do move
    private:
        Impl m_edge_impl;
    };

public:
    UC_edge(std::string id, const Type type,
            UC_node_sp beg, UC_node_sp end) noexcept;

    /* used for analysis */
    const std::string& id() const;
    Type type() const;
    UC_node_sp beg() const;
    UC_node_sp end() const;

    // IJson interface
public:
    nlohmann::json to_whole_json() const override;
    nlohmann::json to_short_json() const override;

    // IValid interface
public:
    bool is_valid() const override;

private:
    Impl m_impl;
};

}

#endif // UC_EDGE_H
