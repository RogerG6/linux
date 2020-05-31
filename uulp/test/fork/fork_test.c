/*************************************************************
 Date       : 2020年 05月 26日 星期二 18:27:33 CST
 Author     : rogerg6
 File       : fork_test.c
 Description:
 	summary : 在父进程中，fork返回 pid
 			  在子进程中，fork返回 0
*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int n, rv = 99;

	//printf("before rv = %d\n", rv);
	for (n = 0; n < 10; n++) {
		printf("my pid = %d, n = %d\n", getpid(), n);
		sleep(1);
		if ((rv = fork()) != 0) {
	//		printf("pid = %d, rv = %d\n", getpid(), rv);
			exit(0);
		}
	//	else
	//		printf("pid = %d, rv = %d\n", getpid(), rv);
	}

	return 0;
}