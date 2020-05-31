/*************************************************************
 Date       : 2020年 05月 19日 星期二 19:34:29 CST
 Author     : rogerg6
 File       : forkdemo2.c
 Description:
 	现象：
		My pid is 2833
		My pid is 2833
		My pid is 2835
		My pid is 2834
		My pid is 2839
		My pid is 2836
		My pid is 2837
		My pid is 2838
		My pid is 2840

	结论: 有 n 个fork()，则会产生 2^n - 1 个子进程，加上父进程是 2^n 个进程
	
*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main()
{
	int ret;

	printf("My pid is %d\n", getpid());
	fork();
	fork();
	ret = fork();
	printf("My pid is %d\n", getpid());

#if 0
	if (ret == 0)
		printf("child, my pid is %d\n", getpid());
	else if (ret > 0)
		printf("parent, my pid is %d\n", getpid());
#endif
	
}