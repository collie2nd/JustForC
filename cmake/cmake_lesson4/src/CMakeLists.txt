include_directories(../lib)

set(CMAKE_APPBUNDLE_PATH main.c)

add_executable(hello.out ${CMAKE_APPBUNDLE_PATH})

target_link_libraries(hello.out libhello)