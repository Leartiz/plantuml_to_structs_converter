#include <iostream>

using namespace std;

struct Basic {
    virtual ~Basic() {}
    virtual void do_some() = 0; // !!!
};

void Basic::do_some() {
    cout << "Basic, do_some()" << endl;
}

struct Dervd1 : Basic {
    ~Dervd1() override {}
    void do_some() override {
        Basic::do_some();
        cout << "Dervd, do_some()" << endl;
    }
};

struct Dervd2 : Basic {
    ~Dervd2() override {}
};

int main() {
    {
        // abstract
        // Basic b;
    }

    {
        Dervd1 d1;
        d1.do_some();
    }

    {
        // abstract
        // Dervd2 d2;
    }
}
