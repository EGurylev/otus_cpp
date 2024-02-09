#pragma once
#include <stddef.h>

template <typename T, size_t N>
class pool_allocator {
   public:
    using value_type = T;
    using pointer = T*;

    pool_allocator() noexcept {
        static_assert(N > 0);

        start = static_cast<pointer>(::operator new(N * sizeof(T)));
        current = start;
        capacity = N;
    }

    pool_allocator(const pool_allocator& other) = delete;
    pool_allocator(const pool_allocator&& other) = delete;

    pointer allocate(std::size_t n) {
        const auto result{current};
        if ((start + capacity) - current >= static_cast<long>(n)) {
            for (size_t i{}; i < n; ++i) {
                ++current;
            }
        } else {
            throw std::bad_alloc();
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
    pointer start{};
    pointer current{};
    size_t capacity{};
};