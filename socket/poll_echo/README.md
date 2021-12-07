# poll()函数
>没有FD_SETSIZE的限制
```c
#include <poll.h>
//function  poll    
//param     *fds    我们要关心的套接口以及事件的指针，通常指向一个数组，将我们关心的IO等事件加入到这个数组中即可
//param     nfds    加入到fds数组中的IO的个数，即要检测的IO个数
//param     timeout 超时时间
//return    int     
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

# events的值及含义


|值|含义|
|---|---|
|POLLIN|有数据到来，文件描述符可读|
|POLLPRI|有紧急数据可读，例如带外数据|
|POLLOUT|文件可写|
|POLLRDHUP|流式套接字半关闭|
|POLLERR|错误发生|
|POLLHUP|关闭|
|POLLNVAL|非法请求|
|POLLRDNORM|与POLLIN相同|
|POLLRDBAND|优先数据可读|
|POLLWRNORM|与POLLOUT相同|
|POLLWRBAND|优先数据可写|

# 与select()的共同点

>内核要遍历所有的文件描述符，直到找到发生事件的文件描述符
当并发数增长的时候，内核需要遍历的文件描述符的数量也要增长，效率会随着并发数线性下降。

# epoll()

```c
#include <sys/epoll.h>

//function  前两个作用都是创建一个epoll实例
//size      代表指定epoll()所能支持的最大并发数，size实际上指定的时内部哈希表的容量
int epoll_create(int size);

//flags     指定的宏定义，epoll实例的设置
int epoll_create1(int flags);

//function  给epoll()实例添加文件描述符
//epfd      代表epoll实例句柄
//op        操作方式（增删改查）
//fd        文件描述符
//*event    指定文件描述符的事件 
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

//function  检测哪些I/O产生了事件
//epfd      epoll()的实例句柄
//events    返回值
//maxevents 事件的最大容量，能够返回的事件的最大个数
//timeout   超时时间
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```

# epoll与select、poll的区别
- 相比于select与poll，epoll最大的好处在于它不会随着监听fd数目的增长而降低效率；
- 内核中的select与poll的实现时采用轮询的方式来处理的，轮询的fd数目越多，自然耗时越多；
- epoll的实现是基于回调的，如果fd有期望的时间发生就通过回调函数将其加入epoll就绪队列中，也就是说它只关心“活跃”的fd，与fd数目无关；
- 内核/用户空间内存拷贝问题，如何让内核把fd消息通知给用户空间呢？在这个问题上select/poll采取了内存拷贝方法，而epoll采取了共享内存的方式；
- epoll不仅会告诉应用程序有I/O事件到来，还会告诉应用程序相关的信息，这些信息时应用程序填充的，因此根据这些信息应用程序就能直接定位到事件，而不必遍历整个fd集合。

# epoll模式
- EPOLLLT   电平触发
- EPOLLET   边沿触发