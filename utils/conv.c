/*
 *  Mon Nov 15 18:47:33 1999 -- Scott Turner
 *
 *  Tools to convert from ints to Oly-style names and vice versa.
 *  Compile with:
 *
 *                cc    -c conv.c -o conv.o
 *
 */
#include	<stdio.h>
#include		<string.h>

#define	tolower(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))
#define	LEN 100

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

char *
int_to_code(int l)
{
  static char buf[LEN];
  int n, a, b, c;

  if (l < 10000)	{
    sprintf(buf, "%d", l);
  } else if (l < 20000)	{
    l -= 10000;

    n = l % 100;
    l /= 100;

    a = l % 26;
    b = l / 26;

    sprintf(buf, "%c%c%02d", letters[b], letters[a], n);
  } else if (l < 26760){	/* CCN */
    l -= 20000;

    n = l % 10;
    l /= 10;

    a = l % 26;
    b = l / 26;

    sprintf(buf, "%c%c%d", letters[b], letters[a], n);
  } else if (l < 33520) {	/* CNC */
    l -= 26760;

    n = l % 26;
    l /= 26;

    a = l % 10;
    b = l / 10;

    sprintf(buf, "%c%d%c", letters[b], a, letters[n]);
  } else if (l < 36120) {	/* CNN */
    l -= 33520;

    n = l % 10;
    l /= 10;

    a = l % 10;
    b = l / 10;

    sprintf(buf, "%c%d%d", letters[b], a, n);
  } else { /* CCNC */
    l -= 36120;

    a = l % 26;
    l /= 26;

    b = l % 10;
    l /= 10;

    c = l % 26;
    l /= 26;

    sprintf(buf, "%c%c%d%c", letters[l], letters[c], b, letters[a]);
  }

  return buf;
}

int
code_to_int(char *s)
{
  char a, b, c, d;

  if (isdigit(*s))
    return atoi(s);

  if (!isalpha(*s))
    return 0;

  switch (strlen(s)) {
  case 3:
    if (isdigit(*(s+1)) && isalpha(*(s+2)))		/* CNC */
      {
	a = tolower(*s) - 'a';
	b = *(s+1) - '0';
	c = tolower(*(s+2)) - 'a';

	return a * 260 + b * 26 + c + 26760;
      }

    if (isalpha(*(s+1)) && isdigit(*(s+2)))		/* CCN */
      {
	a = tolower(*s) - 'a';
	b = tolower(*(s+1)) - 'a';
	c = *(s+2) - '0';

	return a * 260 + b * 10 + c + 20000;
      }

    if (isdigit(*(s+1)) && isdigit(*(s+2)))		/* CNN */
      {
	a = tolower(*s) - 'a';
	b = *(s+1) - '0';
	c = *(s+2) - '0';

	return a * 100 + b * 10 + c + 33520;
      }

    return 0;

  case 4:
    if (isalpha(*(s+1)) && isdigit(*(s+2)) && isdigit(*(s+3)))
      {
	a = tolower(*s) - 'a';
	b = tolower(*(s+1)) - 'a';
	c = *(s+2) - '0';
	d = *(s+3) - '0';


	return a * 2600 + b * 100 + c * 10 + d + 10000;
      }

    if (isalpha(*(s+1)) && isdigit(*(s+2)) && isalpha(*(s+3)))
      {
	a = tolower(*s) - 'a';
	b = tolower(*(s+1)) - 'a';
	c = *(s+2) - '0';
	d = tolower(*(s+3)) - 'a';


	return a * 6760 + b * 260 + c * 26 + d + 36120;
      }
    return 0;

  default:
    return 0;
  }
}


