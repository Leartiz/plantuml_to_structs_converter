#ifndef ORDERGENERATOR_H
#define ORDERGENERATOR_H

#include <string>
#include <cstdint>

#include "igenerator.h"

namespace lenv
{

class OrderGenerator final : public IGenerator
{
    // IGenerator interface
public:
    std::string next() const;

private:
    mutable uint64_t m_counter{ 0 };
};

}

#endif // ORDERGENERATOR_H
