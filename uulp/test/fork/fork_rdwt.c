/*************************************************************
 Date       : 2020年 05月 26日 星期二 19:02:51 CST
 Author     : rogerg6
 File       : for
 	running result： 
 		roger@roger-E7250:~/work/uulp/test/fork$ cat testfile 
		Test 1 2 3..
		Hello, hello
		Hello, hello

*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd;
	int pid;
	char msg1[] = "Test 1 2 3..\n";
	char msg2[] = "Hello, hello\n";

	if ((fd = creat("testfile", 0644)) == -1)
		return 0;
	if (write(fd, msg1, strlen(msg1)) == -1)
		return 0;

	if ((pid = fork()) == -1)
		return 0;
	if (write(fd, msg2, strlen(msg2)) == -1)
		return 0;

	close(fd);
	return 1;
}