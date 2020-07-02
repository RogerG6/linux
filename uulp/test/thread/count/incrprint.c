/*************************************************************
 Date       : 2020年 07月 02日 星期四 20:16:52 CST
 Author     : rogerg6
 File       : incrprint.c
 Description: 一个线程自增，另一个线程打印

*************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM 5

int count = 0;			// global variable

int main(void)
{
	pthread_t t1;		
	int i;

	void *print_count(void *);

	// 创建线程
	pthread_create(&t1, NULL, print_count, (void *)"hello");
	for (i = 0; i < NUM; i++) {
		count++;
		sleep(1);
	}

	// 等待线程返回
	pthread_join(t1, NULL);

	return 0;
}

void *print_count(void *m)
{
	int i;

	for (i = 0; i < NUM; i++) {
		printf("count = %d\n", count);
		sleep(1);
	}

	return NULL;
}