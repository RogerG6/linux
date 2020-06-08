/*************************************************************
 Date       : 2020年 06月 07日 星期日 17:18:16 CST
 Author     : rogerg6
 File       : popen_ex3.c
 Description:
 	do ： 将popen和邮件程序相连接，用来提示用户一些故障

*************************************************************/

#include <stdio.h>

int main(void)
{
	FILE *fp;

	fp = popen("mail admin backup", "w");
	fprintf(fp, "Error with backup!!\n");
	pclose(fp);

	return 0;
}