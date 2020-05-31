/*************************************************************
 Date       : 2020年 05月 19日 星期二 20:05:50 CST
 Author     : rogerg6
 File       : waitdemo1.c
 Description:
 	do : wait 演示

 	结论: 父进程在wait处阻塞，直到子进程返回，唤醒父进程
 		 wait 返回的是子进程的 pid
*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define DELAY 5

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
	exit(200);
}

void parent_code(int childpid)
{
	int wait_rv;

	wait_rv = wait(NULL);
	printf("done waiting for %d. Wait returned: %d\n", childpid, wait_rv);
}