#include "scoped.h"
#include <iostream>
#include <vector>
#include <optional>

using ScopedInt = scoped::scoped<int>;

int main(int argc, char** argv) {
    std::vector<ScopedInt> intvec {1};
    ScopedInt i1(10);
    intvec.push_back(2);
    auto it = ScopedInt::bottom();
    assert(it->value() == 1);
    it = it->prev();
    assert(it->value() == 10);
    it = it->prev();
    assert(it->value() == 2);
    return 0;
}