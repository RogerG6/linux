/*************************************************************
 Date       : 2020年 05月 19日 星期二 17:46:00 CST
 Author     : rogerg6
 File       : exec1.c
 Description:
 	do : 演示 execvp 

 	summary : 只打印第一条消息，没有打印第二条消息。
 			  因为execvp把ls的程序载入当前进程，当前进程原先的程序数据被替换了，
 			  所以ls运行完成后，直接退出
*************************************************************/

#include <stdio.h>
#include <unistd.h>

int main()
{
	char *arglist[3];

	arglist[0] = "ls";
	arglist[1] = "-l";
	arglist[2] = 0;

	printf("*** About to exec ls -l\n");
	execvp("ls", arglist);
	printf("*** ls is done. bye\n");

	return 0;
}