#include <string>
#include <iostream>

#pragma pack()
struct MyStruct final
{
public:
    struct Inner final
    {
        std::string name;
        int age{ 0 };
        double data{ 0 };
    };

    Inner m_inn;
};
#pragma pack()

int main()
{
    MyStruct ms;

    std::string* ms_name = reinterpret_cast<std::string*>(reinterpret_cast<char*>(&ms) + 0);
    *ms_name = std::string("any_long_name");

    int* ms_age = reinterpret_cast<int*>(reinterpret_cast<char*>(&ms) + sizeof(std::string));
    *ms_age = 30;

    // not forget: structure alignment!
    double* ms_data = reinterpret_cast<double*>(reinterpret_cast<char*>(&ms) +
                                                sizeof(std::string) + 2*sizeof(int));
    *ms_data = 55.5;

    std::cout << "name: " << ms.m_inn.name << std::endl;
    std::cout << "age: " << ms.m_inn.age << std::endl;
    std::cout << "data: " << ms.m_inn.data << std::endl;

    return 0;
}
