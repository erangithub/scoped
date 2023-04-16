#include "scoped.h"
#include <optional>

using ScopedInt = scoped::scoped<int>;

int main(int argc, char** argv) {
    std::optional<ScopedInt> optScopedInt;
    
    if (5 % 2 == 1) {
        optScopedInt = 13;
    }
    assert(ScopedInt::top());
    if (auto top = ScopedInt::top()) {
        assert(top->value() == 13);
    }
    
    return 0;
}