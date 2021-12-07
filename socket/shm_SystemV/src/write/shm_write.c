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

int main(int argc, char *argv[]) {
//  第一步：创建并打开指定共享内存
    int shmid = shmget(1234, sizeof(student), IPC_CREAT | 0666);
    if (shmid == -1)
        ERR_EXIT("shmget");

//  第二步：将共享内存段连接到进程地址空间，即可开始通信
    student *stup = shmat(shmid, NULL, 0);
    if (stup == (void *) -1)
        ERR_EXIT("shmat");
    strcpy(stup->name, "liuxing1");
    stup->age = 24;

//  第三步：触发机制被触发时，解除共享内存段和进程地址间的连接，并删除共享内存段
    while (1) {
        if (memcmp(stup, "quit", 4) == 0) {
            shmdt(stup);
            shmctl(shmid, IPC_RMID, NULL);
            break;
        }
    }

    return 0;
}