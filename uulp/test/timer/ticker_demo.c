/*************************************************************
 Date       : 2020年 05月 11日 星期一 21:30:52 CST
 Author     : rogerg6
 File       : ticker_demo.c
 Description:
	do : 倒计时程序
*************************************************************/

#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int set_ticker(int n_msec);
void countdown(int signum);

int main()
{
	signal(SIGALRM, countdown);

	if (set_ticker(1000) == -1)
		perror("set_ticker");
	else
		while (1)
			pause();

	return 0;
}

void countdown(int signum)
{
	static int num = 10;

	printf("%d...\n", num--);
	fflush(stdout);

	if (num < 0) {
		printf("DONE\n");
		exit(0);
	}
}

int set_ticker(int n_msec)
{
	struct itimerval new_timerset;
	long n_sec, n_usec;

	n_sec = n_msec / 1000;					// seconds
	n_usec = (n_msec % 1000) * 1000;		// useconds

	new_timerset.it_value.tv_sec = n_sec;
	new_timerset.it_value.tv_usec = n_usec;

	new_timerset.it_interval.tv_sec = n_sec;
	new_timerset.it_interval.tv_usec = n_usec;

	return setitimer(ITIMER_REAL, &new_timerset, NULL);		// set real timer
}