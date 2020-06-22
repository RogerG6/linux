/*************************************************************
 Date       : 2020年 06月 12日 星期五 21:27:01 CST
 Author     : rogerg6
 File       : serv.c
 Description:
 	do　：向用户端提供当前主机上的时间
 	details　：采用time获取当前时间，用ctime将时间转换成string，写入socket
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "socklib.h"

#define PORTNUM 13000

void process_request(int fd);

main()
{
	int sock, fd;
	int port = PORTNUM;

	sock = make_server_socket(port);
	if (sock == -1) {
		perror("socket");
		exit(1);
	}

	while (1) {
		fd = accept(sock, NULL, NULL);		// take next call
		if (fd == -1)
			break;
		process_request(fd);				// chat with client
		close(fd);
	}
}

void process_request(int fd)
{
	time_t now;
	char *cp;

	time(&now);
	cp = ctime(&now);
	write(fd, cp, strlen(cp));
}