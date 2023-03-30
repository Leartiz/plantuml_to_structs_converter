#ifndef IGENERATOR_H
#define IGENERATOR_H

#include <string>

namespace lenv
{

class IGenerator
{
public:
    virtual ~IGenerator();
    virtual std::string next() const = 0;
};

}

#endif // IGENERATOR_H
