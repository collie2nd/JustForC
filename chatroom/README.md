# 部署

```bash
mkdir bin
cd bin
cmake .. -G "Unix Makefiles"
make
```

# 项目结构

```
|——bin	//编译中间件
|——cli	//客户端
    |——include	//头文件
    |——src	//源码
    |——main.c
    |——CMakeLists.txt
|——srv	//服务器
    |——include	//头文件
    |——lib	//库文件
    |——src	//源码
    |——main.c
    |——CMakeLists.txt
|——.gitignore
|——CMakeLists.txt
```

# service.out

```bash
./service.out
```

# client.out

```bash
./client.out XXX(usrName)
```