# Close()与Shutdown()的区别

- close终止了数据传送的两个方向；

    A(sockA)<--------->(sockB)B
    
    假如A调用了close(sockA);代表A不能通过sockA发送或接收数据了，在sockA处终止了数据传送的两个方向
    
    但并不意味着B不能通过sockB向A发送数据，在调用close(sockA)时（且sockA的引用计数减为0时），会向B发送一个FIN报文段，read()会返回为0，
    
    B收到FIN报文段，仅仅意味着A到B的数据传输终止了，并不意味着B到A的数据传输终止了，B此时调用write()函数发送数据给A时，由于对等方A已经关闭了sockA，所以TCP协议会以一个RST报文段向B进行响应，表示连接重置
    
    当B收到一个RST段之后再次调用write()函数就会收到SIG_PIPE信号

- shutdown可以有选择的终止某个方向的数据传送或者终止数据传送的两个方向；

```c
    int shutdown(int sockfd, int how);
    how:
    SHUT_RD(0)      表示不能往管道中读数据
    SHUT_WR(1)      表示不能往管道中写数据
    SHUT_RDWR(2)    表示既不能读，也不能写了
```

- shutdown how = 1 (SHUT_WR) 就可以保证对等方接收到一个EOF字符( FIN TCP段 )，而不管其他进程是否已经打开了套接字。而close不能保证，直到套接字引用计数减为0时才发送。也就是说直到所有的进程都关闭了套接字，才会发送FIN TCP段。


