/*************************************************************
 Date: 2020年 04月 13日 星期一 19:38:42 CST
 Author: rogerg6
 File: head.c
 Description: 模仿head小程序
*************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 4096
char buf[BUFSIZE];
int lines = 10;		// 默认行数为10

void show_lines(int fd);
int do_head(char *filename);

int main(int argc, char *argv[])
{
	int ac = argc;
	char **av = argv;

	// 处理-n参数
	while (--argc > 0 && *++argv)
		if (**argv == '-') 
			sscanf(*argv + 1, "%d", &lines);

	// 打印行
	while (--ac > 0 && *++av)
		if (**av != '-')
			do_head(*av);		

	return 0;
}

int do_head(char *filename)
{
	int fd;

	if ((fd = open(filename, O_RDONLY)) == -1) {
		printf("Error: can't open file %s\n", filename);
		return 1;
	} 

	show_lines(fd);
}

void show_lines(int fd)
{
	int line = 0;
	int i, len;

	while (line < lines) {
		len = read(fd, buf, BUFSIZE);
		for (i = 0; i < len; i++) {

			if (buf[i] == '\n') 
				line++;
			putchar(buf[i]);
			if (line == lines)
				return;
		}
	}
}