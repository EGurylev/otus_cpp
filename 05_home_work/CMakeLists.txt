add_executable(editor editor.cpp)
add_library(editor_lib controller.cpp model.cpp)

target_link_libraries(editor editor_lib)
target_include_directories(editor_lib
    PRIVATE "${CMAKE_SOURCE_DIR}/05_home_work"
)

set_target_properties(editor editor_lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(editor PRIVATE
        -Wall -Wextra -pedantic -Werror
)

target_compile_options(editor_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
)

install(TARGETS editor RUNTIME DESTINATION bin)