/*************************************************************
 Date       : 2020年 07月 02日 星期四 20:25:54 CST
 Author     : rogerg6
 File       : twocount2.c
 Description: 2个线程分别对２个文件进行单词数统计，最终将结果相加作为结果,
 			  但使用互斥量，从而对total_words这个全局变量的访问是安全的。

 			  缺点：统计每个单词的时候都要上锁和解锁，大大降低了效率
 usage　　　 : twocnt file1 file2

*************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>

int total_words;
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

int main(int ac, char *av[])
{
	pthread_t t1, t2;

	void *count_words(void *);

	if (ac != 3) {
		fprintf(stderr, "Usage : %s file1 file2\n", av[0]);
		exit(1);
	}

	total_words = 0;

	// 创建线程
	pthread_create(&t1, NULL, count_words, (void *)av[1]);
	pthread_create(&t2, NULL, count_words, (void *)av[2]);

	// 等待线程返回
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("%5d: total words\n", total_words);

	return 0;
}

void *count_words(void *f)
{
	char *filename = (char *)f;
	FILE *fp;
	int c, prev = '\0';

	if ((fp = fopen(filename, "r")) != NULL) {
		while ((c = getc(fp)) != EOF) {
			if (!isalnum(c) && isalnum(prev)) {
				pthread_mutex_lock(&counter_lock);		// 锁上
				total_words++;				
				pthread_mutex_unlock(&counter_lock);	// 解锁
			}

			prev = c;
		}
		fclose(fp);
	} else
		perror(filename);

	return NULL;
}