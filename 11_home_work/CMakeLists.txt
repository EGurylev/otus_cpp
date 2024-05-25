find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_library(database database.cpp database_command_processor.cpp)
target_link_libraries(database PRIVATE ${Boost_LIBRARIES})

set_target_properties(database PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(database PRIVATE
        -Wall -Wextra -pedantic -Werror
)

add_executable(join_server main.cpp join_server.cpp)

target_link_libraries(join_server PRIVATE database ${Boost_LIBRARIES})

set_target_properties(join_server PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(join_server PRIVATE
        -Wall -Wextra -pedantic -Werror
)

install(TARGETS join_server RUNTIME DESTINATION bin)