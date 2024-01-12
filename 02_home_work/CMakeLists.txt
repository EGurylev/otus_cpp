add_executable(ip_filter ip_filter.cpp ip_adress_pool.cpp)
add_library(ip_filter_lib ip_adress_pool.cpp)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)