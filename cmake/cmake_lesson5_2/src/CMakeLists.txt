include_directories(${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_APPBUNDLE_PATH main.c)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(hello.out ${CMAKE_APPBUNDLE_PATH})

target_link_libraries(hello.out libhello)