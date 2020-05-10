/*************************************************************
 Date: 2020年 04月 20日 星期一 21:09:18 CST
 Author: rogerg6
 File: rnrp.c
 Description:
 	1. 先用open打开一个文件，用read读一些内容
 	2. 调用sleep（20）等待20秒，20秒间，去掉文件的读权限
 	3. 20秒后，再读一些内容，看看会有什么结果

 	结果：当关闭读权限后，还是能从文件中读出内容
*************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 10

int main(void)
{
	int fd;
	char buf[BUFSIZE];
	int ret;

	if ((fd = open("lcmd", O_RDONLY)) == -1)
		perror("lcmd");

	if (read(fd, buf, BUFSIZE) > 0)
		write(2, buf, BUFSIZE);

	sleep(20);

	while ((ret = read(fd, buf, BUFSIZE)) > 0)
		write(2, buf, ret);


	return 0;
}


