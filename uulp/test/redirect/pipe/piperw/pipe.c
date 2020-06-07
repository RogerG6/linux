/*************************************************************
 Date       : 2020年 06月 04日 星期四 21:12:37 CST
 Author     : rogerg6
 File       : pipe.c
 Description:
	pipe 读写情况：

		write end		do read
		-------------------------
		0				EOF
		>0				block

		read end		do write
		-------------------------
		0				EPIPE(损坏的管道)
		>0				block

	pipe size = 65536 bytes
		
*************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define oops(x, m)	{perror(x); exit(m);}

void initbuf(char *buf, int len)
{
	int i;
	for (i = 0; i < len; i++)
		buf[i] = 'y';
}

int main(void)
{
	int pp[2];
	int pid;
	int wlen, rlen, retval;
	char wbuf[BUFSIZ];
	char rbuf[BUFSIZ];

	//printf("BUFSIZ = %d\n", BUFSIZ);	BUFSIZ = 8192

	if (pipe(pp) == -1)
		oops("can't make pipe", 1);

	if ((pid = fork()) == -1) {
		oops("can't fork", 2);
	}
	else if (pid == 0) {
		// write to pp
		close(pp[0]);
		printf("child: read end closed\n");

		write(pp[1], "mesg1\n", 6);
		sleep(5);
		write(pp[1], "mesg2\n", 6);

		close(pp[1]);
		printf("child: write end closed\n");
	} else if (pid > 0) {
		// read from pp
		close(pp[1]);
		printf("parent: write end closed\n");

		//close(pp[0]);
		//printf("parent: read end closed\n");

#if 0
		while ((rlen = read(pp[0], rbuf, BUFSIZ)) > 0) {
			write(1, rbuf, rlen);
		}

		// test read return val
		if (rlen == 0)
			printf("read EOF\n");
		else if (rlen < 0)
			printf("read error\n");
#else
		//wait(NULL);

		dup2(pp[0], 0);
		close(pp[0]);


		dup2(0, 3);
		printf("after dup2(0, 3)\n");

		while (1) {
			rlen = read(3, rbuf, BUFSIZ);
			if (rlen == -1)
				oops("read error", 8);
			write(1, rbuf, rlen);

		}

		printf("pipe[0] = %d\n", pp[0]);


#endif

	}

	return 0;
}
