/*************************************************************
 Date       : 2020年 06月 16日 星期二 21:34:19 CST
 Author     : rogerg6
 File       : dgsend.c
 Description: datagram sender
 usage		: dgsend hostname portnum "message"
 action		: sends mesg to hostname:portnum

*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define oops(x, m) {perror(x); exit(m);}

int make_dgram_client_socket();
int make_internet_address(char *, int , struct sockaddr_in *);

int main(int ac, char *av[])
{
	int sock;
	char *msg;
	struct sockaddr_in saddr;		// sender's address here

	if (ac != 4) {
		fprintf(stderr, " usage		: dgsend hostname portnum message\n");
		exit(1); 
	}

	msg = av[3];

	// get a datagram socket
	if ((sock = make_dgram_client_socket()) == -1)
		oops("can't make socket", 2);

	// combine hostname and portnum of destination into an address
	make_internet_address(av[1], atoi(av[2]), &saddr);

	// send a string through the socket to that address
	if (sendto(sock, msg, strlen(msg), 0, &saddr, sizeof(saddr)) == -1)
		oops("sendto failed", 3);

	return 0;
}