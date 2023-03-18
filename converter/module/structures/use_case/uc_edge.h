#ifndef UC_EDGE_H
#define UC_EDGE_H

#include <memory>

#include "uc_ptrs.h"

namespace lenv
{

class UC_edge final
{
    friend class Use_Case_dia;

public:
    enum Type
    {
        ASSOCIATION,
        GENERALIZATION,
        INCLUDE,
        EXTEND,
    };

public:
    Type type() const;

private:
    Type m_type{ ASSOCIATION };
    UC_node_wp m_beg;
    UC_node_wp m_end;
};

}

#endif // UC_EDGE_H
