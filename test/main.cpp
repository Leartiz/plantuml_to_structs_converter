#include <sstream>
#include <fstream>
#include <iostream>

#include "graphclass.h"

using namespace std;

void visual_test_0()
{
    std::ifstream fin{ "class_0.txt" };
    if (fin.is_open()) {
        GraphClass classDiag;
        classDiag.read(fin);
    }
}

void visual_test_1()
{

}

int main()
{
    visual_test_0();
    return 0;
}
