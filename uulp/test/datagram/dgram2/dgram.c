/*************************************************************
 Date       : 2020年 06月 16日 星期二 21:34:19 CST
 Author     : rogerg6
 File       : dgram.c
 Description: support functions for datagram based programs

*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define HOSTLEN 256

int make_internet_address();

int make_dgram_server_socket(int portnum)
{
	struct sockaddr_in saddr;		// build our address here
	char hostname[HOSTLEN];			// address
	int sock_id;

	// get a socket
	sock_id = socket(PF_INET, SOCK_DGRAM, 0);

	// build address and bind it to socket
	gethostname(hostname, HOSTLEN);
	make_internet_address(hostname, portnum, &saddr);

	if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
		return -1;
	return sock_id;
}

/* (host, port) -> *addrp */
int make_internet_address(char *hostname, int port, struct sockaddr_in *addrp)
{
	struct hostent *hp;

	bzero((void *)addrp, sizeof(struct sockaddr_in));
	hp = gethostbyname(hostname);
	if (hp == NULL)
		return -1;
	bcopy((void *)hp->h_addr, (void *)&addrp->sin_addr, hp->h_length);
	addrp->sin_port = htons(port);
	addrp->sin_family = PF_INET;
	
	return 0;
}

int make_dgram_client_socket()
{
	return socket(PF_INET, SOCK_DGRAM, 0);
}

/* *addrp -> (host, port) */
int get_internet_address(char *host, int len , int *portp, struct sockaddr_in *addrp)
{
	strncpy(host, inet_ntoa(addrp->sin_addr), len);
	*portp = ntohs(addrp->sin_port);
	return 0;
}
