/*************************************************************
 Date       : 2020年 07月 02日 星期四 19:53:14 CST
 Author     : rogerg6
 File       : hello_multi.c
 Description: 2 个线程同时打印
 			  流程：先是１个进程，然后分为２个线程并行执行，最后２个线程合并为一个
 			  　　　从main中返回
*************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM 5

int main(void)
{
	pthread_t t1, t2;			// 2 threads

	void *print_msg(void *);

	// 创建线程
	pthread_create(&t1, NULL, print_msg, (void *)"hello");
	pthread_create(&t2, NULL, print_msg, (void *)"world\n");

	// 等待线程返回
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return 0;
}

void *print_msg(void *m)
{
	char *cp = (char *)m;
	int i;


	for (i = 0; i < NUM; i++) {
		printf("%s", cp);
		fflush(stdout);
		sleep(1);
	}

	return NULL;
}