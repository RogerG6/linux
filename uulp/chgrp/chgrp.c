/*************************************************************
 Date       : 2020年 04月 21日 星期二 23:24:58 CST
 Author     : rogerg6
 File       : chgrp.c
 Description:
 	Usage: chgrp grpname file
*************************************************************/

#include <sys/types.h>
#include <grp.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct group *getgrnam(const char *name);

#define NAMELEN 100

int main(int argc, char *argv[])
{
	char group[NAMELEN];
	int i;
	gid_t gid;
	struct group *grp;


	if (argc < 3) {
		printf("Usage: chown grpname filename...\n");
		return -1;
	}

	strcpy(group, *++argv);

	if ((grp = getgrnam(group)) == NULL){
		printf("%s is not found\n", group);
		return -1;
	}

	gid = grp->gr_gid;

	// change owner
	for (i = argc - 2; i > 0; i--)
		chown(*++argv, -1, gid);

	return 0;
}

