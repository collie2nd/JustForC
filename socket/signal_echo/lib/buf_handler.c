//
// Created by liuxing1 on 5/28/19.
//


#include "../include/buf_handler.h"

ssize_t readn(int fd, void *buf, size_t count) {

    size_t nleft = count;           //表示待接收字节数
    ssize_t nread;                  //表示接收到的字节数
    char *bufp = (char *) buf;

    while (nleft > 0) {
        if ((nread = read(fd, bufp, nleft)) < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        } else if (0 == nread) {
            return count - nleft;
        }
        bufp += nread;
        nleft -= nread;
    }
    return count;
}

ssize_t writen(int fd, void *buf, size_t count) {

    size_t nleft = count;           //表示剩余待发送的字节数
    ssize_t nwrite;                 //表示已经发送的字节数
    char *bufp = (char *) buf;

    while (nleft > 0) {
        if ((nwrite = write(fd, bufp, nleft)) < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        } else if (0 == nwrite)
            continue;

        bufp += nwrite;
        nleft -= nwrite;
    }
    return count;
}

ssize_t recv_peek(int sockfd, void *buf, size_t len) {
    while (1) {
        int ret = recv(sockfd, buf, len, MSG_PEEK);
        if (ret == -1 && errno == EINTR)
            continue;
        return ret;
    }
}

ssize_t readline(int sockfd, void *buf, size_t maxline){
    int ret;
    int nread;
    char *bufp = buf;
    int nleft = maxline;
    while (1){
        ret = recv_peek(sockfd, bufp, nleft);
        if(ret<0)
            return ret;
        else if(0 == ret)
            return ret;
        nread = ret;
        int i;
        for(i=0;i<nread;++i){
            if(bufp[i]=='\n'){
                ret = readn(sockfd,bufp,i+1);
                if (ret != i+1)
                    exit(EXIT_FAILURE);
                return ret;
            }
        }
        if (nread >nleft)
            exit(EXIT_FAILURE);
        nleft -= nread;
        ret = readn(sockfd, bufp, nread);
        if (ret != nread)
            exit(EXIT_FAILURE);
        bufp+=nread;

    }
    return -1;
}

//1ssize_t send_peek(int sockfd, void *buf, size_t len) {
//
//}