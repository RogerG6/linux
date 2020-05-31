/*************************************************************
 Date       : 2020年 05月 11日 星期一 19:46:47 CST
 Author     : rogerg6
 File       : hello1.c
 Description:
 	do : print "hello world" on center of screen

 	PS : 编译： gcc -o hello1 hello1.c -lcurses
*************************************************************/

#include <stdio.h>
#include <curses.h>

int main()
{
	initscr();					// turn on curses

	clear();					// clear screen
	move(10, 20);				// move to position (10, 20)
	addstr("Hello world");		// draw string "Hello world"
	move(LINES - 1, 0);			// move to left down
	refresh();					// update screen

	getchar();					// wait for input

	endwin();					// turn off curses

	return 0;
}