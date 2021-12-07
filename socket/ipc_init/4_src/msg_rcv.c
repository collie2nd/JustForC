//
// Created by liuxing1 on 6/25/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <getopt.h>

#define ERR_EXIT(m)\
    do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
} while(0)

#define MSGMAX 8192

typedef struct msgbuf {
    long mtype;
    char mtext[1];
} msgbuf;


int main(int argc, char *argv[]) {
    int flag = 0, type = 0, opt;
    while (1) {
        opt = getopt(argc, argv, "nt:");
        if (opt == '?')
            exit(EXIT_FAILURE);
        if (opt == -1)
            break;

        switch (opt) {
            case 'n':
                flag |= IPC_NOWAIT; //设置-n非阻塞形式接收消息
                break;
            case 't':
                type = atoi(optarg);//接收指定的消息类型 -t + (tpyeID)
                break;
        }
    }

    int msgid;
    msgid = msgget(1234, 0);
    if (msgid == -1)
        ERR_EXIT("msgget");

    msgbuf *bufp;
    bufp = (msgbuf *) malloc(sizeof(sizeof(long) + MSGMAX));
    bufp->mtype = type;
    int n;
    if ((n = msgrcv(msgid, bufp, MSGMAX, type, flag)) < 0)
        ERR_EXIT("msgrcv");

    printf("read %d bytes, type = %ld\n", n, bufp->mtype);
    return 0;
}

//从消息队列中接收一条消息
//ssize_t msgrcv(int msgid, void *msgp, size_t msgsz, long msgtyp, int msgflg);

/******************************************
参数：
    msgid:由msgget函数返回的消息队列标识码
    msgp:是一个指针，指向准备接收的消息
    msgsz:是msgp指向的消息长度，这个长度不含保存消息类型的那个long int长整型
    msgtyp:它可以实现接收优先级的简单形式
        =0:返回队列第一条消息
        >0:返回队列第一条类型等于msgtype的消息
        <0:返回队列第一条类型小于等于msgtype绝对值的消息
    msgflg:控制着队列中没有相应类型的消息可供接收时将要发生的事
        =IPC_NOWAIT:队列没有可读消息时不等待，返回ENOMSG错误
        =MSG_NOERROR:消息大小超过msgsz时被截断
        =MSG_EXCEPT(msgtype>0):接收类型不等于msgtype的第一条消息

返回值：
    成功返回实际放到接收缓冲区里去的字符个数，失败返回-1
*******************************************/
