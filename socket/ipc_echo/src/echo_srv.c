//
// Created by liuxing1 on 6/25/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define ERR_EXIT(m)\
    do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
} while(0)

#define MSGMAX 8192

typedef struct msgbuf {
    long mtype;
    char mtext[MSGMAX];
} msgbuf;

void echo_srv(int msgid);

int main(void) {
//  第一步：创建一个KEY为1234的消息队列，保存其消息队列标识码，用这个消息队列进行消息传递
    int msgid = msgget(1234, IPC_CREAT | 0666);
    if (msgid == -1)
        ERR_EXIT("msgget");

    echo_srv(msgid);

    return 0;
}

void echo_srv(int msgid) {
//  第二步，接收msgType=1的消息，这类消息表示是发送给服务器的，其中，目的客户端信息保存在mtext的前四个字节中
    msgbuf buf;
    int n;
    while (1) {
        memset(&buf, 0, sizeof(buf));
        if ((n = msgrcv(msgid, &buf, MSGMAX, 1, 0)) < 0)
            ERR_EXIT("msgrcv");
//  第三步:输出传递的有用信息,将mtext前四个字节记录的目的客户端pid转换为整型保存在mtype,以便让指定客户端接收信息
        int pid = *((int *) buf.mtext);
//      fputs(buf.mtext + 4, stdout);
        printf("[PID%d]:%s", pid, buf.mtext + 4);
        buf.mtype = pid;
        msgsnd(msgid, &buf, n, 0);
    }
}