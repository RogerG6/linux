/*************************************************************
 Date       : 2020年 06月 07日 星期日 20:55:12 CST
 Author     : rogerg6
 File       : timeclnt.c
 Description: time sever程序中的客户端程序，主要向连接的服务端请求时间
 		steps :
 			　　行为					系统调用
 			1. 获取电话线				socket
 			2. 呼叫服务器				connect
 			3. 传送数据				read/write
 			4. 挂断电话				close

*************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>

#define oops(msg)	{perror(msg); exit(1);}

int main(int ac, char *av[])
{
	struct sockaddr_in servaddr;			// the number to call
	struct hostent *hp;						// used to get number
	int sock_id, sock_fd;
	char mesg[BUFSIZ];						// to receive message
	int messlen;							// message length

	// step1 : get a socket
	sock_id = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_id == -1)
		oops("socket");

	// step2 : connect to server
	bzero(&servaddr, sizeof(servaddr));			// zero address

	hp = gethostbyname(av[1]);					// lookup hosts ip #
	if (hp == NULL)
		oops(av[1]);

	bcopy(hp->h_addr, (struct sockaddr *)&servaddr.sin_addr, hp->h_length);
	servaddr.sin_port = htons(atoi(av[2]));		// fill in port number
	servaddr.sin_family = AF_INET;				// fill in socket type

	// now dial
	if (connect(sock_id, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
		oops("connect");

	// step3 : transfer data from server, then hangup
	messlen = read(sock_id, mesg, BUFSIZ);		// read stuff
	if (messlen == -1)
		oops("read");
	if (write(1, mesg, messlen) != messlen)		// print on stdout
		oops("write");

	close(sock_id);

	return 0;
}