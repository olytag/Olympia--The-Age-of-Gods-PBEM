
#define	SYSV

#define	TRUE	1
#define	FALSE	0

#define		LEN		512	/* generic string max length */

#ifdef SYSV
#define bzero(a,n)		memset(a, '\0', n)
#define bcopy(a,b,n)		memcpy(b, a, n)
#endif

#define	abs(n)		((n) < 0 ? ((n) * -1) : (n))

#define	isalpha(c)	(((c)>='a' && (c)<='z') || ((c)>='A' && (c)<='Z'))
#define	isdigit(c)	((c) >= '0' && (c) <= '9')
#define	iswhite(c)	((c) == ' ' || (c) == '\t')

#if 0
#define	tolower(c)	(lower_array[c])
extern char lower_array[];
#else
#define	tolower(c)	(((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))
#endif

#define	toupper(c)	(((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))

#if 0
#define	max(a,b)	((a) > (b) ? (a) : (b))
#define	min(a,b)	((a) < (b) ? (a) : (b))
#endif

extern void *my_malloc(unsigned size);
extern void *my_realloc(void *ptr, unsigned size);
extern void my_free(void *ptr);
extern char *str_save(char *);

extern char *getlin(FILE *);
extern char *getlin_ew(FILE *);
extern int i_strncmp(char *s, char *t, int n);
extern int i_strcmp(char *s, char *t);
extern int fuzzy_strcmp(char *, char *);
extern int rnd(int low, int high);

/*
 *  Assertion verifier
 */

extern void asfail(char *file, int line, char *cond);

#ifdef __STDC__
#define	assert(p)	if(!(p)) asfail(__FILE__, __LINE__, #p);
#else
#define	assert(p)	if(!(p)) asfail(__FILE__, __LINE__, "p");
#endif


/*
 *  'ilist' reallocing array definitions
 */

typedef int *ilist;

#define ilist_len(a)		(((int *)(a)) == NULL ? 0 : ((int *)(a))[-2])

extern void ilist_append(ilist *l, int n);
extern void ilist_prepend(ilist *l, int n);
extern void ilist_delete(ilist *l, int i);
extern void ilist_clear(ilist *l);
extern void ilist_reclaim(ilist *l);
extern int ilist_lookup(ilist l, int n);
extern void ilist_rem_value(ilist *l, int n);
extern void ilist_rem_value_uniq(ilist *l, int n);
extern ilist ilist_copy(ilist l);
extern void ilist_scramble(ilist l);
extern void ilist_insert(ilist *l, int pos, int n);

extern int readfile(char *path);
extern char *readlin();
extern char *readlin_ew();
