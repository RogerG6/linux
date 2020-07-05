/*************************************************************
 Date       : 2020年 07月 02日 星期四 20:25:54 CST
 Author     : rogerg6
 File       : twocount4.c
 Description: 条件变量编写程序
 			  流程：主线程：p0
 			       线程　：p１，p２
 			  	1. 创建p１，p２，并运行，在获取lock时阻塞，因为p0持有lock
 			  	2. p0调用pthread_cond_wait，释放lock，并休眠直到cond满足	<－－－－－－－－－－－－｜
 			  	3. p1或p2竞争，假设是p1获得lock												｜
 			  	   统计完字数																｜
 			  	   调用pthread_cond_signal，p0中的pthread_cond_wait被唤醒，再次获取lock		｜	
 			  	   释放lock																｜
 			  	4. p0和p2竞争lock															｜
 			  	   		if p0 得到lock，则打印字数......－－－－－－－－－－－－－－－－－－－－－－｜
							直到p0发送signal并释放lock后，再统计数字.....p2做与p1一样的功能		｜
																						｜
 			  	   		else p2 得到lock，发现mailbox!=NULL, －－－－－－－－－－－－－－－－－－－
 			  	   		    直到p0发送signal并释放lock后，再统计数字.....做与p1一样的功能
				
			  关于pthread_cond_wait：
			  		1. release lock
			  		2. suspend to wait for signal to wake
			  		3. wake up
			  		4. get lock 


 usage　　　 : twocnt file1 file2

*************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

struct arg_set {
	char *fname;
	int count;
};

struct arg_set *mailbox;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;		// 互斥锁
pthread_cond_t flag = PTHREAD_COND_INITIALIZER;			// 条件变量


int main(int ac, char *av[])
{
	pthread_t t1, t2;
	struct arg_set args1, args2;
	int reports_in = 0;				// 文件计数
	int total_words = 0;
	void *count_words(void *);

	if (ac != 3) {
		fprintf(stderr, "Usage : %s file1 file2\n", av[0]);
		exit(1);
	}

	pthread_mutex_lock(&lock);			//　锁上report box

	args1.fname = av[1];
	args1.count = 0;
	args2.fname = av[2];
	args2.count = 0;


	// 创建线程
	pthread_create(&t1, NULL, count_words, (void *)&args1);
	pthread_create(&t2, NULL, count_words, (void *)&args2);

//sleep(5);
	while(reports_in < 2) {
		printf("MAIN: waiting for flag to go up\n");
		pthread_cond_wait(&flag, &lock);		// wait for notify
		printf("MAIN: Wow! flag was raised, I have the lock\n");
		printf("%7d: %s\n", mailbox->count, mailbox->fname);
		total_words += mailbox->count;

		// 等待线程返回
		if (mailbox == &args1)
			pthread_join(t1, NULL);
		if (mailbox == &args2)
			pthread_join(t2, NULL);

		mailbox = NULL;
		pthread_cond_signal(&flag);
		//sleep(5);
		reports_in++;
	}

	printf("%5d: total words\n", args1.count + args2.count);

	return 0;
}

void *count_words(void *a)
{
	struct arg_set *args = a;
	FILE *fp;
	int c, prev = '\0';

	if ((fp = fopen(args->fname, "r")) != NULL) {
		while ((c = getc(fp)) != EOF) {
			if (!isalnum(c) && isalnum(prev)) {
				args->count++;				
			}

			prev = c;
		}
		fclose(fp);
	} else
		perror(args->fname);

	printf("COUNT: waiting to get lock\n");
	pthread_mutex_lock(&lock);					// get mailbox
	printf("COUNT: have lock, storing data\n");
	if (mailbox != NULL)
		pthread_cond_wait(&flag, &lock);		// 防止线程间对锁的竞争，如果没有这句代码，可能统计了2次，但主线程只知道后一次
	mailbox = args;
	printf("COUNT: raising flag\n");
	pthread_cond_signal(&flag);					// raise flag
	//sleep(5);
	printf("COUNT: unlocking box\n");
	pthread_mutex_unlock(&lock);				// release the mailbox 
	return NULL;

}