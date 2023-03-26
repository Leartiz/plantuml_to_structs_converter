#include <iostream>
#include <cctype>

int main()
{
    std::string str = "привет";
    std::cout << std::islower(str[0]) << std::endl;
    std::cout << std::islower('h') << std::endl;

    std::cout << std::isalpha(str[0]) << std::endl;
    std::cout << std::isalpha('h') << std::endl;

    std::cout << static_cast<int>(str[0]) << std::endl;
    std::cout << std::toupper(str[0]) << std::endl;
    std::cout << std::toupper('h') << std::endl;

    return 0;
}
