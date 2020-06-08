/*************************************************************
 Date       : 2020年 06月 07日 星期日 17:31:39 CST
 Author     : rogerg6
 File       : mypopen.c
 Description:
 	do ： 库函数 FILE *fp = popen("who | sort", "r") 的实现

 	details ： 
	 						pipe(pp)
	 						fork()
	 	parent 				  |						 child
	 		---------------------------------------------
	 		|											|
	 	  close(p[1])								close(p[0])
	 	  fp = fdopen(p[0], "r")					dup2(p[1], 1)
	 	  return fp 							    close(p[1])
	 	  											execl("/bin/sh", "sh", "-c", "who|sort", NULL)

	 	PS : 这execl中运行的就是shell本身，"-c"告诉shell执行完某命令后退出

*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

#define READ  0
#define WRITE 1

FILE *mypopen(const char *cmd, const char *mode)
{
	int parent_end, child_end;
	int pp[2], pid;
	FILE *fp;

	// figure out direction
	if (*mode == 'r') {
		parent_end = READ;
		child_end = WRITE;
	} else if (*mode == 'w') {
		parent_end = WRITE;
		child_end = READ;
	} else
		return NULL;

	// make pipe
	if (pipe(pp) == -1)
		return NULL;

	// fork
	if ((pid = fork()) == -1)
		return NULL;

	/* ***************************  parent process *************************************/
	else if (pid > 0) {
		close(pp[child_end]);
		fp = fdopen(pp[parent_end], mode);
		return fp;
	}

	/* ***************************  child process *************************************/
	if (close(pp[parent_end]) == -1)
		exit(1);

	if (dup2(pp[child_end], child_end) == -1)
		exit(1);

	if (close(pp[child_end]) == -1)
		exit(1);

	execl("/bin/sh", "sh", "-c", cmd, NULL);
	exit(1);
}