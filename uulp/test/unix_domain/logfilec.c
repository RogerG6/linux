/*************************************************************
 Date       : 2020年 07月 01日 星期三 22:25:54 CST
 Author     : rogerg6
 File       : logfilec.c
 Description: unix domain 客户端
 			　向服务器发送请求
 usage      ：logfilec "message"

*************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

#define oops(x, m) {perror(x); exit(m);}
#define SOCKET "/tmp/logfilesock"

int main(int ac, char *av[])
{
	int sock;
	struct sockaddr_un addr;
	socklen_t addrlen;
	char sockname[] = SOCKET;
	char *msg = av[1];

	if (ac != 2) {
		fprintf(stderr, "usage: logfilec message\n");
		exit(1);
	}

	sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (sock == -1)
		oops("socket", 2);

	// build a address
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, sockname);
	addrlen = strlen(sockname) + sizeof(addr.sun_family);

	if (sendto(sock, msg, strlen(msg), 0, &addr, addrlen) == -1)
		oops("sendto", 3);
}
