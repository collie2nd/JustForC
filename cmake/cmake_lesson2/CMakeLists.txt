cmake_minimum_required(VERSION 3.14)
project(cmake_lesson2 C)

set(CMAKE_C_STANDARD 99)

set(SRC_LIST
        main.c
        hello.c)

add_executable(hello.out ${SRC_LIST})