// This example shows how scoped values can be a safe replacement for singletons
// Here a Logger is installed, and a couple of functions use it, if it exists in the scope 
#include "../include/scoped.h"
#include <iostream>

using namespace std;

class Logger
{
public:
    void println(const std::string& text) {
        std::cout << "LOGGER: " << text << std::endl;
    }
};

using ScopedLogger = scoped<Logger, struct ScopedLoggerTag>;

void foo()
{
    if (auto logger = ScopedLogger::get()) {
        logger->println("Calling from foo");
    }
    else {
        std::cout << "No logger found" << std::endl;
    }
}

int main(int argc, char** argv)
{
    foo();
    ScopedLogger logger;
    foo();
    return 0;
}