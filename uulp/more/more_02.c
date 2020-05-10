#include <stdio.h>
#include <stdlib.h>

/* Date: 2020/4/5 
 * more version 2: 从/dev/tty中获取用户的指令
 */

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *fp);
int see_more(FILE *cmd);

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
	FILE *fp_tty;

	fp_tty = fopen("/dev/tty", "r");
	if (fp_tty == NULL)
		exit(1);

	while (fgets(line, LINELEN, fp)) {
		if (lineno == PAGELEN) {
			retval = see_more(fp_tty);
			if (retval == 0)
				break;
			lineno -= retval;
		}
		if (fputs(line, stdout) == EOF)
			exit(1);
		lineno++;
	}

}

int see_more(FILE *cmd)
{
	int c;

	printf("\033[7m more? \033[m");
	while ((c = getc(cmd)) != EOF)
		if (c == 'q')
			return 0;
		if (c == ' ')
			return PAGELEN;
		if (c == '\n')
			return 1;
}