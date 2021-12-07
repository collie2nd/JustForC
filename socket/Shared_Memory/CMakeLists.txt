project(Shared_Memory C)

set(CMAKE_C_STANDARD 99)

add_executable(mmap_write.out src/write/mmap_write.c)

add_executable(mmap_read.out src/read/mmap_read.c)