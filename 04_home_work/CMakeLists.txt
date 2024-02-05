add_executable(print_ip print_ip.cpp)

set_target_properties(print_ip PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(print_ip PRIVATE
        -Wall -Wextra -pedantic -Werror
)

install(TARGETS print_ip RUNTIME DESTINATION bin)