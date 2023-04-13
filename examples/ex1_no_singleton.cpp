/*
The "scoped" template is a useful tool for managing resources that have a limited scope,
and can replace the use of singletons in some cases.

Traditionally, singletons are used to provide a global point of access to a single instance of
a class. However, singletons can be difficult to test and maintain, since they have a global
state that can be accessed from anywhere in the code. Additionally, singletons can cause
issues with multithreading if not properly synchronized.

The "scoped" template provides a way to manage resources with a limited scope, such as
a Logger object that is only needed within a certain section of code. By using "scoped",
you can ensure that the resource is properly constructed and destructed within the scope,
and can safely assume that it is not being accessed by other parts of the code outside
that scope.

Using "scoped" instead of a singleton provides several benefits, including:

Encapsulation: The resource is encapsulated within the scope where it is needed, and 
is not accessible outside that scope.

Testability: Since the resource is created and destroyed within a limited scope, it is
easier to write tests that verify its behavior.

Thread safety: The resource is only accessible within the scope where it is created, so 
there are no issues with concurrent access or synchronization.
*/

#include "../include/scoped.h"

#include <iostream>
#include <string>

// Define a simple Logger class that prints messages to the console
class Logger {
public:
    void println(const std::string& text) {
        std::cout << "LOGGER: " << text << std::endl;
    }
};

// Define a scoped type for the Logger class, using a tag type to avoid naming collisions
using ScopedLogger = scoped::scoped<Logger, struct ScopedLoggerTag>;

// A function that prints a message using the Logger, if one exists in the current scope
void foo() {
    if (auto pScopedLogger = ScopedLogger::top()) {
        pScopedLogger->value().println("Calling from foo");
    }
    else {
        std::cout << "No logger found" << std::endl;
    }
}

int main(int argc, char** argv) {
    // Call "foo" once before creating a Logger object, to demonstrate that no Logger exists yet
    foo();

    // Create a new ScopedLogger object, which automatically installs a Logger in the current scope
    ScopedLogger logger;

    // Call "foo" again after creating the Logger, to demonstrate that it now exists in the current scope
    foo();

    return 0;
}
