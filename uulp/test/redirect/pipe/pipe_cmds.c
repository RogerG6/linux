/*************************************************************
 Date       : 2020年 06月 04日 星期四 23:24:37 CST
 Author     : rogerg6
 File       : pipe_cmds2.c
 Description:
 	 	do : pipe cmd1 cmd2 cmd3 ...
		
		实现方式  :子进程执行cmd, 父进程递归  
		details : 1. 子进程执行完cmd1后，将输出写入pipe，结束子进程
				  2. 父进程得到子进程结束的消息后，输入重定向到pipe[0]
				  3. 递归到下一层后，fork出的子进程和父进程一样依然可以从管道(所有建立的pipe在程序运行中一直存在)中读取上个cmd1的输出，
				     继续执行cmd2
				  4. 如此循环，直到最后一个cmd，输出到终端

				----		
							  stdin									   stdout 
							     |										/|\
								\|/										 |
						-----> child1 -> pipe1			---> child2 -> pipe2
						 |				  |				|		         |
      开始------------> parent -------------------> parent ------------------> parent-----> 结束
		
*************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define oops(x, m)	{perror(x); exit(m);}

void do_pipe(char *av[], int i, int cmdnum);

int main(int ac, char *av[])
{
	int i;

	if (ac < 3) {
		fprintf(stderr, "Usage: pipecmds cmd1 cmd2 ...\n");
		return -1;
	}

	do_pipe(av, 1, ac - 1);

	return 0;
}

/* purpose : 执行终端命令
 * details : 1. make a pipe 															<-----------
 *																								   |
 *			 2. fork --------------> child: close(p[0]), dup2(p[1], 1), close(p1), execlp 		   |
 *					|																			   |
 *					 --------------> parent: close(p[1]), dup2(p[0], 0), close(p0), 进一步递归 ------
 */
void do_pipe(char *av[], int i, int cmdnum)
{
	int pp[2];
	int pid;

	if (i > cmdnum)
		return;

	if (pipe(pp) == -1)
		oops("can't make pipe", 1);

	if ((pid = fork()) == -1) {
		oops("can't fork", 2);
	} else if (pid == 0) {
		/* child do cmds 
		   close read end, dup write end, close write end, execlp
		 */
		close(pp[0]);
		if (i < cmdnum)
			if (dup2(pp[1], 1) != 1) {
				oops("Child: can't dup2 pp[1]", 3);
			}
		close(pp[1]);
		execlp(av[i], av[i], NULL);
		oops(av[i], 4);
	} else if (pid > 0) {
		/* parent recursive 
		   close write end, dup read end, close read end, recursive !
		*/
		close(pp[1]);
		if (i < cmdnum)
			if (dup2(pp[0], 0) != 0) {
				oops("Parent: can't dup2 pp[0]", 3);
			}
		close(pp[0]);
		wait(NULL);

		do_pipe(av, i + 1, cmdnum);

	}
}