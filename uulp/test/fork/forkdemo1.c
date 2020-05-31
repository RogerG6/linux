/*************************************************************
 Date       : 2020年 05月 19日 星期二 19:28:42 CST
 Author     : rogerg6
 File       : forkdemo1.c
 Description:
	现象：
		Before: my pid is 2805							父
		After: my pid is 2805, fork() said 2806			父
		After: my pid is 2806, fork() said 0			子

*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


main()
{
	int ret_from_fork, mypid;

	mypid = getpid();
	printf("Before: my pid is %d\n", mypid);

	ret_from_fork = fork();

	sleep(1);
	printf("After: my pid is %d, fork() said %d\n", getpid(), ret_from_fork);
}