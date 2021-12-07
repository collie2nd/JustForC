# UNIX域

## Unix域协议特点

- Unix域套接字与TCP套接字相比，在同一台主机的传输速度，前者是后者的两倍；
- Unix域套接字可以在同一台主机上各进程之间传递描述符；
- Unix域套接字与传统套接字的区别是用路径名来表示协议族的描述。

## Unix域协议地址结构

```c
#define UNIX_PATH_MAX 108

struct sockaddr_un {
    sa_family_t sun_family;             //AF_UNIX
    char        sun_path[UNIX_PATH_MAX];//pathname
}
```

## Unix域字节流回射客户/服务程序

## Unix域套接字编程注意点

- bind成功将会创建一个文件，权限为0777&~umask
- sun_path最好用一个绝对路径
- UNIX域协议支持流式套接口与报式套接口
- UNIX域流式套接字connect发现监听队列满时，会立刻返回一个ECONNREFUSED，这和TCP不同，如果监听队列满，会忽略到来的SYN，这导致对方重传SYN

## socketpair

- 功能：创建一个全双工的流通道(和pipe()一样也只能用于父子进程间的通信，但不同在于是全双工的流管道)
- 原型
```c
 int socketpair(int domain, int type, int protocol, int sv[2]);
```
- 参数
    - domain:协议家族
    - type:套接字类型
    - protocol:协议类型
    - sv:返回套接字对
- 返回值：成功返回0，失败返回-1

## sendmsg/recvmsg
- 原型
```c
ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
```
- 参数
    - sockfd:目的套接字文件描述字
    - msg:消息类型结构体
    - flags:类型标记
    
## UNIX域套接字传递文件描述符
（send_fd）