/*************************************************************
 Date       : 2020年 05月 10日 星期日 13:20:48 CST
 Author     : rogerg6
 File       : play_again2.c
 Description:
 	do： 1. 无缓冲输入，输入的字符直接生效，无需键入enter
 		 2. 每次只读入一个字符
 		 3. 关闭回显
 		 4. 非阻塞输入，如果输入超时，则返回
 		 5. 当按下ctrl＋c时，ctrl_c_handler会被调用，返回到之前的终端模式
 		 				   也不会响应退出
*************************************************************/

#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <stdlib.h>

#define QUESTION "Do you want another transaction"
#define TRIES 3					// max tries
#define SLEEPTIME 2				// time per try
#define BEEP putchar('\a')		// alert user

int get_response(char *, int);
void set_crmode(void);
void tty_mode(int how);
int get_ok_char(void);
void set_nodelay_mode(void);
void ctrl_c_handler(int signum);

int main()
{
	int response;

	tty_mode(0);	// save mode
	set_crmode();	// set char-by-char mode
	set_nodelay_mode();		// 非阻塞模式
	signal(SIGINT, ctrl_c_handler);			// install handler
	signal(SIGQUIT, SIG_IGN);					// ignore QUIT signal
	response = get_response(QUESTION, TRIES);
	tty_mode(1);	// restore mode

	return response;
}

int get_response(char *qus, int maxtries)
{
	int input;

	printf("%s (y/n)?", qus);

	/* 强制输出，终端驱动程序不仅缓冲输入，还缓冲输出
	   驱动程序的缓冲输出，直到它收到一个换行符或程序试图从终端读取输入。
	   BEEP也被缓冲起来了
	 */ 
	fflush(stdout);				
	while (1) {
		sleep(SLEEPTIME);
		input = tolower(get_ok_char());
		if (input == 'y')
			return 0;
		if (input == 'n')
			return 1;
		if (maxtries-- == 0)
			return 2;
		//printf("123\n");
		BEEP;
	}
}

int get_ok_char(void)
{
	int c;

	while ((c = getchar()) != EOF && strchr("yYNn", c) == NULL)
		;

	return c;
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

void set_nodelay_mode(void)
{
	int flags;

	flags = fcntl(0, F_GETFL);
	flags |= O_NDELAY;
	fcntl(0, F_SETFL, flags);
}

void tty_mode(int how)
{
	static struct termios original_mode;
	static int original_flags;
	static int stored = 0;

	if (how == 0) {
		tcgetattr(0, &original_mode);			// save current mode
		original_flags = fcntl(0, F_GETFL);
		stored = 1;
	}
	else if (stored == 1) {
		tcsetattr(0, TCSANOW, &original_mode);	// restore original mode
		fcntl(0, F_SETFL, original_flags);
	}
}

void ctrl_c_handler(int signum)
{
	tty_mode(1);
	//printf("Handled\n");
	exit(1);
}