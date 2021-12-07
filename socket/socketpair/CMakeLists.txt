cmake_minimum_required(VERSION 3.10.2)
project(socket_lesson16 C)

set(CMAKE_C_STANDARD 99)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(1_SRC src/socketpair.c)

add_executable(socketpair.out ${1_SRC})

set(2_SRC src/send_fd.c)

add_executable(sendfd.out ${2_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Release)

target_link_libraries(sendfd.out libapue.a)