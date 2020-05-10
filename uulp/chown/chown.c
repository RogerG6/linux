/*************************************************************
 Date       : 2020年 04月 20日 星期一 22:59:35 CST
 Author     : rogerg6
 File       : chown.c
 Description:
 	Usage: chown ownername file1 file2 ... 
	改变文件的所有者
*************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#define NAMELEN 100

int main(int argc, char *argv[])
{
	char owner[NAMELEN];
	int i;
	uid_t uid;
	struct passwd *psd;


	if (argc < 3) {
		printf("Usage: chown ownername filename...\n");
		return -1;
	}

	strcpy(owner, *++argv);

	// get uid by name 
	// 从/etc/passwd中读取一项
	while ((psd = getpwent()) != NULL)
		if (strcmp(psd->pw_name, owner) == 0) {
			uid = psd->pw_uid;
			break;
		}

	if (psd == NULL) {
		printf("User %s not found\n", owner);
		return -1;
	}

	// change owner
	for (i = argc - 2; i > 0; i--)
		chown(*++argv, uid, -1);

	return 0;
}

