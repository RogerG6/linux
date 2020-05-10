/*************************************************************
 Date       : 2020年 04月 23日 星期四 11:31:41 CST
 Author     : rogerg6
 File       : pwd.c
 Description: 模仿pwd，列出当前目录路径
 	1. 获得当前dir的inode
 	2. 切换到父目录，获得其finode
 	3. 如果inode == finode，则到达根目录，返回
 	   否则，获得inode对应的路径名
 	4. 重复1～3
*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#define DIRLEN 100

int do_pwd(ino_t cur_inode);
void ino_to_name(ino_t inode, char *name);
ino_t get_inode(char *fname);

int main(void)
{
	do_pwd(get_inode("."));
	putchar('\n');

	return 0;
}

int do_pwd(ino_t cur_inode)
{
	char cur_name[DIRLEN];
	ino_t my_inode;

	if (get_inode("..") != cur_inode) {
		chdir("..");
		ino_to_name(cur_inode, cur_name);
		my_inode = get_inode(".");
		do_pwd(my_inode);
		printf("/%s", cur_name);
	}
}

void ino_to_name(ino_t inode, char *name)
{
	DIR *dirp;
	struct dirent *direntp;

	if ((dirp = opendir(".")) == NULL) {
		perror(".");
		exit(1);
	}

	while ((direntp = readdir(dirp)) != NULL)
		if (direntp->d_ino == inode) {
			strcpy(name, direntp->d_name);
			closedir(dirp);
			return;
		}

	fprintf(stderr, "error looking for inum %ld\n", inode);
	exit(1);
}

ino_t get_inode(char *fname)
{
	struct stat statbuf;

	if (stat(fname, &statbuf) == -1) {
		fprintf(stderr, "can't stat\n");
		perror(fname);
		exit(1);
	}
	return statbuf.st_ino;

}