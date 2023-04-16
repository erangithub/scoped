#include "scoped.h"
#include <cassert>

int main(int argc, char** argv) {
    using ScopedInt = scoped::scoped<int>;
    
    ScopedInt si1 = 1;
    assert(ScopedInt::bottom()->value() == 1);
    assert(ScopedInt::top()->value() == 1);

    ScopedInt si2 = 2;
    assert(ScopedInt::bottom()->value() == 1);
    assert(ScopedInt::top()->value() == 2);

    ScopedInt si3 = 3;
    assert(ScopedInt::bottom()->value() == 1);
    assert(ScopedInt::top()->value() == 3);

    si3 = std::move(si1);
    assert(ScopedInt::bottom()->value() == 2);
    assert(ScopedInt::top()->value() == 1);

    si2 = 20;
    assert(ScopedInt::bottom()->value() == 20);
    assert(ScopedInt::top()->value() == 1);

    si3 = 10;
    assert(ScopedInt::bottom()->value() == 20);
    assert(ScopedInt::top()->value() == 10);

    {
        ScopedInt dummy;
        dummy = std::move(si2);
        dummy = std::move(si3);
    }
    assert(!ScopedInt::top() && !ScopedInt::bottom());
    return 0;
}