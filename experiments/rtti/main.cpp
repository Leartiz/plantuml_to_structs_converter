#include <iostream>

using namespace std;

struct A
{
    A() {}
    virtual ~A() {};
};

struct B : A
{
    B() {}
};

struct C : A
{
    C() {}
};

int main()
{
    {
        A* b = new B();

        cout << typeid(b).name() << endl;

        try {
            auto bb = dynamic_cast<B&>(*b);
            cout << typeid(bb).name() << endl;
        }
        catch(const std::bad_cast& err) {
            std::cerr << err.what() << std::endl;
        }

        try {
            auto cc = dynamic_cast<C&>(*b);
            cout << typeid(cc).name() << endl;
        }
        catch(const std::bad_cast& err) {
            std::cerr << err.what() << std::endl;
        }

        delete b;
    }
    {
        A* a = new A();
        delete a;

        cout << typeid(a).name() << endl;
    }




    return 0;
}
