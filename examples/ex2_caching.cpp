#include "../include/scoped.h"
#include <unordered_map>
#include <vector>
#include <iostream>

using ScopedCache = scoped<std::unordered_map<int, bool>, struct ScopedCacheTag>;

bool is_prime(int n) {
    if (n < 2) return false;

    auto pCache = ScopedCache::get();
    
    if (pCache) {
        if (auto it = pCache->find(n); it != pCache->end()) {
            std::cout << "Cache hit for " << n << std::endl;
            return it->second;
        }
    }

    bool result = true;
    for (int i=2; i*i <= n; ++i) {
        if (n % i == 0) {
            result = false;
            break;
        }
    }
    if (pCache) {
        (*pCache)[n] = result;
    }
    
    return result;
}

int next_prime(int n) {
    auto pCache = ScopedCache::get();
    int k = n+1;
    for (; !is_prime(k); k++);
    return k;
}

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
    ScopedCache primeCache;
    first_n_primes(5);
    for (auto p : first_n_primes(10))
    {
        std::cout << p << std::endl;
    }
}