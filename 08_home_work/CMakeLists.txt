find_package(Boost COMPONENTS program_options filesystem regex REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(bayan main.cpp)

add_library(bayan_lib file_handler.cpp file_group.cpp)
target_link_libraries(bayan_lib ${Boost_LIBRARIES})

target_link_libraries(bayan PRIVATE bayan_lib ${Boost_LIBRARIES})

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(bayan_lib PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
)

install(TARGETS bayan RUNTIME DESTINATION bin)