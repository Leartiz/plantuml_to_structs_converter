#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <memory>
#include <string>
#include <iostream>

#include "project.h"
#include "robustness/rob_ptrs.h"
#include "sequence/seq_ptrs.h"

namespace lenv
{

/*!
    \brief Транслятор

    Переводит файлы проекта в структуры данных.
*/
class Translator
{
public:
    virtual ~Translator();

public:
    virtual Project_sp convert_all(const std::string& project_root_path);
    virtual Use_Case_dia_sp convert_uc_dia(std::istream& in) = 0;
    virtual Robustness_dia_sp convert_rob_dia(std::istream& in) = 0;
    virtual Sequence_dia_sp convert_seq_dia(std::istream& in) = 0;
    virtual Class_dia_sp convert_class_dia(std::istream& in) = 0;
};

}

#endif // TRANSLATOR_H
