set(CMAKE_LIBRARY_PATH hello.c)

add_library(libhello ${CMAKE_LIBRARY_PATH})

set_target_properties(libhello PROPERTIES OUTPUT_NAME "hello")