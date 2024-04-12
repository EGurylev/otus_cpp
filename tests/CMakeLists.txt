add_executable(unittest test_version.cpp test_ip_adress.cpp
    test_allocator.cpp test_matrix.cpp test_bulk.cpp test_bayan.cpp)

target_compile_options(unittest PRIVATE
        -Wall -Wextra -pedantic -Werror
)

set_target_properties(unittest PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
configure_file(test_data.h.in test_data.h)

target_include_directories(unittest
    PRIVATE "${CMAKE_SOURCE_DIR}/01_home_work"
    PRIVATE "${CMAKE_SOURCE_DIR}/02_home_work"
    PRIVATE "${CMAKE_SOURCE_DIR}/03_home_work"
    PRIVATE "${CMAKE_SOURCE_DIR}/06_home_work"
    PRIVATE "${CMAKE_SOURCE_DIR}/07_home_work"
    PRIVATE "${CMAKE_SOURCE_DIR}/08_home_work"
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(unittest helloworld ip_filter_lib bulk_lib bayan_lib)

add_subdirectory(googletest)

target_link_libraries(unittest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unittest)