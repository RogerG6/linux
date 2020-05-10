/*************************************************************
 Date       : 2020年 04月 21日 星期二 23:12:58 CST
 Author     : rogerg6
 File       : file.c
 Description:
 	Usage: file filename
 	大致确定文件类型

 	问题：无法像真正的file一样，呈现出详细的信息
 		 真正的file有三种检测：
 		 	1. filesystem test
 		 	2. magic test
 		 	3. language tests
 		 具体man file
*************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	struct stat statbuf;

	if (argc < 2) {
		printf("Usage: file filename\n");
		return -1;
	}

	while(--argc > 0 && *++argv)
		if (stat(*argv, &statbuf) != -1) {
			printf("%s: ", *argv);
			switch (statbuf.st_mode & S_IFMT) {
	           case S_IFBLK:  printf("block device\n");            break;
	           case S_IFCHR:  printf("character device\n");        break;
	           case S_IFDIR:  printf("directory\n");               break;
	           case S_IFIFO:  printf("FIFO/pipe\n");               break;
	           case S_IFLNK:  printf("symlink\n");                 break;
	           case S_IFREG:  printf("regular file\n");            break;
	           case S_IFSOCK: printf("socket\n");                  break;
	           default:       printf("unknown?\n");                break;
           }
		}

    return 0;

}
