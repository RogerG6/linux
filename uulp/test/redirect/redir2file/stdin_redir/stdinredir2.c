/*************************************************************
 Date       : 2020年 05月 31日 星期日 21:16:05 CST
 Author     : rogerg6
 File       : stdinredir2.c
 Description:
 	open..close..dup..close 策略重定向
 		1. fd = open(newfile)
 		2. close(0)
 		3. 复制fd给0
 		4. close(fd)

 	2 个syscall
 		1. newfd = dup(oldfd);				// dup 复制也是遵循最低可用的文件描述符原则的
 		2. newfd = dup2(oldfd, newfd)
*************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define CLOSE_DUP
#define USE_DUP2

int main()
{
	int fd;
	char line[100];

	// read from stdin, and print 3 lines
	fgets(line, 100, stdin);	printf("%s", line);
	fgets(line, 100, stdin);	printf("%s", line);
	fgets(line, 100, stdin);	printf("%s", line);

	// open a new file
	if ((fd = open("/etc/passwd", O_RDONLY)) < 0) {
		fprintf(stderr, "Could not open file /etc/passwd\n");
		exit(1);
	}

#ifdef CLOSE_DUP
	// close stdin
	close(0);
	dup(fd);
#else
	dup2(fd, 0);
#endif

	fgets(line, 100, stdin);	printf("%s", line);
	fgets(line, 100, stdin);	printf("%s", line);
	fgets(line, 100, stdin);	printf("%s", line);

	return 0;
}