/*************************************************************
 Date       : 2020年 06月 12日 星期五 21:27:32 CST
 Author     : rogerg6
 File       : socklib.c
 Description:
 	do : 封装服务端和客户端的socket步骤
*************************************************************/

#include "socklib.h"

static int make_server_socket_q(int, int);

int make_server_socket(int portnum)
{
	return make_server_socket_q(portnum, BACKLOG);
}

/* purpose : Server : socket, bind, listen
 * returns : -1 for error, sock_id for ok
 */
static int make_server_socket_q(int portnum, int backlog)
{
	struct sockaddr_in saddr;				// build our address here
	struct hostent *hp;						
	char hostname[HOSTLEN];					// address
	int sock_id;							// the socket

	// 1. make a socket
	sock_id = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_id == -1)
		return -1;

	// 2. build address and bind it to socket
	bzero((void *)&saddr, sizeof(saddr));		// clear out struct
	gethostname(hostname, HOSTLEN);				// where am I?
	hp = gethostbyname(hostname);				// get info about host

	bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);			// fill in host part
	saddr.sin_port = htons(portnum);			// fill in socket port
	saddr.sin_family = AF_INET;					// fill in addr family
	if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
		return -1;

	// 3. arrange for incoming calls
	if (listen(sock_id, backlog) != 0)
		return -1;

	return sock_id;
}

/* purpose : Client : socket, connect
 * returns : -1 for error, sock_id for ok
 */
int connect_to_server(char *host, int portnum)
{
	int sock;
	struct sockaddr_in servadd;		// the number to call
	struct hostent *hp;				// used to get number

	// 1. get a socket
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		return -1;

	// 2. connect to server
	bzero(&servadd, sizeof(servadd));		// clear out struct
	hp = gethostbyname(host);				// lookup host's ip#
	if (hp == NULL)
		return -1;

	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);			// fill in host part
	servadd.sin_port = htons(portnum);			// fill in socket port
	servadd.sin_family = AF_INET;					// fill in addr family

	if (connect(sock, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
		return -1;
	return sock;
}