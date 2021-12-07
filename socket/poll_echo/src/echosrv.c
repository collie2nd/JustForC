#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <poll.h>
#include "../include/buf_handler.h"

void start();

void handler_sigchld(int sig);

int main(void) {
    start();
    return 0;
}

void start() {

//    signal(SIGCHLD, SIG_IGN);
    signal(SIGCHLD, handler_sigchld);
    //第一步：创建一个套接字socket(协议族，套接字类型，协议类型常量或0
    int listenfd;
    if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        ERR_EXIT("socket");

//第二步：初始化监听套接字地址，并做字节处理，转换为网络字节序
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int on = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ERR_EXIT("setsockopt");

//第三步：绑定该地址，将本地某个IP地址和端口号绑定到指定监听套接字地址结构
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("bind");

//第四步：将被动/监听套接字从CLOSED状态转换到LISTEN状态，用来接受请求
    if (listen(listenfd, SOMAXCONN) < 0)
        ERR_EXIT("listen");

//第五步：接受请求，从已完成连接队列中的队头返回第一个连接，如果为空，则阻塞
    struct sockaddr_in peeraddr;
    socklen_t peerlen;


    struct pollfd client[2048]; //poll()函数没有FD_SETSIZE限制，并且要用struct pollfd结构体来保存客户端的信息
    int i;
    int maxindex = 0;
    int count = 0;
    for (i = 0; i < 2048; ++i)
        client[i].fd = -1;      //对fd置为-1，表示为空闲状态
    int conn;
    int nready;
//  int maxfd = listenfd;   无需维护最大描述字
//  fd_set rset;
//  fd_set allset;
//  FD_ZERO(&rset);
//  FD_ZERO(&allset);
//  FD_SET(listenfd, &allset);
    client[0].fd = listenfd;    //第一个要添加到poll()中的为监听套接口
    client[0].events = POLLIN;  //可读事件用POLLIN，可写事件为POLLOUT
    while (1) {
//      rset = allset;
        nready = poll(client, maxindex + 1, -1);    //第一个参数为数组，第二个参数为数组内部个数，第三个参数为超时时间，置为-1表示永远等待，直到有事件发生才返回
//      nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (nready == -1) {     //等于-1表示出错，等于0表示超时，和select()函数一样
            if (errno == EINTR)
                continue;
//            ERR_EXIT("select");
            ERR_EXIT("poll");
        } else if (0 == nready)
            continue;
        if (client[0].revents & POLLIN) {           //检测是否有事件发生，如果返回了POLLIN，则表示产生了可读事件
//      if (FD_ISSET(listenfd, &rset)) {
            peerlen = sizeof(peeraddr);
            conn = accept(listenfd, (struct sockaddr *) &peeraddr, &peerlen);
            if (conn == -1)
                ERR_EXIT("accept");

            for (i = 0; i < 2048; ++i) {
                if (client[i].fd < 0) {
                    client[i].fd = conn;
                    if (i > maxindex)
                        maxindex = i;
                    break;
                }
            }
            if (i == 2048) {
                fprintf(stderr, "too many clients\n");
                exit(EXIT_FAILURE);
            }
            printf("IP:%s,PORT:%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
//            FD_SET(conn, &allset);
            printf("count = %d\n", ++count);
//            if (conn > maxfd)
//                maxfd = conn;
            client[i].events = POLLIN;

            if (--nready <= 0)
                continue;
        }
        for (i = 0; i <= maxindex; ++i) {
            conn = client[i].fd;
            if (conn == -1)
                continue;
            if (client[i].revents & POLLIN) {
//            if (FD_ISSET(conn, &rset)) {
                char recvbuf[1024] = {0};
                int ret = readline(conn, recvbuf, sizeof(recvbuf));
                if (ret == -1)
                    ERR_EXIT("readline");
                else if (0 == ret) {
                    printf("client close\n");
//                    FD_CLR(conn, &allset);
                    client[i].fd = -1;
                    close(conn);
                }
                fputs(recvbuf, stdout);
                writen(conn, recvbuf, strlen(recvbuf));

                if (--nready <= 0)
                    break;
            }
        }
    }
}

void handler_sigchld(int sig) {
    wait(NULL);
}
