/*************************************************************
 Date       : 2020年 05月 10日 星期日 10:20:44 CST
 Author     : rogerg6
 File       : passwd.c
 Description:
 	修改密码

 	问题：不知道如何将修改好的密码写入文件，加密
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <crypt.h>

//#define _XOPEN_SOURCE
#define PSWLEN 10

void do_passwd(struct passwd *pwp);

int main(int argc, char const *argv[])
{
	struct passwd *pwp;

	if (argc < 2) {
		fprintf(stderr, "Usage： passwd username\n");
		exit(1);
	}

	// get passwd entry
	while ((pwp = getpwent()) != NULL)
		if (strcmp(argv[1], pwp->pw_name) == 0) {
			do_passwd(pwp);
			break;
		}

	return 0;
}

void do_passwd(struct passwd *pwp)
{
	char pw[PSWLEN];
	FILE *pwfp;
	char encrypted[1000];

	printf("新UNIX密码：");
	scanf("%s", pw);

	crypt(pw, pwp->pw_passwd);
	//strcpy(pwp->pw_passwd, pw);

	if ((pwfp = fopen("/etc/passwd", "a")) == NULL) {
		perror("/etc/passwd");
		return;
	}	

	putpwent(pwp, pwfp);
}