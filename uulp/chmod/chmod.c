/*************************************************************
 Date       : 2020年 04月 21日 星期二 22:33:24 CST
 Author     : rogerg6
 File       : chmod.c
 Description:
 	Usage: chmod 755 file
*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	mode_t mode;
	int i;

	if (argc < 3) {
		printf("Usage: chmod 755 file\n");
		return -1;
	}

	// get mode from cmd
	sscanf(*++argv, "%o", &mode);		// mode 是八进制数

	for (i = argc - 2; i > 0; i--)
		if (chmod(*++argv, mode) == -1)
			printf("can't change mode for %s\n", *argv);;

	return 0;
}