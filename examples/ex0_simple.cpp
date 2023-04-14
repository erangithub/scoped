// This example demonstrates how to use the scoped<> class to implement scope-sensitive programming. 
// The code defines a ScopedThreshold type, which is a scoped integer that represents a threshold value. 
// The print_number() function takes an integer as input and prints either the number itself or the word "BIG", 
// depending on whether the input is greater than or equal to the current threshold value. 
// The main() function demonstrates how to create a scoped threshold object and use it to control the behavior 
// of the print_number() function within a specific scope.

#include "../include/scoped.h"
#include <iostream>

using ScopedThreshold = scoped::scoped<int, struct ScopedThresholdTag>;

void print_number(int x) {
    std::cout << "The number is ";
    if (auto thresh = ScopedThreshold::top()) {
        if (x >= thresh->value()) {
            std::cout << "BIG" << std::endl;
            return;
        }
    }
    std::cout << x << std::endl;
}

int main() {
    {
        ScopedThreshold scoped_threshold{4};
        
        print_number(3);   // Expected: The number is 3
        print_number(10);  // Expected: The number is BIG
    }
    print_number(10);  // Expected: The number is 10
    return 0;
}