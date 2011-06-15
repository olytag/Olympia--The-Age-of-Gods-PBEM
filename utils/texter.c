/*
 *  Mon Nov 15 18:51:01 1999 -- Scott Turner
 *
 *  Filter to convert TAG raw output to text.
 *  Compile with:
 *                cc -o texter texter.c tags.o
 *
 *
 *  Tue Dec 14 07:39:34 1999 -- Scott Turner
 *
 *  Since we're not often outputting tags in the text output (maybe never)
 *  we'll fix this to do text wrapping the "right" way.  That means that
 *  when we go over the wrap limit, we'll go backwards to the first breaking
 *  space, output that much of the line, and then wrap the extra onto the
 *  next line.
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "tags.h"

/*
 *  "Cur" is the current output line.  eocur is the actual end of the
 *  buffer; cur_col is the column not counting embedded tags.
 *
 */
#define LEN 2048
static char cur[LEN];
static int eocur = 0;
static int cur_col = 0;
static int line_start = 0;

/*
 *  Edison added this comment:

;.'                    ,/l[.]ft7,..............................................................................................

*
*/

/*
 *  Tags: are we passing through tags?
 *
 */
static int tags = 0;
static int found_tag = 0;

/*
 *  indent::text
 *  indent:day:text
 *  indent/second-indent:day:text
 *  indent/second-indent::text
 */
