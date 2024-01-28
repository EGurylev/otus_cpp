#pragma once
#include <stddef.h>

template <typename T, size_t N>
class pool_allocator {
   public:
    using value_type = T;
    using pointer = value_type*;

    pool_allocator() noexcept { relocate(N); }

    template <typename U>
    pool_allocator(const pool_allocator<U, N>&) noexcept {}

    pointer allocate(std::size_t n) {
        const auto result{current};
        if ((start + capacity) - current >= static_cast<long>(n)) {
            for (size_t i{}; i < n; ++i) {
                ++current;
            }
        }

        return result;
    }

    void deallocate(pointer, std::size_t n) {
        if (current - start >= static_cast<long>(n)) {
            for (size_t i{}; i < n; ++i) {
                --current;
            }
        }
    }

    template <typename U>
    struct rebind {
        using other = pool_allocator<U, N>;
    };

    ~pool_allocator() { ::operator delete(start); }

   private:
    void relocate(size_t new_capacity) {
        start = static_cast<pointer>(::operator new(new_capacity * sizeof(T)));
        current = start;
        capacity = new_capacity;
    }

    pointer start{};
    pointer current{};
    size_t capacity{};
};