/*************************************************************
 Date: 2020年 04月 13日 星期一 20:20:37 CST
 Author: rogerg6
 File: tail.c
 Description: 
 	模仿tail小程序
 	具体思路：1. 文件指针从末尾向前移BUFSIZE
 			2. 读BUFSIZE，确定buf中的行数
 			3. 够了则输出，不够继续读，向前移2＊BUFSIZE
 			4. 如果移到文件开头的前面，则读取全文件
*************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFSIZE 10
char buf[BUFSIZE];
int lines = 10;		// 默认行数为10

void show_lines(int fd);
int do_tail(char *filename);
int get_pos(int fd);

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
			do_tail(*av);		

	return 0;
}

int do_tail(char *filename)
{
	int fd, pos;

	if ((fd = open(filename, O_RDONLY)) == -1) {
		printf("Error: can't open file %s\n", filename);
		return 1;
	}

	// 定位
	pos = get_pos(fd);
	if (pos > 0)
		lseek(fd, -pos, SEEK_END);
	else
		lseek(fd, 0, SEEK_SET);

	// 打印
	show_lines(fd);
}

int get_pos(int fd)
{
	int bp, pos = 0, line = 0;
	int ret;

	lseek(fd, -BUFSIZE, SEEK_END);
	while (1) {		

		read(fd, buf, BUFSIZE);

		// 数行数
		bp = BUFSIZE - 1;
		while (bp >= 0){
			if (buf[bp--] == '\n') {
				line++;
			}
			if (line == lines + 1)		// 正好移到目标行数
				return pos;
			pos++;
		}
		ret = lseek(fd, -2 * BUFSIZE, SEEK_CUR);		// 当前位置向前2 * BUFSIZE

		// 超过文件开头
		if (ret < 0)
			return 0;
	}
}

void show_lines(int fd)
{
	int line = 0;
	int i, len;

	while ((len = read(fd, buf, BUFSIZE)) > 0) 
		write(1, buf, len);
}