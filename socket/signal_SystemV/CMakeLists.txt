project(SystemV_Sem C)

set(CMAKE_C_STANDARD 99)

add_executable(print.out src/sem_print.c src/sem_op.h)

add_executable(dinner.out src/sem_dinner.c src/sem_op.h)