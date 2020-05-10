/*************************************************************
 Date       : 2020年 05月 10日 星期日 13:20:48 CST
 Author     : rogerg6
 File       : play_again1.c
 Description:
 	do： 1. 无缓冲输入，输入的字符直接生效，无需键入enter
 		 2. 每次只读入一个字符

 	bug: 1. 当输入错误的选择（非y／n）时，总是有错误信息提示，需关闭
 		 2. 没有关闭回显
*************************************************************/

#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_response(char *);
void set_crmode(void);
void tty_mode(int how);

int main()
{
	int response;

	tty_mode(0);	// save mode
	set_crmode();	// set char-by-char mode
	response = get_response(QUESTION);
	tty_mode(1);	// restore mode

	return response;
}

int get_response(char *qus)
{
	int input;

	printf("%s (y/n)?", qus);
	while (1) {
		switch (input = getchar()) {
			case 'y':
			case 'Y': return 0;
			case 'n':
			case 'N':
			case EOF: return 1;
			default: 
				printf("\ncan't understand %c, ", input);
				printf("Please type y or n\n");
		}
	}
}

void set_crmode(void)
{
	struct termios ttystat;

	tcgetattr(0, &ttystat);
	ttystat.c_lflag &= ~ICANON;			// no buffering
	ttystat.c_cc[VMIN] = 1;				// one char one time
	tcsetattr(0, TCSANOW, &ttystat);
}

void tty_mode(int how)
{
	static struct termios original_mode;

	if (how == 0)
		tcgetattr(0, &original_mode);			// save current mode
	else
		tcsetattr(0, TCSANOW, &original_mode);	// restore original mode
}