# 共享内存
共享内存是最快的IPC形式。一旦这样的内存映射到共享它的进程的地址空间，这些进程间数据传递不再涉及到内核，换句话说是进程不再通过执行进入内核的系统调用来传递彼此的数据。

# 共享内存示意图
![](http://ww2.sinaimg.cn/large/006tNc79gy1g4eoqtgoa4j31af0u0nay.jpg)

# 管道/消息队列/共享内存对比
更少调用内核，更多地使用内存操作函数

# mmap函数
- 功能：将文件或者设备空间映射到共享内存区
- 原型：
    - void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
- 参数：
    - addr:     要映射的起始地址，通常指定为NULL，让内核自动选择
    - len:      映射到进程地址空间的字节数
    - prot:     映射区保护方式
    - flags:    标志
    - fd:       文件描述符
    - offset:   从文件头开始的偏移量
- 返回值：成功返回映射到的内存区的起始地址；失败返回-1。

![](http://ww4.sinaimg.cn/large/006tNc79gy1g4ep3ivzryj318v0u0nek.jpg)

# munmap函数
- 功能：取消mmap函数建立的映射
- 原型：
    - int munmap(void *addr, size_t len);
- 参数：
    - addr:映射的内存起始地址
    - len:映射到进程地址空间的字节数
- 返回值：成功返回0；失败返回-1。

# msync函数
- 功能：对映射的共享内存执行同步操作
- 原型：
    - int msync(void *addr, size_t len, int flags);
- 参数：
    - addr:内存起始地址
    - len:长度
    - flags:选项
- 返回值：成功返回0；失败返回-1。

![](http://ww2.sinaimg.cn/large/006tNc79gy1g4fnt692hwj31dy0ek47c.jpg)

# map注意点
- 映射不能改变文件大小
- 可用于进程间通信的有效地址空间不完全受限于被映射文件的大小
- 文件一旦被映射后，所有对映射区域的访问实际上是对内存区域的访问。映射区域内容写回文件时，所写内容不能超过文件的大小。