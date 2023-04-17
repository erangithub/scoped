# Scoped - Scope-Sensitive Programming
## Introduction
Scoped is a C++ header-only library that provides a templated scoped<> class for "Scope-Sensitive Programming". 
This library provides a simple and effective way to control the behavior of code within a scope without requiring 
explicit argument passing. With Scoped, you can easily control the configuration of code blocks by encapsulating 
configuration data within a scoped instance. Scoped eliminates the need for refactoring large APIs by passing many 
configuration arguments and also eliminates the need to lock shared objects in many cases. 
In this README, we'll cover the features of Scoped and how to use it in your C++ projects.

## Features
* Thread-safe mechanism for controlling the behavior of code within a scope.
* Wraps any type `T`, and should only be used as an automatic (local) variable.
* When constructed, the `scoped<T>` is pushed to the top of a stack of `scoped<T>` objects. When destructed, it is popped from the stack.
* The stack of scopes is visible to the entire code within the scope, and can be traversed in both directions.
* Supports polymorphism for easy dependency injection.
* Supports dynamic / conditional scoping using e.g. optional / vector stl containers.
* Eliminates the need for refactoring large APIs by passing many configuration arguments.
* Eliminates the need to lock shared objects in many cases.

## Installation
Scoped is a header-only library and does not require any installation. Simply include the header file scoped.h in your C++ project.

## Usage
To use Scoped in your C++ project, include the scoped.h header file and create a scoped instance, 
passing in the value to be scoped and any optional tags.


```c++
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
```

You can also use `polymorphic_scoped<T,B>` and `abstract_scoped<B>` to scope a value of a different type than its base class:

```c++
class Base { virtual ~Base() = default; };
class Derived : public Base { /* ... */ };

void foo() {
    if (auto thresh = scoped::abstract_scoped<Base>::top()) {
        // Now threash->value() is a reference to the derived_value, as a Base instance 
        // ...
    }
}

int main() {
{
    Derived derived_value;
    scoped::polymorphic_scoped<Derived, Base> scoped_derived{derived_value};
    // The derived_value is now scoped within this block as a Base instance
        
    foo();
    return 0;
}
```

# Why use scoped
Scoped is a powerful tool for controlling the behavior of code within a specific scope, 
offering a way to encapsulate configuration data in a simple and effective way. 
It eliminates the need for refactoring large APIs by passing many configuration arguments 
and eliminates the need to lock shared objects in many cases.

Scoped is particularly useful in situations where you need to adjust the behavior of code within 
a certain scope, without affecting the behavior of the rest of the application. For example, suppose 
you have a long-running application that performs calculations with different configurations, 
such as different thresholds or timeout values. With Scoped, you can easily change the configuration 
data within a scope, without having to rewrite the code that uses it.

Scoped also offers a thread-safe mechanism for controlling the behavior of code within a scope,
which is particularly useful in multi-threaded applications. By using Scoped to control access to shared
resources, you can avoid race conditions and other synchronization issues that can arise when
multiple threads access the same data.

In summary, Scoped is a versatile and powerful library that simplifies the process of scoping and
controlling access to data within your code. Whether you're working with a large API or a complex
multi-threaded application, Scoped can help you achieve greater flexibility and control, while 
reducing the need for complex refactoring and synchronization.

To illustrate the usefulness of Scoped, we've included several examples in the examples/ folder of the Scoped repository:

Singleton: In the singleton example, Scoped is used instead of a traditional singleton.
The Scoped object is created in the scope where it's used, and it's automatically destroyed when it goes out of scope.

Caching: The caching example demonstrates how Scoped can be used for caching results.
The scope of the cache is controlled by the user, ensuring that the cache is only used when it's needed.

Event Counting: The event counting example shows how Scoped can be used to count events within a scope. 
This can be useful for profiling code or for debugging purposes.

Text Decorators: In the text decorators example, Scoped is used to encapsulate text decorators that affect the 
output of a log file in a multithreaded program. This ensures that the log file is properly formatted, even when multiple
threads are writing to it simultaneously.

Dependency Injection: The dependency injection example demonstrates how Scoped can be used for dependency injection.
A calculator uses a scoped error handler to handle division by zero errors. One error handler prints to the console, 
while another throws an exception.

# Code of Conduct
Please read [CODE_OF_CONDUCT](CODE_OF_CONDUCT.md).

# Contributing
Please read [CONTRIBUTING](CONTRIBUTING.md) for the process for submitting pull requests.

# License
Scoped is licensed under the MIT license. See LICENSE for more information.

# Author
Scoped was written by Eran Talmor (the.eran.talmor@gmail.com). If you have any questions or feedback, please don't hesitate to contact me.
