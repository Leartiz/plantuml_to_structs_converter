#ifndef UC_PTRS_H
#define UC_PTRS_H

#include <memory>

namespace lenv
{

class UC_node;
class UC_edge;
class Use_Case_dia;

using UC_node_wp = std::weak_ptr<UC_node>;
using UC_edge_wp = std::weak_ptr<UC_edge>;

using UC_node_sp = std::shared_ptr<UC_node>;
using UC_edge_sp = std::shared_ptr<UC_edge>;

using Use_Case_dia_sp = std::shared_ptr<Use_Case_dia>;

}

#endif // UC_PTRS_H
