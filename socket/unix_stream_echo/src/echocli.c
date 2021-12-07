//#include <arpa/inet.h>
//#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>

#define ERR_EXIT(m)\
        do\
        {\
            perror(m);\
            exit(EXIT_FAILURE);\
        } while(0)

int sock;

//初始化TCP连接
void init();

//通过初始化得到的套接字进行通信
void echo_cli();

int main(void) {
    init();
    echo_cli();
    return 0;
}

void init() {

    //第一步：创建一个套接字socket(协议族，套接字类型，协议类型常量或0
    if ((sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)
        ERR_EXIT("socket");

    //第二步：初始化目的套接字地址，并做字节处理，转换为网络字节序
    struct sockaddr_un servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, "/tmp/test_socket");

    //第三步：调用连接函数，进行连接listen(未连接套接字，目的套接字地址，前者长度）
    if (connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("connect");
}


void echo_cli() {
    //第四步：前者未连接套接字变成已连接状态，通过其进行通信
    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};
    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL) {
        write(sock, sendbuf, strlen(sendbuf));      //unistd.h
        read(sock, recvbuf, sizeof(recvbuf));       //unistd.h
        fputs(recvbuf, stdout);
        bzero(sendbuf, sizeof(sendbuf));
        bzero(recvbuf, sizeof(recvbuf));
    }
    close(sock);
}