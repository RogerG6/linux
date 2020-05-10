/*************************************************************
 Date       : 2020年 05月 10日 星期日 13:20:48 CST
 Author     : rogerg6
 File       : play_again2.c
 Description:
 	do： 1. 无缓冲输入，输入的字符直接生效，无需键入enter
 		 2. 每次只读入一个字符
 		 3. 关闭回显

 	bug: 1. 本程序是一个atm小程序，如果上一个顾客离开，留下y／n，
 			下一个顾客来就能继续上一个顾客的操作，这是非法的
 			最好设有一定的延时，时间到则关闭
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
		}
	}
}

void set_crmode(void)
{
	struct termios ttystat;

	tcgetattr(0, &ttystat);
	ttystat.c_lflag &= ~ICANON;			// no buffering
	ttystat.c_cc[VMIN] = 1;				// one char one time
	ttystat.c_lflag &= ~ECHO;
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