/*
 *  Sat Oct  9 21:12:40 1999 -- Scott Turner
 *
 *  The tag structure holds the information for a single
 *  <tag>.
 *
 *
 */
#ifndef TAGS_H
#define TAGS_H
#define MAX_KEY_NAME 32      /* Maximum size for a key name. */
#define MAX_KEY_VALUE 1024    /* Maximum size for a key value. */
#define MAX_KEYS 20          /* Maximum keys in a single tag. */

struct tag_value {
  char name[MAX_KEY_NAME];
  char value[MAX_KEY_VALUE];
};

struct tag{
  int num_keys;
  int start;
  struct tag_value keys[MAX_KEYS];
};
  
char *read_tag(char *s, struct tag *t);
void print_tag(FILE *fp, struct tag *t);
char *sprint_tag(char *s, struct tag *t);
char *get_key_value(struct tag *t, char *name);
void test_tags();
#endif
