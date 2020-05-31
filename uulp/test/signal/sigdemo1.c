/*************************************************************
 Date       : 2020年 05月 10日 星期日 16:04:15 CST
 Author     : rogerg6
 File       : sigdemo1.c
 Description:
 	do : 捕捉信号

 		 当按下ctrl＋C时，会调用函数f，执行完后返回到主调函数
 		 过程好像子过程调用
*************************************************************/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void f(int signum)
{
	printf("OUCH!\n");
}

int main()
{
	int i;

	signal(SIGINT, f);		// install handler

	for (i = 0; i < 5; i++) {
		printf("hello\n");
		sleep(1);
	}

	return 0;
}