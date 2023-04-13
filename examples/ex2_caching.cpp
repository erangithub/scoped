// This example demonstrates how the scoped template can be used to create a cache for 
// prime numbers. This allows the user to control exactly when the cache is cleaned.

#include "../include/scoped.h"
#include <unordered_map>
#include <vector>
#include <iostream>

// Create a new scoped cache for caching prime numbers
using ScopedPrimeCache = scoped::scoped<std::unordered_map<int, bool>, struct ScopedPrimeCacheTag>;

// Check if a given number is a prime number
bool is_prime(int n) {
    if (n < 2) return false;

    // Retrieve the prime number cache, taking the bottom (outer) most scope
    auto pScopedCache = ScopedPrimeCache::bottom();
    
    // If previously cached, return the result
    if (pScopedCache) {
        auto& cache = pScopedCache->value();
        if (auto it = cache.find(n); it != cache.end()) {
            std::cout << "Cache hit for " << n << std::endl;
            return it->second;
        }
    }

    // Calculate whether the number is prime or not, and cache the result
    bool result = true;
    for (int i=2; i*i <= n; ++i) {
        if (n % i == 0) {
            result = false;
            break;
        }
    }
    if (pScopedCache) {
        (pScopedCache->value())[n] = result;
    }
    
    return result;
}

// Find the next prime number greater than a given number
int next_prime(int n) {
    int k = n+1;
    for (; !is_prime(k); k++);
    return k;
}

// Find the first n prime numbers
std::vector<int> first_n_primes(int n) {
    std::vector<int> primes;
    int p = 0;
    for (int i=0; i < n; ++i) {
        p = next_prime(p);
        primes.push_back(p);
    }
    return primes;
}

int main()
{
    // Create a new scoped cache for caching prime numbers
    ScopedPrimeCache primeCache;

    first_n_primes(5);
    for (auto p : first_n_primes(10))
    {
        std::cout << p << std::endl;
    }
}