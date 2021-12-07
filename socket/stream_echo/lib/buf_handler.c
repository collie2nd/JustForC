//
// Created by liuxing1 on 5/28/19.
//

#include "../include/buf_handler.h"

ssize_t readn(int fd, void *buf,size_t count) {
    
    size_t nleft = count;           //表示待接收字节数
    ssize_t nread;                  //表示接收到的字节数
    char *bufp = (char*)buf;

    while(nleft > 0){
        if ((nread = read(fd, bufp, nleft))< 0){
            if (errno == EINTR)
                continue;
            return -1;
        } else if (0 == nread){
            return count - nleft;
        }
        bufp  += nread;
        nleft -=nread;
    }
    return count;
}

ssize_t writen(int fd, void *buf,size_t count){

    size_t nleft = count;           //表示剩余待发送的字节数
    ssize_t nwrite;                 //表示已经发送的字节数
    char *bufp = (char*)buf;

    while(nleft > 0){
        if ((nwrite = write(fd, bufp, nleft))< 0){
            if (errno == EINTR)
                continue;
            return -1;
        } else if (0 == nwrite)
            continue;
        
        bufp  += nwrite;
        nleft -=nwrite;
    }
    return count;
}