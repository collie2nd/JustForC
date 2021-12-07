# 信号量与PV原语
- 信号量和P、V原语由Dijkstra(迪杰斯特拉)提出
- 信号量
    - 互斥：P、V在同一个进程中
    - 同步：P、V在不同进程中
- 信号量值含义
    - S>0: S表示可用资源个数
    - S=0: 表示无可用资源，无等待进程
    - S<0: |S|表示等待队列中进程个数
    
```c
//信号量
struct semaphore{
    int value;
    pointer_PCB queue;
}

//P原语
P(s){
    s.value = s.value--;
    if(s.value < 0){
        ①将该进程状态置为等待状态
        ②将该进程的PCB插入相应的等待队列s.queue末尾
    }
}

//V原语
P(s){
    s.value = s.value++;
    if(s.value <= 0){
        ①唤醒相应等待队列s.queue中等待的一个进程
        ②将该进程状态置为就绪状态，并将其插入就绪队列
    }
}
```   

# 信号量集结构
![](http://ww4.sinaimg.cn/large/006tNc79gy1g4k5glyecfj31i80h4tn1.jpg)


# 信号量集函数
![]( http://ww3.sinaimg.cn/large/006tNc79gy1g4k5j4le5yj31ms0u0nmo.jpg)
```c
int semget(key_t key, int nsems, int semflg);
/***********************************************
功能：用来创建和访问一个消息队列
参数：
    - key:信号集的名字
    - nsems:信号集中信号量的个数
    - semflg:由九个权限标志构成，它们的用法和创建文件时使用的mode模式标志是一样的
返回值：成功返回一个非负整数，即该信号集的标识码；失败返回-1。
***********************************************/

int semctl(int semid, int semnum, int cmd,...);
/***********************************************
功能：用于控制信号量集
参数：
    - semid:由semget返回的信号量集标识码
    - semnum:信号集中信号量的序号
    - cmd:将要采取的动作（有三个可取消）
返回值：成功返回0；失败返回-1。
***********************************************/

int semop(int semid, struct sembuf *sops, unsigned nsops);
/***********************************************
功能：用来创建和访问一个信号量集
参数：
    - semid:由semget返回的信号量集标识码000
    - sops:是个指向一个结构数值的指针
    - nsops:信号量的个数
返回值：成功返回0；失败返回-1。
***********************************************/

//sembuf结构体:
struct sembuf{
    short sem_num;
    short sem_op;
    short sem_flg;
};
/***********************************************
sem_num 是信号量的编号
sem_op  是信号量一次PV操作时加减的数值，一般只会用到两个值，±1，-1是P操作，等待信号量变得可用，+1是V操作，发出信号量已经变得可用
sem_flag的两个取值是IPC_NOWAIT/SEM_UNDO
***********************************************/
```
![](http://ww2.sinaimg.cn/large/006tNc79gy1g4k63xtetyj31po0scasi.jpg)

# 信号量示例