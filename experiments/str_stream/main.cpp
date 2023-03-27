#include <iostream>
#include <sstream>

int main()
{
    std::stringstream sin{ "@startuml basic    string" };
    std::cout << "sin.str: " << sin.str() << std::endl;

    std::string word;

    sin >> word;
    std::cout << word << std::endl;

    sin >> word;
    std::cout << word << std::endl;

    char ch{ 0 };
    sin >> ch;
    std::cout << ch << std::endl;
    sin.unget();

    sin >> word;
    std::cout << word << std::endl;

    std::cout << "sin.str: " << sin.str() << std::endl;

    return 0;
}
