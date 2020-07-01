/*************************************************************
 Date       : 2020年 06月 23日 星期二 22:16:00 CST
 Author     : rogerg6
 File       : lclnt.c
 Description: client
 	License server client version 1
 	link with lclnt_funcs1.o dgram.o

*************************************************************/

#include <stdio.h>

void validate();

int main(int ac, char *av[])
{
	setup();
	if (get_ticket() != 0)
		exit(0);

	do_regular_work();

	release_ticket();
	shut_down();
}

void do_regular_work(void)
{
	printf("SuperSleep version 1.0 Running-Licensed Software\n");
	sleep(10);			// our patented sleep algorithm
	validate();
	sleep(5);
}