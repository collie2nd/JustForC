project(cJson C)

set(CMAKE_C_STANDARD 99)

add_executable(print.out src/main.c src/cJSON.c src/cJSON.h)

ADD_EXECUTABLE(demo1.out src/demo1.c src/cJSON.c src/cJSON.h)

ADD_EXECUTABLE(demo2.out src/demo2.c src/cJSON.c src/cJSON.h)