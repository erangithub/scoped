// This is an example of using scoped template to apply decorators to log messages.
// Decorators are objects that modify or enhance the behavior of other objects.
// In this example, the TextDecorator base class defines the interface for decorators
// to transform log messages. The UpperCaseDecorator and IndentDecorator subclasses
// implement different types of text transformations. The scoped template is used to 
// apply these decorators to log messages, with each decorator being applied within a
// specific scope. 
// We also demonstrate how to use this logging system in a multithreaded 
// environment by having two threads log messages with different decorators, with
// interleaving messages.
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include "../include/scoped.h"

class TextDecorator {
public:
    virtual std::string apply(const std::string& text) const = 0;
    virtual ~TextDecorator() {};
};

class UpperCaseDecorator : public TextDecorator {
public:
    std::string apply(const std::string& text) const override {
        std::string result;
        std::transform(text.begin(), text.end(), std::back_inserter(result), ::toupper);
        return result;
    }
};

class IndentDecorator : public TextDecorator {
public:
    std::string apply(const std::string& text) const override {
        std::string result = "    ";
        for (size_t i=0; i<text.size(); ++i) {
            result += text[i];
            if (text[i]=='\n' && i+1 < text.size()) {
                result += "    ";
            }
        }
        return result;
    }
};

using namespace scoped;
using ScopedDecorator = abstract_scoped<TextDecorator>;
using ScopedUpperCaseDecorator = polymorphic_scoped<UpperCaseDecorator, TextDecorator>;
using ScopedIndentDecorator = polymorphic_scoped<IndentDecorator, TextDecorator>;

// Define a logging function that uses scoped<> to apply the decorators
void log(const std::string& message) {
    // Use the Scoped<> type to apply the decorators to the message
    std::string decoratedMessage = message;
    for (auto pScope = ScopedDecorator::top(); pScope; pScope = pScope->next()) {
        decoratedMessage = pScope->value().apply(decoratedMessage);
    }

    // Synchronize access to the standard output and print the decorated message
    static std::mutex logMutex;
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << decoratedMessage << std::endl;
}

void threadFunc1() {
    ScopedUpperCaseDecorator upper;
    for (int i = 0; i < 5; ++i) {
        log("Thread 1: This message is upper case");
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void threadFunc2() {
    ScopedIndentDecorator indent;
    ScopedUpperCaseDecorator upper;
    for (int i = 0; i < 5; ++i) {
        log("Thread 2: This message is upper case\nand indented");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    std::thread t1(threadFunc1);
    std::thread t2(threadFunc2);

    for (int i = 0; i < 5; ++i) {
        log("Main thread: This is a regular message");
        std::this_thread::sleep_for(std::chrono::milliseconds(70));
    }

    t1.join();
    t2.join();

    return 0;
}
