/*************************************************************
 Date       : 2020年 05月 03日 星期日 22:19:40 CST
 Author     : rogerg6
 File       : append.c
 Description:
 	测试fopen(filename, "a")中的 a 模式
 	是通过添加模式开启O_APPEND, 还是打开文件后定位在文件末尾

	现象：
		运行前test内容：
		a test file

		运行后test内容：
		a test file
		Info 1 content
		Info 3 content
		Info 2 contents
		Info 4 contents

 	结果： 通过添加模式开启O_APPEND, 因为不存在fp2覆盖fp1的情况
 	      且 先append fp1的内容，再append fp2的内容
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *fp1, *fp2;
	char info1[] = "Info 1 content\n";
	char info2[] = "Info 2 contents\n";
	char info3[] = "Info 3 content\n";
	char info4[] = "Info 4 contents\n";

	if (argc < 2) {
		fprintf(stderr, "Usage: append filename\n");
		exit(1);
	}

	// 打开2个文件指针
	if ((fp1 = fopen(argv[1], "a")) == NULL) {
		perror(argv[1]);
		exit(1);
	}
	if ((fp2 = fopen(argv[1], "a")) == NULL) {
		perror(argv[1]);
		exit(1);
	}

	// 分别写入字符串
	fwrite(info1, sizeof(char), strlen(info1), fp1);
	fwrite(info2, sizeof(char), strlen(info2), fp2);

	fwrite(info3, sizeof(char), strlen(info3), fp1);
	fwrite(info4, sizeof(char), strlen(info4), fp2);

	fclose(fp1);
	fclose(fp2);
}