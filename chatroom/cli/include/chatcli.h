//
// Created by Vousmevoyez 刘 on 2019-05-24.
//

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m)\
    do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
} while(0)

#define MSGLENGTH 1024

typedef struct sockaddr SA;

// function init初始化开启客户端
void init();

// function start对一个用户名为usrname的用户开启一个客户端
// param    usrname表示传入的用户名char型指针
void start(char *usrname);
