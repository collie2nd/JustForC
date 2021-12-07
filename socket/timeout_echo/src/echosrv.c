#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <unp.h>
#include "../include/buf_handler.h"

void start();

void handler_sigchld(int sig);

void handler_sigpipe(int sig);

int main(void) {
    start();
    return 0;
}

void start() {

//  signal(SIGCHLD, SIG_IGN);
//  signal(SIGCHLD, handler_sigchld);
//  signal(SIGPIPE, SIG_IGN);
    signal(SIGPIPE, handler_sigpipe);


    //第一步：创建一个套接字socket(协议族，套接字类型，协议类型常量或0
    int listenfd;
    if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        ERR_EXIT("socket");

//第二步：初始化监听套接字地址，并做字节处理，转换为网络字节序
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int on = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ERR_EXIT("setsockopt");

//第三步：绑定该地址，将本地某个IP地址和端口号绑定到指定监听套接字地址结构
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("bind");
    /* bind(socket函数生成的套接字，要绑定的地址结构，地址长度
     * 其作用类似于赋值，可以不绑定，也可以只绑定部分，IP or PORT */

//第四步：将被动/监听套接字从CLOSED状态转换到LISTEN状态，用来接受请求
    if (listen(listenfd, SOMAXCONN) < 0)
        ERR_EXIT("listen");

//第五步：接受请求，从已完成连接队列中的队头返回第一个连接，如果为空，则阻塞
    struct sockaddr_in peeraddr;
    socklen_t peerlen;

    /*
    pid_t pid;
    while (1) {
        if ((conn = accept(listenfd, (struct sockaddr *) &peeraddr, &peerlen)) < 0)
            ERR_EXIT("accept");
        printf("IP:%s,PORT:%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
        pid = fork();
        if(pid==-1)
            ERR_EXIT("fork");
        if (0==pid){
            close(listenfd);
            start();
            exit(EXIT_SUCCESS);
        }
        close(conn);
    }
     */

// 重写以上代码
// 以select()方式代替创建子进程的方式来处理多个套接口的IO操作
    int client[FD_SETSIZE]; //client[]数组保存conn套接字信息，FD_SETSIZE为select()的限制信息，为select函数的最大处理量
    int i, maxindex = 0;    //maxindex记录client数组中的最大不空闲位置，提高一定的遍历效率
    for (i = 0; i < FD_SETSIZE; ++i)
        client[i] = -1;     //初始化套接字数组，等于-1表示空闲
    int conn;
    int nready;             //表示监测到的事件个数
    int maxfd = listenfd;   //最开始将监听套接口放在集合当中，设置成最大值（listenfd默认为3）
    fd_set rset;            //可读集合
    fd_set allset;          //所有描述符的集合,用来临时保存select()监测筛选之前的可读集合
    FD_ZERO(&rset);
    FD_ZERO(&allset);       //将集合都初始化置空
    FD_SET(listenfd, &allset);
    while (1) {
        rset = allset;      //监听套接口先赋值给了allset，然后再赋值给rset
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);//这里只关心可读集合
        if (nready == -1) { //监测到的事件数nready为-1时，表示出错
            if (errno == EINTR) // errno出错码为EINTR代表系统被信号中断，select()无法重启继续监听
                continue;       // continue续循环回来，继续监听
            ERR_EXIT("select");
        } else if (0 == nready) //超时的时候nready会等于0，时间到了还未监测到事件发生，此时未设置超时时间不太可能为0
            continue;

        //运行到这里，代表nready大于1，代表有可读事件发生，首个循环中代表监听套接口发生了事件
        if (FD_ISSET(listenfd, &rset)) {    //代表监听套接字有可读事件，有客户端发起连接请求三次握手已经成功了，connect()成功，已完成连接队列中条目不为空，需要进行处理
            peerlen = sizeof(peeraddr);     //一定要有初始值，是输入输出参数，要有初始值才可以返回出所需值
            conn = accept(listenfd, (struct sockaddr *) &peeraddr, &peerlen);   //这时调用accept()一定不会阻塞，因为已完成连接队列非空
            if (conn == -1)
                ERR_EXIT("accept");
            // 多个客户端连接时，需要解决conn值被覆盖的问题
            // pid=fork()方案中，因为每个子进程都有一个conn值，为父进程的一个副本，所以不会被覆盖，而此处会被覆盖
            // 保存conn后才能加入到rset集合中检测各个客户端连接的IO状态，监听各个套接口的可读事件
            for (i = 0; i < FD_SETSIZE; ++i) {
                if (client[i] < 0) {        //将conn保存到client数组里空闲的位置
                    client[i] = conn;
                    if (i > maxindex)       //说明最大不空闲位置发生了改变，则更新最大不空闲位置
                        maxindex = i;
                    break;
                }
            }
            if (i == FD_SETSIZE) {          //代表连接数过多，超出了select()可以处理数据量的范围
                fprintf(stderr, "too many clients\n");
                exit(EXIT_FAILURE);         //连接数过多时，放弃这个conn，结束与客户端的连接
            }
            printf("IP:%s,PORT:%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
            FD_SET(conn, &allset);          //得到的这个conn，下一次也要关心其可读事件，所以要保存到allset中，避免在select()函数中被修改丢失

            if (conn > maxfd)               //多个套接口（监听套接口和已连接套接口）时要更新maxfd
                maxfd = conn;
            //处理完了listenfd监听套接口检测到的事件，新增加了一个conn之后，要将nready减一，因为可能nready大于1，还有某个conn发生了可读事件需要进行后续处理
            if (--nready <= 0)              //说明我们检测到的事件（此处代表检测到listenfd中有可读事件发生）已经处理完了，
                continue;                   //因为此时nready<=0了，代表无conn产生可读事件，此时应当继续进行监听，没有必要进行以下代码了
        }
        for (i = 0; i <=
                    maxindex; ++i) {        //因为不清楚，有效的conn保存在client数组的哪个位置，所以要完全遍历整个client数组，并不是conn统一保存在前nready个位置中(加入maxindex后，则遍历到最大不空闲位置即可)
            conn = client[i];
            if (conn == -1)                 //表示该位置空闲
                continue;
            if (FD_ISSET(conn, &rset)) {    //判定该conn是否发生了可读事件，然后进行相应处理
                char recvbuf[1024] = {0};
                int ret = readline(conn, recvbuf, sizeof(recvbuf)); //此时readline()不会被阻塞，因为conn发生了可读事件，缓冲区有可读数据
                if (ret == -1)
                    ERR_EXIT("readn");
                else if (0 == ret) {        //ret等于0代表对端关闭
                    printf("client close\n");
                    FD_CLR(conn, &allset);  //将conn从allset中移除，代表它断开连接，不再关注其可读事件
                    client[i] = -1;         //并将其从client数组中清除，对应位置置为-1
                    close(conn);            //通知客户端彻底关闭套接口，避免客户端还保留可读管道
                }
                fputs(recvbuf, stdout);     //先将接收到的recvbuf发送到标准输出
                sleep(3);                   //等待4秒后再回射
                writen(conn, recvbuf, strlen(recvbuf));             //然后将recvbuf回射回去

                if (--nready <= 0)          //处理完一个conn后nready要减一，代表处理完一个事件
                    break;                  //nready<=0代表没有未处理的可读事件了，结束client的遍历
            }
        }
    }
}

void handler_sigchld(int sig) {
    wait(NULL);
}

void handler_sigpipe(int sig) {
    printf("recv a sig = %d\n", sig);
}