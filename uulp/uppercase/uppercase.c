/*************************************************************
 Date       : 2020年 05月 10日 星期日 11:43:33 CST
 Author     : rogerg6
 File       : uppercase.c
 Description:
 	跟踪struct termios中o_flags中的OLCUC位的状态
*************************************************************/

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	struct termios termi;

	
	while (1) {
		if (tcgetattr(0, &termi) == -1) {
			perror("Error");
			exit(1);
		}
		
		getchar();
		
		if (termi.c_oflag & OLCUC)
			printf("olcuc is on\n");
		else
			printf("olcuc is off\n");		
	}


	return 0;
}