#include <memory>
#include <iostream>

using namespace std;

int main()
{
    shared_ptr<int> p1{ new int{ 0 } };
    shared_ptr<int> p2 = p1;
    cout << (p1 == p2) << endl;

    shared_ptr<int> p3{ new int{ 0 } };
    cout << (p1 == p3) << endl;

    return 0;
}
