#include "../include/udp_echo.h"

void echo_cli(int sock, struct sockaddr_in servaddr);

int main() {
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        ERR_EXIT("socket");
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    //和服务器不同在于，客户端是主动方，需要知道服务器确定的IP地址和端口号，且不需要绑定自身的IP或是端口号
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    echo_cli(sock, servaddr);

    return 0;
}

void echo_cli(int sock, struct sockaddr_in servaddr) {
    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};
    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL) {
        //通过本机套接口将发送数据发送到服务器对应的套接口地址上，第一次调用sendto()时，sock会自动绑定一个本地地址
        sendto(sock, sendbuf, strlen(sendbuf), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
        //通过本机套接口接收数据，不关心是哪个对端发来的，因为实际也就是服务器端的套接口地址，等同于servaddr
        recvfrom(sock, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
        fputs(recvbuf, stdout);
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }
    close(sock);
}
