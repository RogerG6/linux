/*************************************************************
 Date       : 2020年 06月 27日 星期六 15:24:49 CST
 Author     : rogerg6
 File       : lserv.c
 Description: License server server program version 2
 		      
 opt        : 处理client崩溃：
	 			  用poll的方式，定期查询ticket_array中，是否有被已结束的进程所占用的ticket
	 		      如果有，则收回
	 		  处理server崩溃：
	 		      client定期向server询问ticket是否合法，如果合法则继续持有；如果不合法则do nothing
	 		      最后收回failed ticket

*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/errno.h>
#define MSGLEN 128
#define RECLAIM_INTERVAL 60					// 每隔６０s检查是否有结束的进程

int setup(void);
void narrate(char *msg1, char *msg2, struct sockaddr_in *clientp);
void handle_request(char *req, struct sockaddr_in *client, socklen_t addlen);
void ticket_reclaim(void);

int main(int ac, char *av[])
{
	struct sockaddr_in clinet_addr;
	socklen_t addrlen = sizeof(clinet_addr);
	char buf[MSGLEN];
	int ret, sock;
	unsigned time_left;

	sock = setup();
	signal(SIGALRM, ticket_reclaim);
	alarm(RECLAIM_INTERVAL);

	while (1) {
		addrlen = sizeof(clinet_addr);
		ret = recvfrom(sock, buf, MSGLEN, 0, &clinet_addr, &addrlen);
		if (ret != -1) {
			buf[ret] = '\0';
			narrate("GOT:", buf, &clinet_addr);

			time_left = alarm(0);		// cancel alarm
			handle_request(buf, &clinet_addr, addrlen);
			alarm(time_left);			// restore alarm
		}

		else if (errno != EINTR)
			perror("recvfrom");
	}
}
