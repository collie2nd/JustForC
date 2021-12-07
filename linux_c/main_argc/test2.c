#include <stdio.h>

int main(int argv, char* argc[]){
	printf("argv is %d \n",argv);//反应了输入的附加参数个数为argv-1
	int i;//C98标准中for循环里不可以初始化定义int i = 0;
	for(i=0 ; i<argv; i++){
		printf("argc[%d] = %s\n", i, argc[i]);
	}
	return 0;
}
