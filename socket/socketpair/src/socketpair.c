#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

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
        int val;
        close(sockfds[0]);
        while (1) {
            read(sockfds[1], &val, sizeof(val));
            ++val;
            write(sockfds[1], &val, sizeof(val));
        }
    } else {
        int val = 0;
        close(sockfds[1]);
        while (1) {
            ++val;
            printf("sending data: %d\n", val);
            write(sockfds[0], &val, sizeof(val));
            read(sockfds[0], &val, sizeof(val));
            printf("data received: %d\n", val);
            sleep(1);
        }
    }
    return 0;
}