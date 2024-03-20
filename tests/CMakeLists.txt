add_executable(unittest test_version.cpp test_ip_adress.cpp
    test_allocator.cpp test_matrix.cpp test_bulk.cpp)

target_compile_options(unittest PRIVATE
        -Wall -Wextra -pedantic -Werror
)

target_include_directories(unittest
    PRIVATE "${CMAKE_SOURCE_DIR}/01_home_work"
    PRIVATE "${CMAKE_SOURCE_DIR}/02_home_work"
    PRIVATE "${CMAKE_SOURCE_DIR}/03_home_work"
    PRIVATE "${CMAKE_SOURCE_DIR}/06_home_work"
    PRIVATE "${CMAKE_SOURCE_DIR}/07_home_work"
)
target_link_libraries(unittest helloworld ip_filter_lib bulk_lib)

add_subdirectory(googletest)

target_link_libraries(unittest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unittest)