#ifndef IJSON_H
#define IJSON_H

#include "nlohmann/json.hpp"

namespace lenv
{

class IJson
{
public:
    virtual ~IJson() {}

    virtual nlohmann::json to_whole_json() const = 0;
    virtual nlohmann::json to_short_json() const = 0;
};

}

#endif // IJSON_H
