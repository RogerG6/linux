/*************************************************************
 Date       : 2020年 05月 14日 星期四 18:04:37 CST
 Author     : rogerg6
 File       : bounce2d.c
 Description:
 	do : 实现小球在2D方向上的运动

 		 键盘： 通过getchar阻塞的方式获得用户的输入
 		 运动： 通过SIGALRM来控制，几个周期信号移动几次
 		 	   x 方向一个timer， x_ttg, x_ttm为初始，后续间隔
 		 	   y 方向一个timer， y_ttg, y_ttm为初始，后续间隔
*************************************************************/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <curses.h>
#include "bounce.h"

void set_up(void);
void wrap_up(void);
int set_ticker(int n_msec);
void ball_move(int signum);
int bounce_or_lose(struct ppball *bp);
void draw_wall(void);

struct ppball ball;		// ball 的实体

int main()
{
	int c;

	set_up();

	while ((c = getchar()) != 'Q') {
		if (c == 'f')
			ball.x_ttm--;
		else if (c == 's')
			ball.x_ttm++;
		else if (c == 'F')
			ball.y_ttm--;
		else if (c == 'S')
			ball.y_ttm++;
	}

	wrap_up();

	return 0;
}


void set_up(void)
{
	ball.y_pos = Y_INIT;
	ball.x_pos = X_INIT;
	ball.y_ttg = ball.y_ttm = Y_TTM;
	ball.x_ttg = ball.x_ttm = X_TTM;
	ball.y_dir = 1;
	ball.x_dir = 1;
	ball.symbol = DFL_SYMBOL;

	initscr();
	noecho();
	crmode();

	signal(SIGINT, SIG_IGN);

	draw_wall();

	mvaddch(ball.y_pos, ball.x_pos, ball.symbol);
	refresh();

	signal(SIGALRM, ball_move);
	set_ticker(1000 / TICKS_PER_SEC);
}

// 画围栏
void draw_wall(void)
{
	int i, y_start, x_start;

	// top, bottom
	x_start = LEFT_EDGE - 1;
	for (i = 0; i < RIGHT_EDGE - LEFT_EDGE + 2; i++) {
		mvaddch(TOP_ROW - 1, x_start + i, '+');
		mvaddch(BOT_ROW + 1, x_start + i, '+');
	}

	// left, right edge
	y_start = TOP_ROW - 1;
	for (i = 0; i < BOT_ROW - TOP_ROW + 3; i++) {
		mvaddch(y_start + i, LEFT_EDGE - 1, '+');
		//mvaddch(y_start + i, RIGHT_EDGE + 1, '+');
	}
}

void ball_move(int signum)
{
	int y_cur, x_cur, moved;

	signal(SIGALRM, SIG_IGN);		// don't get caught now
	y_cur = ball.y_pos;
	x_cur = ball.x_pos;
	moved = 0;

	if (ball.y_ttm > 0 && ball.y_ttg-- == 1) {
		ball.y_pos += ball.y_dir;		// move
		ball.y_ttg = ball.y_ttm;		// reset
		moved = 1;
	}

	if (ball.x_ttm > 0 && ball.x_ttg-- == 1) {
		ball.x_pos += ball.x_dir;		// move
		ball.x_ttg = ball.x_ttm;		// reset
		moved = 1;
	}

	if (moved) {
		mvaddch(y_cur, x_cur, BLANK);
		mvaddch(y_cur, x_cur, BLANK);
		mvaddch(ball.y_pos, ball.x_pos, ball.symbol);
		bounce_or_lose(&ball);
		move(LINES - 1, COLS - 1);
		refresh();
	}

	signal(SIGALRM, ball_move);	// for unreliable system
}

int bounce_or_lose(struct ppball *bp)
{
	int retval = 0;

	// out of row
	if (bp->y_pos == TOP_ROW) {
		bp->y_dir = 1;
		retval = 1;
	} else if (bp->y_pos == BOT_ROW) {
		bp->y_dir = -1;
		retval = 1;
	}

	// out of col
	if (bp->x_pos == LEFT_EDGE) {
		bp->x_dir = 1;
		retval = 1;
	} else if (bp->x_pos == RIGHT_EDGE) {
		bp->x_dir = -1;
		retval = 1;
	}

	return retval;	
}

void wrap_up(void)
{
	set_ticker(0);		// put back to normal
	endwin();
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


