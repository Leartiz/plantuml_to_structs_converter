#include <string>

#include <iostream>
#include <fstream>
#include <filesystem>

#include <codecvt>
#include <locale>

int main()
{    
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    std::wstring in_fname{ converter.from_bytes("файл.txt") };
    std::ifstream fin{ std::filesystem::path(in_fname.c_str()) }; // utf-16le
    std::string str; fin >> str;           // utf-8

    {
        std::cout << str.length() << std::endl;
        std::cout << str.c_str() << std::endl;
    }

    std::wstring out_fname{ converter.from_bytes(str) };
    std::ofstream fout{ std::filesystem::path(out_fname.c_str()) };
    fout << "🤬 содержимое" << std::endl;

    return 0;
}
