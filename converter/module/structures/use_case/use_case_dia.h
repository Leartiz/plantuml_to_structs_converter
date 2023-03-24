 #ifndef USE_CASE_DIA_H
#define USE_CASE_DIA_H

#include <map>

#include "uc_ptrs.h"
#include "common/ijson.h"

namespace lenv
{

class Use_Case_dia final : public IJson
{
public:
    static const std::string id;
    struct Field final
    {
        static const std::string id;
        static const std::string nodes;
        static const std::string edges;
    };

public:
    //bool contains_node();

    // TODO: Сделать BUILDER


    // IJson interface
public:
    nlohmann::json to_whole_json() const override;
    nlohmann::json to_short_json() const override;

private:
    nlohmann::json::array_t nodes_to_whole_json() const;
    nlohmann::json::array_t edges_to_whole_json() const;

private:
    /* id and node/edge */
    // Поместить в билдер
    // А вообще хранить в векторе
    std::map<std::string, UC_node_sp> m_nodes;
    std::map<std::string, UC_edge_sp> m_edges;
};

}

#endif // USE_CASE_DIA_H
