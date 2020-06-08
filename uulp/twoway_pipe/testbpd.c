/*************************************************************
 Date       : 2020年 06月 08日 星期一 21:49:50 CST
 Author     : rogerg6
 File       : testbpd.c
 Description:
 	do : 测试该系统是否支持双向pipe

 	     如果支持双向管道，在管道内部有２个队列
 	     一个从pipe[0]到pipe[1], 另一个从pipe[1]到pipe[0]

	result : write into pipe[0] failed: Bad file descriptor
			 本系统不支持双向管道
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

main()
{
	int p[2];

	if (pipe(p) == -1)
		exit(1);

	//close(p[1]);

	if (write(p[0], "hello", 5) == -1)
		perror("write into pipe[0] failed");
	else
		printf("write into pipe[0] worked\n");
}