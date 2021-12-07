cmake_minimum_required(VERSION 3.10.2)
project(socket_lesson16 C)

set(CMAKE_C_STANDARD 99)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(SERVICE_SRC src/echosrv.c)

set(CLIENT_SRC src/echocli.c)

add_executable(service.out ${SERVICE_SRC})

add_executable(client.out ${CLIENT_SRC})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Release)