#include <stdio.h>
#include <stdlib.h>

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *fp);
int see_more(void);

int main(int argc, char *argv[])
{
	FILE *fp;

	if (argc == 1)		// 从stdin获取输入
		do_more(stdin);
	else				// 从文件中获取输入
		while (--argc)
			if ((fp = fopen(*++argv, "r")) != NULL) {
				do_more(fp);
				fclose(fp);
			} else
				exit(1);
	return 0;
}

void do_more(FILE *fp)
{
	char line[LINELEN];
	int lineno = 0;
	int retval;

	while (fgets(line, LINELEN, fp)) {
		if (lineno == PAGELEN) {
			retval = see_more();
			if (retval == 0)
				break;
			lineno -= retval;
		}
		if (fputs(line, stdout) == EOF)
			exit(1);
		lineno++;
	}

}

int see_more(void)
{
	int c;

	printf("\033[7m more? \033[m");
	while ((c = getchar()) != EOF)
		if (c == 'q')
			return 0;
		if (c == ' ')
			return PAGELEN;
		if (c == '\n')
			return 1;
}