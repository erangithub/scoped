#include "scoped.h"
#include <iostream>

using ScopedThreshold = scoped::scoped<int, struct ScopedThresholdTag>;

int get_number(int x) {
    if (auto thresh = ScopedThreshold::top()) {
        if (x >= thresh->value()) {
            return -1;
        }
    }
    return x;
}

int main() {
    {
        ScopedThreshold scoped_threshold{4};
        assert(get_number(3) == 3);   // Expected: 3
        assert(get_number(10) == -1); // Expected: -1
        {
            ScopedThreshold::shield shield;
            assert(get_number(10) == 10);
        }
        assert(get_number(10) == -1);  // Expected: -1
    }
    assert(get_number(10) == 10);  // Expected: 10
    return 0;
}