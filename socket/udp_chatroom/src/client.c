#include "../include/udp_chatroom.h"



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



    return 0;
}