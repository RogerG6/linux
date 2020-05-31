/*************************************************************
 Date       : 2020年 05月 11日 星期一 22:11:11 CST
 Author     : rogerg6
 File       : sigdemo3.c
 Description:
 	do : 测试当前系统中，进程是如何响应信号组合的

 	结果：
 		1. ^C^C^C^C 	只能处理2个，并且是第一个处理完再处理第二个。没有杀死进程，也没有递归处理
 		2. ^\^C^\^C		^\
 							^C  在上一个handler中按下
 							^C  在上一个handler中按下^C^\，它先处理上一个^C，再处理这个^C，然后处理下一个^\
 						^\		
 		3. hel^Clo		先处理^C，再输入lo，最后读取的是lo

*************************************************************/

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define INPUTLEN 100

void inthandler(int signum);
void quithandler(int signum);

int main()
{
	char input[INPUTLEN];
	int n_chars;

	signal(SIGINT, inthandler);
	signal(SIGQUIT, quithandler);

	do {
		printf("Type a message: ");
		n_chars = read(0, input, INPUTLEN);

		if (n_chars == -1)
			perror("read returned error");
		else {
			input[n_chars] = '\0';
			printf("You typed: %s\n", input);
		}
	}while (strncmp(input, "quit", 4) != 0);

	return 0;
}

void inthandler(int signum)
{
	printf("Received signal %d .. waiting\n", signum);
	sleep(2);
	printf("Leaving inthandler\n");
}

void quithandler(int signum)
{
	printf("Received signal %d .. waiting\n", signum);
	sleep(3);
	printf("Leaving quithandler\n");
}