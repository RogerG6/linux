/*************************************************************
 Date       : 2020年 05月 31日 星期日 17:06:46 CST
 Author     : rogerg6
 File       : varlib.c
 Description:
 	interface:
 		VLstore(name, value)
 		VLlookup(name)
 		VLlist()				prints out current table

 	environment related function:
 		VLexport(name)			add name vars to env vars
 		VLtable2environ()		copy from table to environ
 		VLenviron2table()		copy from environ to table
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#define MAXVARS 200		// max env vars, maybe linked list would be nicer

struct var {
	char *str;		// var name
	int global;		// global flag bit
};

static struct var tab[MAXVARS];		// table
static char *new_string(char *name, char *val);
static struct var *find_item(char *name, int first_blank);


/* purpose : store name=val in table
 * returns : 0 -> ok, 1 -> fail
 */
int VLstore(char *name, char *val)
{
	struct var *itemp;
	char *s;
	int rv = 1;

	if ((itemp = find_item(name, 1)) != NULL && (s = new_string(name, val)) != NULL) {
		if (itemp->str) 
			free(itemp->str);
		itemp->str = s;
		rv = 0;			// ok
	}
	return rv;
}

/* purpose : put name=val in a string, and return str ptr
 * returns : NULL for fail
 */
static char *new_string(char *name, char *val)
{
	int len = strlen(name) + 2 + strlen(val);
	char *cp;

	cp = (char *)malloc(len);
	if (cp != NULL)
		sprintf(cp, "%s=%s", name, val);

	return cp;
}


/* purpose : return val by name
 * returns : NULL for fail
 */
char *VLlookup(char *name)
{
	struct var *itemp;

	if ((itemp = find_item(name, 0)) != NULL) {
		return itemp->str + 1 + strlen(name);
	}

	return "";
}

/* purpose : marks a var for export, adds it if not here
 * returns : 0 -> ok, 1 -> no
 */
int VLexport(char *name)
{
	int rv = 1;
	struct var *itemp;

	if ((itemp = find_item(name, 0)) != NULL) {		// found
		itemp->global = 1;
		rv = 0;
	} else {
		if (VLstore(name, "") == 1)				// store name in vartab
			rv = VLexport(name);
	}

	return rv;
}

/* purpose : find item by name 
 * returns : NULL for fail
 * details : 
 *		2 modes:
 *			first_blank == 0 : only find exist && has value
 *			first_blank == 1 : find, if exist return, else not return blank position
 */
static struct var *find_item(char *name, int first_blank)
{
	int i;
	char *s;
	int len = strlen(name);

	for (i = 0; i < MAXVARS && tab[i].str != NULL; i++) {
		s = tab[i].str;
		if (strncmp(s, name, len) == 0 && s[len] == '=')			// found && has value
			return &tab[i];
	}

	if (i < MAXVARS && first_blank)		// not found || no value
		return &tab[i];

	return NULL;
}

/* purpose : List the contents of the vartab
 *			 gloabl with '*' symbol
 */
void VLlist()
{
	int i;

	for (i = 0; i < MAXVARS && tab[i].str != NULL; i++) {
		if (tab[i].global == 1)
			printf("* %s\n", tab[i].str);
		else
			printf("%s\n", tab[i].str);
	}
}


/* purpose : copy env vars to vartab
 * returns : 0 -> ok, 1 -> no
 */
int VLenviron2table(char *env[])
{
	int i;
	char *s;

	for (i = 0; env[i] != NULL; i++) {
		if (i == MAXVARS)		// exceed MAXVARS
			return 0;

		s = (char *)malloc(strlen(env[i]) + 1);
		if (s == NULL)
			return 0;

		strcpy(s, env[i]);
		tab[i].str = s;
		tab[i].global = 1;
	}

	while (i < MAXVARS) {
		tab[i].str = NULL;
		tab[i++].global = 0;
	}

	return 1;
}

/* purpose : make a environment
 * returns : NULL for fail
 */
char **VLtable2environ()
{
	int i, j;
	int n = 0;		// global var cnt
	char **envtab;

	for (i = 0; i < MAXVARS && tab[i].str != NULL; i++) {
		if (tab[i].global == 1)
			n++;
	}

	envtab = (char **)malloc((n + 1) * sizeof(char *));
	if (envtab == NULL)
		return NULL;

	for (i = 0; i < MAXVARS && tab[i].str != NULL; i++) {
		if (tab[i].global == 1)
			envtab[j++] = tab[i].str;
	}

	envtab[i] = NULL;

	return envtab;
}