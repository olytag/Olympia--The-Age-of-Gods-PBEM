/*
 *  Sat Oct  9 08:20:20 1999 -- Scott Turner
 *
 *  This is a version of "rep" that simply untangles the
 *  includes and does nothing else.  This is the only portion of
 *  rep that can't act as a filter (i.e., it must work on a file
 *  it can rewind and lseek on).
 *
 *  This also strips off the box and unit nums from the front of the
 *  line, so that the line is indent/second:day:text
 *
 *  Wed Oct 13 10:14:02 1999 -- Scott Turner
 *
 *  Changed tag so we don't have to worry about "unit".
 *
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#define		iswhite(c)	((c) == ' ' || (c) == '\t')
#define		LEN		2048
#define		TRUE		1
#define		FALSE		0


int indent;
char unit[100];

main(argc, argv)
int argc;
char **argv;
{
	FILE *fp;

	fp = fopen(argv[1], "r");
	if (fp == NULL)
	{
		fprintf(stderr, "can't read %s: ", argv[1]);
		perror("");
		exit(1);
	}

	parse_includes(fp);
	fclose(fp);

	exit(0);
}

/*
 *
 */
out_line(s)
char *s;
{
  char *start = strchr(s, ':')+1;
  fputs(start, stdout);
}


out_line_include(fp, s)
FILE *fp;
char *s;
{
  char *start;

  start = strrchr(s,':');
  start++;

  if (strncmp(start, "#include ", 9) == 0)
    print_by_num(fp, atoi(start+9));
  else
    out_line(s);
}


print_by_num(fp, num)
FILE *fp;
int num;
{
	char buf[LEN];
	long l;

	l = ftell(fp);
	rewind(fp);

	while (fgets(buf, LEN, fp) != NULL)
	{
		if (atoi(buf) == num)
			out_line(buf);
	}

	fseek(fp, l, 0);
}


parse_includes(fp)
FILE *fp;
{
	char buf[LEN];

        while (fgets(buf, LEN, fp) != NULL)
	{
		if (atoi(buf) == 1)
			out_line_include(fp, buf);
	}
}


