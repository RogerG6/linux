/*************************************************************
 Date       : 2020年 05月 19日 星期二 20:05:50 CST
 Author     : rogerg6
 File       : waitdemo2.c
 Description:
 	do : wait 演示 状态位

 	结论: wait(&status) 中 status 的高8位记录exit value
 								  低7位记录signal number
 								  位7  记录 发生错误时产生了内核映像

 		 在Unix中，进程只能以以下3种方式之一结束：
 		 	1. 成功，调用exit(0)
 		 	2. 失败，调用exit()，传给它一个非0值，man中有记录
 		 	3. 被信号杀死，如来自键盘，计时器的信号
*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define DELAY 10

void parent_code(int childpid);
void child_code(int delay);

int main()
{
	int newpid;

	printf("before: mypid is %d\n", getpid());

	if ((newpid = fork()) == -1)
		perror("fork");
	else if (newpid == 0)
		child_code(DELAY);
	else
		parent_code(newpid);

	return 0;
}

void child_code(int delay)
{
	printf("child %d here. will sleep for %d seconds\n", getpid(), delay);
	sleep(delay);
	printf("child done. about to exit\n");
	exit(17);
}

void parent_code(int childpid)
{
	int wait_rv;
	int child_status;
	int high_8, low_7, bit_7;

	wait_rv = wait(&child_status);		// 子进程返回的pid在wait_rv中，运行结果在child_status中

	high_8 = child_status >> 8;		// 1111 1111 0000 0000		exit value
	low_7  = child_status & 0x7f;	// 0000 0000 0111 1111		signal number
	bit_7  = child_status & 0x80;	// 0000 0000 1000 0000		core dump

	printf("done waiting for %d. Wait returned: %d\n", childpid, wait_rv);
	printf("status: exit = %d, sig = %d, core = %d\n", high_8, low_7, bit_7);
}