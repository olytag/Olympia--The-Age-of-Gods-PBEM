

#include    <stdio.h>
#include    <string.h>
#include    "z.h"
#include    "oly.h"


/*
 *  Entity coding system
 *
 *  range	      extent	use
 *  1-9999	       9999	reserved (items, skills)
 *
 *  10,000-19,999    10,000	provinces		(CCNN: AA00-DV99)
 *  20,000-26,759      6760	player entities		(CCN)
 *  26,760-33,519      6760	lucky characters	(CNC)
 *  33,520-36,119      2600     lucky locs		(CNN)
 *  36,120-102,400   66,279	sublocs, runoff		(CCNC)
 */


static char *letters = "abcdefghijklmnopqrstuvwxyz";

main() {
    char *stuff;
    int_to_code(57913);
}

char *
int_to_code(int l) {
    int n, a, b, c;

    if (l < 10000) {
        printf("%d", l);
    }

    if (l < 20000)            /* CCNN */
    {
        l -= 10000;

        n = l % 100;
        l /= 100;

        a = l % 26;
        b = l / 26;

        printf("%c%c%02d", letters[b], letters[a], n);
    }

    if (l < 26760)            /* CCN */
    {
        l -= 20000;

        n = l % 10;
        l /= 10;

        a = l % 26;
        b = l / 26;

        printf("%c%c%d", letters[b], letters[a], n);
    }

    if (l < 33520)            /* CNC */
    {
        l -= 26760;

        n = l % 26;
        l /= 26;

        a = l % 10;
        b = l / 10;

        printf("%c%d%c", letters[b], a, letters[n]);
    }

    if (l < 36120)            /* CNN */
    {
        l -= 33520;

        n = l % 10;
        l /= 10;

        a = l % 10;
        b = l / 10;

        printf("%c%d%d", letters[b], a, n);
    }

    {                /* CCNC */
        l -= 36120;

        a = l % 26;
        l /= 26;

        b = l % 10;
        l /= 10;

        c = l % 26;
        l /= 26;

        printf("%c%c%d%c", letters[l], letters[c], b, letters[a]);
    }
}

