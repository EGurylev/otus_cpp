add_executable(run_allocator run_allocator.cpp)

set_target_properties(run_allocator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(run_allocator PRIVATE
        -Wall -Wextra -pedantic -Werror
)

install(TARGETS run_allocator RUNTIME DESTINATION bin)