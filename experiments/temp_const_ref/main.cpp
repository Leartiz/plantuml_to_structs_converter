#include <string>
#include <iostream>

struct TempCRefHolder final
{
    explicit TempCRefHolder(const std::string& one)
        : value{ one } {}

    const std::string& value;
};

int main()
{
    {
        TempCRefHolder tmp{ "1234567890" };
        std::cout << tmp.value << std::endl;
    }

    {
        auto v = new std::string{ "temp_temp" };
        TempCRefHolder tmp{ *v };
        //delete v; v = nullptr;

        std::cout << tmp.value << std::endl;

        delete v;
    }
    return 0;
}
