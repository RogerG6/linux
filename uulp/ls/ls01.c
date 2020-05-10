/*************************************************************
 Date: 2020年 04月 15日 星期三 18:13:01 CST
 Author: rogerg6
 File: ls.c
 Description: 
	1. 完成option: 
		l: long info list
		a: all file list (无法整齐显示文件)
	
*************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>

#define MAXFILES 4096

// option
int longinfo = 0;
int allinfo = 0;

// 最长文件名的文件名长度
int longnamefile = 0;

void do_ls(char *filename);
void do_stat(char *path, char *filename);
void show_info(char *filename, struct stat *statbufp);
void mode_to_letter(int mode, char *modestr);
char *uid_to_name(long uid);
char *gid_to_name(long gid);
int my_strcmp(const void *p1, const void *p2);
void show_fmt_info(char *filename, struct stat *statbufp);
void show_all_info(char *filename, struct stat *statbufp);

int main(int argc, char *argv[])
{
	int ac = argc;
	char **av = argv;
	int c, obj = 0;		// obj是查看的目录个数
	int len;

	// 处理option
	while (--argc > 0 && *++argv) {
		if (**argv == '-') {
			while (c = *(++(*argv)))
				switch (c) {
					case 'l': longinfo = 1; break;
					case 'a': allinfo = 1; break;
				}
		}
		else {
			if ((len = strlen(*argv)) > longnamefile)
				longnamefile = len;
			obj++;
		}
		//printf("*%s*\n", *argv);
	}

	//printf("l: %d, a: %d, obj: %d\n", longinfo, allinfo, obj);

	// 查看当前目录
	if (obj == 0)
		do_ls(".");
	else {
		// 查看非当前目录
		while (--ac > 0 && *++av) {
			if (**av != '-' && **av != 0) {
				//printf("*%s*\n", *av);
				do_ls(*av);	
			}
			//printf("*%d*\n", **av);
		}

	}

	return 0;
}

void do_ls(char *filename)
{
	DIR *dir;
	struct dirent *dirp;
	char path[100];
	char *file[MAXFILES];
	int i;
	int cnt = 0;

	// 打开目录
	if ((dir = opendir(filename)) == NULL)
		fprintf(stderr, "can't open direct %s\n", filename);
	else {
		// 读取目录项
		i = 0;
		while ((dirp = readdir(dir)) != NULL) {
			file[i++] = dirp->d_name;
			cnt++;
		}

		// 按字母表排序
		qsort(file, cnt, sizeof(char *), my_strcmp);

		// 显示文件内容
		printf("总文件数 %d\n", cnt);
		for (i = 0; i < cnt; i++) {

			// 目录项的具体路径
			strcpy(path, filename);
			if (path[strlen(path)- 1] != '/')
				strcat(path, "/");

			strcat(path, file[i]);
			//printf("%s\n", path);

			// 读取目录项的信息
			do_stat(path, file[i]);
		}
		putchar('\n');

		// 关闭目录
		closedir(dir);

	}
}

void do_stat(char *path, char *filename)
{
	struct stat statbuf;

	//printf("path: %s ||| ", path);
	if (stat(path, &statbuf) == -1)
		perror(path);
	else
		show_info(filename, &statbuf);
}

// 显示所有文件，包括.file
void show_all_info(char *filename, struct stat *statbufp)
{
#if 1
	char modestr[11];
	char fname[100];

	mode_to_letter(statbufp->st_mode, modestr);

	// 如果是目录，则在其后加 ／
	strcpy(fname, filename);
	if (modestr[0] == 'd')
		strcat(fname, "/");
#endif
	//printf("%-*s ", longnamefile, filename);		
	printf("%s ", fname);
}

// 以长格式显示文件，包括.file
void show_fmt_info(char *filename, struct stat *statbufp)
{
	char modestr[11];

	mode_to_letter(statbufp->st_mode, modestr);

	printf("%s ", modestr);
	printf("%-3ld ", statbufp->st_nlink);
	printf("%s ", uid_to_name(statbufp->st_uid));
	printf("%s ", gid_to_name(statbufp->st_gid));
	printf("%-8ld ", statbufp->st_size);
	printf("%12.12s ", ctime(&statbufp->st_mtime) + 4);
	printf("%s%c\n", filename, modestr[0] == 'd' ? '/' : ' ');
}

// 格式化打印信息
void show_info(char *filename, struct stat *statbufp)
{
	if (longinfo == 1 && allinfo == 1) 	{	// -la
		show_fmt_info(filename, statbufp);
	}
	else if (longinfo == 1 && allinfo == 0) {	// -l
		if (*filename != '.')
			show_fmt_info(filename, statbufp);
	}
	else if (longinfo == 0 && allinfo == 1)	{	// -a 
		show_all_info(filename, statbufp);
	}
	else {
		if (*filename != '.')
			show_all_info(filename, statbufp);
	}

}

// 将mode位转换为目标格式
void mode_to_letter(int mode, char *modestr)
{
	strcpy(modestr, "----------");
	if (S_ISDIR(mode))	modestr[0] = 'd';

	if (mode & S_IRUSR)		modestr[1] = 'r';
	if (mode & S_IWUSR)		modestr[2] = 'w';
	if (mode & S_IXUSR)		modestr[3] = 'x';

	if (mode & S_IRGRP)		modestr[4] = 'r';
	if (mode & S_IWGRP)		modestr[5] = 'w';
	if (mode & S_IXGRP)		modestr[6] = 'x';

	if (mode & S_IROTH)		modestr[7] = 'r';
	if (mode & S_IWOTH)		modestr[8] = 'w';
	if (mode & S_IXOTH)		modestr[9] = 'x';
}

// 将uid转换为名字
char *uid_to_name(long uid)
{
	struct passwd *pw_ptr;
	static char numstr[10];

	if ((pw_ptr = getpwuid(uid)) == NULL) {		//这个uid对应的用户不存在
		sprintf(numstr, "%ld", uid);
		return numstr;
	}else
		return pw_ptr->pw_name;
}

// 将gid转换为名字
char *gid_to_name(long gid)
{
	struct group *grp_ptr;
	static char numstr[10];

	if ((grp_ptr = getgrgid(gid)) == NULL) {	//这个gid对应的用户组不存在
		sprintf(numstr, "%ld", gid);
		return numstr;
	} else
		return grp_ptr->gr_name;
}


// p1, p2指向的是数组中的元素,是char **，而strcmp中p1, p2是char *
int my_strcmp(const void *p1, const void *p2)
{
	return strcmp(* (char * const *) p1, * (char * const *) p2);
}