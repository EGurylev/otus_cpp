#include <iostream>
#include <map>

#include "pool_allocator.h"
#include "simple_forward_list.h"

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
        std::cout << el.first << " " << el.second << std::endl;
    }

    for (auto& el : factorials_custom_allocator) {
        std::cout << el.first << " " << el.second << std::endl;
    }

    simple_forward_list<int> list;
    for (int i{}; i < 5; ++i) {
        list.push_front(i);
    }
    for (auto& el : list) {
        std::cout << el << std::endl;
    }

    simple_forward_list<int, pool_allocator<int, 10>> list_custom_allocator;
    for (int i{}; i < 5; ++i) {
        list_custom_allocator.push_front(i);
    }
    for (auto it{list_custom_allocator.begin()};
         it != list_custom_allocator.end(); ++it) {
        std::cout << *it << std::endl;
    }
}