/*************************************************************
 Date       : 2020年 06月 12日 星期五 21:52:22 CST
 Author     : rogerg6
 File       : clnt.c
 Description:
 	do ：向服务端请求时间服务
 	　　　Ｕsage：　clnt hostname port
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "socklib.h"

#define LEN 4096
void talk_with_server(int fd);


int main(int ac, char *av[])
{
	int fd;

	if (ac != 3) {
		fprintf(stderr, "Usage : clnt host port\n");
		return -1;
	}

	fd = connect_to_server(av[1], atoi(av[2]));			// host, port
	if (fd == -1)
		exit(1);

	talk_with_server(fd);		// chat with server
	close(fd);

	return 0;
}

void talk_with_server(int fd)
{
	char buf[LEN];
	int n;

	n = read(fd, buf, LEN);
	write(1, buf, n);
}