#ifndef PROJECT_H
#define PROJECT_H

#include "use_case/uc_ptrs.h"
#include "class/class_ptrs.h"

namespace lenv
{

class Project final
{
public:
    Project();

private:
    Use_Case_dia_sp m_use_case_dia;
    Class_dia_sp m_class_dia;
};

}

#endif // PROJECT_H
