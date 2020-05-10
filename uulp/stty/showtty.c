/*************************************************************
 Date       : 2020年 05月 03日 星期日 18:04:22 CST
 Author     : rogerg6
 File       : showtty.c
 Description:
 	显示当前tty的一些属性
*************************************************************/

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


struct flaginfo {
	int val;
	char *fl_name;
};

struct flaginfo input_flags[] = {
	IGNBRK, "Ignore break condition",
	BRKINT, "Signal interrupt on break",
	IGNPAR, "Ignore chars with parity errors",
	PARMRK, "Mark parity errors",
	INPCK,  "Enable input parity check",
	ISTRIP, "Strip character",
	INLCR,  "Map NL to CR on input",
	IGNCR,  "Ignore CR",
	ICRNL,  "Map CR to NL on input",
	IXON,   "Enable start/stop output control",
	IXOFF,  "Enable start/stop input control",
	0, NULL
};

struct flaginfo local_flags[] = {
	ISIG, "Enable signals",
	ICANON, "Canonical input(erase and kill)",
	ECHO, "Enable echo",
	ECHOE, "Echo ERASE as BS-SPACE-BS",
	ECHOK, "Echo KILL by starting new line",
	0, NULL
};

void showbaud(speed_t bd);
void show_some_flags(struct termios *info);
void show_flagset(int value, struct flaginfo flagarry[]);

int main(void)
{
	struct termios info;

	if (tcgetattr(0, &info) == -1) {
		perror("tcgetattr");
		exit(1);
	}

	// show baud rate
	showbaud(cfgetospeed(&info));

	// erase, kill charater
	printf("The earse charater is ascii %d, Ctrl-%c\n", info.c_cc[VERASE], info.c_cc[VERASE] - 1 + 'A');
	printf("The line kill charater is ascii %d, Ctrl-%c\n", info.c_cc[VKILL], info.c_cc[VKILL] - 1 + 'A');		

	// show flags
	show_some_flags(&info);

	return 0;
}

// 显示波特率
void showbaud(speed_t bd)
{
	printf("The baud rate: ");
	switch (bd) {
		case B0      : printf("0\n"); break;
		case B50     : printf("50\n"); break;
		case B75     : printf("75\n"); break;
		case B110    : printf("110\n"); break;
		case B134    : printf("134\n"); break;
		case B150    : printf("150\n"); break;
		case B200    : printf("200\n"); break;
		case B300    : printf("300\n"); break;
		case B600    : printf("600\n"); break;
		case B1200   : printf("1200\n"); break;
		case B1800   : printf("1800\n"); break;
		case B2400   : printf("2400\n"); break;
		case B4800   : printf("4800\n"); break;
		case B9600   : printf("9600\n"); break;
		case B19200  : printf("19200\n"); break;
		case B38400  : printf("38400\n"); break;
		case B57600  : printf("57600\n"); break;
		case B115200 : printf("115200\n"); break;
		case B230400 : printf("230400\n"); break;
		default: printf("faster\n");
	}
}

// 显示c_iflag, c_lflag
void show_some_flags(struct termios *info)
{
	show_flagset(info->c_iflag, input_flags);
	show_flagset(info->c_lflag, local_flags);
}

void show_flagset(int value, struct flaginfo flagarry[])
{
	int i;

	for (i = 0; flagarry[i].val; i++) {
		printf("%s is ", flagarry[i].fl_name);
		if (flagarry[i].val & value)
			printf("ON\n");
		else
			printf("OFF\n");
	}
}