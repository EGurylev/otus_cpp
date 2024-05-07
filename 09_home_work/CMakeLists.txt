add_executable(main_async main.cpp)
add_library(async SHARED async.cpp async_bulk.cpp async_loggers.cpp)

target_link_libraries(async PRIVATE bulk_lib)

target_link_libraries(main_async PRIVATE async)

target_include_directories(async
    PRIVATE "${CMAKE_SOURCE_DIR}/07_home_work"
)

set_target_properties(async PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(main_async PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(async PRIVATE
        -Wall -Wextra -pedantic -Werror
)

target_compile_options(main_async PRIVATE
        -Wall -Wextra -pedantic -Werror
)

install(TARGETS async RUNTIME DESTINATION lib)