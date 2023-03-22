 #ifndef USE_CASE_DIA_H
#define USE_CASE_DIA_H

#include <vector>

#include "uc_ptrs.h"

namespace lenv
{

class Use_Case_dia final
{
public:
    Use_Case_dia();

public:
    bool contains_node();

private:
    std::vector<UC_node_sp> m_nodes; // unique
    std::vector<UC_edge_sp> m_edges; // mb rpt
};

}

#endif // USE_CASE_DIA_H
