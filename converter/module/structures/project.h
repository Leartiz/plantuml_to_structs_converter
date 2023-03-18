#ifndef PROJECT_H
#define PROJECT_H

#include "structures/use_case/uc_ptrs.h"

namespace lenv
{

class Project final
{
public:
    Project();

private:
    Use_Case_dia_sp m_use_case_dia;
};

}

#endif // PROJECT_H
