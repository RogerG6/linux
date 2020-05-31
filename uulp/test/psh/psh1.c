/*************************************************************
 Date       : 2020年 05月 19日 星期二 17:53:36 CST
 Author     : rogerg6
 File       : psh1.c
 Description:
 	do : 带提示符的shell

 		 每个字符串单独输入，最后输入回车
 		 eg. ls -l:
 		 		第一次：ls
 		 		第二次：-l
 		 		第三次：回车

 	bug : 运行一次ls就会退出shell
*************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAXARGS 20
#define ARGLEN  100

char * makestring(char *buf);
int execute(char *arglist[]);

int main()
{
	char *arglist[MAXARGS + 1];
	char argbuf[ARGLEN];
	int numargs = 0;
	int i;

	while (1) {
		printf("arg[%d] @ ", numargs);
		if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
			arglist[numargs++] = makestring(argbuf);
		else {
			arglist[numargs] = NULL;
			execute(arglist);

			numargs = 0;
		}
	}

	return 0;
}

int execute(char *arglist[])
{
	execvp(arglist[0], arglist);
	//printf("***%s : ", arglist[0]);
	perror("execvp failed");
	exit(1);
}

char * makestring(char *buf)
{
	char *cp;
	int len = strlen(buf);

	buf[len - 1] = '\0';			// fgets末尾有\n，将它替换成\0
	cp = malloc(len + 1);
	if (cp == NULL) {
		fprintf(stderr, "no memory\n");
		exit(1);
	}

	strcpy(cp, buf);
	return cp;
}