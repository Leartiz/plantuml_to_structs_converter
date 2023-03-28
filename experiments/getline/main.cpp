#include <iostream>
#include <sstream>
#include <string>

int main()
{
    std::istringstream sin{ ":Reg as Regg" };
    char start_ch{ 0 };
    sin >> start_ch;

    std::string name;
    if (std::getline(sin, name, ':')) {
        std::cout << name;
    }




    return 0;
}
