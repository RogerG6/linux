/*************************************************************
 Date       : 2020年 06月 07日 星期日 15:38:17 CST
 Author     : rogerg6
 File       : tinybc.c
 Description:
 	do : a tiny bc (calculator)
 	details : 
				+++++++++													+++++++++ 
				|		 |	-----------------> pipe1 ----------------->		|		|
	stdin------>|		 |													|		|
				|	bc	 | 									      			|   dc  | 
	stdout<---- |		 |													|		|
				|		 |	<----------------- pipe2 <------------------	|		|
				+++++++++ 													+++++++++ 

*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

#define oops(x, m) {perror(x); exit(m);}

void fatal(char *s);
void be_dc(int in[], int out[]);
void be_bc(int todc[], int fromdc[]);
void countdown(int sec);

int main(void)
{
	int todc[2], fromdc[2];		// 2 pipes
	int pid;

	// make 2 pipes
	if (pipe(todc) == -1 || pipe(fromdc) == -1)
		oops("cannot make pipe", 1);

	// fork
	if ((pid = fork()) == -1) {
		oops("fork failed", 2);
	} else if (pid == 0) {
		be_dc(todc, fromdc);
	} else {
		be_bc(todc, fromdc);
		wait(NULL);
		printf("child ends\n");
	}

	return 0;
}

void be_dc(int in[], int out[])
{
	// read from in[2]
	if (dup2(in[0], 0) == -1) {
		oops("cannot dup2 in[0]", 3);
	}
	close(in[1]);
	close(in[0]);

	// write to out[2]
	if (dup2(out[1], 1) == -1) {
		oops("cannot dup2 out[1]", 4);
	}
	close(out[1]);
	close(out[0]);

	//countdown(10);

	// exec dc with - option
	execlp("dc", "dc", "-", NULL);
	oops("cannot run dc", 5);
}

void be_bc(int todc[], int fromdc[])
{
	int num1, num2;
	char operation[BUFSIZ], message[BUFSIZ];
	FILE *fpout, *fpin;		// out/in file stream

	// setup
	close(todc[0]);
	close(fromdc[1]);

	// convert fd to file stream
	fpout = fdopen(todc[1], "w");
	fpin   = fdopen(fromdc[0], "r");
	if (fpout == NULL || fpin == NULL)
		fatal("Error convering pipes to stream");

	// main loop
	while (printf("tinybc: "), fgets(message, BUFSIZ, stdin) != NULL) {
		// parse input
		if (sscanf(message, "%d%[-+*/]%d", &num1, operation, &num2) != 3) {
			fprintf(stderr, "syntax error\n");
			continue;
		}

		// write to todc[2]
		if (fprintf(fpout, "%d\n%d\n%c\np\n", num1, num2, *operation) == EOF)
			fatal("Error writing");

		// flush buffer
		fflush(fpout);
		if (fgets(message, BUFSIZ, fpin) == NULL) {
			fprintf(stdout, "sent mesg failed\n");
			break;
		}
		printf("%d %c %d = %s\n", num1, *operation, num2, message);
	}

	fclose(fpout);		// dc will see EOF, and dc terminate
	fclose(fpin);		// close pipe
	
}

void fatal(char *s)
{
	fprintf(stderr, "Error: %s\n", s);
	exit(1);
}

// 倒计时
void countdown(int sec)
{
	while (sec) {
		fprintf(stderr, "%d...\n", sec--);
		sleep(1);
	}
}