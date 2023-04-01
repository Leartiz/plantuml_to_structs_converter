#include <iostream>
#include <sstream>
#include <string>

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

    {
        sin.clear(); // necessarily?
        sin.str("actor \"User\"");
        std::cout << "sin.str: " << sin.str() << std::endl;

        sin >> word;
        std::cout << word << std::endl;
    }
    std::cout << std::endl;

    // wchar
    {
        std::istringstream emoji_istream{ "😇8😇8😇8" };
        int counter{ 0 };
        while (!emoji_istream.eof()) {
            auto one = emoji_istream.get(); counter++;
            if (counter % 5 == 0) {
                std::cout << static_cast<char>(one) << std::endl;
            }
            else {
                std::cout << one << std::endl;
            }
        }
    }
    std::cout << std::endl;

    // wchar
    {
        std::istringstream emoji_istream{ "😇8😇8😇8" };
        int counter{ 0 };
        while (!emoji_istream.eof()) {
            unsigned char one{ 0 };
            emoji_istream >> one;

            counter++;
            if (counter % 5 == 0) {
                std::cout << one << std::endl;
            }
            else {
                std::cout << static_cast<int>(one) << std::endl;
            }
        }
    }
    std::cout << std::endl;

    // uchar
    {
        if ('z' == static_cast<unsigned char>('z')) {
            std::cout << "OK" << std::endl;
        }
        if (L'о' == L'о') {
            std::cout << "OK" << std::endl;
        }
    }
    std::cout << std::endl;

    // \n\n\n
    {
        std::stringstream sin{ "\nabs" };
        std::istream& in_stream{ sin };
        auto space = in_stream.get();
        if (std::isspace(space)) {
            std::cout << "space0" << std::endl;
            in_stream.unget();
        }
        space = in_stream.get();
        if (std::isspace(space)) {
            std::cout << "space1" << std::endl;
            in_stream.unget();
        }
    }
    std::cout << std::endl;

    // get, unget
    {
        std::stringstream strsin{ "12" };
        std::istream& sin = strsin;
        //sin.clear(std::ios::badbit);

        std::cout << "good: " << sin.good() << std::endl;
        std::cout << "eof: " << sin.eof() << std::endl;
        std::cout << "bad: " << sin.bad() << std::endl;

        std::cout << "peek: " << sin.peek() << std::endl;
        std::cout << "peek: " << sin.peek() << std::endl;

        sin.get();
        sin.get();

        std::cout << "good: " << sin.good() << std::endl;
        std::cout << "eof: " << sin.eof() << std::endl;
        std::cout << "bad: " << sin.bad() << std::endl;

        std::cout << "peek: " << sin.peek() << std::endl;
        //std::cout << "peek: " << sin.peek() << std::endl;

        std::cout << "good: " << sin.good() << std::endl;
        std::cout << "eof: " << sin.eof() << std::endl;
        std::cout << "bad: " << sin.bad() << std::endl;

        sin.unget();
        std::cout << "peek: " << sin.peek() << std::endl;
        std::cout << "peek: " << sin.peek() << std::endl;

        std::cout << "good: " << sin.good() << std::endl;
        std::cout << "eof: " << sin.eof() << std::endl;
        std::cout << "bad: " << sin.bad() << std::endl;
    }

    std::cout << std::endl;
    return 0;
}
