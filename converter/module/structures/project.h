#ifndef PROJECT_H
#define PROJECT_H

#include <memory>

#include "use_case/uc_ptrs.h"
#include "class/class_ptrs.h"
#include "layout_flow/lw_ptrs.h"

namespace lenv
{

class Project final
{
public:
    Project();

private:
    Use_Case_dia_sp m_use_case_dia;
    Class_dia_sp m_class_dia;

private:
    Layout_Flow_dia_sp m_layout_flow_dia;
};

using Project_sp = std::shared_ptr<Project>;
using Project_wp = std::weak_ptr<Project>;

}

#endif // PROJECT_H
