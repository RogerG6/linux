/*************************************************************
 Date       : 2020年 05月 10日 星期日 16:10:18 CST
 Author     : rogerg6
 File       : sigdemo2.c
 Description:
 	do : 捕捉信号，并忽略它
*************************************************************/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	int i;

	signal(SIGINT, SIG_IGN);		// ignore signal

	for (i = 0; i < 5; i++) {
		printf("hello\n");
		sleep(1);
	}

	return 0;
}
