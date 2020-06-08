/*************************************************************
 Date       : 2020年 06月 07日 星期日 16:56:16 CST
 Author     : rogerg6
 File       : popendemo.c
 Description:
 	purpose : 演示库函数popen的用法

 	popen   ： 打开一个指向进程的带缓冲的连接，fp需要用pclose关闭，否则子进程会变成僵尸进程
*************************************************************/

#include <stdio.h>
#include <stdlib.h>

//FILE *popen(const char *cmd, const char *mode);


int main(void)
{
	FILE *fp;
	char buf[100];

	fp = popen("ls | sort", "r");

	while (fgets(buf, 100, fp) != NULL)
		printf("%s", buf);

	pclose(fp);

	return 0;
}