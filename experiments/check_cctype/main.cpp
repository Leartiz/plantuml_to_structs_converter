#include <iostream>
#include <clocale>
#include <cctype>

/*

- It is not clear how the locale works.

*/

std::string get_rus_string()
{
    return std::string{ "привет" };
}

int main()
{
    std::cout << std::locale::classic().name() << std::endl;
    std::cout << "\n---\n" << std::endl;

    {
        std::string str = "hello";
        str = get_rus_string();

        // ***

        std::cout << std::islower(str[0]) << std::endl;
        std::cout << std::islower('h') << std::endl;

        std::cout << std::isalpha(str[0]) << std::endl;
        std::cout << std::isalpha('h') << std::endl;

        std::cout << static_cast<int>(str[0]) << std::endl;
        std::cout << static_cast<int>(std::toupper(str[0])) << std::endl;
        std::cout << static_cast<int>(std::toupper('h')) << std::endl;
    }
    std::cout << "\n---\n" << std::endl;
    {
        std::string str = "hello";
        str = get_rus_string();

        // ***

        std::cout << std::islower(str[0], std::locale::classic()) << std::endl;
        std::cout << std::islower('h', std::locale::classic()) << std::endl;

        std::cout << std::isalpha(str[0], std::locale::classic()) << std::endl;
        std::cout << std::isalpha('h', std::locale::classic()) << std::endl;

        std::cout << static_cast<int>(str[0]) << std::endl;
        std::cout << static_cast<int>(std::toupper(str[0], std::locale::classic())) << std::endl;
        std::cout << static_cast<int>(std::toupper('h', std::locale::classic())) << std::endl;
    }
    std::cout << "\n---\n" << std::endl;
    setlocale(LC_ALL, "russian");
    {
        std::string str = "hello";
        str = get_rus_string();

        // ***

        std::cout << std::islower(str[0]) << std::endl;
        std::cout << std::islower('h') << std::endl;

        std::cout << std::isalpha((uint8_t)str[0]) << std::endl;
        std::cout << std::isalpha('h') << std::endl;

        std::cout << static_cast<int>((uint8_t)str[0]) << std::endl;
        std::cout << static_cast<int>(std::toupper(str[0])) << std::endl;
        std::cout << static_cast<int>(std::toupper('h')) << std::endl;
    }
    // ?
    return 0;
}
