cmake_minimum_required(VERSION 3.14)
project(cmake_lesson3 C)

set(CMAKE_C_STANDARD 99)

set(BUILD_SHARED_LIBS hello.c)

set(CMAKE_APPBUNDLE_PATH main.c)

add_library(libhello ${BUILD_SHARED_LIBS})

add_executable(hello.out ${CMAKE_APPBUNDLE_PATH})

target_link_libraries(hello.out libhello)

set_target_properties(libhello PROPERTIES OUTPUT_NAME "hello")