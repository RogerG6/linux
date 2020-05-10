/*************************************************************
 Date: 2020年 05月 08日 星期五 20:29:41 CST
 Author: rogerg6
 File: write02.c
 Description:
 	模仿write 小程序，用于user之间终端的通讯

 	有这一行： Message from roger@roger-E7250 on pts/3 at 22:04 ...

 	问题： 无法显示roger@roger-E7250
*************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <utmp.h>
#include <ttyent.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>

#define BUFSIZE 100

void do_write(char *dev);
void hello_mesg(int fd);
void print_ttyinfo(void);

int main(int ac, char *av[])
{
	struct utmp cur_utmp;
	int utmpfd, result;
	int reclen = sizeof(struct utmp);
	char dev[100];

	if (ac < 2) {
		fprintf(stderr, "Usage: write username\n");
		exit(1);
	}

	// open utmp file
	if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
		perror(UTMP_FILE);
		exit(1);		
	}

	// find username
 	while (result = read(utmpfd, &cur_utmp, reclen) == reclen) 
 		if (strcmp(av[1], cur_utmp.ut_name) == 0) {
 			strcpy(dev, "/dev/");
 			strcat(dev, cur_utmp.ut_line);
 			printf("%s\n", cur_utmp.ut_line);
 			
 			do_write(dev);
 		}

 	close(utmpfd);

	return 0;
}

void do_write(char *dev)
{
	int ttyfd;
	char buf[BUFSIZE];

	if ((ttyfd = open(dev, O_WRONLY)) == -1) {
		perror(dev);
		return;		
	}

	hello_mesg(ttyfd);
	//print_ttyinfo();

	// write somethings
	while (fgets(buf, BUFSIZE, stdin) != NULL)
		if (write(ttyfd, buf, strlen(buf)) == -1) 
			break;

	close(ttyfd);	
}

// print hello msg on tty that write to 
// Message from roger@roger-E7250 on pts/3 at 22:04 ...
void hello_mesg(int fd)
{
	char msg[100];
	time_t cur_time;
	struct tm *broken_tm;
	uid_t uid;
	struct passwd *pw;

	// get time
	time(&cur_time);
	broken_tm = localtime(&cur_time);

	uid = getuid();
	pw = getpwuid(uid);
	// get username 
	
	sprintf(msg, "Message from %s on %s at %d:%d...\n", pw->pw_name, ttyname(0), broken_tm->tm_hour, broken_tm->tm_min);
	write(fd, msg, strlen(msg));
}

void print_ttyinfo(void)
{
	struct ttyent *ttyp;

    ttyp = getttyent();
    if (ttyp == NULL)
    	perror("ttyen");

    printf("Name: %s\n", ttyp->ty_name);
    printf("cmd : %s\n", ttyp->ty_getty);
    printf("Type: %s\n", ttyp->ty_type);
    printf("Staus: %d\n", ttyp->ty_status);
    printf("Window: %s\n", ttyp->ty_window);
    printf("Comment: %s\n", ttyp->ty_comment);

}