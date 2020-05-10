/*************************************************************
 Date       : 2020年 05月 03日 星期日 17:30:05 CST
 Author     : rogerg6
 File       : echostat.c
 Description:
	 文件描述符0的驱动器回显位的状态

	 如果 fd＝0，则./echostat >> echostat.log后，echostat.log文件中
	 有echo is on

	 如果 fd＝1，则./echostat >> echostat.log后，会出现以下信息：
		tcgetattr: Inappropriate ioctl for device
		
	 ./echostat > /dev/pts/2后，信息显示在/dev/pts2上

	 即： 当fd＝1时，不能将内容定位到普通文件中，可以定位到终端设备上
	 因此，用fd＝0更好

*************************************************************/

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	struct termios info;
	int rv;

	//rv = tcgetattr(0, &info);
	rv = tcgetattr(1, &info);
	if (rv == -1){
		perror("tcgetattr");
		exit(1);
	}

	if (info.c_lflag & ECHO)
		printf("echo is on\n");
	else
		printf("echo is off\n");

	return 0;
}
