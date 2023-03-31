#ifndef ORDER_GENERATOR_H
#define ORDER_GENERATOR_H

#include <string>
#include <cstdint>

#include "igenerator.h"

namespace lenv
{

class Order_generator final : public IGenerator
{
    // IGenerator interface
public:
    std::string next() const;

private:
    mutable uint64_t m_counter{ 0 };
};

}

#endif // ORDER_GENERATOR_H
