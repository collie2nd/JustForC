//
// Created by liuxing1 on 7/1/19.
//

#ifndef SYSTEMV_SEM_SEM_OP_H
#define SYSTEMV_SEM_SEM_OP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>
//#include <sys/shm.h>
//#include <fcntl.h>
//#include <unistd.h>

#define ERR_EXIT(m)\
    do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
} while(0)

//union semun {
//    int val;
//    struct semid_ds *buf;
//    unsigned short *array;
//    struct seminfo *_buf;
//};

//创建一个信号量集
int sem_create(key_t key) {
    int semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid == -1)
        ERR_EXIT("semget");
    return semid;
}

//打开一个信号量集
int sem_open(key_t key) {
    int semid = semget(key, 0, 0);
    if (semid == -1)
        ERR_EXIT("semget");
    return semid;
}

//设置信号量集中的初始值
int sem_setval(int semid, int val) {
    union semun su;
    su.val = val;
    int ret = semctl(semid, 0, SETVAL, su);
    if (ret == -1)
        ERR_EXIT("semctl");
    return 0;
}

//获取信号量集中的计数值
int sem_getval(int semid) {
    int ret = semctl(semid, 0, GETVAL, 0);
    if (ret == -1)
        ERR_EXIT("semctl");
    return ret;
}

//删除信号量集
int sem_d(int semid) {
    int ret = semctl(semid, 0, IPC_RMID, 0);
    if (ret == -1)
        ERR_EXIT("semctl");
    return 0;
}

//P操作
int sem_p(int semid) {
    struct sembuf sb = {0, -1, 0};
    int ret = semop(semid, &sb, 1);
    if (ret == -1)
        ERR_EXIT("semop");
    return ret;
}

//V操作
int sem_v(int semid) {
    struct sembuf sb = {0, 1, 0};
    int ret = semop(semid, &sb, 1);
    if (ret == -1)
        ERR_EXIT("semop");
    return ret;
}

#endif //SYSTEMV_SEM_SEM_OP_H
