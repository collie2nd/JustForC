#include <arpa/inet.h>
#include <signal.h>
#include "../include/buf_handler.h"

int sock;

//初始化TCP连接
void init();

//通过初始化得到的套接字进行通信
void start();

int main(void) {
    init();
    start();
    return 0;
}

void init() {
    signal(SIGPIPE, SIG_IGN);

    //第一步：创建一个套接字socket(协议族，套接字类型，协议类型常量或0
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        ERR_EXIT("socket");

    //第二步：初始化目的套接字地址，并做字节处理，转换为网络字节序
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
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
    if (connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("connect");
    /* (SA *)&(某特定协议的套接字地址结构) = (struct sockaddr*)&(...)  */
    /* 套接字函数被定义为
     * 其中一个参数位指向某个通用套接字地址结构的一个指针
     * 所以这些函数的任何调用都必须要
     * 将指向特定协议的套接字地址的指针进行类型强制转换
     * 变成指向某个通用套接字地址结构的指针            */

    struct sockaddr_in localaddr;
    socklen_t addrlen = sizeof(localaddr);
    if (getsockname(sock, (struct sockaddr *) &localaddr, &addrlen) < 0)
        ERR_EXIT("getsockname");
    printf("IP:%s , PROT:%d\n", inet_ntoa(localaddr.sin_addr), ntohs(localaddr.sin_port));

}


void start() {
    //第四步：前者未连接套接字变成已连接状态，通过其进行通信

    //select统一管理标准输入IO和套接口IO

    fd_set rset;            //定义一个IO集合出来
    FD_ZERO(&rset);         //将这个集合清空

    int nready, maxfd;      //nready:表示检测到的事件个数

    int fd_stdin = fileno(stdin);/* fileno()转化文件描述符
                                  * STD_FILENO等于0,不能保证标准输入stdio是否被重定向，
                                  * 如果被重定向后，有可能stdio的文件描述符并不是0     */
    //确定文件描述符的集合中的最大值
    if (fd_stdin > sock)
        maxfd = fd_stdin;
    else
        maxfd = sock;

    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};

    //死循环检测IO可读集合r(ead)set是否产生了可读事件
    while (1) {
        FD_SET(fd_stdin, &rset);    //将描述符加入到rset
        FD_SET(sock, &rset);        //不可外置，因为rset会在select()中变更为发生了事件的文件描述符，所以要重新添加到集合当中

        //maxfd+1的原因是select()遍历文件描述符的区间是[0,maxfd+1)
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);//timeout填NULL,表示不会超时，直到有事件到来才会返回
        if (nready == -1)
            ERR_EXIT("select");
        else if (0 == nready)       //此处不太可能等于0,因为没有指定超时时间
            continue;

        //此处nready（检测到的事件）可能等于1,也可能等于2，需要判定，stdio/sock哪个描述符检测到了事件
        //rset为输入输出参数，检测描述符是否在集合中，就知道是否检测到了事件FD_ISSET()
        //如果套接口检测到了可读事件，那么进行对应的IO操作
        if (FD_ISSET(sock, &rset)) {
            int ret = readline(sock, &recvbuf, sizeof(recvbuf));
            if (ret == -1)
                ERR_EXIT("readline");
            else if (0 == ret) {
                printf("service close!\n");
                break;
            }
            fputs(recvbuf, stdout);
            memset(recvbuf, 0, sizeof(recvbuf));
        }

        //如果标准输入检测到了可读事件，那么进行对应的IO操作
        if (FD_ISSET(fd_stdin, &rset)) {
            //如果为null指客户端得到了一个EOF(结束符)，则break出循环，否则将从标准输入获取到的一行数据发送给服务器端
            if (fgets(sendbuf, sizeof(sendbuf), stdin) == NULL)
                break;
            writen(sock, sendbuf, strlen(sendbuf));
            memset(sendbuf, 0, sizeof(sendbuf));
        }
    }
    close(sock);
}