cmake_minimum_required(VERSION 3.10.2)
project(Unix_chatroom C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(srv)
add_subdirectory(cli)