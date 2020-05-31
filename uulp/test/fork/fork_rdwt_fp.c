/*************************************************************
 Date       : 2020年 05月 26日 星期二 19:02:51 CST
 Author     : rogerg6
 File       : fork_rdwt_fp.c
 Description:
 	running result: 
 		roger@roger-E7250:~/work/uulp/test/fork$ cat testfile2
		Test 1 2 3..
		Hello, hello
		Test 1 2 3..
		child

	summary : 文件指针FILE *fp的操作：fread，fwrite，fprintf，fclose
			  这些库函数是自带缓冲区的
			  当fork出子进程时，父进程的缓冲区也被复制到子进程
			  当子进程退出时，冲洗缓冲区，将其中的内容写入testfile2中
			  所以会出现2次 Test 1 2 3..

			  而在 fork_rdwt.c中，系统调用write是无缓冲的，直接写入文件


			  =============================================================>>
			 标准IO库提供缓冲：
			 1. 重定向到文件                 ： 全缓冲
			 2. 重定向到stdin/stdout/终端设备 ： 行缓冲
			 3. 标准错误                     ：无缓冲


			 系统调用：read，write是无缓冲的


*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	FILE *fp;
	int pid;
	char msg1[] = "Test 1 2 3..\n";
	char msg2[] = "Hello, hello\n";

	if ((fp = fopen("testfile2", "w")) == NULL)
		return 0;
	fprintf(fp, "%s", msg1);

	if ((pid = fork()) == -1)
		return 0;
	else if (pid == 0)
		fprintf(fp, "child\n");
	else
		fprintf(fp, "%s", msg2);

	fclose(fp);
	return 1;
}