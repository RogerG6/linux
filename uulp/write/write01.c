/*************************************************************
 Date: 2020年 05月 08日 星期五 20:29:41 CST
 Author: rogerg6
 File: write01.c
 Description:
 	模仿write 小程序，用于user之间终端的通讯

 	没有键盘连接到其他用户屏幕的特征
*************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 100

int main(int ac, char *av[])
{
	int fd;
	char buf[BUFSIZE];

	if (ac < 2) {
		fprintf(stderr, "Usage: write ttyname\n");
		exit(1);
	}

	if ((fd = open(av[1], O_WRONLY)) == -1) {
		perror(av[1]);
		exit(1);
	}

	while (fgets(buf, BUFSIZE, stdin) != NULL)
		if (write(fd, buf, strlen(buf)) == -1)
			break;

	close(fd);

	return 0;
}