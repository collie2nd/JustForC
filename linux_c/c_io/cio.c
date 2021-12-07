#include <stdio.h>
/* unix把所有设备，所有外设，所有程序都当成是文件
 * 启动一个程序的时候，会当和一个文件交互
 * 默认创建三个文件stdio stdout stderr*/
int main(){
	printf("hello word\n");
	int a;
	scanf("%d", &a);
	printf("input value is %d", a);
	return 0;
}
