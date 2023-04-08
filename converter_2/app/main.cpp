#include <sstream>
#include <fstream>
#include <iostream>

#include "usecasegraph.h"

using namespace std;

void visual_test_0()
{
    std::ifstream fin{ "uc_0.txt" };
    if (fin.is_open()) {
        UseCaseGraph ucDiag;
        ucDiag.read_puml(fin);
        ucDiag.write_json(cout);
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
