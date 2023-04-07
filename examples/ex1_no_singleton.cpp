// This example shows how scoped values can be used instead of singletons
// The main benefit is that that the object lifetime is controlled explicitely.
// Here a Logger is installed, and the function "foo" uses it, if it exists in the scope 
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
using ScopedLogger = scoped<Logger, struct ScopedLoggerTag>;

// A function that prints a message using the Logger, if one exists in the current scope
void foo() {
    if (auto logger = ScopedLogger::get()) {
        logger->println("Calling from foo");
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
