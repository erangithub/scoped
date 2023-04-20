#include "scoped.h"
#include "scoped_manifest.h"

// Header
struct get_number_manifest {
    using Thresh = scoped::scoped<int, struct ThresholdTag>;
};

int get_number(int x);

SCOPED_DECLARE_FUNCTION_MANIFEST(get_number, get_number_manifest);

// Body
int get_number(int x) {
    if (auto thresh = SCOPED_GET_FUNCTION_MANIFEST(get_number)::Thresh::top()) {
        if (x >= thresh->value()) {
            return -1;
        }
    }
    return x;
}

// User
int main() {
    {
        SCOPED_GET_FUNCTION_MANIFEST(get_number)::Thresh scoped_threshold(4);

        assert(get_number(3) == 3); 
        assert(get_number(10) == -1);
    }

    assert(get_number(10) == 10);
    return 0;
}


