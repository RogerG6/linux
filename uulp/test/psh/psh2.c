/*************************************************************
 Date       : 2020年 05月 19日 星期二 17:53:36 CST
 Author     : rogerg6
 File       : psh2.c
 Description:
 	do : 带提示符的shell
 		 使用fork，使得运行完ls后，依旧显示提示符等待输入

 		 每个字符串单独输入，最后输入回车
 		 eg. ls -l:
 		 		第一次：ls
 		 		第二次：-l
 		 		第三次：回车

 	bug : 按下ctrl ＋ c ，不仅会终止ls（如果ls运行很长时间），也会终止psh2 
*************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

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
	int pid, exitstatus;

	pid = fork();

	switch (pid) {
		case -1:
			perror("fork failed");
			exit(1);
		case 0:
			execvp(arglist[0], arglist);
			//printf("***%s : ", arglist[0]);
			perror("execvp failed");
			exit(1);
		default:
			while (wait(&exitstatus) != pid)
				;
			printf("child exited with status %d, %d\n", exitstatus>>8, exitstatus & 0x377);
	}
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