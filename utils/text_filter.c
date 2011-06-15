/*
 *  Sat Oct  9 11:43:39 1999 -- Scott Turner
 *
 *  Reads an "untangled" TAG output file and generates a text-format
 *  output.
 *
 *  Works only as a filter, so to use on a file you need to redirect.
 */
#include	<stdio.h>
#include <unistd.h>
#include	<stdlib.h>
#include	<string.h>
#ifndef TAGS_H
#include "tags.h"
#endif TAGS_H

#define		iswhite(c)	((c) == ' ' || (c) == '\t')
#define		LEN		2048
#define		TRUE		1
#define		FALSE		0


int indent;
char unit[100];
int second_indent;
int day;
int tags = 0;

main(argc, argv)
int argc;
char **argv;
{
	FILE *fp = stdin;
	extern int optind;
	extern char *optarg;
	char c;
	
	while ((c = getopt(argc, argv, "tf:")) != EOF)
	  {
	    switch (c)
	      {
	      case 't':
		tags = TRUE;
		break;
	      case 'f':
		fp = fopen(optarg, "r");
		if (fp == NULL)
		  {
		    fprintf(stderr, "can't read %s: ", argv[1]);
		    perror("");
		    exit(1);
		  }
	      };
	    
	};

	init_spaces();
	parse_it(fp);
	fclose(fp);

	exit(0);
}

int skipping = 0;

output(s)
char *s;
{
  char *p;

  for (p = s; *p; p++)
    if (*p == '~')
      *p = ' ';

  if (day >= 0)
    printf("%2d: %s", day, unit);
  else
    printf("%s", unit);

  /*
   *  Output S, skipping <...>.  Note this is inefficient,
   *  since we're going char-by-char, but hey, computer time
   *  is cheap :-).
   *
   */
  for(p=s; *p; p++) {
    if (!tags && *p == '<' &&
	*(p+1) && *(p+1) == 't' &&
	*(p+2) && *(p+2) == 'a' &&
	*(p+3) && *(p+3) == 'g') skipping++;
    if (!tags && *p == '<' &&
	*(p+1) && *(p+1) == '/' &&
	*(p+2) && *(p+2) == 't' &&
	*(p+3) && *(p+3) == 'a' &&
	*(p+4) && *(p+4) == 'g') skipping++;
    if (!skipping) putchar(*p);
    if (!tags && skipping && *p == '>') skipping--;

  };
  putchar('\n');
}


/*
 *  Word wrapper
 */


static char cur[LEN];
static int wrap_pos;
static char *spaces;
static int spaces_len;
static int wrap_output;


init_spaces()
{
	int i;

	spaces_len = 150;
	spaces = malloc(spaces_len+1);

	if (spaces == NULL)
	{
		fprintf(stderr, "out of memory\n");
		exit(1);
	}

	for (i = 0; i <	spaces_len; i++)
		spaces[i] = ' ';

	spaces[spaces_len] = '\0';
}


/*
 *  To use:
 *	wrap_set(who);
 *	wrap(s, ...)			as many as needed
 *	wrap_done();
 */


void
wrap_start()
{

	strcpy(cur, &spaces[spaces_len - indent]);
	wrap_output = FALSE;
}


wrap_done_sup()
{

	output(cur);
	strcpy(cur, &spaces[spaces_len - indent]);
	wrap_output = TRUE;
}


cur_has_something()
{
	char *p;

	for (p = cur; *p && iswhite(*p); p++)
		;

	if (*p)
		return TRUE;
	return FALSE;
}


wrap_done()
{

	if (cur_has_something() || !wrap_output)
		wrap_done_sup();
}

int strlen_tags(char *p)
{
  int count = 0;
  int skipping = 0;

  for(;*p;p++) {
    if (!tags && *p == '<' &&
	*(p+1) && *(p+1) == 't' &&
	*(p+2) && *(p+2) == 'a' &&
	*(p+3) && *(p+3) == 'g') skipping++;
    if (!tags && *p == '<' &&
	*(p+1) && *(p+1) == '/' &&
	*(p+2) && *(p+2) == 't' &&
	*(p+3) && *(p+3) == 'a' &&
	*(p+4) && *(p+4) == 'g') skipping++;
    if (!skipping) count++;
    if (!tags && skipping && *p == '>') skipping--;
  };
  return count;
};

