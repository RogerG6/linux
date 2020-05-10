/*************************************************************
 Date       : 2020年 05月 03日 星期日 17:47:36 CST
 Author     : rogerg6
 File       : setecho.c
 Description:
 	开启终端回显与否
*************************************************************/

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct termios info;

	if (argc < 2) {		
		fprintf(stderr, "Usage: setecho y/n\n");
		exit(1);
	}

	// 获取设置
	//if (tcgetattr(0, &info) == -1) {
	if (tcgetattr(1, &info) == -1) {
		perror("tcgetattr");
		exit(1);
	}

	// 修改设置
	if (argv[1][0] == 'y')
		info.c_lflag |= ECHO;
	else
		info.c_lflag &= ~ECHO;

	// 写入设置
	tcsetattr(0, TCSANOW, &info);

	return 0;
}
