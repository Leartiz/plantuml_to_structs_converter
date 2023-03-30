#include <string>
#include <iostream>
#include <fstream>

#include <boost/tokenizer.hpp>

int main() {
    auto fin = std::ifstream("example.txt");
    if (!fin.is_open()) {
        std::cout << "err open file." << std::endl;
        return 0;
    }

    std::string line;
    std::string text;
    while (std::getline(fin, line, '\n')) {
        text += line + '\n';
    }

    boost::tokenizer<> tok(text);
    for (auto token : tok) {
        std::cout << token << "\n";
    }
}