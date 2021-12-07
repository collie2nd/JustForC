project(SystemV_SharedMemory C)

set(CMAKE_C_STANDARD 99)

add_executable(Write.out src/write/shm_write.c)

add_executable(Read.out src/read/shm_read.c)