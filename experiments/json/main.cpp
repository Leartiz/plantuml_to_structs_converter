#include <string>
#include <iostream>
#include <algorithm>

#include "nlohmann/json.hpp"

int main()
{
    const std::string field_name{ "id" };
    nlohmann::json::array_t json_arr; /* across */

    // sort.
    {
        for (int i = 0; i < 10; ++i) {
            nlohmann::json one = {{ field_name, std::rand() % 10 }};
            json_arr.push_back(one);
        }
        std::cout << "original: " << json_arr << std::endl;

        std::sort(std::begin(json_arr), std::end(json_arr), [&](nlohmann::json lhs, nlohmann::json rhs){
            return lhs[field_name] < rhs[field_name];
        });
        std::cout << "sorted: " << json_arr << std::endl;
    }

    std::cout << "\n---\n" << std::endl;

    // move json arr.
    {
        std::cout << "json_arr.size: " << json_arr.size() << std::endl;
        nlohmann::json json_obj;

        json_obj["arr"] = std::move(json_arr);
        //json_obj["arr"] = json_arr;

        std::cout << "json_arr.size: " << json_arr.size() << std::endl;
        std::cout << json_obj << std::endl;
    }
    return 0;
}
