#include <sstream>
#include <fstream>
#include <iostream>

#include "graphusecase.h"

using namespace std;

void visual_test_0()
{
    std::ifstream fin{ "uc_0.txt" };
    if (fin.is_open()) {
        GraphUseCase ucDiag;
        ucDiag.read_puml(fin);
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
