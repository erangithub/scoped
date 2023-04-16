#include "scoped.h"
#include <iostream>
#include <vector>
#include <optional>

using ScopedInt = scoped::scoped<int>;

int main(int argc, char** argv) {
    std::vector<ScopedInt> intvec {1,2,3,4,5};
    auto it = ScopedInt::bottom();
    for (auto& i : intvec) {
        assert(i.value() == it->value());
        it = it->prev();
    }
    return 0;
}