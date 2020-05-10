/*************************************************************
 Date       : 2020年 04月 26日 星期日 15:06:59 CST
 Author     : rogerg6
 File       : find.c
 Description:
 	实现： find filename dirname/

 	实现方式：通过chdir来切换当前目录，如果找到目标文件，用pwd打印当前路径即为目标路径
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>


#define PATHLEN 100


void oops(char *fname, char *info);
int get_index(void);
int isDir(char *fname);
void do_find(char *fname, char *dirname);

int main(int argc, char *argv[])
{
	if (argc < 3)
		oops("", "Usage: find filename dirname");

	if (!isDir(argv[2]))
		oops(argv[2], "is not a directory");

	// current dir
	strcat(path, argv[2]);	
	if (path[strlen(path) - 1] != '/')
		strcat(path, "/");

	do_find(argv[1], path);

	return 0;
}


int isDir(char *fname)
{
	struct stat info;

	if (stat(fname, &info) == -1)
		perror(fname);

	if (S_ISDIR(info.st_mode))
		return 1;
	else
		return 0;
}

void do_find(char *fname, char *dirname)
{
	DIR *dir;
	struct dirent *dirp;


	//printf("-path: %s\n", path);

	if ((dir = opendir(path)) == NULL)
		oops(dirname, "can't be opened");

	while ((dirp = readdir(dir)) != NULL) {
		if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
			strcat(path, dirp->d_name);					// 加上文件名，eg. path: /usr/file

			if (isDir(path)) {
				strcat(path, "/");						
				//printf("--path: %s\n", path);
				do_find(fname, path);
				path[get_index()] = '\0';				// 去掉文件名，eg. path: /usr/
			}
			else
				if (strcmp(fname, dirp->d_name) == 0) {
					printf("target: %s\n", path);
					exit(0);
				}else
					path[get_index()] = '\0';

		}
	}
}

int get_index(void)
{
	int i;
	for (i = strlen(path) - 2; i >= 0 && path[i] != '/'; i--)
		;
	return i + 1;
}

void oops(char *fname, char *info)
{
	printf("Error: %s ", fname);
	printf("%s\n", info);
	exit(1);
}