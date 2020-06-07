/*************************************************************
 Date       : 2020年 05月 31日 星期日 22:20:33 CST
 Author     : rogerg6
 File       : whotofile.c
 Description:
 	purpose : show how to redirect output to file for another program
 	idea    : fork -> redirect output in child -> exec
 			  还是利用了最低可用的文件描述符原则

*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	int fd, pid;

	printf("About to run who into a file\n");

	// create a new process or quit
	if ((pid = fork()) == -1) {
		perror("fork");
		exit(1);
	} else if (pid == 0) {			// child process
		close(1);
		if ((fd = creat("userlist", S_IRWXU)) < 0) {
			fprintf(stderr, "can't creat file userlist\n");
			exit(1);
		}
		execlp("who", "who", NULL);
		perror("execlp");
		exit(1);
	}

	if (pid != 0) {
		wait(NULL);
		printf("Done running who, results in userlist\n");
	}

	return 0;
}