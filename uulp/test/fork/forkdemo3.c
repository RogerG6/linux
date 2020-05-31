/*************************************************************
 Date       : 2020年 05月 26日 星期二 19:48:28 CST
 Author     : rogerg6
 File       : forkdemo3.c
 Description:

*************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

main()
{
	int i;

	if (fork() != 0)		// 当父进程exit后，程序就感觉是切换到后台进行了
		exit(0);

	for (i = 1; i <= 10; i++) {
		printf("still here..\n");
		sleep(i);
	}
	return 0;
}