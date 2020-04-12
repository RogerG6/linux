#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 4096
char buf[BUFSIZE];

int main()
{
	int fd;
	int offset, choice;

	if ((fd = open("testfile", O_RDWR)) == -1) {
		printf("Error: can't open %s\n", "testfile");
		exit(1);
	}

	printf("请输入是读还是写1 for read, 2 for write: ");
	scanf("%d", &choice);

	if (choice == 1) {
		printf("请输入offset: ");
		scanf("%d", &offset);

		lseek(fd, offset, 2);

		// 读文件末尾后offset处的内容
		while (read(fd, buf, BUFSIZE) > 0)
			printf("reading: %s\n", buf);

	} else {
		printf("请输入offset: ");
		scanf("%d", &offset);

		lseek(fd, offset, 2);

		// 写文件末尾后offset的地方
		fgets(buf, BUFSIZE, stdin);
		write(fd, buf, BUFSIZE);		
	}
	return 0;
}


