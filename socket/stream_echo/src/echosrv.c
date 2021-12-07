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

int conn;


void init();

void start();

int main(void)
{
    init();
    start();
	return 0;
}

void init(){

    //第一步：创建一个套接字socket(协议族，套接字类型，协议类型常量或0
    int listenfd;
    if ( (listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        ERR_EXIT("socket");
    /* POSIX规范指定socket函数的第一个参数为PF_值，
        * 而AF_值用于套接字地址结构，如下 */

//第二步：初始化监听套接字地址，并做字节处理，转换为网络字节序
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* htons 16位主机字节序转换为16位网络字节序
     * htonl 32位主机字节序转换为32位网络字节序
     * ntohs 16位网络字节序转换为16位主机字节序
     * ntohl 32位网络字节序转换为32位主机字节序*/

    /* servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
     * inet_aton("127.0.0.1",$servaddr.sin_addr);
     *
     * inet_aton 字符串转换为32位网络字节序，成功返回给第二个参数所指的指针
         * inet_addr 字符串转换为32位网络字节序，并返回给in_addr_t
         * inet_ntoa 32位网络字节序转换为字符串，并返回给char*        */

    int on = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ERR_EXIT("setsockopt");

//第三步：绑定该地址，将本地某个IP地址和端口号绑定到指定监听套接字地址结构
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("bind");
    /* bind(socket函数生成的套接字，要绑定的地址结构，地址长度
     * 其作用类似于赋值，可以不绑定，也可以只绑定部分，IP or PORT */

//第四步：将被动/监听套接字从CLOSED状态转换到LISTEN状态，用来接受请求
    if (listen(listenfd, SOMAXCONN) < 0)
        ERR_EXIT("listen");

//第五步：接受请求，从已完成连接队列中的队头返回第一个连接，如果为空，则阻塞
    struct sockaddr_in peeraddr;
    socklen_t peerlen = sizeof(peeraddr);

    if ((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen))<0)
        ERR_EXIT("accept");
    /* accept()函数成功返回一个非负整数，表示创建了一个新的套接字（conn）
     * (conn)为主动套接字，不能用来接受连接，区别于listenfd
     * 失败返回-1，成功返回已连接套接字描述符
     * 第一个参数为监听套接字描述符（由socket函数创建
     * 随后用作bind函数和listen函数的第一个参数的描述符）
     * 第二个参数cliaddr*，与第三个参数addrlen*用来返回
     * 已连接的对端进程（客户）的协议地址，addrlen是值-结果参数
     * 值-结果参数必须进行初始化
     * 如果对客户身份不感兴趣，可以将后两个参数置空		*/
}

void start(){

    pack recvbuf;
//    char recvbuf[1024];
    int n;

    while(1) {
        memset(&recvbuf, 0, sizeof(recvbuf));

        int ret = readn(conn, &recvbuf.len, 4);
        if (ret == -1)
            ERR_EXIT("readn");
        else if (ret < 4){
             printf("client close\n");
             break;
         }

        n = ntohl(recvbuf.len);

        ret = readn(conn, recvbuf.buf, n);
        if (ret == -1)
            ERR_EXIT("readn");
        else if (ret < n){
            printf("client close\n");
            break;
        }

        fputs( recvbuf.buf, stdout);
        writen(conn, &recvbuf, 4+n);
    }
}