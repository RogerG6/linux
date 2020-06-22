/*************************************************************
 Date       : 2020年 06月 14日 星期日 10:25:40 CST
 Author     : rogerg6
 File       : socklib.h
 Description:
*************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>

#define HOSTLEN 256
#define BACKLOG 1

int make_server_socket(int portnum);
int connect_to_server(char *host, int portnum);
