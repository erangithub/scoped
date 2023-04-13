// The example illustrates how the scoped template can be used to keep track of the number
// of operations performed by the Calculator within a certain scope.

#include "../include/scoped.h"
#include <iostream>
#include <vector>

class Calculator {
public:
    enum Event {
        ADD=0, SUB=1, MUL=2, DIV=3
    };

    class Counter {
    public:
        void increment(Event event) { count_[event]++; }
        void report(const std::string& prefix) {
            std::cout << prefix
                    << " ADD: " << count_[ADD]
                    << " SUB: " << count_[SUB]
                    << " MUL: " << count_[MUL]
                    << " DIV: " << count_[DIV] << std::endl;
        }
    private:
        std::vector<int> count_ = {0,0,0,0};
    };

    using ScopedCounter = scoped::scoped<Counter, struct ScopedCounterTag>;

    Calculator() : result_(0.0) {}
    void add(double x) {
        increment(ADD);
        result_ += x;
    }
    void subtract(double x) {
        increment(SUB);
        result_ -= x;
    }
    void multiply(double x) {
        increment(MUL);
        result_ *= x;
    }
    void divide(double x) {
        increment(DIV);
        result_ /= x;
    }
    void clear() {
        result_ = 0.0;
    }
    double getResult() const {
        return result_;
    }
private:
    // Increment event count in all currently scoped counters
    void increment(Event event) {
        for (auto pScopedCounter = ScopedCounter::top(); pScopedCounter; pScopedCounter = pScopedCounter->next()) {
            pScopedCounter->value().increment(event);
        }
    }
    double result_;
};

double average(const std::vector<double>& v, Calculator& calc) {
    Calculator::ScopedCounter counter;
    calc.clear();
    for (double val : v) {
        calc.add(val);
    }
    calc.divide(v.size());
    counter.value().report("calc event count in average(): ");
    return calc.getResult();
}

int main() 
{
    Calculator calc;
    Calculator::ScopedCounter counter;
    auto avg = average({1.0, 2.0, 3.0, 4.0, 5.0}, calc);
    std::cout << "Average: " << avg << std::endl;

    calc.clear(); calc.add(2); calc.multiply(3); calc.multiply(7);
    std::cout << "The meaning of life is: " << calc.getResult() << std::endl;

    counter.value().report("Total calc event count in main():");
}