// This code demonstrates how to use the abstract_scoped and polymorphic_scoped classes to implement 
// dependency injection with a polymorphic error handling mechanism in a Calculator class.
// The Calculator class defines a divide function that takes two double-precision floating-point
// numbers as input and returns their quotient. If the second argument is zero, an error occurs 
// and the divide function uses the error handler to report it. This example provides two 
// implementations of an error handler: ConsoleErrorHandler, which prints an error message to 
// the console, and ThrowingErrorHandler, which throws an exception. By using scoped and 
// polymorphic_scoped, we can inject an instance of the appropriate error handler at runtime 
// and ensure that the correct error handling behavior is used.

#include <iostream>
#include <string>
#include <stdexcept>
#include "../include/scoped.h"

// Abstract base class for error handlers
class ErrorHandler {
public:
    virtual void handle_error(const std::string& message) const = 0;
    virtual ~ErrorHandler() {};
};

// Concrete error handler that writes error messages to the console
class ConsoleErrorHandler : public ErrorHandler {
public:
    void handle_error(const std::string& message) const override {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

// Concrete error handler that throws a runtime_error when an error occurs
class ThrowingErrorHandler : public ErrorHandler {
public:
    void handle_error(const std::string& message) const override {
        throw std::runtime_error(message);
    }
};

// Define aliases for scoped and polymorphic scoped error handlers
using ScopedErrorHandler = abstract_scoped<ErrorHandler>;
using ScopedConsoleErrorHandler = polymorphic_scoped<ConsoleErrorHandler, ErrorHandler>;
using ScopedThrowingErrorHandler = polymorphic_scoped<ThrowingErrorHandler, ErrorHandler>;

// Simple calculator class that can divide two numbers
class Calculator {
public:
    // Divide two numbers, and handle divide-by-zero errors using the scoped error handler
    double divide(double x, double y) {
        if (y == 0) {
            // Check if an error handler is available in the current scope
            if (auto errorHandler = ScopedErrorHandler::top()) {
                // If an error handler is available, call its handle_error() function with an error message
                errorHandler->value().handle_error("Cannot divide by zero");
            }
            return std::numeric_limits<double>::quiet_NaN();
        } else {
            return x / y;
        }
    }
};

int main() {
    Calculator calculator;

    // Use a scoped console error handler to write error messages to the console
    ScopedConsoleErrorHandler consoleErrorHandler;

    std::cout << "5 / 2 = " << calculator.divide(5, 2) << std::endl;
    std::cout << "5 / 0 = " << calculator.divide(5, 0) << std::endl;

    try {
        // Use a scoped throwing error handler to throw runtime errors when an error occurs
        ScopedThrowingErrorHandler throwingErrorHandler;
    
        std::cout << "5 / 2 = " << calculator.divide(5, 2) << std::endl;
        std::cout << "5 / 0 = " << calculator.divide(5, 0) << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    return 0;
}