//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "../include/buf_handler.h"

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
void start();


int main(void)
{
    init();
    start();
	return 0;
}

void init(){
    //第一步：创建一个套接字socket(协议族，套接字类型，协议类型常量或0
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        ERR_EXIT("socket");

//第二步：初始化目的套接字地址，并做字节处理，转换为网络字节序
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof( servaddr));
    /*bzero(&servaddr, sizeof( servaddr));*/

    servaddr.sin_family = AF_INET;//PF_INET包含AF_INET

    servaddr.sin_port = htons(5188);
    /* htons 16位主机字节序转换为16位网络字节序
     * htonl 32位主机字节序转换为32位网络字节序
     * ntohs 16位网络字节序转换为16位主机字节序
     * ntohl 32位网络字节序转换为32位主机字节序*/

    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    /* inet_aton 字符串转换为32位网络字节序，成功返回给第二个参数所指的指针
     * inet_addr 字符串转换为32位网络字节序，并返回给in_addr_t
     * inet_ntoa 32位网络字节序转换为字符串，并返回给char*        */

//第三步：调用连接函数，进行连接listen(未连接套接字，目的套接字地址，前者长度）
    if (connect(sock, (struct sockaddr*)&servaddr, sizeof( servaddr)) < 0)
        ERR_EXIT("connect");
    /* (SA *)&(某特定协议的套接字地址结构) = (struct sockaddr*)&(...)  */
    /* 套接字函数被定义为
     * 其中一个参数位指向某个通用套接字地址结构的一个指针
     * 所以这些函数的任何调用都必须要
     * 将指向特定协议的套接字地址的指针进行类型强制转换
     * 变成指向某个通用套接字地址结构的指针            */
}


void start(){
    //第四步：前者未连接套接字变成已连接状态，通过其进行通信
    pack sendbuf;
    pack recvbuf;

    memset(&sendbuf, 0, sizeof(sendbuf));
    memset(&recvbuf, 0, sizeof(recvbuf));
    int n;
//    char sendbuf[1024] = {0};
//    char recvbuf[1024] = {0};

    while (fgets(sendbuf.buf, sizeof(sendbuf.buf), stdin) != NULL)
    {
        n = strlen(sendbuf.buf);
        sendbuf.len = htonl(n);
        writen(sock, &sendbuf, 4+n);

        int ret = readn(sock, &recvbuf.len, 4);
        if (ret == -1)
            ERR_EXIT("readn");
        else if (ret < 4){
            printf("client close\n");
            break;
        }

        n = ntohl(recvbuf.len);

        ret = readn(sock, recvbuf.buf, n);
        if (ret == -1)
            ERR_EXIT("readn");
        else if (ret < n){
            printf("client close\n");
            break;
        }

        fputs(recvbuf.buf, stdout);
        memset(&sendbuf, 0, sizeof(sendbuf));
        memset(&recvbuf, 0, sizeof(recvbuf));
    }

    close(sock);
}