/* Date: 2020/4/6
   Func: who 版本一*/

#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define SHOWHOST

void show_info(struct utmp *utbufp);

int main(void)
{
	struct utmp current_record;
	int utmpfd;
	int result;
	int reclen = sizeof(current_record);

	// 打开utmp文件
	if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
		perror(UTMP_FILE);
		exit(1);
	}

	//读utmp文件
	while (result = read(utmpfd, &current_record, reclen) == reclen) {
		//printf("reading\n");
		show_info(&current_record);
	}

	close(utmpfd);

	return 0;
}

void show_info(struct utmp *utbufp)
{
	printf("%-8.8s ", utbufp->ut_name);
	printf("%-8.8s ", utbufp->ut_line);
	printf("%10ld ", utbufp->ut_time);
#ifdef SHOWHOST
	printf("(%s)\n", utbufp->ut_host);
#endif
}