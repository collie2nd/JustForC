//#include <arpa/inet.h>
//#include <signal.h>
//#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>


//stdlib.h+stdio.h
#define ERR_EXIT(m)\
        do\
        {\
            perror(m);\
            exit(EXIT_FAILURE);\
        } while(0)

int listenfd;

void init();

void start();

void echo_srv();

int main(void) {
    init();
    start();
    return 0;
}

void init() {
    //第一步：创建一个监听套接字 PF_UNIX,SOCK_STREAM Unix域的流式套接字
    if ((listenfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)
        ERR_EXIT("socket");

    //第二步：初始化监听套接字地址，绑定监听，这里不需要设置套接字地址重复利用
    unlink("/tmp/test_socket");                      //删除生成的socket文件，解除地址占用
    struct sockaddr_un servaddr;                //sys/un.h
    memset(&servaddr, 0, sizeof(servaddr));     //string.h
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, "/tmp/test_socket");

    //第三步：绑定该地址，将本地某个IP地址和端口号绑定到指定监听套接字地址结构
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("bind");

    //第四步：将被动/监听套接字从CLOSED状态转换到LISTEN状态，用来接受请求
    if (listen(listenfd, SOMAXCONN) < 0)        //SOMAXCONN监听队列最大值
        ERR_EXIT("listen");
}

void start() {
    //第五步：接受请求，从已完成连接队列中的队头返回第一个连接，如果为空，则阻塞
    int conn;
    pid_t pid;
    while (1) {
        conn = accept(listenfd, NULL, NULL);
        if (conn == -1) {
            if (conn == EINTR)                  //errno.h
                continue;
            ERR_EXIT("accept");
        }
        pid = fork();                           //unistd.h
        if (pid == -1)
            ERR_EXIT("fork");
        else if (pid == 0) {
            close(listenfd);
            echo_srv(conn);
            exit(EXIT_SUCCESS);
        }
        close(conn);
    }
}

void echo_srv(int conn) {
    char recvbuf[1024];
    while (1) {
        bzero(&recvbuf, sizeof(recvbuf));
        int n = read(conn, recvbuf, sizeof(recvbuf));
        if (n == -1) {
            if (n == EINTR)
                continue;
            ERR_EXIT("read");
        } else if (0 == n) {
            printf("client close\n");
            break;
        }
        fputs(recvbuf, stdout);
        write(conn, recvbuf, strlen(recvbuf));
    }
    close(conn);
}
