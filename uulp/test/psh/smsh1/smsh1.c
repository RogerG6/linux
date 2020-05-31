/*************************************************************
 Date       : 2020年 05月 26日 星期二 20:21:19 CST
 Author     : rogerg6
 File       : smsh1.c
 Description:
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"

#define DFL_PROMPT ">"

void setup(void);

int main()
{
	char *cmdline, *prompt, **arglist;
	int result;

	prompt = DFL_PROMPT;
	setup();

	while ((cmdline = next_cmd(prompt, stdin)) != NULL) {
		//fprintf(stdout, "cmdline: %s\n", cmdline);
		if ((arglist = splitline(cmdline)) != NULL) {
			result = execute(arglist);
			freelist(arglist);
		}
		free(cmdline);
	}

	return 0;
}

void setup(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
	fprintf(stderr, "Error: %s, %s\n", s1, s2);
	exit(n);
}