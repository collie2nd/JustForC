#include <stdio.h>

int main(){

	printf("input the int value i:\n");

	int i, j;
	scanf("%d", &i);
	
	printf("input the int value j:\n");
	scanf("%d", &j);

	
	if(0 == j ){
		/* 0==j编码规范，假如少打了等号会报错提醒*/
		fprintf(stderr, "\"j\" must != 0\n");
		return 1;
	}else{
		printf("i/j=%d\n", i/j);
	}
	return 0;
}

//	./a.out >> a.txt (等价于./a.out 1>>a.txt )
//	标准输出流重定向到a.txt
//	>>不会覆盖原文件，在文件末尾以追加的形式输出
//	cat a.txt(cat 显示文件内容)会出现在文件末尾
//	>单箭头会覆盖原文件，只保留最新的内容
//
//	./a.out < input.txt
//	<标准输入流重定向到input.txt
//	输入流没有追加和覆盖的区分故没有<<
//
//	./a.out 2>f.txt 2>>f.txt
//	标准错误流重定向到f.txt，也分覆盖和追加两种模式
