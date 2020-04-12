/* Date: 2020/4/9
   Func: whoami -- 模仿whoami程序，显示当前有效用户的用户名
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

	printf("%s\n", utbufp->ut_name);
#if 0
	printf("%-8.8s ", utbufp->ut_line);
	show_time(utbufp->ut_time);
	printf(" ");
#ifdef SHOWHOST
	printf("(%s)\n", utbufp->ut_host);
#endif
#endif
}

void show_time(long timeval)
{
	char *cp;
	cp = ctime(&timeval);

	// 从第四个字符开始显示，显示12个字符，只显示日期 时间
	printf("%12.12s", cp + 4);		
}
