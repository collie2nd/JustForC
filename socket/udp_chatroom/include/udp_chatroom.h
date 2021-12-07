//
// Created by liuxing1 on 5/31/19.
//

#ifndef SOCKET_LESSON6_UDP_CHATROOM_H
#define SOCKET_LESSON6_UDP_CHATROOM_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ERR_EXIT(m) \
        do  \
        {   \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while(0)



#endif //SOCKET_LESSON6_UDP_CHATROOM_H
