# UDP特点

- 无连接
- 基于消息的数据传输服务
- 不可靠
- 一般情况下UDP更高效

# UDP CS基本模型

![UDP客户服务器模型](http://lx.hanyajun.com/UDP%E5%AE%A2%E6%88%B7%E6%9C%8D%E5%8A%A1%E5%99%A8%E6%A8%A1%E5%9E%8B)

# UDP回射客户服务器

![UDP回射客户服务器](http://lx.hanyajun.com/UDP%E5%9B%9E%E5%B0%84%E5%AE%A2%E6%88%B7%E6%9C%8D%E5%8A%A1%E5%99%A8)

# UDP注意点

- UDP报文可能会丢失，重复

>处理方法：
丢失：超时处理机制；
重复：维护数据报的序号。

- UDP报文可能会乱序

>处理方法：维护数据报的序号。

- UDP缺乏流量控制

>处理方法：模拟TCP的滑动窗口机制，模拟流量控制机制

- UDP协议数据报文截断

>接收到的数据报大于接收报文的缓冲区，报文就会被截断，多的部分就会被丢弃

- recvfrom返回0，不代表连接关闭，因为UDP本身无连接

- ICMP异步错误

- UDP connect

```c

//UDP的connect只做套接字的维护作用，使这个sock只能够与servaddr通信，不能和其他对等方通信了，并不做三次握手

connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr));

//ethocli中若做了UDP connect维护了sock的对等方信息，那么就可以不再指定

/* sendto(sock, sendbuf, strlen(sendbuf), 0, (struct sockaddr *) &servaddr, sizeof(serv);*/
sendto(sock, recvbuf, sizeof(recvbuf), 0, NULL, NULL);

//甚至可将sendto()替换为send()，recvfrom()替换为recv()函数了

```

- UDP外出接口的确定

>假定客户端有多个IP，外出接口会在sengto()时选择更适合的网段进行选择，如果connect()提前绑定了对端网段，那么sendto()可能无法选择可用的网段，可能导致对端不可达。