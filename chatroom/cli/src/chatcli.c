//
// p2pchat.c
// C_Learning
//
// Created by Vousmevoyez Liu on 2019/5/12.
//
// Description:
// 点对点聊天 C/S模型 全双工通信聊天工具设计

#include "../include/chatcli.h"

//sockfd    为当前客户端和服务器连接生成的套接字，作为全局变量，以供调用套接字通信
int sockfd;

//init      开启客户端，创立和服务器的连接
void init() {
    if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        ERR_EXIT("socket");
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = inet_addr("172.30.1.200");           //连接到指定服务器，指定目的IP和端口号

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("connect");
    printf("client start!\n");
}

//start     开始处理客户端进程，通过init初始化生成的套接字进行通信
void start(char *usrname) {
    send(sockfd, usrname, strlen(usrname), 0);                      //发送用户名给服务器端
    pid_t pid;                                                      //创建父子进程分别进行接收消息和发送消息
    pid = fork();
    if (pid == -1) {
        ERR_EXIT("fork");
    } else if (pid == 0) {                                          //子进程循环接收消息
        char recvbuf[MSGLENGTH];
        while (1) {
            bzero(&recvbuf, sizeof(recvbuf));
            int ret = read(sockfd, recvbuf, sizeof(recvbuf));
            if (ret == -1) {
                ERR_EXIT("read");
            } else if (ret == 0) {
                printf("peer close\n");
                break;
            } else {
                fputs(recvbuf, stdout);
            }
        }
        printf("child close\n");                                    //若停止接收消息，则杀死子进程
        kill(getppid(), SIGKILL);                                   //并通知终止父进程，关闭客户端
        close(sockfd);
    } else {
        char sendbuf[MSGLENGTH];
        bzero(&sendbuf, sizeof(sendbuf));
        while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL) {    //当从标准输入接收到的字符不为空，则将数据发送给服务器
            write(sockfd, sendbuf, strlen(sendbuf));
            bzero(&sendbuf, sizeof(sendbuf));
        }
        kill(pid, SIGKILL);                                         //当停止发送数据后，立刻杀死子进程，停止接收服务器发来的消息
        printf("parent close\n");
        close(sockfd);
    }
}


