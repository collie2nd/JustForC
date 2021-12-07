set(CMAKE_LIBRARY_PATH hello.c)

add_definitions("-DLIBHELLO_BUILD")

add_library(libhello SHARED ${CMAKE_LIBRARY_PATH})

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set_target_properties(libhello PROPERTIES OUTPUT_NAME "hello")