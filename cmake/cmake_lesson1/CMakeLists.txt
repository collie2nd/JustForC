cmake_minimum_required(VERSION 3.14)
project(Hello)

set(CMAKE_C_STANDARD 99)

set(SRC_LIST main.c)

add_executable(hello.out ${SRC_LIST})