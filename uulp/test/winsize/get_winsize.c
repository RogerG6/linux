/*************************************************************
 Date: 2020年 04月 18日 星期六 16:26:56 CST
 Author: rogerg6
 File: get_winsize.c
 Description:
 	用于获得当前terimal 窗口的尺寸信息
*************************************************************/

#include <stdio.h>
#include <sys/ioctl.h>

int main()
{
	struct winsize terminal;

	// 获得win信息，放在struct terminal中
	ioctl(2, TIOCGWINSZ, &terminal);

	printf("Win size: row: %d, colum: %d\n", terminal.ws_row, terminal.ws_col);

	return 0;
}