cmake_minimum_required(VERSION 3.14)
project(cmake_lesson5 C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(src bin)

add_subdirectory(lib)