# 共享内存数据结构
![](http://ww4.sinaimg.cn/large/006tNc79gy1g4gz6yyif1j31kc0te7tn.jpg)


# 共享内存函数
```c
#include <sys/ipc.h>
#include <sys/shm.h>

int shmget(key_t key, size_t size, int shmflg);
/******************************************************
- 功能：用来创建共享内存
- 参数：
    - key:这个共享内存段的名字
    - size:共享内存大小
    - shmflg:由九个权限标志构成，它们的用法和创建文件时使用的mode模式标志是一样的
- 返回值：成功返回一个非负整数，即该共享内存段的标识码；失败返回-1。
*******************************************************/

void *shmat(int shmid, const void *shmaddr, int shmflg);
/******************************************************
- 功能：将共享内存段连接到进程地址空间
- 参数：
    - shmid:共享内存标识
    - shmaddr:指定连接的地址
    - shmflg:它的两个可能取值是SHM_RND和SHM_RDONLY
- 返回值：成功返回一个指针，指向共享内存第一个节；失败返回-1。
- 备注：
    - shmaddr为NULL，核心自动选择一个地址
    - shmaddr不为NULL且shmflg无SHM_RND标记，则以shmaddr为连接地址
    - shmaddr不为NULL且shmflg设置了SHM_RND标记，则连接的地址会自动向下调整为SHMLBA的整数倍。公式：shmaddr-(shmaddr % SHMLBA)
    - shmflg = SHM_RDONLY，表示连接操作用来只读共享内存
*******************************************************/

int shmdt(const void *shmaddr);
/******************************************************
- 功能：将共享内存与当前进程脱离
- 参数：
    - shmaddr:由shmat所返回的指针
- 返回值：成功返回0；失败返回-1。
- 备注：将共享内存段与当前进程脱离不等于删除共享内存段
*******************************************************/

int shmctl(int shmid, int cmd, struct shmid_ds *buf);
/******************************************************
- 功能：控制操作共享内存
- 参数：
    - shmid:由shmget返回的共享内存标识码
    - cmd:将要采取的动作（有三个可取值）
    - buf:指向一个保存着共享内存的模式状态和访问权限的数据结构
- 返回值：成功返回0；失败返回-1。
*******************************************************/

```
![](http://ww1.sinaimg.cn/large/006tNc79gy1g4h1eirfzej31pg0k4qif.jpg)

# 共享内存示例