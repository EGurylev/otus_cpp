#include <iostream>
#include <list>
#include <vector>

template <typename T>
auto print_ip(T&& ip) -> decltype(void(T() * 0), void()) {
    std::uint8_t* byte{reinterpret_cast<std::uint8_t*>(&ip) + sizeof(T) - 1};
    for (size_t i{}; i < sizeof(T); ++i) {
        if (i != 0) {
            std::cout << '.';
        }
        std::cout << static_cast<int>(*byte);
        --byte;
    }
    std::cout << std::endl;
}

template <typename T>
auto print_ip(T&& ip)
    -> decltype(void(T(1, std::allocator<typename T::value_type>())), void()) {
    for (auto it = ip.begin(); it != ip.end(); ++it) {
        if (it != ip.begin()) {
            std::cout << '.';
        }
        std::cout << *it;
    }
    std::cout << std::endl;
}

template <typename T>
auto print_ip(T&& ip) -> decltype(void(T().c_str()), void()) {
    std::cout << ip << std::endl;
}

int main() {
    print_ip(int8_t{-1});
    print_ip(int16_t{0});
    print_ip(int32_t{2130706433});
    print_ip(int64_t{8875824491850138409});
    print_ip(std::string{"Hello, world"});
    print_ip(std::vector<int>{100, 200, 300, 400});
    print_ip(std::list<short>{400, 300, 200, 100});
}