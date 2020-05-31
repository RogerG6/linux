/*************************************************************
 Date       : 2020年 05月 26日 星期二 21:48:34 CST
 Author     : rogerg6
 File       : smsh.h
 Description:
*************************************************************/

#define YES 1
#define NO  0

char *next_cmd(char *prompt, FILE *fp);
char **splitline(char *line);
char * newstr(char *start, int len);
void freelist(char **list);
void *emalloc(size_t n);
void *erealloc(void *p, size_t n);
void fatal(char *s1, char *s2, int n);
int execute(char *argv[]);
