/* Date: 2020/4/6
   Func: who 版本三，修改了以下内容：
   		1. 运用了utmplib.c中的缓存技术：
   			每次从utmp中获得16个struct utmp, 放入缓冲区，utemp_next从缓冲区中获得一个struct utmp
   			减少系统调用次数为原来的1/16
 */

#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include "utmplib.h"

#define SHOWHOST


void show_info(struct utmp *utbufp);
void show_time(long timeval);

int main(void)
{
	struct utmp *utbufp; 	// 保存指向下一个struct utmp的指针

	// 打开utmp文件
	if (utmp_open(UTMP_FILE) == -1){
		perror(UTMP_FILE);
		exit(1);
	}

	//读utmp文件
	while ((utbufp = utmp_next()) != (struct utmp *)NULL) {
		//printf("reading\n");
		show_info(utbufp);
	}

	utmp_close();

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