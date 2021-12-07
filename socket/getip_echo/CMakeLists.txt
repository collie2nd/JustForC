cmake_minimum_required(VERSION 3.10.2)
project(socket_lesson5 C)

set(CMAKE_C_STANDARD 99)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_library(getip include/getip_handler.h
        lib/getip_handler.c)

set(SRC src/getip.c)

add_executable(getip.out ${SRC})

target_link_libraries(getip.out getip)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Release)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/lib)