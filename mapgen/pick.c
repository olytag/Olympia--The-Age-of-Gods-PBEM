
#include	<stdio.h>
#include	"z.h"


char *names[13000];
int top = 0;

main() {
	char *line;
	char *tmp;
	int one, two;
	int i;

	load_seed();

	while (line = getlin(stdin))
	{
		names[top++] = str_save(line);
	}

	for (i = 1; i <= top; i++)
	{
		one = rnd(0, top-1);
		two = rnd(0, top-1);
		tmp = names[one];
		names[one] = names[two];
		names[two] = tmp;
	}

	for (i = 0; i < 500; i++)
	{
		do {
			one = rnd(0, top-1);
		} while (names[one] == NULL || strlen(names[one]) != 8);

		printf("%03d%s%03d\n", rnd(0,999), names[one], rnd(0,999));
		names[one] = NULL;
	}
}
