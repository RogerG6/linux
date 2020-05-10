/*************************************************************
 Date       : 2020年 04月 26日 星期日 10:03:56 CST
 Author     : rogerg6
 File       : mv.c
 Description:
 	实现： mv src dest/  or mv src dest
*************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define PATHLEN 100
#define FNAMELEN 50	
#define DEFMODE 755

void get_fname_path(char *path, char *fname);
void do_rename(char *src, char *dst);
int isDir(char *fname);

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("Usage; mv src dest\n");
		exit(1);
	}

	do_rename(argv[1], argv[2]);

	return 0;
}


void do_rename(char *src, char *dst)
{
	char path[PATHLEN];
	char srcfn[FNAMELEN];		// src filename without path, only filename

	if (isDir(dst) == 1) {		// dst exist && is a directory
		strcpy(path, dst);
		if (path[strlen(path) - 1] != '/')
			strcat(path, "/");
		
		get_fname_path(src, srcfn);
		strcat(path, srcfn);
		if (creat(path, DEFMODE) == -1) {
			printf("can't creat %s\n", path);
			exit(1);
		}

		rename(src, path);
	} else 						// dst is not a directory
		rename(src, dst);

	
}

// get filename from path
void get_fname_path(char *path, char *fname)
{
	int i;
	for (i = strlen(path); i >= 0 && path[i] != '/'; i--)
		;
	strcpy(fname, &path[i + 1])	;
}

int isDir(char *fname)
{
	struct stat info;

	if (stat(fname, &info) == -1) 		// fname is not exist
		return -1;

	if (S_ISDIR(info.st_mode))
		return 1;
	else
		return 0;
}
