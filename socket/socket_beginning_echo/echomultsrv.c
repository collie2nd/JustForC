#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m)\
	do\
{\
	perror(m);\
	exit(EXIT_FAILURE);\
} while(0)


void do_service(int conn)
{
	 char recvbuf[1024];
	 while(1)
	 {
  		 memset(recvbuf, 0, sizeof(recvbuf));
		 int ret = read(conn, recvbuf, sizeof(recvbuf));
		 if (ret == 0)/* 判断客户端是否关闭，关闭的话，break退出循环*/
		 {
			 printf("client close\n");
			 break;
		 }else
			 ERR_EXIT("read");
		 fputs( recvbuf, stdout);
		 write(conn, recvbuf, ret);
	 }
}


int main(void)
{
//第一步：创建一个套接字socket(协议族，套接字类型，协议类型常量或0
	int listenfd;
	if ( (listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket");
	 /* POSIX规范指定socket函数的第一个参数为PF_值，
         * 而AF_值用于套接字地址结构，如下 */

//第二步：初始化监听套接字地址，并做字节处理，转换为网络字节序
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        /* htons 16位主机字节序转换为16位网络字节序
         * htonl 32位主机字节序转换为32位网络字节序
         * ntohs 16位网络字节序转换为16位主机字节序
         * ntohl 32位网络字节序转换为32位主机字节序*/

	/* servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	 * inet_aton("127.0.0.1",$servaddr.sin_addr);
	 *
	 * inet_aton 字符串转换为32位网络字节序，成功返回给第二个参数所指的指针
         * inet_addr 字符串转换为32位网络字节序，并返回给in_addr_t
         * inet_ntoa 32位网络字节序转换为字符串，并返回给char*        */

//第三步：绑定该地址，将本地某个IP地址和端口号绑定到指定监听套接字地址结构
	/* 绑定前最好设置一下套接字选项setsockopt()*/
	int on = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		ERR_EXIT("setsockopt");

	if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("bind");
	/* bind(socket函数生成的套接字，要绑定的地址结构，地址长度
	 * 其作用类似于赋值，可以不绑定，也可以只绑定部分，IP or PORT */

//第四步：将被动/监听套接字从CLOSED状态转换到LISTEN状态，用来接受请求
	if (listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");

//第五步：接受请求，从已完成连接队列中的队头返回第一个连接，如果为空，则阻塞
	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	int conn;


	/* 将accept()放在循环中，并发处理客户端请求*/
	pid_t pid;/* 创建进程ID*/
	while(1)
	{
		if ((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen))<0)
			ERR_EXIT("accept");
		/* accept()函数成功返回一个非负整数，表示创建了一个新的套接字（conn）
		 * (conn)为主动套接字，不能用来接受连接，区别于listenfd
		 * 失败返回-1，成功返回已连接套接字描述符
		 * 第一个参数为监听套接字描述符（由socket函数创建
		 * 随后用作bind函数和listen函数的第一个参数的描述符）
		 * 第二个参数cliaddr*，与第三个参数addrlen*用来返回
		 * 已连接的对端进程（客户）的协议地址，addrlen是值-结果参数
		 * 如果对客户身份不感兴趣，可以将后两个参数置空		*/
	
		printf("ip = %s, port = %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

		pid = fork();/* 拷贝分裂出一个子进程处理客户请求*/
		if (pid == -1)
			ERR_EXIT("fork");
		if (pid == 0)
		{
			close(listenfd);
			do_service(conn);
			exit(EXIT_SUCCESS);/* 客户端关闭后退出循环
					      以免操作结束后，又返回到accept()等待连接*/
		}
		else
			close(conn);
	}
	return 0;
}
