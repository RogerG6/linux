/* Date : 2020/4/9
   Func : 模仿cat程序，将文件中的内容打印到stdout
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 10
char buf[BUFSIZE];

void oops(char *s1, char *s2);


int main(int argc, char *argv[])
{
	int fd;
	int ret;

	if (argc < 2) {
		printf("Usage: cat file1 file2 ...\n");
		exit(1);
	}

	while (--argc > 0 && **++argv) {
		if ((fd = open(*argv, O_RDONLY)) == -1) {
			oops("can't open", *argv);
			continue;
		}

		// 读文件
		while ((ret = read(fd, buf, BUFSIZE)) > 0)
			write(1, buf, ret);
		if (ret == -1)
			oops("reading error in", *argv);
	}


	return 0;
}

void oops(char *s1, char *s2)
{
	fprintf(stderr, "Error: %s ", s1);
	perror(s2);
	exit(1);
}
