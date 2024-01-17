add_executable(ip_filter ip_filter.cpp)
add_library(ip_filter_lib ip_adress_pool.cpp)
target_link_libraries(ip_filter PRIVATE ip_filter_lib)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
)

install(TARGETS ip_filter RUNTIME DESTINATION bin)