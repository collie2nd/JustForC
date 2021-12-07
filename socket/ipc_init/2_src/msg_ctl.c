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

int main(void) {
    int msgid;
    msgid = msgget(1234, 0);    //0表示仅打开指定消息队列
    if (msgid == -1)
        ERR_EXIT("msgget");
    printf("msgget success!\n");
    printf("msgid = %d\n", msgid);


//    msgctl(msgid, IPC_RMID, NULL); //IPC_RMID表示删除对应消息队列


    struct msqid_ds buf;
    msgctl(msgid, IPC_STAT, &buf);  //获取当前消息队列的状态，保存到msqid_ds结构体中
    printf("mode = %o\n", buf.msg_perm.mode);   //输出消息队列的权限值
    printf("bytes = %ld\n", buf.__msg_cbytes);  //输出消息队列当前字节数
    printf("number = %ld\n", buf.msg_qnum);     //输出消息队列当前消息的个数
    printf("msgmnb = %ld\n", buf.msg_qbytes);   //输出消息队列能容纳的字节总数


    sscanf("666", "%o", (unsigned int *) &buf.msg_perm.mode);
    msgctl(msgid, IPC_SET, &buf);   //修改buf后，通过IPC_SET设置消息队列的属性值

    msgctl(msgid, IPC_STAT, &buf);
    printf("mode = %o\n", buf.msg_perm.mode);

    return 0;
}