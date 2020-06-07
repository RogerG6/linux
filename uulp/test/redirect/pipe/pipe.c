/*************************************************************
 Date       : 2020年 06月 01日 星期一 20:23:19 CST
 Author     : rogerg6
 File       : pipe.c
 Description:
 	do : pipe cmd1 cmd2		// 实现管道 cmd1 ｜ cmd2

 						fork
 		+-----------------+----------------+
 		|									|
 	child								  parent

 	close(p[0])							close(p[1])
 	dup2(p[1], 1)						dup2(p[0], 0)
 	close(p[1])							close(p[0])
 	exec(av1)							exec(av2)

	管道读写：父子进程通过管道读写时，都不关闭读写端，则失败
							   1写多读，则成功
							   多写1读，则失败

*************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define oops(x, m)	{perror(x); exit(m);}

int main(int ac, char *av[])
{
	int thepipe[2];
	int newfd;
	int pid;

	if (ac != 3) {
		fprintf(stderr, "Usage : pipe cmd1 cmd2\n");
		exit(1);
	}

	// get a pipe
	if (pipe(thepipe) == -1)
		oops("cannot get pipe", 1);

	// fork
	if ((pid = fork()) == -1)
		oops("cannot fork", 2);

	// parent process, exec av[2]
	if (pid > 0) {
		close(thepipe[1]);							// close write
#if 1
		if (dup2(thepipe[0], 0) == -1)
			oops("cannot redirect stdin", 3);
#endif
		close(thepipe[0]);							// stdin is duped, close pipe

		wait(NULL);

		execlp(av[2], av[2], NULL);
		oops(av[2], 4);
	}

	// child process, exec av[1]
	close(thepipe[0]);								// child doesn't read from pipe
#if 1
	if (dup2(thepipe[1], 1) == -1)
		oops("cannot redirect stdout", 5);
#endif
	close(thepipe[1]);								// stdout is duped, close pipe
	execlp(av[1], av[1], NULL);
	oops(av[1], 6);

	return 0;
}