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
//  第一步：打开一个文件（以创建/可读写/清空的方式打开该文件）
    int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd == -1)
        ERR_EXIT("open");

//  第二步：从文件开头处，开始定位cfo（读写指针），定位到39个字节的位置，最后写入一个字节“”空字符，此时文件为40个字节，创建一个空洞文件
    lseek(fd, sizeof(student) * 5 - 1, SEEK_SET);
    write(fd, "", 1);

    printf("file len:%lld\n", lseek(fd, 0, SEEK_END));    //获取文件长度

/*  ls -l <file>    可以查看文件字节数等信息
 *  od -c <file>    可以以8进制打开文件，查看文件信息   */

//  第三步：添加映射关系，将文件映射到内存区当中
    student *stup;
    stup = mmap(NULL, sizeof(student) * 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //可用于进程间通信的映射地址空间不完全受限于文件大小
    if (stup == NULL)
        ERR_EXIT("mmap");

//  第四步：在共享内存空间进行读写操作（由于不能改变文件大小，所以最终只能改变40个字节的文件，但80个字节可以在进程存在期间进行通信）
    char ch = 'a';
    int i;
    for (i = 0; i < 10; i++) {
        memcpy((stup + i)->name, &ch, 1);
        (stup + i)->age = 20 + i;
        ++ch;
    }

    while (i > 0) {
        printf("%ds\n", i--);
        sleep(1);
    }

    printf("initialize over!\n");

//  第五步：取消内存映射
    munmap(stup, sizeof(student) * 5);
    printf("exit...\n");

    return 0;
}