#include <string>
#include <iostream>
#include <algorithm>

#include "nlohmann/json.hpp"

int main()
{
    const std::string field_name{ "id" };

    /* across */
    nlohmann::json::array_t lhs_json_arr;
    nlohmann::json::array_t rhs_json_arr;

    // sort.
    {
        for (int i = 0; i < 10; ++i) {
            nlohmann::json one = {{ field_name, std::rand() % 10 }};
            lhs_json_arr.push_back(one);

            rhs_json_arr.push_back(one);
        }
        std::cout << "original: " << lhs_json_arr << std::endl;

        std::sort(std::begin(lhs_json_arr), std::end(lhs_json_arr),
                  [&](nlohmann::json lhs, nlohmann::json rhs) -> bool {
            return lhs[field_name] < rhs[field_name];
        });
        std::cout << "sorted: " << lhs_json_arr << std::endl;
    }

    std::cout << "\n---\n" << std::endl;

    // array comparison, depends on position of objects in json array!
    {
        std::cout << "lhs == rhs: " << (lhs_json_arr == rhs_json_arr) << std::endl;
    }

    std::cout << "\n---\n" << std::endl;

    // move json arr.
    {
        std::cout << "json_arr.size: " << lhs_json_arr.size() << std::endl;
        nlohmann::json json_obj;

        json_obj["arr"] = std::move(lhs_json_arr);
        //json_obj["arr"] = json_arr;

        std::cout << "json_arr.size: " << lhs_json_arr.size() << std::endl;
        std::cout << json_obj << std::endl;
    }
    return 0;
}
