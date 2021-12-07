//
// Created by liuxing1 on 6/25/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

#define ERR_EXIT(m)\
    do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
} while(0)

typedef struct msgbuf {
    long mtype;
    char mtext[1];
} msgbuf;


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage:%s <bytes> <type>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int len = atoi(argv[1]);
    int type = atoi(argv[2]);
    int msgid;
    msgid = msgget(1234, 0);
    if (msgid == -1)
        ERR_EXIT("msgget");

    msgbuf *bufp;
    bufp = (msgbuf *) malloc(sizeof(msgbuf));
    bufp->mtype = type;
    if (msgsnd(msgid, bufp, len, IPC_NOWAIT) < 0)
        ERR_EXIT("msgsnd");
    return 0;
}

//往消息队列中发送一条消息
//int msgsnd(int msgid, const void *msgp,size_t msgsz, int msgflg);

/******************************************
参数：
    msgid:由msgget函数返回的消息队列标识码
    msgp:是一个指针，指向准备发送的消息
    msgsz:是msgp指向的消息长度，这个长度不含保存消息类型的那个long int长整型
        消息结构在两方面受制约，一方面，必须小于系统规定的上限；其次，必须以一个长整型开始，接收者函数将利用这个长整型确定消息的类型。
    msgflg:控制着当前消息队列满或达到系统上限时要做的处理
        = IPC_NOWAIT表示队列满不等待，返回EAGAIN错误，填0则队列满时已阻塞的方式等待

返回值：
    成功返回0，失败返回-1
*******************************************/
