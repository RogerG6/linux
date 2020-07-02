/*************************************************************
 Date       : 2020年 07月 02日 星期四 20:25:54 CST
 Author     : rogerg6
 File       : twocount3.c
 Description: 2个线程分别对２个文件进行单词数统计，最终将结果相加作为结果,
 			  但每个线程使用各自的计数器，不干涉，保证数据安全

 usage　　　 : twocnt file1 file2

*************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>

struct arg_set {
	char *fname;
	int count;
};


int main(int ac, char *av[])
{
	pthread_t t1, t2;
	struct arg_set args1, args2;

	void *count_words(void *);

	if (ac != 3) {
		fprintf(stderr, "Usage : %s file1 file2\n", av[0]);
		exit(1);
	}

	args1.fname = av[1];
	args1.count = 0;
	args2.fname = av[2];
	args2.count = 0;


	// 创建线程
	pthread_create(&t1, NULL, count_words, (void *)&args1);
	pthread_create(&t2, NULL, count_words, (void *)&args2);

	// 等待线程返回
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("%5d: %s\n", args1.count, av[1]);
	printf("%5d: %s\n", args2.count, av[2]);
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

	return NULL;
}