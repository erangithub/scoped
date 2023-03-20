#include "scoped.h"
#include <iostream>
#include <vector>

using ScopedInt = scoped<int, struct ScopedIntTag>;

void foo()
{
    if (auto si = ScopedInt::top()) {
        std::cout << "Found " << si->value() << std::endl;
    }
    if (auto si = ScopedInt::get()) {
        std::cout << "Top value " << *si << std::endl;
    }
    else {
        std::cout << "Not found" << std::endl;
    }
}

int main(int argc, char** argv)
{
    foo();
    ScopedInt si(5);
    std::cout << "Hello" << std::endl;
    foo();
    return 0;
}