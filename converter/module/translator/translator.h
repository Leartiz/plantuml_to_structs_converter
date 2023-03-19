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
class Translator final
{
public:
    Translator();

public:
    void convert_all(const std::string& project_root_path);
    Use_Case_dia_sp convert_uc_dia(std::istream& in);
    Robustness_dia_sp convert_rob_dia(std::istream& in);
    Sequence_dia_sp convert_seq_dia(std::istream& in);

    bool last_error();

private:
    std::shared_ptr<Project> m_proj;
};

}

#endif // TRANSLATOR_H
