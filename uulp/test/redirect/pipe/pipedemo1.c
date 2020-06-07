/*************************************************************
 Date       : 2020年 06月 01日 星期一 19:30:18 CST
 Author     : rogerg6
 File       : pipedemo1.c
 Description:
 	purpose : 演示 pipe(apipe[2) 的用法
 			  apipe[1] : 写入端fd
 			  apipe[2] : 读取端fd
*************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	int len, i, apipe[2];		// two file descriptors
	char buf[BUFSIZ];

	// get a pipe
	if (pipe(apipe) == -1) {
		perror("could not make a pipe");
		exit(1);
	}

	while (fgets(buf, BUFSIZ, stdin)) {					// stdin to buf
		len = strlen(buf);

		if (write(apipe[1], buf, len) != len) {			// buf to pipe
			perror("writing to a pipe");
			break;
		}

		// reset buf
		for (i = 0; i < BUFSIZ; i++)
			buf[i] = 'x';

		len = read(apipe[0], buf, BUFSIZ);				// pipe to buf
		if (len == -1) {
			perror("reading from a pipe");
			break;
		}

		if (write(1, buf, len) != len) {				// buf to stdout
			perror("writing to stdout");
			break;
		}
	}

	return 0;
}