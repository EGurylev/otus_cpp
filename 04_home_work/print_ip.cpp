#include <iostream>
#include <list>
#include <tuple>
#include <vector>

template <typename T,
          std::enable_if_t<std::is_integral_v<std::decay_t<T>>, bool> = true>
void print_ip(T&& ip) {
    std::decay_t<T> ip_decayed{ip};
    std::uint8_t* byte{reinterpret_cast<std::uint8_t*>(&ip_decayed) +
                       sizeof(T) - 1};
    for (size_t i{}; i < sizeof(T); ++i) {
        if (i != 0) {
            std::cout << '.';
        }
        std::cout << static_cast<int>(*byte);
        --byte;
    }
    std::cout << std::endl;
}

template <
    typename T,
    std::enable_if_t<
        std::is_same_v<std::decay_t<T>,
                       std::vector<typename std::decay_t<T>::value_type>> ||
            std::is_same_v<std::decay_t<T>,
                           std::list<typename std::decay_t<T>::value_type>>,
        bool> = true>
void print_ip(T&& ip) {
    for (auto it = ip.begin(); it != ip.end(); ++it) {
        if (it != ip.begin()) {
            std::cout << '.';
        }
        std::cout << *it;
    }
    std::cout << std::endl;
}

template <
    typename T,
    std::enable_if_t<std::is_same_v<std::decay_t<T>, std::string>, bool> = true>
void print_ip(T&& ip) {
    std::cout << ip << std::endl;
}

template <typename T>
struct is_tuple_impl : std::false_type {};

template <typename... U>
struct is_tuple_impl<std::tuple<U...>> : std::true_type {};

template <typename T>
constexpr bool is_tuple() {
    return is_tuple_impl<std::decay_t<T>>::value;
}

template <typename First, typename... T>
struct all_same_type_tuple {
    constexpr static bool value =
        std::is_same_v<std::tuple<std::decay_t<First>, std::decay_t<T>...>,
                       std::tuple<std::decay_t<T>..., std::decay_t<First>>>;
};

template <typename... T>
struct all_same_type_tuple<std::tuple<T...>> : all_same_type_tuple<T...> {};

template <typename... T>
constexpr bool all_same_type_tuple_v = all_same_type_tuple<T...>::value;

template <typename T, size_t... I>
void print_tuple_ip(T&& ip, std::index_sequence<I...>) {
    (..., (std::cout << (I == 0 ? "" : ".") << std::get<I>(ip)));
    std::cout << std::endl;
}

template <typename T, std::enable_if_t<is_tuple<T>(), bool> = true>
void print_ip(T&& ip) {
    static_assert(all_same_type_tuple_v<T>);
    print_tuple_ip(
        std::forward<T>(ip),
        std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>());
}

int main() {
    print_ip(int8_t{-1});
    print_ip(int16_t{0});
    print_ip(int32_t{2130706433});
    print_ip(int64_t{8875824491850138409});
    print_ip(std::string{"Hello, world!"});
    print_ip(std::vector<int>{100, 200, 300, 400});
    print_ip(std::list<short>{400, 300, 200, 100});
    print_ip(std::make_tuple(123, 456, 789, 0));
}