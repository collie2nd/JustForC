//
// Created by liuxing1 on 6/26/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

void echo_cli(int msgid);

int main(void) {
//  第一步:打开key为1234的消息队列
    int msgid = msgget(1234, 0);
    if (msgid == -1)
        ERR_EXIT("msgget");

    echo_cli(msgid);

    return 0;
}

void echo_cli(int msgid) {
//  第二步:设置该客户端的buf.mtext,将客户端进程存放其中,以便通知给服务器消息的目的地
    int pid = getpid();
    msgbuf buf;
    memset(&buf, 0, sizeof(buf));
    *((int *) buf.mtext) = pid; //将字符数组指针强制类型转换成int型后,保存int pid,会以4个字节的形式保存在字符数组中

//  第三步:将输入字符保存到mtext4个字节位置之后,设置mtype为1,从而将消息发送给服务器
    while (fgets(buf.mtext + 4, MSGMAX, stdin) != NULL) {
        buf.mtype = 1;
        if (msgsnd(msgid, &buf, 4 + strlen(buf.mtext + 4), 0) < 0)
            ERR_EXIT("msgsnd");
        memset(&buf, 0, sizeof(buf));

//  第四步:接收msgType等于当前客户端pid的消息,输出mtext四个字节后的字符,即为服务器反馈的字符信息
        if (msgrcv(msgid, &buf, MSGMAX, pid, 0) < 0)
            ERR_EXIT("msgrcv");
        fputs(buf.mtext + 4, stdout);
        memset(buf.mtext + 4, 0, MSGMAX - 4);   //保留mtext前四个字节,标识本客户端
    }
}