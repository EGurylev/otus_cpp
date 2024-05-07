find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(bulk_server main.cpp bulk_server.cpp bulk_router.cpp)

target_link_libraries(bulk_server PRIVATE async ${Boost_LIBRARIES})

target_include_directories(bulk_server
    PRIVATE "${CMAKE_SOURCE_DIR}/09_home_work"
)

set_target_properties(bulk_server PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror
)

install(TARGETS bulk_server RUNTIME DESTINATION bin)