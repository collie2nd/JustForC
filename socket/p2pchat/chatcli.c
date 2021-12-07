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
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket");
	/* POSIX规范指定socket函数的第一个参数为PF_值，
	 * 而AF_值用于套接字地址结构，如下 */

//第二步：初始化目的套接字地址，并做字节处理，转换为网络字节序
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof( servaddr));		/*memset(&servaddr, 0, sizeof( servaddr));*/
	servaddr.sin_family = AF_INET;			/*PF_INET包含AF_INET*/
	servaddr.sin_port = htons(5188);
	/* htons 16位主机字节序转换为16位网络字节序
	 * htonl 32位主机字节序转换为32位网络字节序
	 * ntohs 16位网络字节序转换为16位主机字节序
	 * ntohl 32位网络字节序转换为32位主机字节序*/
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* inet_aton 字符串转换为32位网络字节序，成功返回给第二个参数所指的指针
	 * inet_addr 字符串转换为32位网络字节序，并返回给in_addr_t
	 * inet_ntoa 32位网络字节序转换为字符串，并返回给char*        */

//第三步：调用连接函数，进行连接listen(未连接套接字，目的套接字地址，前者长度）
	if (connect(sock, (struct sockaddr*)&servaddr, sizeof( servaddr)) < 0)
		ERR_EXIT("connect");
	/* (SA *)&(某特定协议的套接字地址结构) = (struct sockaddr*)&(...)  */
	/* 套接字函数被定义为
	 * 其中一个参数位指向某个通用套接字地址结构的一个指针
	 * 所以这些函数的任何调用都必须要
	 * 将指向特定协议的套接字地址的指针进行类型强制转换
	 * 变成指向某个通用套接字地址结构的指针            */


//第四步：前者未连接套接字变成已连接状态，通过其进行通信
	pid_t pid;
	pid = fork();
	/* pid_t fork(void);
	 * 返回：
	 * 在子进程中为0，
	 * 在父进程中为子进程ID
	 * 若失败则为-1
	 * 理解说明：fork()调用它一次，却返回两次
	 * 在调用进程（父进程）中返回一次，返回值是新派生进程（称为子进程）的进程ID号
	 * 在子进程中又返回一次，返回值是0，因此，返回值本身告知当前进程是子进程还是父进程*/

	if (pid == -1){
		ERR_EXIT("fork");
	}else if (pid == 0){
		signal(SIGUSR1, child_handler);
		char recvbuf[1024];
		while(1){
			bzero(&recvbuf, sizeof(recvbuf));
			int ret = read(sock, recvbuf, sizeof(recvbuf));
			if (ret == 1){
				ERR_EXIT("read");
			}else if (ret == 0){
				printf("peer close\n");
				break;
			}else{
				fputs(recvbuf, stdout);
			}
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
		close(sock);
	}else{
		signal(SIGUSR1, parent_handler);/* prtcl(PR_SET_PDEATHSIG, SIGHUP);(需要sys/prtcl.h库文件)
						   设置父进程退出后，自动结束掉子进程			*/
		char sendbuf[1024] = {0};
		while (fgets( sendbuf, sizeof( sendbuf), stdin) != NULL){	
			write(sock, sendbuf, strlen(sendbuf));
			bzero(&sendbuf, sizeof(sendbuf));
		}
		kill(pid, SIGUSR1);
		printf("parent close\n");
		close(sock);
	}
	
	return 0;
}

