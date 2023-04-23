#include <sstream>
#include <fstream>
#include <iostream>

#include "usecasegraph.h"
#include "robustnessgraph.h"

using namespace std;

void visual_test_uc_0()
{
    ifstream fin{ "uc_0.txt" };
    if (fin.is_open()) {
        UseCaseGraph ucDiag;
        ucDiag.read_puml(fin);
        ucDiag.write_json(cout);
    }
    else {
        cerr << "file is not open\n";
    }
}

void visual_test_rob_0()
{
    ifstream fin{ "rob_0.txt" };
    if (fin.is_open()) {
        RobustnessGraph robG;
        robG.read_puml(fin);
        robG.write_json(cout);
    }
    else {
        cerr << "file is not open\n";
    }
}

// -----------------------------------------------------------------------

// TODO: переименовать проект в консоль. app

int main()
{
    visual_test_rob_0();
    return 0;
}
