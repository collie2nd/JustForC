#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
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

/* 信号处理函数，用于进程间的通信*/
void parent_handler(int sig){
        printf("recv a sig = %d,parent close", sig);
        exit(EXIT_SUCCESS);
}

void child_handler(int sig){
        printf("recv a sig = %d,child close", sig);
        exit(EXIT_SUCCESS);
}

int main(void){
//第一步：创建一个套接字socket(协议族，套接字类型，协议类型常量或0
	int listenfd;
	if ( (listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket");
	 /* POSIX规范指定socket函数的第一个参数为PF_值，
         * 而AF_值用于套接字地址结构，如下 */

//第二步：初始化监听套接字地址，并做字节处理，转换为网络字节序
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));		/*memset(&servaddr, 0, sizeof( servaddr));*/
	servaddr.sin_family = AF_INET;			/*PF_INET包含AF_INET*/
	servaddr.sin_port = htons(5188);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	/* servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
							 * inet_aton("127.0.0.1",$servaddr.sin_addr);	*/
        /* htons 16位主机字节序转换为16位网络字节序
         * htonl 32位主机字节序转换为32位网络字节序
         * ntohs 16位网络字节序转换为16位主机字节序
         * ntohl 32位网络字节序转换为32位主机字节序
	 * inet_aton 字符串转换为32位网络字节序，成功返回给第二个参数所指的指针
         * inet_addr 字符串转换为32位网络字节序，并返回给in_addr_t
         * inet_ntoa 32位网络字节序转换为字符串，并返回给char*        */

//第三步：绑定该地址，将本地某个IP地址和端口号绑定到指定监听套接字地址结构
	/* 绑定前最好设置一下套接字选项setsockopt()*/
	int on = 1;
	/* int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	 * int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
	 * sockfd:必须指向一个打开的套接字描述符
	 * level:指定系统中解释选项的代码，或为通用套接字代码，或为某个特定于协议的代码（例如IPV4/IPV6/TCP或SCTP）
	 * optval:是一个指向某个变量(*optval)的指针
	 * setsockopt从*optval中取得选项待设置的新值，getsockopt则把已获取的选项当前值存放到*optval中
	 * *optval的大小由最后一个参数指定，它对于setsockopt()来说是一个值参数，对于getsockopt()来说是一个值-结果参数 */

	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)/* SO_REUSEADDR 允许重用本地地址*/
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

	pid_t pid;/* 创建进程，一个用来接收数据，另一个用来发送数据*/
	pid = fork();/* fork返回值，在子进程中为0，父进程中为子进程ID，出错则为-1*/
	if (pid == -1){
		ERR_EXIT("fork");
	}else if (pid == 0){
		signal(SIGUSR1, child_handler);
		char sendbuf[1024] = {0};
		while( fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
		{
			write(conn, sendbuf, strlen(sendbuf));
			bzero(&sendbuf, sizeof(sendbuf));
		}
		printf("child close\n");
		kill(getppid(), SIGUSR1);/* 子进程通过调用pid_t getppid(void)取得父进程的进程ID*/
                /* int kill(pid_t pid, int sig);
                 * 用于向任何进程组或进程发送信号
                 * pid说明:
                 * pid > 0，pid是信号欲送往的进程的标识
                 * pid = 0，信号将送往所有与调用kill()的那个进程属同一个使用组的进程
                 * pid = -1,信号将送往所有调用进程有权给其发送信号的进程，除了进程1(init)
                 * pid < -1,信号将送往以-pid为组标识的进程
                 * sig说明:
                 * sig为准备发送的信号代码，假如其值为零，则没有任何信号送出，但是系统会执行错误检查
                 * 通常会利用设sig为零值，来检验某个进程是否仍在运行
                 * 返回值说明:
                 * 成功执行时，返回0，失败，返回-1，errno被设为以下的某个值
                 * EINVAL:指定的信号码无效(参数sig不合法)
                 * EPERM:权限不够无法传送信号给指定进程ESRCH
                 * ESRCH:参数pid所指定的进程或进程组不存在*/
		exit(EXIT_SUCCESS);
	}else{
		signal(SIGUSR1, parent_handler);/* 设置父进程退出后，自动结束掉子进程*/
		char recvbuf[1024];
	        while(1){
			bzero(&recvbuf, sizeof(recvbuf));
	                int ret = read(conn, recvbuf, sizeof(recvbuf));
        	        if (ret == -1){
				ERR_EXIT("read");
			}
			else if (ret == 0){
				printf("peer close\n");	
				break;
			}
			else
				fputs(recvbuf, stdout);
	        }
		kill(pid, SIGUSR1);
		printf("parent close\n");
		exit(EXIT_SUCCESS);
	}

	return 0;
}
