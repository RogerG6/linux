/*************************************************************
 Date       : 2020年 05月 28日 星期四 21:35:04 CST
 Author     : rogerg6
 File       : controlflow.c
 Description:
	
	控制流通过划分下列区域实现：
		中立区   					: 命令可以直接执行
		want then 				: 获取if 后表达式的值
		then block/else block 	：执行代码

	-------------------------------
	 中立区      ：ls, who, if diff file1 file1.bak
	 want_then  : then
	 then_block : rm file1.bak
	 			  echo removing backup
	 			  else
	 else_block : chmod -w file1.bak
	 		      fi
	 中立区      : date

*************************************************************/

#include <stdio.h>
#include <string.h>
#include "smsh.h"

enum states {NEUTRAL, WANT_THEN, THEN_BLOCK};
enum results {SUCCESS, FAIL};

static int if_state = NEUTRAL;
static int if_results = SUCCESS;
static int last_stat = 0;


/* return value: 1 -> execute, 0 -> no execute
*/
int ok_to_execute(void)
{
	int rv = 1;		// default is positive

	if (if_state == WANT_THEN) {
		syn_err("then expected");
		rv = 0;
	}
	else if (if_state == THEN_BLOCK && if_results == SUCCESS)
		rv = 1;
	else if (if_state == THEN_BLOCK && if_results == FAIL)
		rv = 0;

	return rv;
}

/* purpose : if command is control cmd or not */
int is_control_command(char *s)
{
	return (strcmp(s, "if") == 0 || strcmp(s, "then") == 0 || strcmp(s, "fi") == 0);
}

/* purpose : processing "if", "then", "fi"-- change state or detect error
   returns : 0 -> ok, -1 -> syntax error
 */
int do_control_command(char **args)
{
	char *cmd = args[0];		// first args in arglist
	int rv = -1;

	if (strcmp(cmd, "if") == 0) {
		if (if_state != NEUTRAL)
			rv = syn_err("if unexpected");		// 未料到的if
		else {
			last_stat = process(args + 1);
			if_results = last_stat == 0 ? SUCCESS : FAIL;
			if_state = WANT_THEN;				// set stat as WANT_THEN
			rv = 0;
		}
	} else if (strcmp(cmd, "then") == 0) {
		if (if_state != WANT_THEN)
			rv = syn_err("then unexpected");
		else {
			if_state = THEN_BLOCK;
			rv = 0;
		}
	} else if (strcmp(cmd, "fi") == 0) {
		if (if_state != THEN_BLOCK)
			rv = syn_err("fi unexpected");
		else {
			if_state = NEUTRAL;
			rv = 0;
		}
	} else
		fatal("internal error processing:", cmd, 2);	// 除if/then/fi 外的cmd

	return rv;
}

/* purpose : handle syntax error in control flow
 */
int syn_err(char *msg)
{
	if_state = NEUTRAL;		// reset state to neutral

	fprintf(stderr, "syntax error: %s\n", msg);
	return -1;
}