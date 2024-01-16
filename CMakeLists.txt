cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(otus_homeworks VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_subdirectory(01_home_work)
add_subdirectory(02_home_work)
add_subdirectory(tests)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT evgeniy.gurilev@gmail.com)
set(CPACK_PACKAGE_NAME ip_filter)

include(CPack)

enable_testing()