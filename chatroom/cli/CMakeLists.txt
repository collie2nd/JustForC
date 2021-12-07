include_directories(cli/include)

set(CLIENT_SRC include/chatcli.h
        src/chatcli.c
        main.c)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Release)

add_executable(client.out ${CLIENT_SRC})
