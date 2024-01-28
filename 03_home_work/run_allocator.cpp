#include <iostream>
#include <map>

#include "pool_allocator.h"

int main() {
    std::map<int, int> factorials;
    std::map<int, int, std::less<int>,
             pool_allocator<std::pair<const int, int>, 10>>
        factorials_custom_allocator;
    int fact{1};

    for (int i{}; i < 10; ++i) {
        factorials[i] = fact;
        factorials_custom_allocator[i] = fact;
        fact *= i + 1;
    }

    for (auto& el : factorials) {
        std::cout << el.first << " " << el.second << "\n";
    }

    for (auto& el : factorials_custom_allocator) {
        std::cout << el.first << " " << el.second << "\n";
    }
}