/*************************************************************
 Date       : 2020年 05月 26日 星期二 20:21:19 CST
 Author     : rogerg6
 File       : smsh1.c
 Description:
 	do : 1. 从终端读入cmd执行
 		 2. 加入shell逻辑：if，then，fi
 		 3. environment
 		 4. 变量

 	bug : 
 	
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"
#include "varlib.h"

#define DFL_PROMPT ">"

void setup(void);

int main(void)
{
	char *cmdline, *prompt, **arglist;
	int result;

	prompt = DFL_PROMPT;
	setup();

	while ((cmdline = next_cmd(prompt, stdin)) != NULL) {
		//fprintf(stdout, "cmdline: %s\n", cmdline);
		if ((arglist = splitline(cmdline)) != NULL) {
			result = process(arglist);
			freelist(arglist);
		}
		free(cmdline);
	}

	return 0;
}

void setup(void)
{
	extern char **environ;

	VLenviron2table(environ);		// environment to vartab
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
	fprintf(stderr, "Error: %s, %s\n", s1, s2);
	exit(n);
}