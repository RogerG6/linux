/* Date: 2020/4/19
   Func: 模仿cp的一个程序：
   		 1. 同一个文件不能复制
   		 2. 加入的-i 选项，复制前确认是否要覆盖
   		 3. 从file->file, file->dir, dir->dir(dir中的所有文件的复制)

   		 问题：
   			1. 不能进行目录到目录的递归复制
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#define BUFSIZE 4096
#define MAXPATHLEN 200
#define SINGLEFILENAME 100
#define COPYMODE 0644

void oops(char *s1, char *s2);
int do_cp(char *cp, char *dest, char *src, int confirm);
void cp_dir2dir(char *cp, char *dest, char *src, int confirm);
void cp_file2dir(char *cp, char *dest, char *src, int confirm);
void cp_file2file(char *cp, char *dest, char *src, int confirm);

int main(int argc, char *argv[])
{
	int c;
	char *filename[3];
	int i = 0, confirm = 0;		//confirm: 确认是否要覆盖，默认覆盖

	if (argc < 3) {
		fprintf(stderr, "Usage: cp from to\n");
		exit(1);
	}

	filename[2] = argv[0];

	// 处理option
	while (--argc > 0 && **++argv)
		if (**argv == '-')
			while ((c = *((*argv)++)) != '\0') {
				if (c == 'i')
					confirm = 1;
			}
		else 
			filename[i++] = *argv;
	
	do_cp(filename[2], filename[1], filename[0], confirm);

	return 0;
}

int do_cp(char *cp, char *dest, char *src, int confirm)
{
	int srcdir = 0, destdir = 0;	// flags 如果是目录设为1
	DIR *dir;

	// 如果源与目的是同一文件，则返回
	if (strcmp(src, dest) == 0) {
		printf("%s: \'%s\' 与 \'%s\' 为同一文件\n", cp, src, dest);
		return -1;
	}

	// src, dest is director ???
	if ((dir = opendir(src)) != NULL) {
		srcdir = 1;
		closedir(dir);
	}
	if ((dir = opendir(dest)) != NULL) {
		destdir = 1;
		closedir(dir);
	}

	// do cp by case
	if (srcdir == 1 && destdir == 1)			// d->d
		cp_dir2dir(cp, dest, src, confirm);
	else if (srcdir == 1 && destdir == 0) {		//d->f
		printf("can't cp directory: %s to file: %s\n", src, dest);
		return -1;
	} else if (srcdir == 0 && destdir == 1)		//f->d
		cp_file2dir(cp, dest, src, confirm);
	else if (srcdir == 0 && destdir == 0)		//f->f
		cp_file2file(cp, dest, src, confirm);

}

// dir -> dir, 是dir中的所有文件cp到dir中
void cp_dir2dir(char *cp, char *dest, char *src, int confirm)
{
	int i;
	char srcpath[MAXPATHLEN], tmppath[MAXPATHLEN];
	DIR *srcd;
	struct dirent *dirp;

	if ((srcd = opendir(src)) == NULL) {
		printf("can't open %s\n", src);
		return;
	}

	// srcpath/
	strcpy(tmppath, src);
	if (tmppath[strlen(tmppath) - 1] != '/')
		strcat(tmppath, "/");

	while ((dirp = readdir(srcd)) != NULL) {
		// 跳过.file
		if (dirp->d_name[0] == '.')
			continue;

		// srcpath/srcfilename
		strcpy(srcpath, tmppath);
		strcat(srcpath, dirp->d_name);

		printf("srcpath: %s\n", srcpath);
		// do copy
		cp_file2dir(cp, dest, srcpath, confirm);
	}
}

// file -> dir
void cp_file2dir(char *cp, char *dest, char *src, int confirm)
{
	char path[MAXPATHLEN];		// 目的文件全名
	//char srcfilename[SINGLEFILENAME];
	int i;

	// destdir/destfilename
	strcpy(path, dest);
	if (path[strlen(path) - 1] != '/')
		strcat(path, "/");

	for (i = strlen(src) - 1; i >= 0 && src[i] != '/'; i--)
		;
	strcat(path, &src[i + 1]);	

	cp_file2file(cp, path, src, confirm);	
}

// file -> file
void cp_file2file(char *cp, char *dest, char *src, int confirm)
{
	int fromfd, tofd;
	char buf[BUFSIZE];
	int n_char;

	if (confirm == 1) {
		printf("%s: 是否要覆盖 \'%s\'?", cp, dest);
		if (getchar() == 'n')
			return;
	}

	//打开源，目的文件
	if ((fromfd = open(src, O_RDONLY)) == -1)
		oops("Can't open", src);
	if ((tofd = creat(dest, COPYMODE)) == -1)
		oops("cp_file2file: Can't creat", dest);

	// 从源文件读BUFSIZE字节到目的文件
	while ((n_char = read(fromfd, buf, BUFSIZE)) > 0)
		write(tofd, buf, n_char);

	// 读源文件出错
	if (n_char == -1)
		oops("Reading error from", src);

	// 关闭源，目的文件
	if (close(fromfd) == -1)
		oops("Error closing files", src);	
	if (close(tofd) == -1)
		oops("Error closing files", dest);	

}

void oops(char *s1, char *s2)
{
	fprintf(stderr, "Error: %s ", s1);
	perror(s2);
	exit(1);
}