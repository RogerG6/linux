/*************************************************************
 Date       : 2020年 06月 07日 星期日 20:31:52 CST
 Author     : rogerg6
 File       : timeserv.c
 Description: time sever程序中的服务端程序，主要向连接的客户提供本机的时间
 		steps :
 			　　行为					系统调用
 			1. 获取电话线				socket
 			2. 分配号码				bind
 			3. 允许接入调用			listen
 			4. 等待电话				accept
 			5. 传送数据				read/write
 			6. 挂断电话				close

*************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <strings.h>

#define PORTNUM 13000
#define HOSTLEN 256
#define oops(msg)	{perror(msg); exit(1);}

int main(int ac, char *av[])
{
	struct sockaddr_in saddr;		// build our address here
	struct hostent *hp;				// this is part of our
	char hostname[HOSTLEN];			// address
	int sock_id, sock_fd;			// line id, file desc
	FILE *sock_fp;
	time_t thetime;					// the time we report

	// step1 : ask kernel for a socket
	sock_id = socket(PF_INET, SOCK_STREAM, 0);		// get a socket
	if (sock_id == -1)
		oops("socket");

	// step2 : bind address to socket, Address is host && port !!!
	bzero((void *)&saddr, sizeof(saddr));	// clear out struct

	gethostname(hostname, HOSTLEN);			// where am i
	hp = gethostbyname(hostname);			// get info about host


	bcopy((void *)hp->h_addr, (void *)&saddr, hp->h_length);		// fill in host part
	saddr.sin_port = htons(PORTNUM);		// fill in socket port
	saddr.sin_family = AF_INET;				// fill in addr family

	// step3 : allow incoming calls with Qsize=1 on socket
	if (listen(sock_id, 1) != 0)
		oops("listen");

	/* main loop : 
	 * 		step4 : accept() -> wait for call
	 *		step5 : write()  -> send data
	 *		step6 : close()  -> release connection
	 */
	while (1) {
		sock_fd = accept(sock_id, NULL, NULL);			// wait for call
		printf("Wow! got a call\n");
		if (sock_fd == -1)
			oops("accept");

		sock_fp = fdopen(sock_fd, "w");		// we'll write to socket as a stream
		if (sock_fp == NULL)
			oops("fdopen");

		thetime = time(NULL);

		fprintf(sock_fp, "The time here is ..");
		fprintf(sock_fp, "%s", ctime(&thetime));
		fclose(sock_fp);
	}

	return 0;
}