/*
 *  Split s at or before pos, on a space.
 *
 *  General approach: count up from the front of the string, skipping
 *  tags , and keeping track of the last space.
 *
 *  In this function, it doesn't matter whether we're passing tags through
 *  or not.  They're presumed not to add anything to the line length, so
 *  we always want to split based on the non-tag line.
 *
 */
char *
word_split(s, pos)
char *s;
int pos;
{
  int where, count = 0, len, last_space = 0;
  int skipping = 0;


  len = strlen(s);
  for(where=0;s[where] && count < pos;where++) {
    if (s[where] == '<' &&
	s[where+1] && s[where+1] == 't' &&
	s[where+2] && s[where+2] == 'a' &&
	s[where+3] && s[where+3] == 'g') skipping++;
    if (s[where] == '<' &&
	s[where+1] && s[where+1] == '/' &&
	s[where+2] && s[where+2] == 't' &&
	s[where+3] && s[where+3] == 'a' &&
	s[where+4] && s[where+4] == 'g') skipping++;
    if (!skipping) count++;
    if (!skipping && s[where] == ' ') last_space = where;
    if (skipping && s[where] == '>') skipping--;
  };

  if (!last_space) return(NULL);

  s[last_space] = '\0';

  return(&s[last_space + 1]);
}


wrap_append(s, t)
char *s;
char *t;
{
	int slen;

	if (strcmp(t, "\"") != 0) {
		slen = strlen_tags(s);

		if (slen > 0 &&	s[slen - 1] != ' ' && s[slen - 1] != '"') {
			strcat(s, " ");
			slen++;
		}

		if (slen > 1 &&
			(s[slen	- 2] ==	'.'
				|| s[slen - 2] == ':'
				|| s[slen - 2] == '?'
				|| s[slen - 2] == '!') )
			strcat(s, " ");
	}

	strcat(s, t);
}


wrap(s)
char *s;
{
	char *t;
	int len;

	len = strlen_tags(cur);

	if (strlen_tags(s) + len < wrap_pos)
		wrap_append(cur, s);
	else {
		t = word_split(s, wrap_pos - len);
		if (t == NULL && cur_has_something()) {
			wrap_done_sup();
			wrap(s);
		} else {
			wrap_append(cur, s);
			wrap_done_sup();
			if (t)
				wrap(t);
		}
	}
}


/*
 *  indent::text
 *  indent:day:text
 *  indent/second-indent:day:text
 *  indent/second-indent::text
 */

char *
strip_stuff(s)
char *s;
{
	char *p;
/*
 *  Now get indentation:  %d or %d/%d
 */

	indent = atoi(s);

	while (*s && *s != ':' && *s != '/')
		s++;

	if (*s == '/')
	{
		s++;
		second_indent = atoi(s);
		while (*s && *s != ':')
			s++;
	}
	else
		second_indent = 0;

	if (*s == ':')
		s++;

/*
 *  Now get day, if any
 */

	if (*s == ':')
		day = -1;
	else
		day = atoi(s);

	while (*s && *s != ':')
		s++;

	if (*s == ':')
		s++;

	return s;
}


out_line(s)
char *s; 
{
	char *p;

	for (p = s; *p && *p != '\n'; p++)
			;
	*p = '\0';

	if (day >= 0)
		wrap_pos = 72 - strlen_tags(unit);	/* left margin has 2d: on it */
	else
		wrap_pos = 76 - strlen_tags(unit);

	wrap_start();
	indent += second_indent;
	wrap(s);
	wrap_done();
	indent -= second_indent;
}


parse_it(fp)
FILE *fp;
{
	char buf[LEN];

        while (fgets(buf, LEN, fp) != NULL)
	{
	  out_line(strip_stuff(buf));
	}
}

