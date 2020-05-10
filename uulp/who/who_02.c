/* Date: 2020/4/6
   Func: who 版本二，修改了以下内容：
   		1. 过滤了一些非用户的登录信息
   		2. 正确显示了时间
 */

#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SHOWHOST

void show_info(struct utmp *utbufp);
void show_time(long timeval);

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
	// 过滤非用户进程的记录
	if (utbufp->ut_type != USER_PROCESS)
		return;

	printf("%-8.8s ", utbufp->ut_name);
	printf("%-8.8s ", utbufp->ut_line);
	show_time(utbufp->ut_time);
	printf(" ");
#ifdef SHOWHOST
	printf("(%s)\n", utbufp->ut_host);
#endif
}

void show_time(long timeval)
{
	char *cp;
	cp = ctime(&timeval);

	// 从第四个字符开始显示，显示12个字符，只显示日期 时间
	printf("%12.12s", cp + 4);		
}