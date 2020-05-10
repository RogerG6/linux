/*************************************************************
 Date: 2020年 04月 20日 星期一 20:34:08 CST
 Author: rogerg6
 File: ltr_to_mode.c
 Description:
 	1. 由rwx-rx--x转换成st_mode
*************************************************************/

#include <stdio.h>
#include <string.h>

void itob(short n);
void letters2mode(char *ltr, short *mode);

int main(void)
{
	char *letters = "rwxr-x--x";
	short mode = 0;
	char st_mode[16];
	//printf("%d", sizeof(mode));

	letters2mode(letters, &mode);
	itob(mode);
	putchar('\n');


	return 0;
}

void letters2mode(char *ltr, short *mode)
{
	int len = strlen(ltr);
	int i;

	for (i = 0; i < len; i++) {
		if (ltr[i] != '-') {
			*mode |= 1 << (len - i - 1);
		}
	}
}

void itob(short n)
{
	if (n == 0)
		return;
	itob(n >> 1);
	printf("%d", n & 1);
}