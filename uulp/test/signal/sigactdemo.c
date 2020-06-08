/*************************************************************
 Date       : 2020年 05月 12日 星期二 20:11:33 CST
 Author     : rogerg6
 File       : sigactdemo.c
 Description:
 	do : 演示 sigaction 的用法
 		------------------------------------
		signal ：提供一种简单但是不完整的信号处理机制
		sigaction ： 提供了复杂，明确定义的方法来控制进程如何应对各种信号组合的反应
 		------------------------------------

*************************************************************/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define INPUTLEN 100

void inthandler(int s);

int main()
{
	struct sigaction newhandler;		// new setting
	sigset_t blocked;					// set of blocked sigs
	char x[INPUTLEN];

	newhandler.sa_handler = inthandler;
	newhandler.sa_flags = SA_RESTART | SA_RESETHAND;

	sigemptyset(&blocked);			// clear all bits
	sigaddset(&blocked, SIGQUIT);	// add SIGQUIT to list
	newhandler.sa_mask = blocked;	// store blockmask

	if (sigaction(SIGINT, &newhandler, NULL) == -1)
		perror("sigaciton");
	else
		while (1) {
			fgets(x, INPUTLEN, stdin);
			printf("input: %s", x);
		}

	return 0;
}

void inthandler(int s)
{
	printf("Called with signal %d\n", s);
	sleep(s);
	printf("Done handling signal %d\n", s);;
}
