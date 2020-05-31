/*************************************************************
 Date       : 2020年 05月 29日 星期五 21:37:17 CST
 Author     : rogerg6
 File       : builtin.c
 Description:
*************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "varlib.h"

int assign(char *str);
int okname(char *s);

/* purpose : run a builtin command
 * returns : 1 -> builtin command, 0 -> not
 */
int builtin_cmd(char **args, int *resultp)
{
	int rv = 0;

	if (strcmp(args[0], "set") == 0) {					// set command ?
		VLlist();
		*resultp = 0;
		rv = 1;
	} else if (strchr(args[0], '=') != NULL) {
		*resultp = assign(args[0]);
		if (*resultp != -1)								// variable name is not ok
			rv = 1;
	} else if (strcmp(args[0], "export") == 0) {
		if (args[1] != NULL && okname(args[1]))
			*resultp = VLexport(args[1]);
		else
			*resultp = 1;
		rv = 1;
	}

	return rv;
}

/* purpose : execute name=val && check name is legal
 * returns : -1 -> illegal lval, or result of VLstore
 */
int assign(char *str)
{
	char *cp;
	int rv;

	cp = strchr(str, '=');
	*cp = '\0';				// a=val => a val

	rv = (okname(str) ? VLstore(str, cp + 1) : -1);

	*cp = '=';				// restore '='
	return rv;
}

/* purpose : check name is legal or not
 * returns : 0 -> illegal, !0 -> legal
 */
int okname(char *s)
{
	char *cp;

	for (cp = s; *cp; cp++) {
		if ((isdigit(*cp) && cp == s) || !isalnum(*cp) || *cp == '_')
			return 0;
	}

	return (cp != s);		// empty string return 0, ok return !0
}