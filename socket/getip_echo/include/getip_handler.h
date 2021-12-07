//
// Created by liuxing1 on 5/28/19.
//

#ifndef ECHO_STREAM_BUF_HANDLER_H
#define ECHO_STREAM_BUF_HANDLER_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>//read(),write()
#include <netdb.h>//gethostname()
#include <arpa/inet.h>
#include <sys/socket.h>

#define ERR_EXIT(m)\
    do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
} while(0)

typedef struct hostent hostent, *hostentp;

void getiplist();

int getlocalip(char *ip);

#endif //ECHO_STREAM_BUF_HANDLER_H