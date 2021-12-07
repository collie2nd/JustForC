//
// Created by liuxing1 on 6/27/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define ERR_EXIT(m)\
    do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
} while(0)

typedef struct stu {
    char name[4];
    int age;
} student;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

//  第一步：以读写方式打开文件
    int fd = open(argv[1], O_RDWR);
    if (fd == -1)
        ERR_EXIT("open");

//  第二步：添加映射关系，将文件映射到内存区当中
    student *stup;
    stup = mmap(NULL, sizeof(student) * 5, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (stup == NULL)
        ERR_EXIT("mmap");

//  第三步：读取共享内存空间的数据，不受限于文件大小，该片共享内存在此时可用于进程间通信
    int i;
    for (i = 0; i < 10; i++) {
        printf("name = %s , age = %d\n", (stup + i)->name, (stup + i)->age);
    }

//  第四步：取消内存映射
    munmap(stup, sizeof(student) * 5);
    printf("exit...\n");

    return 0;
}