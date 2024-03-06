add_executable(matrix main.cpp)

set_target_properties(matrix PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(matrix PRIVATE
        -Wall -Wextra -pedantic -Werror
)

install(TARGETS matrix RUNTIME DESTINATION bin)