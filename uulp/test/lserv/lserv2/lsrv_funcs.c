/*************************************************************
 Date       : 2020年 06月 27日 星期六 15:33:35 CST
 Author     : rogerg6
 File       : lsrv_funcs.c
 Description: functions for license server
*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <sys/errno.h>
#include <stdlib.h>

#define SERVER_PORTNUM 2022
#define MSGLEN 128
#define TICKET_AVIAL 0
#define MAXUSERS 3
#define oops(x) {perror(x); exit(-1);}
#define RECLAIM_INTERVAL 60					// 每隔６０s检查是否有结束的进程

/* Important variables ********************************************** */
int ticket_array[MAXUSERS];			// our ticket array
int sd = -1;						// our socket
int num_tickets_out = 0;			// Number of tickets outstanding

char *do_hello(char *);
char *do_goodbye(char *);

void free_all_tickets(void);
void narrate(char *msg1, char *msg2, struct sockaddr_in *clientp);
char *do_hello(char *msg_p);
char *do_goodbye(char *msg_p);
static char *do_validate(char *msg);

/************************************************************* 
 * initialize license server 
 */

int setup(void)
{
	sd = make_dgram_server_socket(SERVER_PORTNUM);
	if (sd == -1)
		oops("make socket");

	free_all_tickets();
	return sd;
}

void free_all_tickets(void)
{
	int i;

	for (i = 0; i < MAXUSERS; i++)
		ticket_array[i] = TICKET_AVIAL;
}

/************************************************************* 
 * close down license server
 */
void shut_down(void)
{
	close(sd);
}

/************************************************************* 
 * branch on code in request
 */
void handle_request(char *req, struct sockaddr_in *client, socklen_t addlen)
{
	char *response;
	int ret;

	// act and compose a response
	if (strncmp(req, "HELO", 4) == 0)
		response = do_hello(req);
	else if (strncmp(req, "GBYE", 4) == 0)
		response = do_goodbye(req);
	else if (strncmp(req, "VALD", 4) == 0)
		response = do_validate(req);
	else
		response = "FAIL　invalid request";

	// send the response to the client
	narrate("SAID:", response, client);
	ret = sendto(sd, response, strlen(response), 0, client, addlen);
	if (ret == -1)
		perror("SERVER sendto failed");
}

/************************************************************* 
 * do hello
 * Give out a ticket if any are avialable
 * IＮ　msg_p				message recieved from client
 * Results: ptr to response
 * Note: return is in static buffer overwritten by each call
 */
char *do_hello(char *msg_p)
{
	int x;
	static char replybuf[MSGLEN];

	if (num_tickets_out >= MAXUSERS)
		return ("FAIL　no tickets avialable");

	for (x = 0; x < MAXUSERS && ticket_array[x] != TICKET_AVIAL; x++)
		;

	// a sanity check - should never happen
	if (x == MAXUSERS) {
		narrate("database corrupt", "", NULL);
		return ("FAIL database corrupt");
	}

	/* Found a free ticket. Record "name" of user(pid) in array
	 * Generate ticket of form: pid.slot
	 */
	ticket_array[x] = atoi(msg_p + 5);		// get pid in msg
	sprintf(replybuf, "TICK %d.%d", ticket_array[x], x);
	num_tickets_out++;
	return replybuf;
}

/************************************************************* 
 * do goodbye
 * Take back ticket client is returning
 * IＮ　msg_p				message recieved from client
 * Results: ptr to response
 * Note: return is in static buffer overwritten by each call
 */
char *do_goodbye(char *msg_p)
{
	int pid, slot;				// components of ticket

	/* The user's giving us back a ticket. First we need to get
	 * the ticket out of message, which looks like:
	 *
	 * GBYE pid.slot 
	 */
	if ((sscanf((msg_p + 5), "%d.%d", &pid, &slot) != 2) || (ticket_array[slot] != pid)){
		narrate("Bogus ticket", msg_p + 5, NULL);
		return ("FAIL invalid ticket");
	}

	// the ticket is valid, release it
	ticket_array[slot] = TICKET_AVIAL;
	num_tickets_out--;

	return ("THNX See ya!");
}


/************************************************************* 
 * do validate
 * Validate client's ticket
 * IN msg_p		message received from client
 * rets : ptr to response
 * NOTE : return is in static buffer overwritten by each call
 */
static char *do_validate(char *msg)
{
	int pid, slot;

	// msg looks like VALD pid.slot -- parse it and validate
	if (sscanf(msg + 5, "%d.%d", &pid, &slot) == 2 && ticket_array[slot] == pid)
		return "GOOD Valid ticket";

	narrate("Bogus ticket", msg + 5, NULL);
	return ("FAIL invalid ticket");
}


/************************************************************* 
 * reclaim tickets when client dies or corrupts
 */
void ticket_reclaim(void)
{
	int i;
	char tick[BUFSIZ];

	for (i = 0; i < MAXUSERS; i++) {
		if (ticket_array[i] != TICKET_AVIAL && (kill(ticket_array[i], 0) == -1 && errno == ESRCH)) {
			//process is gone, free up slot
			sprintf(tick, "%d.%d", ticket_array[i], i);
			narrate("freeing", tick, NULL);
			ticket_array[i] = TICKET_AVIAL;
			num_tickets_out--;
		}
	}

	alarm(RECLAIM_INTERVAL);		// reset alarm clock
}

/************************************************************* 
 * chatty news for debugging and logging purposes
 */
void narrate(char *msg1, char *msg2, struct sockaddr_in *clientp)
{
	fprintf(stderr, "\t\tSERVER: %s %s", msg1, msg2);
#if 0
	if (clientp)
		fprintf(stderr, "(%s: %d)", inet_ntoa(clientp->sin_addr), ntohs(clientp->sin_port));
#endif
	putc('\n', stderr);
}