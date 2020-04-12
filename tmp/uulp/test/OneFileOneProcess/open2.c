#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 4096

void read_finfo(int fd);
int write_finfo(int fd);

int main(int argc, char *argv[])
{
	int fd2, fd3;

	if (argc != 2) {
		printf("Usage: open_same_file file\n");
		exit(1);
	}

	if ((fd2 = open(argv[1], O_WRONLY)) == -1) {
		printf("Error: Can't open file %s\n", argv[1]);
	}
	write_finfo(fd2);

	if ((fd3 = open(argv[1], O_RDONLY)) == -1) {
		printf("Error: Can't open file %s\n", argv[1]);
	}
	read_finfo(fd3);

	printf("fd2 = %d, fd3 = %d\n", fd2, fd3);

	close(fd2);
	close(fd3);

	return 0;
}


char buf[BUFSIZE];

void read_finfo(int fd)
{
	printf("open2 reading: \n");
	while (read(fd, buf, BUFSIZE) > 0)
		printf("open2: %s\n", buf);
}

int write_finfo(int fd)
{
	printf("open2 writing: \n");
	fgets(buf, BUFSIZE, stdin);
	write(fd, buf, BUFSIZE);
}