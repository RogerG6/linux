/*************************************************************
 Date       : 2020年 05月 11日 星期一 20:28:23 CST
 Author     : rogerg6
 File       : hello5.c
 Description:
 	do : string 在屏幕上来回移动
*************************************************************/

#include <stdio.h>
#include <curses.h>
#include <unistd.h>

#define LEFTEDGE  10
#define RIGHTEDGE 30
#define ROW 	  10

int main()
{
	char mesg[]  = "Hello";
	char blank[] = "     ";
	int dir = +1;
	int pos = LEFTEDGE;

	initscr();
	clear();

	while (1) {
		move(ROW, pos);
		addstr(mesg);
		move(LINES - 1, COLS - 1);
		refresh();

		sleep(1);

		move(ROW, pos);
		addstr(blank);
		pos += dir;

		if (pos <= LEFTEDGE)
			dir = +1;
		if (pos >= RIGHTEDGE)
			dir = -1;
	}	


	return 0;
}