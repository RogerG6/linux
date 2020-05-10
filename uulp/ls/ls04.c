/*************************************************************
 Date: 2020年 04月 15日 星期三 18:13:01 CST
 Author: rogerg6
 File: ls.c
 Description: 
	1. 完成option: 
		l: long info list
		a: all file list 
		r: reverse list 
		q: list without sorted
		R: list recursive
		i: list inode num

	问题：

*************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>

#define MAXFILES 4096

// option
int longinfo = 0;		// -l
int allinfo = 0;		// -a
int reverse = 0;		// -r 逆序排序
int sorted = 1;			// 顺序排序，sorted = 1; 不排序，sorted = 0
int recur = 0;			// 递归显示目录
int inode = 0;

//int totfiles = 0;		// 目录下所有的文件，包括子目录下的文件

// 最长名目录项
int longestentry = 0;

void do_ls(char *filename);
void do_stat(char *prepath, char *filename);
void mode_to_letter(int mode, char *modestr);
char *uid_to_name(long uid);
char *gid_to_name(long gid);
int my_strcmp(const void *p1, const void *p2);
int my_rstrcmp(const void *p1, const void *p2);
void show_info_fmt(char *filename, struct stat *statbufp);
void show_info_nofmt(char *filename, struct stat *statbufp);
void sort(char **file, int cnt);
void list_files(char *prepath, char **file, int *cnt);
int read_from_dir(DIR *dir, char *prepath, char **file, int *cnt);
int isDir(struct stat *statbuf);

int main(int argc, char *argv[])
{
	int ac = argc;
	char **av = argv;
	int c, obj = 0;		// obj是查看的目录个数
	int len;

	// 处理option
	while (--argc > 0 && *++argv) 
		if (**argv == '-') {
			while (c = *(++(*argv)))
				switch (c) {
					case 'l': longinfo = 1; break;
					case 'a': allinfo = 1; break;
					case 'r': reverse = 1; break;
					case 'q': sorted = 0; break;
					case 'R': recur = 1; break;
					case 'i': inode = 1; break;
				}
		}
		else 
			obj++;

	// 查看当前目录
	if (obj == 0)
		do_ls(".");
	else {
		// 查看非当前目录
		while (--ac > 0 && *++av) 
			if (**av != '-' && **av != 0) 
				do_ls(*av);	

		//printf("\n总文件数 %d\n", totfiles);
	}
		
	return 0;
}

void do_ls(char *prepath)
{
	char *file[MAXFILES];
	int cnt = 0;			// 目录下总文件数
	DIR *dir;


	// can't open dir
	if (read_from_dir(dir, prepath, file, &cnt) == -1)
		return;

	// 排序
	if (sorted == 1)
		sort(file, cnt);
#if 0
	int i;
	for (i =0; i < cnt; i++)
		printf("--filename: %s\n", file[i]);
	putchar('\n');
#endif

	// 显示文件内容
	list_files(prepath, file, &cnt);

	closedir(dir);		// 必须在最后关闭，否则会出现各种奇怪问题

#if 0
	//int i;
	for (i =0; i < cnt; i++)
		printf("--filename: %s\n", file[i]);
	putchar('\n');
#endif
}

// 从目录中读取文件到 file[] 中
int read_from_dir(DIR *dir, char *prepath, char **file, int *cnt)
{
	struct dirent *dirp;
	int len, i;

	// 打开目录
	if ((dir = opendir(prepath)) == NULL) {
		fprintf(stderr, "can't open direct %s\n", prepath);
		return -1;
	}
	else {
		// 读取目录项
		i = 0;
		while ((dirp = readdir(dir)) != NULL) {
			// 没有-a，则路过.file
			if (allinfo == 0 && dirp->d_name[0] == '.')
				continue;

			file[i++] = dirp->d_name;
			if ((len = strlen(dirp->d_name)) > longestentry)	// 记录最长名目录项的长度
				longestentry = len;

			(*cnt)++;
		}
		//closedir(dir);
		//totfiles += *cnt;
	}
	return 0;
}

/* 
   prepath: /dir/
   filename: filname
   组合成: /dir/filename
 */
