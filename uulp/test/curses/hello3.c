/*************************************************************
 Date       : 2020年 05月 11日 星期一 20:04:16 CST
 Author     : rogerg6
 File       : hello3.c
 Description:
 	do : 每过一秒打印一行hello world，正常反色交替进行
*************************************************************/

#include <stdio.h>
#include <curses.h>
#include <unistd.h>

int main()
{
	int i;

	initscr();					// turn on curses

	clear();					// clear screen

	for (i = 0; i < LINES; i++) {
		move(i, i + 1);

		if (i % 2 == 1)
			standout();			// turn on stand mode, 一般为反色
		addstr("Hello World");	// write a string

		if (i % 2 == 1)
			standend();			// turn off stand mode

		sleep(1);
		refresh();					// update screen

	}


	getchar();					// wait for input

	endwin();					// turn off curses

	return 0;
}