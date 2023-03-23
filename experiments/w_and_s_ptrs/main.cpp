#include <memory>
#include <iostream>
#include <stdexcept>

int main()
{
    std::weak_ptr<int> int_wp;
    if (int_wp.expired()) {
        std::cout << "int_wp expired" << std::endl;
    }

    try {
        *int_wp.lock() = 0; // don't catch?
    }
    catch (...) {
        std::cout << "err!" << std::endl;
    }

    return 0;
}
