/*************************************************************
 Date       : 2020年 05月 14日 星期四 21:39:15 CST
 Author     : rogerg6
 File       : bounce_async.c
 Description:
 	do : 用 O_ASYNC 的方法实现 键盘控制 hello 字符的运动方向

 		 1. 设置键盘输入的 handler							// 进程收到信号做相应的动作
 		 2. F_SETOWN 告诉内核发送输入通知信号给 指定进程		// 内核将信号发送给进程
 		 3. 设置 fd＝0 的O_ASYNC 位来打开输入信号			// 终端可以输入，产生信号
*************************************************************/

#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#define MESSAGE "hello"
#define BLANK   "     "

void enable_kbd_signals(void);
void on_alarm(int signum);
void on_input(int signum);
int set_ticker(int n_msec);

int row = 10;
int col = 0;
int dir = 1;
int delay = 200;
int done = 0;

int main()
{
	initscr();
	crmode();
	noecho();
	clear();

	signal(SIGIO, on_input);		// handler for input
	enable_kbd_signals();			// turn on kbd signals

	signal(SIGALRM, on_alarm);		// handler for alarm
	set_ticker(delay);

	// main loop
	while (!done)					
		pause();

	endwin();

	return 0;
}

void on_input(int signum)
{
	int c = getch();

	if (c == 'Q')
		done = 1;
	else if (c == ' ')
		dir = -dir;
}

void on_alarm(int signum)
{
	signal(SIGALRM, on_alarm);

	move(row, col);			// clear prev string
	addstr(BLANK);

	col += dir;				// move to next position

	move(row, col);			// draw new string
	addstr(MESSAGE);		
	refresh();

	if (dir == -1 && col <= 0)
		dir = 1;
	else if (dir == 1 && col >= COLS)
		dir = -1;
}

void enable_kbd_signals(void)
{
	int fd_flags;

	fcntl(0, F_SETOWN, getpid());
	fd_flags = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, fd_flags | O_ASYNC);
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
