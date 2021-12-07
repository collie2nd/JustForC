//
// Created by liuxing1 on 5/27/19.
//

#include <stdlib.h>
#include <stdio.h>

#define ERR_EXIT(m)\
    do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
} while(0)

//#define INVALID_SOCKET -1

typedef struct _Client {
    int conn;
    char usrname[1024];
    char IP[20];
    unsigned short port;
    struct _Client *next;
} Client, *pClient;

/*
* function  初始化链表
*/
void InitLink();

/*
* function  获取头节点
* return    返回头节点
*/
pClient GetHeadNode();

/*
* function  添加一个客户端
* param     client表示一个客户端对象
*/
void AddClient(pClient client);

/*
* function  删除一个客户端
* param     flag标识一个客户端对象
* return    返回true表示删除成功，false表示失败
*/
void RemoveClient(int flag);

/*
* function  根据套接字查找指定客户端
* param     fd是指定客户端的连接套接字描述符
* return    返回一个Client指针表示查找成功，返回NULL表示无此用户
*/
pClient FindClient(int fd);

/*
* function  计算客户端连接数
* param     client表示一个客户端对象
* return    返回连接数
*/
int CountConn();

/*
* function  清空链表
*/
void ClearClient();

/*
* function  检查连接状态并关闭一个连接
*/
//void CheckConnection();
