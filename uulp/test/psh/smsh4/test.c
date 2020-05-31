/*************************************************************
 Date       : 2020年 05月 31日 星期日 18:46:58 CST
 Author     : rogerg6
 File       : test.c
 Description:
 	测试
*************************************************************/

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char * arglist[3];

	if (argc != 2) {
		fprintf(stderr, "Usage: test $var\n");
		return 1;
	}

	arglist[0] = "echo";
	arglist[1] = argv[1];
	arglist[2] = NULL;

	//execvp(arglist[0], arglist);
	printf("%s\n", argv[1]);
	
	return 0;
}