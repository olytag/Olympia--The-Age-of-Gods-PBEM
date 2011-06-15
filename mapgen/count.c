

#include <stdio.h>


main() {
        int c;
        int count[256];
        int i;
        int total = 0;
        int land = 0;

        for (i = 0; i < 256; i++)
                count[i] = 0;

        while ((c = getchar()) != EOF)
        {
                if (c == '\r' || c == '\n')
                        continue;
                count[c]++;
		total++;
        }

        for (i = 0; i < 256; i++)
                if (count[i])
                {
                        printf("%c  %d\n", i, count[i]);
                        if (i != '~' && i != ' ' && i != '.' && i != ',' &&
			    i != '"' && i != ';')
                                land += count[i];
                }

        printf("total land = %d\n", land);
        printf("land = %d%%\n", land * 100 / total);
}

