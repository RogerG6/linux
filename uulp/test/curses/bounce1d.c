/*************************************************************
 Date       : 2020年 05月 12日 星期二 21:09:05 CST
 Author     : rogerg6
 File       : bounce1d.c
 Description:
 	do : hello 字符串在屏幕上来回移动，
 		 空格 ： 反方向运动
 		 s   ： 减小移动速度，slower
 		 f   ： 增加移动速度，faster
 		 q   ： 退出游戏

 	bug : 按下q， 退回到终端有问题，会变成noecho模式
*************************************************************/

#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

#define MESSAGE "hello"
#define BLANK	"     "

void move_mesg(int signum);
int set_ticker(int n_msec);

int row;		// string current row
int col;		// string current column
int dir;		// move direction

int main()
{
	int delay;
	int ndelay;		// new delay
	int c;

	initscr();
	//crmode();
	//noecho();
	clear();

	row = 10;
	col = 0;
	dir = 1;
	delay = 200;		// 200ms

	move(row, col);
	addstr(MESSAGE);
	signal(SIGALRM, move_mesg);
	set_ticker(delay);

	while (1) {
		ndelay = 0;
		c = getchar();

		if (c == 'q')				// quit
			break;
		if (c == ' ')				// change direction
			dir = -dir;
		if (c == 's')				// slower
			ndelay = delay * 2;
		if (c == 'f' && delay > 2)	// faster
			ndelay = delay / 2;

		if (ndelay > 0)
			set_ticker(delay = ndelay);
	}

	return 0;
}

void move_mesg(int signum)
{
	//signal(SIGALRM, move_mesg);	// reset, just in case

	move(row, col);
	addstr(BLANK);		// erase prev string

	col += dir;
	move(row, col);
	addstr(MESSAGE);	// draw current string

	refresh();

	if (dir == -1 && col <= 0)
		dir = 1;
	else if (dir == 1 && col + strlen(MESSAGE) >= COLS)
		dir = -1;
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