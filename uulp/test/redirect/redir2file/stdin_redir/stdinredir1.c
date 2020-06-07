/*************************************************************
 Date       : 2020年 05月 31日 星期日 21:16:05 CST
 Author     : rogerg6
 File       : stdinredir1.c
 Description:
 	close and open 策略重定向
 		1. 启动程序时，进程有3个fd，分别是0，1，2 对应stdin，stdout，stderr
 		2. 当关闭stdin后，再打开文件，当前最低可用的文件描述符是0
 		3. 因此，文件被定向到stdin
 		
*************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int fd;
	char line[100];

	// read from stdin, and print 3 lines
	fgets(line, 100, stdin);	printf("%s", line);
	fgets(line, 100, stdin);	printf("%s", line);
	fgets(line, 100, stdin);	printf("%s", line);

	// close stdin
	close(0);

	// open a new file
	if ((fd = open("/etc/passwd", O_RDONLY)) != 0) {
		fprintf(stderr, "Could not open data as fd0\n");
		exit(1);
	}

	fgets(line, 100, stdin);	printf("%s", line);
	fgets(line, 100, stdin);	printf("%s", line);
	fgets(line, 100, stdin);	printf("%s", line);

	return 0;
}