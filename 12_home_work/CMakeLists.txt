find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(mapreduce main.cpp)

target_link_libraries(mapreduce PRIVATE ${Boost_LIBRARIES})

set_target_properties(mapreduce PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(mapreduce PRIVATE
        -Wall -Wextra -pedantic -Werror
)

install(TARGETS mapreduce RUNTIME DESTINATION bin)