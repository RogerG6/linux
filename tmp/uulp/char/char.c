#include <stdio.h>

#define ASCII 128

int main()
{
	int i, j = 1;

	for (i = 0; i < ASCII; i++) {
		if (i % 10 == 0) {
			putchar('\n');
			printf("%2d : ", j++);
		}
		printf("%4c ", i);
	}
	putchar('\n');
	return 0;
}