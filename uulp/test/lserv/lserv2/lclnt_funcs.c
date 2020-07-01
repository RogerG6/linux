/*************************************************************
 Date       : 2020年 06月 23日 星期二 22:19:52 CST
 Author     : rogerg6
 File       : lclnt_funcs.c
 Description: functions for the client of the license server
*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// important variables used throughout
static int pid = -1;			// our PID
static int sd = -1;				// our communications socket
static struct sockaddr serv_addr;	// server address
static socklen_t serv_alen;			// length of address
static char ticket_buf[128];		// buffer to hold our ticket
static have_ticket = 0;				// set when we have a ticket

#define MSGLEN 128
#define SERVER_PORTNUM 2022
#define HOSTLEN 512
#define oops(p) {perror(p); exit(1);}

char *do_transaction();

/* get pid, socket, and address of license server
 * IN no args
 * RET nothing, dies on error
 * notes: assumes server is on same host as client
 */
void setup(void)
{
	char hostname[BUFSIZ];

	pid = getpid();							//for tickets and msg
	sd = make_dgram_client_socket();		// to talk to server
	if (sd == -1)
		oops("Cannot create socket");
	gethostname(hostname, HOSTLEN);
	make_internet_address(hostname, SERVER_PORTNUM, &serv_addr);
	serv_alen = sizeof(serv_addr);
}

void shut_down(void)
{
	close(sd);
}

/* get a ticket from the license server
 * rets : 0 for success, -1 for error
 */
int get_ticket(void)
{
	char *response;
	char buf[MSGLEN];

	if (have_ticket)
		return 0;				// don't be greedy

	sprintf(buf, "HELO %d", pid);		// compose request

	if ((response = do_transaction(buf)) == NULL)
		return -1;

	/* parse the response from server and see if we got a ticket
	 * on success, the mesg is: TICK ticket-string
	 * on failure, the mesg is: FAIL failure-msg
	 */
	if (strncmp(response, "TICK", 4) == 0) {
		strcpy(ticket_buf, response + 5);			// grab ticket id
		have_ticket = 1;							// set this flag
		narrate("got ticket", ticket_buf);
		return 0;
	}

	if (strncmp(response, "FAIL", 4) == 0)
		narrate("Could not get ticket", response);
	else
		narrate("Unkown message:", response);

	return -1;
}

int release_ticket()
{
	char buf[MSGLEN];
	char *response;

	// don't hold a ticket
	if (!have_ticket)
		return 0;

	sprintf(buf, "GBYE %s", ticket_buf);			// compose message
	if ((response = do_transaction(buf)) == NULL)
		return -1;

	/* examine response
	 * success: THNX info-string
	 * failure: FAIL error-string
	 */
	if (strncmp(response, "THNX", 4) == 0){
		narrate("released ticket 0K", "");
		return 0;
	}

	if (strncmp(response, "FAIL", 4) == 0)
		narrate("release failed", response + 5);
	else
		narrate("Unkown message", response);

	return -1;
}


/* do_transaction
 * Send a request to the server and get a response back
 * IN msg_p					message to send
 * Results : pointer to message string, or NULL for error
 * 			NOTE: pointer returned is to static storage
 *				  overwritten by each successive call.
 * note: for extra security, compare retaddr to serv_addr(why?)
 */
char *do_transaction(char *msg)
{
	static char buf[MSGLEN];
	struct sockaddr retaddr;
	socklen_t addrlen = sizeof(retaddr);
	int ret;

	ret = sendto(sd, msg, strlen(msg), 0, &serv_addr, serv_alen);
	if (ret == -1) {
		syserr("sendto");
		return NULL;
	}

	// get the response back
	ret = recvfrom(sd, buf, MSGLEN, 0, &retaddr, &addrlen);
	if (ret == -1) {
		syserr("recvfrom");
		return NULL;
	}

	// now return  the message itself
	return buf;
}

/* ask server to make sure the ticket is valid or not */
void validate()
{
	char *valid;
	char msg[BUFSIZ];

	sprintf(msg, "VALD %s", ticket_buf);
	valid = do_transaction(msg);

	if (strncmp(valid, "GOOD", 4) == 0) {
		narrate("Validated ticket", valid);
		return;
	}
	else if (strncmp(valid, "FAIL", 4) == 0)			// do nothing
		;
}


/* narrate: print messages to stderr for debugging and demo purpose
 * IN msg1, msg2: strings to print along with pid and title
 * RET nothing, dies on error
 */
void narrate(char *msg1, char *msg2)
{
	fprintf(stderr, "CLIENT[%d]: %s %s\n", pid, msg1, msg2);
}

void syserr(char *msg)
{
	char buf[MSGLEN];
	sprintf(buf, "CLIENT[%d]: %s", pid, msg);
	perror(buf);
}