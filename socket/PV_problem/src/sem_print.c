//
// Created by liuxing1 on 6/28/19.
//

#include "sem_op.h"

int semid;

void print(char op_char) {
    int pause_time;
    srand(getpid());
    int i;
    for (i = 0; i < 10; i++) {
        sem_p(semid);
        printf("%c", op_char);
        fflush(stdout);
        pause_time = rand() % 3;
        sleep(pause_time);
        printf("%c", op_char);
        fflush(stdout);
        sem_v(semid);
        pause_time = rand() % 2;
        sleep(pause_time);
    }
}

int main() {
    semid = sem_create(IPC_PRIVATE);
    sem_setval(semid, 0);
    pid_t pid = fork();
    if (pid == -1)
        ERR_EXIT("fork");
    else if (pid == 0) {
        print('X');
    } else {
        sem_setval(semid, 1);
        print('O');
        wait(NULL);
        sem_d(semid);
    }
    return 0;
}