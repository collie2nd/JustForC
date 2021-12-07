//
// Created by liuxing1 on 7/1/19.
//

#include "sem_op.h"


#define DELAY (rand() % 5 + 1)

int semid;

void wait_for_2fork(int no) {
    int left = no;
    int right = (no + 1) % 5;
    struct sembuf buf[2] = {
            {left,  -1, 0},
            {right, -1, 0}
    };
    semop(semid, buf, 2);
}

int wait_for_1fork(int no) {
    struct sembuf buf = {no, -1, 0};
    int ret = semop(semid, &buf, 1);
    if (ret == -1)
        ERR_EXIT("semop");
    return ret;
}

void free_2fork(int no) {
    int left = no;
    int right = (no + 1) % 5;
    struct sembuf buf[2] = {
            {left,  1, 0},
            {right, 1, 0}
    };
    semop(semid, buf, 2);
}

void philosophere(int no) {
    srand(getpid());

//非死锁进程
//    while (1) {
//        printf("%d is thinking\n", no);
//        sleep(DELAY);
//        printf("%d is hungry\n", no);
//        wait_for_2fork(no);
//        printf("%d is eating\n", no);
//        sleep(DELAY);
//        free_2fork(no);
//    }


//死锁进程
    while (1) {
        int left = no;
        int right = (no + 1) % 5;
        printf("%d is thinking\n", no);
        sleep(DELAY);
        printf("%d is hungry\n", no);
        wait_for_1fork(left);
        sleep(DELAY);
        wait_for_1fork(right);
        printf("%d is eating\n", no);
        sleep(DELAY);
        free_2fork(no);
    }
}

int main() {
    semid = semget(IPC_PRIVATE, 5, IPC_CREAT | 0666);
    if (semid == -1)
        ERR_EXIT("semget");


    union semun su;
    su.val = 1;
    for (int i = 0; i < 5; ++i) {
        semctl(semid, i, SETVAL, su);
    }
    int no = 0;
    pid_t pid;
    for (int j = 1; j < 5; ++j) {
        pid = fork();
        if (pid == -1)
            ERR_EXIT("fork");
        else if (pid == 0) {
            no = j;
            break;
        }
    }
    philosophere(no);
    return 0;
}