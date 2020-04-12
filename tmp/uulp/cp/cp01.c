/* Date: 2020/4/9
   Func: 模仿cp的一个程序：
   		 1. 同一个文件不能复制
   		 2. 加入的-i 选项，复制前确认是否要覆盖

   		 问题：
   			1. 只有从文件1复制到文件2, 而不能进行目录的复制
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 4096
#define COPYMODE 0644

void oops(char *s1, char *s2);

int main(int argc, char *argv[])
{
	int fromfd, tofd;
	char buf[BUFSIZE];
	char *filename[3];
	int i = 0, confirm = 0;		//confirm: 确认是否要覆盖，默认覆盖
	int n_char, c;

	if (argc < 3) {
		fprintf(stderr, "Usage: cp from to\n");
		exit(1);
	}

	filename[2] = argv[0];

	// 处理option
	while (--argc > 0 && **++argv)
		if (**argv == '-')
			while ((c = *((*argv)++)) != '\0') {
				if (c == 'i')
					confirm = 1;
			}
		else 
			filename[i++] = *argv;
	
	if (confirm == 1) {
		printf("%s: 是否要覆盖 \'%s\'?", filename[2], filename[1]);
		if (getchar() == 'n')
			return -1;
	}

	// 如果源与目的是同一文件，则返回
	if (strcmp(filename[0], filename[1]) == 0) {
		printf("%s: \'%s\' 与 \'%s\' 为同一文件\n", argv[0], filename[0], filename[1]);
		return -1;
	}

	//打开源，目的文件
	if ((fromfd = open(filename[0], O_RDONLY)) == -1)
		oops("Can't open", filename[0]);
	if ((tofd = creat(filename[1], COPYMODE)) == -1)
		oops("Can't creat", filename[1]);

	// 从源文件读BUFSIZE字节到目的文件
	while ((n_char = read(fromfd, buf, BUFSIZE)) > 0)
		write(tofd, buf, n_char);

	// 读源文件出错
	if (n_char == -1)
		oops("Reading error from", argv[1]);

	// 关闭源，目的文件
	if (close(fromfd) == -1 || close(tofd) == -1)
		oops("Error closing files", "");

	return 0;
}

void oops(char *s1, char *s2)
{
	fprintf(stderr, "Error: %s ", s1);
	perror(s2);
	exit(1);
}