/*************************************************************
 Date       : 2020年 05月 10日 星期日 13:20:48 CST
 Author     : rogerg6
 File       : play_again0.c
 Description:
 	bug ： 不能识别sure， ok等等输入
*************************************************************/

#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_response(char *);

int main()
{
	int response;

	response = get_response(QUESTION);

	return response;
}

int get_response(char *qus)
{
	printf("%s (y/n)?", qus);
	while (1) {
		switch (getchar()) {
			case 'y':
			case 'Y': return 0;
			case 'n':
			case 'N':
			case EOF: return 1;

		}
	}
}