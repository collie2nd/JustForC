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
//    msgid = msgget(13164, 0666);  //0666：拥有者具有读写权限，读用户具有读写权限，其他用户具有读写权限
//    msgid = msgget(13164, 0666 | IPC_CREAT);  //若key所代表的消息队列不存在，则会返回No such file or directory，msgflg要指定为IPC_CREAT
//    msgid = msgget(13164, 0666 | IPC_CREAT | IPC_EXCL);   //若创建的是，已经存在的消息队列，增加IPC_EXCL会使得返回一个File exists的错误
//    msgid = msgget(IPC_PRIVATE, 0666);//KEY可以为IPC_PRIVATE，代指创建一个新的消息队列，每次调用，产生的key值都不同
    msgget(1234, 0666 | IPC_CREAT);
    if (msgid == -1)
        ERR_EXIT("msgget");
    printf("msgget success!\n");
    printf("msgid = %d\n", msgid);
    return 0;

//    ipcs命令可查看系统当前消息队列
//    ipcrm命令可以删除指定的消息队列，-Q对应的key,-q对应的msgid
}