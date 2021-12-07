#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>//包含了time()相关函数
#include "usrmgr.h"

#define ERR_EXIT(m)\
    do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
} while(0)//stdio.h+string.h
//#define SEND_OVER 1
//#define SEND_WAIT 0
#define MAXSIZE 10
#define MSGLENGTH 1024
typedef struct sockaddr SA;
typedef struct sockaddr_in SOCKET;


//function	初始化服务器连接
void init();

//function	用户登录
//param		fd表示该用户的套接字描述符
//param		peeraddr表示登录连接对端客户端的SOCKET
void login(int fd, SOCKET peeraddr);

//function	将消息发送给其他客户端
//param		buf表示消息缓存，thisfd表示当前客户端的套接字描述符
void SendMsgToOthers(char *buf, int thisfd);

//function	接受和发送消息
//param		pthread_create函数第三个参数的标准格式
void *chat(void *arg);

//function	启动服务器
void service();
