#include <iostream>
#include <sstream>
#include <string>

int main()
{
    // ***

    {
        std::istringstream sin{ ":Reg" };
        char start_ch{ 0 };
        sin >> start_ch;

        std::string name;
        std::getline(sin, name, ':');
        std::cout << "> " << name << std::endl;

        if (sin.good()) {
            std::cout << "sin (:Reg) is good." << std::endl;
        }
        if (sin.eof()) {
            std::cout << "sin (:Reg) is eof." << std::endl;
        }
        if (sin.bad()) {
            std::cout << "sin (:Reg) is bad." << std::endl;
        }

        char ch{ 0 };
        sin.unget();
        sin >> ch;
        std::cout << "> " << ch << std::endl;

        if (sin.good()) {
            std::cout << "sin (:Reg) is good." << std::endl;
        }
        if (sin.eof()) {
            std::cout << "sin (:Reg) is eof." << std::endl;
        }
        if (sin.bad()) {
            std::cout << "sin (:Reg) is bad." << std::endl;
        }
        std::cout << std::endl;
    }

    // ***

    {
        std::istringstream sin{ "hello" };
        std::string val;
        sin >> val;
        std::cout << "> " << val << std::endl;

        if (sin.good()) {
            std::cout << "sin (hello) is good." << std::endl;
        }
        if (sin.eof()) {
            std::cout << "sin (hello) is eof." << std::endl;
        }
        if (sin.bad()) {
            std::cout << "sin (hello) is bad." << std::endl;
        }
        std::cout << std::endl;
    }

    // ***

    {
        std::istringstream sin{ "h" };
        char val;
        sin >> val;
        std::cout << "> " << val << std::endl;

        if (sin.good()) {
            std::cout << "sin (h) is good." << std::endl;
        }
        if (sin.eof()) {
            std::cout << "sin (h) is eof." << std::endl;
        }
        if (sin.bad()) {
            std::cout << "sin (h) is bad." << std::endl;
        }
        std::cout << std::endl;
    }

    // ***

    {
        std::istringstream sin{ "hi:" };
        std::string word;
        std::getline(sin, word, ':');
        std::cout << "> " << word << std::endl;

        if (sin.good()) {
            std::cout << "sin (hi:) is good." << std::endl;
        }
        if (sin.eof()) {
            std::cout << "sin (hi:) is eof." << std::endl;
        }
        if (sin.bad()) {
            std::cout << "sin (hi:) is bad." << std::endl;
        }
        std::cout << std::endl;
    }

    // ***

    return 0;
}
