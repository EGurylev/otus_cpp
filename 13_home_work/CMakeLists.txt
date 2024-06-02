find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(fashio_mnist main.cpp)

target_link_libraries(fashio_mnist PRIVATE ${Boost_LIBRARIES})

set_target_properties(fashio_mnist PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(fashio_mnist PRIVATE
        -Wall -Wextra -pedantic -Werror
)

install(TARGETS fashio_mnist RUNTIME DESTINATION bin)