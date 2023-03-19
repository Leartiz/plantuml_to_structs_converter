#ifndef CLASS_PTRS_H
#define CLASS_PTRS_H

#include <memory>

namespace lenv
{

class Class_dia;

using Class_dia_sp = std::shared_ptr<Class_dia>;

}

#endif // CLASS_PTRS_H
