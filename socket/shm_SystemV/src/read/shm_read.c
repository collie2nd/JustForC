//
// Created by liuxing1 on 6/28/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>

#define ERR_EXIT(m)\
    do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
} while(0)

typedef struct stu {
    char name[32];
    int age;
} student;

int main() {
//  第一步：打开指定共享内存段
    int shmid = shmget(1234, 0, 0);
    if (shmid == -1)
        ERR_EXIT("shmget");

//  第二步：将共享内存段连接到进程地址空间，即可开始通信
    student *stup = shmat(shmid, NULL, 0);
    if (stup == (void *) -1)
        ERR_EXIT("shmat");

//  第三步：输出通信接收到的信息后，解除共享内存段和进程间的连接
    printf("name = %s , age = %d", stup->name, stup->age);


//  第四步：设置触发点，触发删除共享内存段
    memcpy(stup, "quit", 4);
    shmdt(stup);

    return 0;
}