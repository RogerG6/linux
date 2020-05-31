/*************************************************************
 Date       : 2020年 05月 28日 星期四 21:31:05 CST
 Author     : rogerg6
 File       : process.c
 Description:
 	command processing layer
 	
*************************************************************/
#include "smsh.h"
#include <stdio.h>

int process(char *argv[])
{
	int rv = 0;

	if (argv[0] == NULL)
		rv = 0;
	else if (is_control_command(argv[0]))		// if then fi
		rv = do_control_command(argv);
	else if (ok_to_execute())					// ok to execute
		rv = execute(argv);

	return rv;
}