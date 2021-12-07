#include "../include/chatsrv.h"


//
int listenfd;

//
void init() {
    if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) //第三个系数(protocol)为协议类型常值
        ERR_EXIT("socket");                                         //也可以为0，代表系统默认值
    SOCKET servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = inet_addr("172.30.1.200");           //服务器端要绑定一个本地套接口以供客户端做指定连接
    int on = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ERR_EXIT("setsockopt");
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("bind");
    if (listen(listenfd, SOMAXCONN) < 0)
        ERR_EXIT("listen");

    InitLink();
}


void login(int fd, SOCKET peeraddr) {

    char usrname[1024];    				//登录时首先接收用户名
    bzero(usrname, sizeof(usrname));
    int ret = recv(fd, usrname, sizeof(usrname), 0);    //相比read只多了一个flag标记，可以置0不管
    if (ret < 0)
        ERR_EXIT("recv");
    else if (0 == ret) {
        close(fd);                                      //用户未发送用户名，则关闭套接口连接
        printf("client illegal anonymity\n");
    } else {
        printf("%s login!\n", usrname);

        pClient cli = (pClient) malloc(sizeof(Client)); //初始化一个用户结构体，以保存当前用户信息
        if (cli == NULL)
            ERR_EXIT("malloc");
        bzero(cli, sizeof(Client));

        cli->conn = fd;
        strcpy(cli->usrname, usrname);
        strcpy(cli->IP, inet_ntoa(peeraddr.sin_addr));  //arpa/inet.h
        cli->port = ntohs(peeraddr.sin_port);           //netinet/in.h
        AddClient(cli);                                 //将当前用户结构体保存到用户链表中

        char initbuf[MSGLENGTH];                        //定义发送一条登录成功的通知
        bzero(&initbuf, sizeof(initbuf));
        sprintf(initbuf, "%s:onlion!\n", usrname);
        pClient p = GetHeadNode()->next;                //开始遍历用户链表，将登录成功的通知发送给所有用户
        while (p != NULL) {
            send(p->conn, initbuf, strlen(initbuf), 0);
            p = p->next;
        }
    }
}

void SendMsgToOthers(char *buf, int thisfd) {
    //通过当前套接字找到当前用户的结构体指针,p指向用户链表第一位用户
    pClient thiscli = FindClient(thisfd), p = GetHeadNode()->next;
    //初始化定义时间变量，返回当前发送信息的发送时间
    time_t now;
    struct tm *t;
    time(&now);
    t = gmtime(&now);
    char sendbuf[MSGLENGTH];
    //初始化发送信息，其中注意小时数要加8,有八小时时差
    bzero(&sendbuf, sizeof(sendbuf));
    sprintf(sendbuf, "%02d:%02d:%02d->>%s:%s", 8 + t->tm_hour, t->tm_min, t->tm_sec, thiscli->usrname, buf);
    //遍历用户链表，将信息发送给所有非当前用户
    while (p != NULL) {
        if (p != thiscli)
            send(p->conn, sendbuf, strlen(sendbuf), 0);
        p = p->next;
    }
}

void *chat(void *arg) {                                     //定义void* handler(void*)类型函数为pthread_creat()子进程的操作函数
    int fd = *(int *) arg;                                  //将参数转换为套接口的int类型
    while (1) {
        char buf[MSGLENGTH];
        bzero(&buf, sizeof(buf));
        int ret = recv(fd, buf, sizeof(buf), 0);            //循环接收套接口发送来的消息
        if (ret <= 0) {                                     //若返回值非正数则判断错误类型,将cli指向当前用户，p指向链表第一个用户
            pClient cli = FindClient(fd), p = GetHeadNode()->next;
            if (cli == NULL || p == NULL)                   //链表错误
                ERR_EXIT("UsrLink ERROR");
            sprintf(buf, "%s : logout!\n", cli->usrname);   //非用户链表错误，则发送退出信息
            while (p != NULL) {                             //遍历发送通知，通知当前用户已退出
                if (p != cli)
                    send(p->conn, buf, strlen(buf), 0);
                p = p->next;
            }
            printf("%s:logout!\n",cli->usrname);
            RemoveClient(fd);                               //将当前套接口对应的用户信息删除
            close(fd);                                      //显示关闭当前套接字
            pthread_exit("client close!\n");                //结束当前用户进程
        }
        SendMsgToOthers(buf, fd);                           //将接收到的消息转发出去
    }
}

void service() {
    printf("service start!\n");
/*	void* thread_ret;*/
    while (1) {                                                     //开启服务器，循环接收新的客户端连接
        SOCKET peeraddr;
        socklen_t peerlen = sizeof(peeraddr);
        int conn;
        if ((conn = accept(listenfd, (SA *) &peeraddr, &peerlen)) < 0)
            ERR_EXIT("accept");

        if (CountConn() == MAXSIZE) {                               //判断用户连接数是否达到饱和
            char *errStr = "The current connection is full, please connect later!";
            send(conn, errStr, strlen(errStr), 0);                  //饱和则发送错误反馈，并关闭该连接
            close(conn);                                            //unistd.h
        }

        login(conn, peeraddr);                                      //登录，在用户链表中注册连接
/*		CheckConnection();*/
        printf("Current connection : %d\n", CountConn());           //发送当前连接客户端个数
        pthread_t pid;
        int ret = pthread_create(&pid, NULL, chat, (void *) &conn); //创建一个子进程处理当前客户端的聊天机制
        if (ret != 0)                                               //ptread_create()成功返回0
            ERR_EXIT("ptread_create");
/*		ret = pthread_join(pid, &thread_ret);
		if(ret!=0)
			ERR_EXIT("ptread_join");
		printf("%s\n",(char*)thread_ret);*/
    }
    close(listenfd);                                                //最后关闭监听套接口，关闭服务端
    ClearClient();                                                  //并清空用户信息，归还存储空间
    printf("service close!\n");
}