int indent, second_indent, day;
char unit[100];
char *strip_stuff(char *s)
{
  char *p;

  /*
   *  Pull off unit string
   */
  for (p = unit; *s && *s != ':'; s++)
    *p++ = *s;

  if (p > unit) {
      *p++ = ':';
      *p++ = ' ';
    }
  *p = '\0';
  s++;

  /*
   *  Now get indentation:  %d or %d/%d
   */
  indent = atoi(s);

  while (*s && *s != ':' && *s != '/')
    s++;

  if (*s == '/') {
    s++;
    second_indent = atoi(s);
    while (*s && *s != ':')
      s++;
  } else
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

/*
 *  Put the header on the front of the output buffer.
 *
 */
void do_header()
{
  int i;
  if (day >= 0) {
    sprintf(&cur[eocur], "%2d: ", day);
    eocur += 4;
    cur_col += 4;
  };

  if (unit[0]) {
    strcat(cur, unit);
    eocur += strlen(unit);
    cur_col += strlen(unit);
  };
  
  /*
   *  Possibly add in some indent.
   *
   */
  for(i=0;i<indent;i++) {
    cur[eocur++] = ' ';
  };
  cur_col += indent;

  line_start = cur_col;
  
};

/*
 *  Initialize the output buffer.
 *
 *  Strip the :: stuff off the front of the line, save it
 *  in global variables, reset the output buffer (cur), etc.
 *
 *  Mon Oct 11 09:50:01 1999 -- Scott Turner
 *
 *  Reset "found_tag".  We're not outputting empty lines
 *  with only a tag on them (unless we're passing tags, of course).
 *
 */
char *initialize_output_buffer(char *line)
{
  int front = 0;
  /*
   *  Reset the output buffer.
   *
   */
  eocur = 0;
  cur_col = 0;
  cur[eocur] = '\0';
  /*
   *  Strip stuff off the front.
   *
   */
  line = strip_stuff(line);
  /*
   *  Put header on front of output buffer as appropriate.
   *
   */
  do_header();

  /*
   *  Reset found_tag.
   *
   */
  found_tag = 0;

  return line;
};

/*
 *  Check the output buffer to see if we need to output it.
 *
 *  The "right way" to wrap lines is unfortunately too hard for us
 *  to do correctly, because of the problems with embedded tags (which
 *  don't "count").  So what we do instead is output this line at the
 *  first break character (space) *after* the WRAP point.
 *
 *  Tue Dec 14 07:37:22 1999 -- Scott Turner
 *
 *  We'll wrap on the assumption that we're not outputting tags.
 *
 */
#define WRAP 80
void check_output()
{
  int i, end, old_eol;
  if (cur_col > WRAP) {
    /*
     *  Search backwards for the first blank space.
     *
     */
    for(i=eocur-1;i>0 && cur[i] != ' ';i--);
    /*
     *  If i == 0, then just give up and output it all.
     *
     *  Tue Feb  8 10:40:49 2000 -- Scott Turner
     *
     *  That's wrong.  What we need to do is just give up; then at
     *  some point we'll either find a space we can break on, or
     *  flush_output_buffer will output the entire long line.
     *
     * */
    if (i == 0) return;
    /*
     *  Now "cleanse" up to i;
     *
     */
    end = i;
    for(i=0;i<end;i++)
      if (cur[i] == '~') cur[i] = ' ';
    /*
     *  Now put in the the eol at end.
     *
     */
    cur[end] = 0;
    /*
     *  And output it.
     *
     */
    printf("%s\n",cur);
    /*
     *  Now reset the line, put on the header, etc.  Save
     *  the old eol for our wrapping.
     *
     */
    old_eol = eocur;
    eocur = 0;
    cur_col = 0;
    cur[eocur] = '\0';
    do_header();
    /*
     *  And add in any secondary indent.
     *
     */
    for(i=0;i<second_indent;i++)
      cur[eocur++] = ' ';
    cur_col += second_indent;
    /*
     *  Now copy in anything left over from the wrapped line.
     *
     */
    for(i=end+1;i<old_eol;i++) {
      cur[eocur++] = cur[i];
      cur_col++;
    };
  };
};

/*
 *  Process a char: push it onto the output buffer and
 *  then see if you need to do anything with the buffer.
 *
 */
char *process_char(char *line)
{
  /*
   *  Put it on the output buffer.  Update the column, since
   *  this is a regular character.
   *
   */
  cur[eocur++] = *line;
  cur_col++;
  /*
   *  Check to see if we need to wrap, etc.
   *
   */
  check_output();

  return ++line;
};

/*
 *  Process a (possible) tag.
 *
 */
char *process_tag(char *line)
{
  struct tag t;
  char *s;
  int i;
  
  /*
   *  Try to read it.
   *
   */
  s = read_tag(line, &t);
  /*
   *  Maybe it wasn't a legitimate tag?
   *
   */
  if (s == line) {
    /*
     *  Treat this as a normal character
     *  and then return.
     *
     */
    process_char(line);
    return ++line;
  };
  /*
   *  Note that we found one.
   *
   */
  found_tag++;
  
  /*
   *  See if there's any specific tag processing to be done.
   *
   */
  if (strncasecmp(get_key_value(&t, "type"), "tab", 3) == 0) {
    int col = atoi(get_key_value(&t, "col"));
    if (col && col > cur_col) {
      for(i=0;i<(col - cur_col);i++)
	cur[eocur++] = ' ';
      cur_col = col;
    };
  };

  /*
   *  Otherwise it was a tag.  If tags are on, we're going
   *  to dump this on the output, no moving the current_column.
   *
   */
  if (tags) {
    char *end;
    end = sprint_tag(&cur[eocur], &t);
    eocur += (end - &cur[eocur]);
  };
  
  return s;
};

/*
 *  Mon Oct 11 09:54:00 1999 -- Scott Turner
 *
 *  Don't output empty lines that have a tag on them.
 *
 *  Fri Oct 22 07:11:01 1999 -- Scott Turner
 *
 *  Hmm.  We need to account for the possible "day" and "unit" stuff
 *  at the beginning of the line as well.  "line_start" will mark where
 *  the line *really* started.
 *
 *  Tue Dec 14 07:50:32 1999 -- Scott Turner
 *
 *  Have to "cleanse" the line.
 *
 */
void flush_output_buffer()
{
  int i;
  
  if (eocur > line_start+1 || !found_tag) {
    cur[eocur-1] = '\0';
    for(i=0;i<eocur-1;i++)
      if (cur[i] == '~') cur[i]=' ';
    printf("%s\n",cur);
  };
};

/*
 *  Process a line: Step through it char-by-char.  Normal chars
 *  you can buffer for output; for tags you need to parse the tag
 *  and see if there's anything to be done for that tag.
 *
 */
void process_line(char *line)
{
  int i = 0;
  
  /*
   *  Initialization at beginning of line.
   *
   */
 line = initialize_output_buffer(line);

  /*
   *  Now walk through the rest of the line.
   *
   */
 while(*line) {
    if (*line == '<') {
      /*
       *  Possible tag processing.
       *
       */
      line = process_tag(line);
    } else {
      /*
       *  A normal character.
       *
       */
      line =  process_char(line);
    };
 };
 /*
  *  End of line processing.
  *
  */
 flush_output_buffer();
};

/*
 *  The main function: process the input stream line by line.
 *  This works well because we promise not to split tags across
 *  lines.  At worst, you could have a very long line (containing the
 *  entire input).
 *
 */
void process_file(FILE *input)
{
  char line[LEN];
  while(fgets(line, LEN, input) != NULL) 
    process_line(line);
};

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
		tags = 1;
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

	process_file(fp);
	fclose(fp);
	exit(0);
}
