add_executable(bulk bulk.cpp)

add_library(bulk_lib command_block.cpp command_processor.cpp loggers.cpp)

set_property(TARGET bulk_lib PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(bulk PRIVATE bulk_lib)

set_target_properties(bulk PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
)

install(TARGETS bulk RUNTIME DESTINATION bin)