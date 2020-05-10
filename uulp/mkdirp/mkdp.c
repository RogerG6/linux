/*************************************************************
 Date       : 2020年 04月 25日 星期六 21:49:42 CST
 Author     : rogerg6
 File       : mkdp.c
 Description:
 	实现mkdir -p
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define FNLEN 50

int parent = 0;		// option -p
int lastname = 0;	// flag
mode_t mode = 0755;

int isExist(char *path);
void do_mkdir(char *path);

int main(int argc, char *argv[])
{
	int c;
	int ac = argc;
	char **av = argv;

	if (argc < 2) {
		printf("Usage: mkdir dirname\n");
		exit(1);
	}

	while (--argc > 0 && *++argv)
		if (**argv == '-')
			while ((c = *++(*argv)) != '\0')
				switch (c) {
					case 'p': parent = 1; break;
				}

	while (--ac > 0 && **++av != '\0') {
		if (**av != '-') {
			//printf("main: %s\n", *av);
			do_mkdir(*av);			
		}
	}

	return 0;
}

void do_mkdir(char *path)
{
	char file[FNLEN];
	int i = 0;
	int len = strlen(path);

#if 0
	// mkdir int current dir
	if (path[0] != '/' && path[0] != '~' && path[0] != '.')
		strcpy(file, "./");
#endif
	do {
		for (; i <= len ; i++)
			if (path[i] == '/' ||  path[i] == '\0') {
				strncpy(file, path, i++);
				break;
			}

		if (i >= len)
			lastname = 1;

		if (!isExist(file)) {
			if (parent == 0) {			//mkdir dirname
				if (lastname) {			
					mkdir(file, mode);
					return;
				}
				printf("Error: dir %s is not exist\n", file);
				exit(1);
			} else {					// mkdir dirname -p
				mkdir(file, mode);
			}
		}else
			printf("exist\n");
	} while (i < len);

}

int isExist(char *path)
{
	struct stat statbuf;
	nlink_t   nlink;

	//printf("path: %s\n", path);
	if (stat(path, &statbuf) == -1)
		return 0;
	else
		return 1;
}

