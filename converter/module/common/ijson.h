#ifndef IJSON_H
#define IJSON_H

#include "dependency/json.hpp"

namespace lenv
{

class IJson
{
public:
    virtual ~IJson() {}

    virtual nlohmann::json to_json() const = 0;
};

}

#endif // IJSON_H
