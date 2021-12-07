#include "../include/udp_echo.h"

void echo_srv(int sock);

int main() {
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        ERR_EXIT("socket");
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   //INADDR_ANY表示本机的任意一个IP地址
    //服务器端需要绑定本机的IP以及端口号，以便客户端能够找到对应的服务器
    if (bind(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("bind");

    echo_srv(sock);
    return 0;

}

void echo_srv(int sock) {
    char recvbuf[1024] = {0};
    struct sockaddr_in peeraddr;
    socklen_t peerlen;
    int n;
    while (1) {
        peerlen = sizeof(peeraddr);
        memset(recvbuf, 0, sizeof(recvbuf));
        //recvfrom()通过绑定的sock套接口接收数据，因为回射时需要直到客户端的地址，所以通过recvfrom最后两个输入输出参数返回发送数据过来的对等方的套接口地址
        n = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *) &peeraddr, &peerlen);
        if (n == -1) {
            if (errno == EINTR)
                continue;
            ERR_EXIT("recvfrom");
        } else if (n > 0) {
            fputs(recvbuf, stdout);
            //将发送过来的数据，通过返回的对等方套接口地址发送回去
            sendto(sock, recvbuf, n, 0, (struct sockaddr *) &peeraddr, peerlen);
        }
    }
    close(sock);
}