#include <gtest/gtest.h>

#include <set>

#include "pool_allocator.h"
#include "simple_forward_list.h"

TEST(PoolAllocatorTest, UseWithStandartContainers) {
    std::set<int, std::less<int>, pool_allocator<int, 10>> s;

    s.insert(1);
    s.insert(2);

    EXPECT_TRUE(s.size() == 2);

    s.erase(1);
    EXPECT_TRUE(s.size() == 1);
}

TEST(PoolAllocatorTest, UseWithCustomContainer) {
    simple_forward_list<double, pool_allocator<double, 10>>
        list_custom_allocator;
    EXPECT_TRUE(list_custom_allocator.size() == 0);
    list_custom_allocator.push_front(3.0);
    list_custom_allocator.push_front(4.0);

    EXPECT_TRUE(list_custom_allocator.size() == 2);
    EXPECT_THROW(
        for (int i{}; i < 10;
             ++i) { list_custom_allocator.push_front(static_cast<double>(i)); },
        std::bad_alloc);
}