void list_files(char *prepath, char **file, int *cnt)
{
	int i, j, k, t;
	int row, col, remainder;	// 行列数，最后一行余数
	struct winsize term_size;		// 当前窗口size info	
	int files = 0;

#if 0
	int a;
	for (a =0; a < *cnt; a++)
		printf("--filename: %s\n", file[a]);
	putchar('\n');
#endif

	// ls -l
	if (longinfo == 1) {
		for (i = 0; i < *cnt; i++) {
			do_stat(prepath, file[i]);
		}
	} else {
		// ls no -l

		// 获得win信息，放在struct term_size中
		ioctl(2, TIOCGWINSZ, &term_size);
		col = term_size.ws_col / (longestentry + 10);

		remainder = (*cnt) % col;
		row = (*cnt) / col + (remainder == 0 ? 0 : 1);
		//printf("row = %d, col = %d\n", row, col);
		for (i = 0; i < row; i++) {
			t = 0;
			for (j = 0; j < col; j++) {
				do_stat(prepath, file[i + t]);
				if (++files == *cnt) {
					putchar('\n');
					return;
				}

				if (remainder == 0)
					k = row;
				else
					k = j < remainder ? row : (row - 1);
				t += k;
			}
				putchar('\n');
		}

	}


}


void do_stat(char *prepath, char *filename)
{
	char path[100];
	struct stat statbuf;

//printf("--filename: %s\n", filename);
	// 目录项的具体路径
	strcpy(path, prepath);
	if (path[strlen(path)- 1] != '/')
		strcat(path, "/");

	strcat(path, filename);
//printf("--filename: %s\n", filename);

	//printf("path: %s ||| ", path);
	if (stat(path, &statbuf) == -1)
		perror(path);

	// 如果是dir，则递归显示文件
	if (recur == 1 && isDir(&statbuf)) {
		do_ls(path);
		return;
	}

	if (longinfo == 1)
		show_info_fmt(filename, &statbuf);
	else {
		show_info_nofmt(filename, &statbuf);
	}
}

int isDir(struct stat *statbuf)
{
	if (S_ISDIR(statbuf->st_mode))
		return 1;
	else
		return 0;
}

// 显示所有文件，包括.file
void show_info_nofmt(char *filename, struct stat *statbufp)
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
#if 1
	if (inode == 1)	{
		printf("%-ld ", statbufp->st_ino);
		printf("%-*s ", longestentry, fname);
	}
	else
#endif	
		printf("%-*s ", longestentry, fname);
		//printf("%s ", fname);
}

// 以长格式显示文件，包括.file
void show_info_fmt(char *filename, struct stat *statbufp)
{
	char modestr[11];

	mode_to_letter(statbufp->st_mode, modestr);

	if (inode == 1)
		printf("%ld ", statbufp->st_ino);
	printf("%s ", modestr);
	printf("%-3ld ", statbufp->st_nlink);
	printf("%s ", uid_to_name(statbufp->st_uid));
	printf("%s ", gid_to_name(statbufp->st_gid));
	printf("%-11ld ", statbufp->st_size);
	printf("%12.12s ", ctime(&statbufp->st_mtime) + 4);
	printf("%s%c\n", filename, modestr[0] == 'd' ? '/' : ' ');
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

// p1, p2指向的是数组中的元素,是char **，而strcmp中p1, p2是char *
/* p1 < p2, > 0
   p1 = p2, = 0
   p1 > p2, < 0 
 */
int my_rstrcmp(const void *p1, const void *p2)
{
	return strcmp(* (char * const *) p2, * (char * const *) p1);
}

void sort(char **file, int cnt)
{
	if (reverse == 0)
		qsort(file, cnt, sizeof(char *), my_strcmp);
	else 
		qsort(file, cnt, sizeof(char *), my_rstrcmp);
}

