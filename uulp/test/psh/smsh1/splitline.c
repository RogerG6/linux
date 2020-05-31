/*************************************************************
 Date       : 2020年 05月 26日 星期二 20:27:58 CST
 Author     : rogerg6
 File       : splitline.c
 Description:
 	do : 读取输入行，并且将输入分成各个参数，写入arglist
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

char *next_cmd(char *prompt, FILE *fp)
{
	char *buf;			// cmdline buffer
	int bufspace = 0;	// total size
	int pos = 0;		// current position
	int c;

	printf("%s", prompt);
	while ((c = getc(fp)) != EOF) {
		if (pos + 1 >= bufspace) {
			if (bufspace == 0)				// 1st time alloc
				buf = emalloc(BUFSIZ);
			else							// change size
				buf = erealloc(buf, bufspace + BUFSIZ);
			bufspace += BUFSIZ;			
		}

		if (c == '\n')
			break;
		buf[pos++] = c;
	}

	if (c == EOF || pos == 0)
		return NULL;

	buf[pos] = '\0';
	return buf;
}

#define is_delim(x) ((x) == ' ' || (x) == '\t')

char **splitline(char *line)
{
	char **args;
	int spots = 0;		// spots in table
	int bufspace = 0;	// bytes in table
	int argnum = 0;		// slots used
	int len;
	char *cp = line;
	char *start;

	if (line == NULL)
		return NULL;

	args = emalloc(BUFSIZ);
	bufspace = BUFSIZ;
	spots = BUFSIZ / sizeof(char *);

	while (*cp != '\0') {
		while (is_delim(*cp))
			cp++;
		if (*cp == '\0')
			break;

		// 动态分配arglist
		if (argnum + 1 >= spots) {
			args = erealloc(args, bufspace + BUFSIZ);
			bufspace += BUFSIZ;
			spots += BUFSIZ / sizeof(char *);
		}
		
		// 标记开头，找到结尾
		start = cp;
		len = 1;
		while (*++cp != '\0' && !is_delim(*cp))
			len++;

		args[argnum++] = newstr(start, len);
	}
	args[argnum] = NULL;

	return args;
}

char * newstr(char *start, int len)
{
	char *rv = emalloc(len + 1);

	rv[len] = '\0';
	strncpy(rv, start, len);
	return rv;
}

void freelist(char **list)
{
	char **cp = list;
	while (*cp)
		free(*cp++);
	free(list);
}

void *emalloc(size_t n)
{
	void *rv;

	if ((rv = malloc(n)) == NULL)
		fatal("out of memory", "", 1);
	return rv;
}

void *erealloc(void *p, size_t n)
{
	void *rv;

	if ((rv = realloc(p, n)) == NULL)
		fatal("realloc() failed", "", 1);
	return rv;
}
