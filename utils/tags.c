/*
 *  Sat Oct  9 21:16:30 1999 -- Scott Turner
 *
 *  Routines for reading, writing and manipulating tags.
 *  Compile with:
 *
 *             cc -c tags.c
 *
 */
#include <stdio.h>
#include <string.h>
#include "tags.h"

/*
 *  Parse out a "token".
 *
 *  A token ends at the first space, '=', or '>' character, unless
 *  it is quoted, in which case it ends at the end quote character.
 *
 *  Skip over all spaces and the '=' char before starting the token.
 *
 *  In case of failure, return null.
 *
 */
static char *parse_next(char *p, char *dest, int max_size)
{
  int size = 0;
  char closing_char = ' ';
  int quoted = 0;
  
  /*
   *  Skip spaces and equal signs.
   *
   */
  while (*p && (*p == ' ' || *p == '=')) p++;
  /*
   *  Are we quoted?
   *
   */
  if (*p == '"') {
    /*
     *  End with another quote.
     *
     */
    closing_char = '"';
    quoted = 1;
    /*
     *  Skip the opening quote.
     *
     */
    p++;
  };
  /*
   *  If you're quoted, you can only end on the closing char.
   *  If you're not quoted, you can end on space, > or =.
   *
   */
  while((quoted && *p != closing_char) ||
	(!quoted && *p != closing_char && *p != '>' && *p != '=')) {
    /*
     *  Missing close?
     *
     */
    if (!*p) return NULL;
      
    /*
     *  Check for overflow.
     *
     */
    if (size >= max_size) return NULL;
      
    *dest++ = *p++;
    size++;
  };
  /*
   *  Advance past the closing char, unless it's a '>' (which is 
   *  used at the upper level to detect end-of-tag).
   *
   */
  if (*p != '>') p++;
  /*
   *  End the dest with a null.
   *
   */
  *dest = '\0';
  return p;
};

/*
 *  Read a tag from a string, returning the first character
 *  after the end of the tag.
 *
 *  If no tag is found, just returns the string.
 *
 */
char *read_tag(char *s, struct tag *t)
{
  char *p, *end;
  int state = 0;
  int key_num = 0;
  
  /*
   *  First let's make sure there's really a "tag" here.
   *
   */
  if (strncasecmp(s, "<tag ", 5) == 0) {
    p = s + 5;
    t->start = 1;
  } else if (strncasecmp(s, "<tag>", 5) == 0) {
    p = s + 4;
    t->start = 1;
  } else if (strncasecmp(s, "</tag ", 6) == 0) {
    p = s + 6;
    t->start = 0;
  } else if (strncasecmp(s, "</tag>", 6) == 0) {
    p = s + 5;
    t->start = 0;
  } else {
    return s;
  };

  /*
   *  Now we can start parsing (after the "<tag ").
   *
   *  We have a bit of a state machine here.  We're either
   *  parsing a key (state = 0) , or parsing a value (state = 1).
   *
   */
  while (*p && *p != '>') {
    /*
     *  Alternately parse out keys and values.
     *
     */
    if (!state) {
      /*
       *  Possible overflow of key space?
       *
       */
      if (key_num >= MAX_KEYS) return s;
      /*
       *  Otherwise parse out the key.
       *
       */
      end = parse_next(p, t->keys[key_num].name, MAX_KEY_NAME);
      /*
       *  Possibly it was badly-formed!
       *
       */
      if (end == NULL) return s;
    } else {
      end = parse_next(p, t->keys[key_num].value, MAX_KEY_VALUE);
      /*
       *  Possibly it was badly-formed!
       *
       */
      if (end == NULL) return s;
      /*
       *  Move on to the next key.
       *
       */
      key_num++;
    };
    /*
     *  We're now in the other state.
     *
     */
    state = !state;
    /*
     *  Advance p
     *
     */
    p = end;
  };
  /*
   *  Set the number of keys we found.
   *
   */
  t->num_keys = key_num;
  /*
   *  Advance past the >, if necessary.
   *
   */
  if (*p == '>') p++;

  return p;
};

/*
 *  Print out a tag.
 *
 */
void print_tag(FILE *fp, struct tag *t)
{
  int i;

  fprintf(fp,"<tag");
  for(i=0;i<t->num_keys && i<MAX_KEYS;i++)
    if (strchr(t->keys[i].value,' '))
      fprintf(fp," %s=\"%s\"", t->keys[i].name, t->keys[i].value);
    else
      fprintf(fp," %s=%s", t->keys[i].name, t->keys[i].value);
  fprintf(fp,">");
};

/*
 *  Sprintf a tag.
 *
 */
char *sprint_tag(char *s, struct tag *t)
{
  int i;

  if (t->start == 1) {
    sprintf(s,"<tag");
    s+=4;
  } else {
    sprintf(s,"</tag");
    s+=5;
  };
    
  for(i=0;i<t->num_keys && i<MAX_KEYS;i++)
    if (strchr(t->keys[i].value,' ')) {
      sprintf(s," %s=\"%s\"", t->keys[i].name, t->keys[i].value);
      s += 6 + strlen(t->keys[i].name) + strlen(t->keys[i].value);
    } else {
      sprintf(s," %s=%s", t->keys[i].name, t->keys[i].value);
      s += 2 + strlen(t->keys[i].name) + strlen(t->keys[i].value);
    };
  sprintf(s,">");
  s++;
  return s;
};

/*
 *  Get a key value, given a tag and a key name.  Returns null if no such
 *  key name.
 *
 *  Mon Oct 11 08:12:27 1999 -- Scott Turner
 *
 *  Returns empty string if key not found.
 *
 */
char *get_key_value(struct tag *t, char *name)
{
  int i;
  for(i=0; i < t->num_keys && i < MAX_KEYS; i++)
    if (strcmp(name, t->keys[i].name) == 0) return t->keys[i].value;
  return "";
};

void test_tags()
{
  struct tag t;
  char test[2048];

  strcpy(test,"<tag type=tab col=30>");
  (void) read_tag(test, &t);
  print_tag(stdout, &t);
  printf("\ntype=%s\n", get_key_value(&t,"type"));
  strcpy(test,"<tag type=name name=\"Hebekiah the Jesuit\">");
  (void) read_tag(test, &t);
  print_tag(stdout, &t);
  printf("\nname=%s\n", get_key_value(&t,"name"));
};
