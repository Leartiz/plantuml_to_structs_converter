#include <string>

#include <iostream>
#include <fstream>

#include <filesystem>
#include <codecvt>

/*

- How does a string with large characters work.
- Open a file with a complex name.

*/
int main()
{    
    auto cur_path{ std::filesystem::current_path() };
    cur_path = cur_path.parent_path().parent_path();
    cur_path = cur_path / "experiments" / "std_string";
    std::cout << cur_path << std::endl;

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    cur_path /= converter.from_bytes("файл.txt");  // -> utf-16le
    std::cout << cur_path << std::endl;

    std::ifstream fin{ cur_path };                 // utf-16le
    if (!fin.is_open()) {
        std::cout << "File: " << cur_path.filename() << " not opened!" << std::endl;
        return 0;
    }
    std::string str; std::getline(fin, str, '\n'); // utf-8

    {
        std::cout << str.length() << std::endl;
        std::cout << str.c_str() << std::endl;
    }

    cur_path = cur_path.parent_path();
    cur_path /= converter.from_bytes(str);         // -> utf-16le
    std::cout << cur_path << std::endl;

    std::ofstream fout{ cur_path };
    if (!fout.is_open()) {
        std::cout << "File: " << cur_path.filename() << " not opened!" << std::endl;
        return 0;
    }
    fout << "🤬 содержимое" << std::endl;

    return 0;
}
