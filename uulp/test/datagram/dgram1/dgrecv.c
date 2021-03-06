/*************************************************************
 Date       : 2020年 06月 16日 星期二 21:34:19 CST
 Author     : rogerg6
 File       : dgrecv.c
 Description: datagram reciever
 usage		: dgrecv portnum
 action     : listens at the specfied port and reports mesg

*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define oops(x, m) {perror(x); exit(m);}

int make_dgram_server_socket(int);
int get_internet_address(char *, int, int *, struct sockaddr_in *);
void say_who_called(struct sockaddr_in *);

int main(int ac, char *av[])
{
	int port;			// portnum
	int sock;
	char buf[BUFSIZ];	// store received data
	size_t msglen;		// data length
	struct sockaddr_in saddr;			// sender's address
	socklen_t saddrlen;					// and it's length

	if (ac == 1 || (port = atoi(av[1])) <= 0) {
		fprintf(stderr, "usage		: dgrecv portnum\n");
		exit(1);
	}

	// get a socket and assign it a port number
	if ((sock = make_dgram_server_socket(port)) == -1)
		oops("can't make socket", 2);

	// recieve mesg on that socket
	saddrlen = sizeof(saddr);
	while ((msglen = recvfrom(sock, buf, BUFSIZ, 0, &saddr, &saddrlen)) > 0) {
		buf[msglen] = '\0';
		printf("dgrecv: got a message: %s\n", buf);
		say_who_called(&saddr);
	}

	return 0;
}


void say_who_called(struct sockaddr_in *addrp)
{
	char host[BUFSIZ];
	int port;

	get_internet_address(host, BUFSIZ, &port, addrp);
	printf("from: %s: %d\n", host, port);
}