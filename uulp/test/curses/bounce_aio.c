/*************************************************************
 Date       : 2020年 05月 15日 星期五 20:34:40 CST
 Author     : rogerg6
 File       : bounce_aio.c
 Description:
 	do : 用 aio read 的方法实现 键盘控制 hello 字符的运动方向 

 		 1. 设置struct kbcbuf中的变量，指明从哪里读，读到哪里，读多少，偏移量....
 		 								通知方式是信号还是..., 是哪个信号
 		 2. 发起ai_read 递交读入请求, 它并不会阻塞进程，相反会在完成时发送信号，读到的内容
 		    存放在 kbcbuf 中
*************************************************************/

#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <aio.h>

#define MESSAGE "hello"
#define BLANK   "     "

int row = 10;
int col = 0;
int dir = 1;
int delay = 200;
int done = 0;

struct aiocb kbcbuf;		// an aio control buf

int main()
{
	initscr();
	crmode();
	noecho();
	clear();

	signal(SIGIO, on_input);
	setup_aio_buffer();
	aio_read(&kbcbuf);			// 发起aio read

	signal(SIGALRM, on_alarm);
	set_ticker(delay);

	mvaddstr(row, col, MESSAGE);

	while (!done)
		pause();
	endwin();

	return 0;
}

void on_input(int signum)
{
	char *cp = (char *)kbcbuf.aio_buf;

	// check for errors
	if (aio_error(&kbcbuf) != 0)
		perror("reading failed");
	else
		// get number of chars read
		if (aio_return(&kbcbuf) == 1) {
			c = *cp;
			if (c == 'Q' || c == EOF)
				done = 1;
			else if (c == ' ')
				dir = -dir;
		}

	aio_read(&kbcbuf);			// 发起aio read
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

void setup_aio_buffer(void)
{
	static char input[1];		// 1 char of input

	kbcbuf.aio_fildes = 0;		// stdin
	kbcbuf.aio_buf    = input;	// buffer
	kbcbuf.aio_nbytes = 1;		// number to read
	kbcbuf.aio_offset = 0;		// offset in file

	kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;		// 通知方式为信号
	kbcbuf.aio_sigevent.sigev_signo  = SIGIO;				// 信号为SIGIO
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
