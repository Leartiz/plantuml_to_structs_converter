#ifndef UC_PTRS_H
#define UC_PTRS_H

#include <memory>
#include <vector>

namespace lenv
{

class UC_node;
class UC_edge;
class Use_Case_dia;

using Use_Case_dia_sp = std::shared_ptr<Use_Case_dia>;

// -----------------------------------------------------------------------

using UC_node_wp = std::weak_ptr<UC_node>;
using UC_edge_wp = std::weak_ptr<UC_edge>;

using UC_node_wps = std::vector<UC_node_wp>;
using UC_edge_wps = std::vector<UC_edge_wp>;

// -----------------------------------------------------------------------

using UC_node_sp = std::shared_ptr<UC_node>;
using UC_edge_sp = std::shared_ptr<UC_edge>;

using UC_node_sps = std::vector<UC_node_sp>;
using UC_edge_sps = std::vector<UC_edge_sp>;

}

#endif // UC_PTRS_H
