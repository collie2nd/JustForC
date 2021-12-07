//
// Created by liuxing1 on 6/3/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include "apue.h"

#define ERR_EXIT(m)\
        do\
        {\
            perror(m);\
            exit(EXIT_FAILURE);\
        } while(0)


int main(void) {
    int sockfds[2];
    if (socketpair(PF_UNIX, SOCK_STREAM, 0, sockfds) < 0)
        ERR_EXIT("socketpair");
    pid_t pid;
    pid = fork();
    if (pid == -1)
        ERR_EXIT("fork");
    else if (0 == pid) {
        close(sockfds[0]);
        int fd;
        fd = open("../tmp/test.txt", O_RDONLY);
        if (fd == -1)
            ERR_EXIT("open");
        send_fd(sockfds[1], fd);
    } else {
        close(sockfds[1]);
        int fd = recv_fd(sockfds[0], NULL);
        char buf[1024] = {0};
        read(fd, buf, sizeof(buf));
        printf("%s", buf);
    }
    return 0;
}