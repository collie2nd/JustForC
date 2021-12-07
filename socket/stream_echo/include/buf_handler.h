//
// Created by liuxing1 on 5/28/19.
//

#ifndef ECHO_STREAM_BUF_HANDLER_H
#define ECHO_STREAM_BUF_HANDLER_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>//read(),write()


typedef struct packet{
    int len;
    char buf[1024];
}pack;

/* read()函数的封装
 * fd       参数fd指待传输文件STDIN等
 * buf      接收字节流的内存指针
 * count    确定接收count个字节数
 * ssize_t  有符号数，负数代表出错，0代表接收结束，正数代表接收到的字节数
 * size_t   无符号数
 */
ssize_t readn(int fd, void *buf,size_t count);

/* write()函数的封装
 * fd       参数fd指待传输文件STDOUT等
 * buf      接收字节流的内存指针
 * count    确定发送count个字节数
 * ssize_t  有符号数，负数代表出错，0代表发送结束，正数代表发送成功的字节数
 * size_t   无符号数
 */
ssize_t writen(int fd, void *buf,size_t count);

#endif //ECHO_STREAM_BUF_HANDLER_H
