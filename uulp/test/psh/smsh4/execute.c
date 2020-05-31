/*************************************************************
 Date       : 2020年 05月 26日 星期二 21:52:35 CST
 Author     : rogerg6
 File       : execute.c
 Description:
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "varlib.h"

int execute(char *argv[])
{
	extern char **environ;		// current evnironment

	int pid;
	int child_info = -1;

	if (argv[0] == NULL)
		return 0;

	if ((pid = fork()) == -1)
		perror("fork");
	else if (pid == 0) {
		environ = VLtable2environ();		// update environment
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execvp(argv[0], argv);
		perror("cannot execute command");
		exit(1);
	} else {
		if (wait(&child_info) == -1)
			perror("wait");
	}
	return child_info;
}