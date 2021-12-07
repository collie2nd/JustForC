# 套接字I/O超时设置方法

## alarm
>信号的方式进行超时设置
```c
void handler(int sig) {                 //不需要做任何处理，只需要打断read()即可
    return 0;
}

signal(SIGALRM, handler);
alarm(5);                               //5秒之后产生一个SIGALRM信号
int ret = read(fd, buf, sizeof(buf));   //假如阻塞5秒仍没有收到可读数据，read()函数就会被SIGALRM信号打断
if (ret == -1 && errno == EINTR) {
    errno == ETIMEDOUT;                 //将错误码设置为ETIMEDOUT
} else if (ret >= 0) {
    alarm(0);                           //如果5秒内收到了数据，那么要关闭闹钟
}
```
缺陷：alarm()可能会作为其他的用途，这时会产生冲突，冲突的解决比较麻烦，所以一般不用这种方式实现超时

## 套接字选项

- SO_SNDTIMEO   表示发送的超时时间
- SO_RCVTIMEO   表示接收的超时时间

```c
setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, 5);   //表示设置接收数据的超时时间为5s
int ret = read(sock, buf, sizeof(buf));         //假如阻塞5秒仍没有收到可读数据，read()函数就会被SIGALRM信号打断
if (ret == -1 && errno == EWOULDBLOCK) {
    errno == ETIMEDOUT;                         //将错误码设置为ETIMEDOUT
}
```
缺陷：有一些TCP/IP的协议不支持这两个套接字选项，所以可移植性不强，也不建议使用该方法。

# 用select实现超时

## read_timeout()函数封装(readable_timeo())

```c
int read_timeout(int fd, unsigned int wait_seconds);
```
成功（未超时）返回0，失败返回-1，超时返回-1，且errno = ETIMEDOUT

```c
int ret = read_timeout(fd, 5);                  //设置套接口超时时间5秒钟，若这里设置为0，那么返回值直接返回0
if (0 == ret) {                                 //ret为0意味着成功未超时
    read(fd, ...);
} else if (ret == -1 && errno == ETIMEDOUT) {   //失败返回-1，超时返回-1，且errno为ETIMEDOUT
    timeout...;
} else {
    ERR_EXIT("read_timeout");
}
```

## write_timeout()函数封装(writable_timeo())

```c
int write_timeout(int fd, unsigned int wait_seconds);
```
成功（未超时）返回0，失败返回-1，超时返回-1，且errno = ETIMEDOUT

## accept_timeout()函数封装

```c
int accept_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds);
```
与上面不同，该函数带有accept()操作，成功（未超时）返回已连接套接字，超时返回-1，且errno = ETIMEDOUT

## connect_timeout()函数封装(connect_timeo())

```c
int connect_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds);
```
与上面不同，该函数带有connect()操作，成功（未超时）返回0，失败返回-1，超时返回-1，且errno = ETIMEDOUT