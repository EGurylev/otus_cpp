cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(otus_homeworks VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(01_home_work)
add_subdirectory(02_home_work)
add_subdirectory(03_home_work)
add_subdirectory(04_home_work)
add_subdirectory(05_home_work)
add_subdirectory(06_home_work)
add_subdirectory(07_home_work)
add_subdirectory(08_home_work)
add_subdirectory(09_home_work)
add_subdirectory(10_home_work)

enable_testing()
add_subdirectory(tests)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT evgeniy.gurilev@gmail.com)
set(CPACK_PACKAGE_NAME print_ip)

include(CPack)