#include <stdio.h>
/* unix把所有设备，所有外设，所有程序都当成是文件
 * 启动一个程序的时候，会当和一个文件交互
 * 默认创建三个文件stdin stdout stderr*/
int main(){

	//printf("hello world!\n");是对下列函数的封装
	fprintf(stdout, "hello world!\n");
	//f+printf:传入文件句柄就可以往一个文件里面写入数据，默认的stdout就是屏幕
	
	int a;
	//scanf("%d", &a);
	fscanf(stdin, "%d", &a);
	//f+scanf:传入文件句柄就可以从一个文件里面读取数据，默认的stdin就是键盘
	
	if(a < 0){
		fprintf(stderr, "the value must > 0");
		return 1;
	}

	return 0;
}
