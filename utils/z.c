
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	"z.h"


#if 1

/*	@(#)drand48.c	2.2	*/
/*LINTLIBRARY*/
/*
 *	drand48, etc. pseudo-random number generator
 *	This implementation assumes unsigned short integers of at least
 *	16 bits, long integers of at least 32 bits, and ignores
 *	overflows on adding or multiplying two unsigned integers.
 *	Two's-complement representation is assumed in a few places.
 *	Some extra masking is done if unsigneds are exactly 16 bits
 *	or longs are exactly 32 bits, but so what?
 *	An assembly-language implementation would run significantly faster.
 */
#ifndef HAVEFP
#define HAVEFP 1
#endif
#define N	16
#define MASK	((unsigned)(1 << (N - 1)) + (1 << (N - 1)) - 1)
#define LOW(x)	((unsigned)(x) & MASK)
#define HIGH(x)	LOW((x) >> N)
#define MUL(x, y, z)	{ long l = (long)(x) * (long)(y); \
		(z)[0] = LOW(l); (z)[1] = HIGH(l); }
#define CARRY(x, y)	((long)(x) + (long)(y) > MASK)
#define ADDEQU(x, y, z)	(z = CARRY(x, (y)), x = LOW(x + (y)))
#define X0	0x330E
#define X1	0xABCD
#define X2	0x1234
#define A0	0xE66D
#define A1	0xDEEC
#define A2	0x5
#define C	0xB
#define SET3(x, x0, x1, x2)	((x)[0] = (x0), (x)[1] = (x1), (x)[2] = (x2))
#define SETLOW(x, y, n) SET3(x, LOW((y)[n]), LOW((y)[(n)+1]), LOW((y)[(n)+2]))
#define SEED(x0, x1, x2) (SET3(x, x0, x1, x2), SET3(a, A0, A1, A2), c = C)
#define REST(v)	for (i = 0; i < 3; i++) { xsubi[i] = x[i]; x[i] = temp[i]; } \
		return (v);
#define NEST(TYPE, f, F)	TYPE f(xsubi) register unsigned short *xsubi; { \
	register int i; register TYPE v; unsigned temp[3]; \
	for (i = 0; i < 3; i++) { temp[i] = x[i]; x[i] = LOW(xsubi[i]); }  \
	v = F(); REST(v); }
#define HI_BIT	(1L << (2 * N - 1))

static unsigned x[3] = { X0, X1, X2 }, a[3] = { A0, A1, A2 }, c = C;
static unsigned short lastx[3];
static void next();

#if HAVEFP
double
drand48()
{
#if pdp11
	static double two16m; /* old pdp11 cc can't compile an expression */
	two16m = 1.0 / (1L << N); /* in "double" initializer! */
#else
	static double two16m = 1.0 / (1L << N);
#endif

	next();
	return (two16m * (two16m * (two16m * x[0] + x[1]) + x[2]));
}

NEST(double, erand48, drand48);

#else

long
irand48(m)
/* Treat x[i] as a 48-bit fraction, and multiply it by the 16-bit
 * multiplier m.  Return integer part as result.
 */
register unsigned short m;
{
	unsigned r[4], p[2], carry0 = 0;

	next();
	MUL(m, x[0], &r[0]);
	MUL(m, x[2], &r[2]);
	MUL(m, x[1], p);
	if (CARRY(r[1], p[0]))
		ADDEQU(r[2], 1, carry0);
	return (r[3] + carry0 + CARRY(r[2], p[1]));
}

long
krand48(xsubi, m)
/* same as irand48, except user provides storage in xsubi[] */
register unsigned short *xsubi;
unsigned short m;
{
	register int i;
	register long iv;
	unsigned temp[3];

	for (i = 0; i < 3; i++) {
		temp[i] = x[i];
		x[i] = xsubi[i];
	}
	iv = irand48(m);
	REST(iv);
}
#endif

long
lrand48()
{
	next();
	return (((long)x[2] << (N - 1)) + (x[1] >> 1));
}

long
mrand48()
{
	register long l;

	next();
	/* sign-extend in case length of a long > 32 bits
						(as on Honeywell) */
	return ((l = ((long)x[2] << N) + x[1]) & HI_BIT ? l | -HI_BIT : l);
}

static void
next()
{
	unsigned p[2], q[2], r[2], carry0, carry1;

	MUL(a[0], x[0], p);
	ADDEQU(p[0], c, carry0);
	ADDEQU(p[1], carry0, carry1);
	MUL(a[0], x[1], q);
	ADDEQU(p[1], q[0], carry0);
	MUL(a[1], x[0], r);
	x[2] = LOW(carry0 + carry1 + CARRY(p[1], r[0]) + q[1] + r[1] +
		a[0] * x[2] + a[1] * x[1] + a[2] * x[0]);
	x[1] = LOW(p[1] + r[0]);
	x[0] = LOW(p[0]);
}

void
srand48(seedval)
long seedval;
{
	SEED(X0, LOW(seedval), HIGH(seedval));
}

unsigned short *
seed48(seed16v)
unsigned short seed16v[3];
{
	SETLOW(lastx, x, 0);
	SEED(LOW(seed16v[0]), LOW(seed16v[1]), LOW(seed16v[2]));
	return (lastx);
}

void
lcong48(param)
unsigned short param[7];
{
	SETLOW(x, param, 0);
	SETLOW(a, param, 3);
	c = LOW(param[6]);
}

NEST(long, nrand48, lrand48);

NEST(long, jrand48, mrand48);

#ifdef DRIVER
/*
	This should print the sequences of integers in Tables 2
		and 1 of the TM:
	1623, 3442, 1447, 1829, 1305, ...
	657EB7255101, D72A0C966378, 5A743C062A23, ...
 */
#include <stdio.h>

main()
{
	int i;

	for (i = 0; i < 80; i++) {
		printf("%4d ", (int)(4096 * drand48()));
		printf("%.4X%.4X%.4X\n", x[2], x[1], x[0]);
	}
}
#endif

#endif



/*
 *  malloc safety checks:
 *
 *	Space for two extra ints is is allocated beyond what the client
 *	asks for.  The size of the malloc'd region is stored at the
 *	beginning, and a magic number is placed at the end.  realloc's
 *	and free's check the integrity of these markers.  This protects
 *	against overruns, makes sure that non-malloc'd memory isn't freed,
 *	and that memory isn't freed twice.
 */

void *
my_malloc(unsigned size)
{
	char *p;
	/*TMS extern char *malloc(); */

	size += sizeof(int);

	p = malloc(size + sizeof(int));

	if (p == NULL)
	{
		fprintf(stderr, "my_malloc: out of memory (can't malloc "
				"%d bytes)\n", size);
		exit(1);
	}

	bzero(p, size);

	*((int *) p) = size;
	*((int *) (p + size)) = 0xABCF;

	return p + sizeof(int);
}


void *
my_realloc(void *ptr, unsigned size)
{
	char *p = ptr;
	/*TMS extern char *realloc(); */
	/*TMS extern char *malloc(); */

	if (p == NULL)
		return my_malloc(size);

	size += sizeof(int);
	p -= sizeof(int);

	assert(*((int *) (p + *(int *) p)) == 0xABCF);

	p = realloc(p, size + sizeof(int));

	*((int *)p) = size;
	*((int *) (p + size)) = 0xABCF;

	if (p == NULL)
	{
		fprintf(stderr, "my_realloc: out of memory (can't realloc "
				"%d bytes)\n", size);
		exit(1);
	}

	return p + sizeof(int);
}


void
my_free(void *ptr)
{
	char *p = ptr;

	p -= sizeof(int);

	assert(*((int *) (p + *(int *) p)) == 0xABCF);
	*((int *) (p + *(int *) p)) = 0;
	*((int *) p) = 0;

	free(p);
}


char *
str_save(char *s)
{
	char *p;

	p = my_malloc(strlen(s) + 1);
	strcpy(p, s);

	return p;
}


void
asfail(char *file, int line, char *cond)
{
	fprintf(stderr, "assertion failure: %s (%d): %s\n",
						file, line, cond);
	abort();
	exit(1);
}


void
lcase(s)
char *s;
{

	while (*s)
	{
		*s = tolower(*s);
		s++;
	}
}


/*
 *  Line reader with no size limits
 *  strips newline off end of line
 */

#define	GETLIN_ALLOC	255

char *
getlin(FILE *fp)
{
	static char *buf = NULL;
	static unsigned int size = 0;
	int len;
	int c;

	len = 0;

	while ((c = fgetc(fp)) != EOF)
	{
		if (len >= size)
		{
			size += GETLIN_ALLOC;
			buf = my_realloc(buf, size + 1);
		}

		if (c == '\n')
		{
			buf[len] = '\0';
			return buf;
		}

		buf[len++] = (char) c;
	}

	if (len == 0)
		return NULL;

	buf[len] = '\0';

	return buf;
}


/*
 *  Get line, remove leading and trailing whitespace
 */

char *
getlin_ew(FILE *fp)
{
	char *line;
	char *p;

	line = getlin(fp);

	if (line)
	{
		while (*line && iswhite(*line))
			line++;			/* eat leading whitespace */

		for (p = line; *p; p++)
			if (*p < 32 || *p == '\t')	/* remove ctrl chars */
				*p = ' ';
		p--;
		while (p >= line && iswhite(*p))
		{				/* eat trailing whitespace */
			*p = '\0';
			p--;
		}
	}

	return line;
}

#define MAX_BUF         8192

static char linebuf[MAX_BUF];
static int nread;
static int line_fd = -1;
static char *point;


int
readfile(char *path)
{

	if (line_fd >= 0)
		close(line_fd);

	line_fd = open(path, 0);

	if (line_fd < 0)
	{
		fprintf(stderr, "can't open %s: ", path);
		perror("");
		return FALSE;
	}

	nread = read(line_fd, linebuf, MAX_BUF);
	point = linebuf;

	return TRUE;
}


char *
readlin()
{
	static char *buf = NULL;
	static unsigned int size = 0;
	int len;
	int c;

	len = 0;

	while (1)
	{
		if (point >= &linebuf[nread])
		{
			if (nread > 0)
				nread = read(line_fd, linebuf, MAX_BUF);

			if (nread < 1)
				break;

			point = linebuf;
		}

		c = *point++;

		if (len >= size)
		{
			size += GETLIN_ALLOC;
			buf = my_realloc(buf, size + 1);
		}

		if (c == '\n')
		{
			buf[len] = '\0';
			return buf;
		}

		buf[len++] = (char) c;
	}

	if (len == 0)
		return NULL;

	buf[len] = '\0';

	return buf;
}


char *
readlin_ew()
{
	char *line;
	char *p;

	line = readlin();

	if (line)
	{
		while (*line && iswhite(*line))
			line++;			/* eat leading whitespace */

		for (p = line; *p; p++)
			if (*p < 32 || *p == '\t')	/* remove ctrl chars */
				*p = ' ';
		p--;
		while (p >= line && iswhite(*p))
		{				/* eat trailing whitespace */
			*p = '\0';
			p--;
		}
	}

	return line;
}



#define	COPY_LEN	1024

void
copy_fp(a, b)
FILE *a;
FILE *b;
{
	char buf[COPY_LEN];

	while (fgets(buf, COPY_LEN, a) != NULL)
		fputs(buf, b);
}


char lower_array[256];


void
init_lower()
{
	int i;

	for (i = 0; i < 256; i++)
		lower_array[i] = i;

	for (i = 'A'; i <= 'Z'; i++)
		lower_array[i] = i - 'A' + 'a';
}


int
i_strcmp(char *s, char *t)
{
	char a, b;

	do {
		a = tolower(*s);
		b = tolower(*t);
		s++;
		t++;
		if (a != b)
			return a - b;
	} while (a);

	return 0;
}


int
i_strncmp(char *s, char *t, int n)
{
	char a, b;

	do {
		a = tolower(*s);
		b = tolower(*t);
		if (a != b)
			return a - b;
		s++;
		t++;
		n--;
	} while (a && n > 0);

	return 0;
}


static int
fuzzy_transpose(char *one, char *two, int l1, int l2)
{
	int i;
	char buf[LEN];
	char tmp;

	if (l1 != l2)
		return FALSE;

	strcpy(buf, two);

	for (i = 0; i < l2 - 1; i++)
	{
		tmp = buf[i];
		buf[i] = buf[i+1];
		buf[i+1] = tmp;

		if (strcmp(one, buf) == 0)
			return TRUE;

		tmp = buf[i];
		buf[i] = buf[i+1];
		buf[i+1] = tmp;
	}

	return FALSE;
}


static int
fuzzy_one_less(char *one, char *two, int l1, int l2)
{
	int count = 0;
	int i, j;

	if (l1 != l2 + 1)
		return FALSE;

	for (j = 0, i = 0; j < l2; i++, j++)
	{
		if (one[i] != two[j])
		{
			if (count++)
				return FALSE;
			j--;
		}
	}

	return TRUE;
}


static int
fuzzy_one_extra(char *one, char *two, int l1, int l2)
{
	int count = 0;
	int i, j;

	if (l1 != l2 - 1)
		return FALSE;

	for (j = 0, i = 0; i < l1; i++, j++)
	{
		if (one[i] != two[j])
		{
			if (count++)
				return FALSE;
			i--;
		}
	}

	return TRUE;
}


static int
fuzzy_one_bad(char *one, char *two, int l1, int l2)
{
	int count = 0;
	int i;

	if (l1 != l2)
		return FALSE;

	for (i = 0; i < l2; i++)
		if (one[i] != two[i] && count++)
			return FALSE;

	return TRUE;
}


int
fuzzy_strcmp(char *one, char *two)
{
	int l1 = strlen(one);
	int l2 = strlen(two);

	if (l2 >= 4 && fuzzy_transpose(one, two, l1, l2))
		return TRUE;

	if (l2 >= 4 && fuzzy_one_less(one, two, l1, l2))
		return TRUE;

	if (l2 >= 5 && fuzzy_one_extra(one, two, l1, l2))
		return TRUE;

	if (l2 >= 5 && fuzzy_one_bad(one, two, l1, l2))
		return TRUE;

	return FALSE;
}


#ifdef SYSV

unsigned short seed[3];

void
init_random()
{
	long l;

	if (seed[0] == 0 && seed[1] == 0 && seed[2] == 0)
	{
		l = time(NULL);
		seed[0] = l & 0xFFFF;
		seed[1] = getpid();
		seed[2] = l >> 16;
	}
}


int
rnd(int low, int high)
{
	extern double erand48();

	return (int) (erand48(seed) * (high - low + 1) + low);
}

#else	/* ifdef SYSV */

init_random() {
	long l;

	srandom(l);
}


rnd(low, high)
int low;
int high;
{

	return random() % (high - low + 1) + low;
}
#endif	/* ifdef SYSV */


void
test_random()
{
	int i;

	if (isatty(1))
	    for (i = 0; i < 10; i++)
		printf("%3d  %3d  %3d  %3d  %3d  %3d  %3d  %3d  %3d  %3d\n",
			rnd(1, 10), rnd(1, 10), rnd(1, 10), rnd(1, 10),
			rnd(1, 10), rnd(1, 10), rnd(1, 10), rnd(1, 10),
			rnd(1, 10), rnd(1, 10));
	else
	    for (i = 0; i < 100; i++)
		printf("%d\n", rnd(1, 10));

	for (i = -10; i >= -16; i--)
		printf("rnd(%d, %d) == %d\n", -3, i, rnd(-3, i));

	for (i = 0; i < 100; i++)
		printf("%d\n", rnd(1000,9999));

	{
		ilist l = NULL;
		int i;

		for (i = 1; i <= 10; i++)
			ilist_append(&l, i);

		ilist_scramble(l);

		printf("Scramble:\n");

		for (i = 0; i < ilist_len(l); i++)
			printf("%d\n", l[i]);
	}
}



#define		ILIST_ALLOC	6	/* doubles with each realloc */


/*
 *  Reallocing array handler
 *
 *  Length is stored in ilist[0], maximum in ilist[1].
 *  The user-visible ilist is shifted to &ilist[2], so
 *  that iterations can proceed from index 0.
 */

void
ilist_append(ilist *l, int n)
{
	int *base;

	if (*l == NULL)
	{
		base = my_malloc(sizeof(**l) * ILIST_ALLOC);
		base[1] = ILIST_ALLOC;

		*l = &base[2];
	}
	else
	{
		base = (*l)-2;
		assert(&base[2] == *l);

		if (base[0] + 2 >= base[1])
		{
			base[1] *= 2;
			base = my_realloc(base, base[1] * sizeof(*base));
			*l = &base[2];
		}
	}

	base[ base[0] + 2] = n;
	base[0]++;
}


void
ilist_prepend(ilist *l, int n)
{
	int *base;
	int i;

	if (*l == NULL)
	{
		base = my_malloc(sizeof(**l) * ILIST_ALLOC);
		base[1] = ILIST_ALLOC;

		*l = &base[2];
	}
	else
	{
		base = (*l)-2;
		assert(&base[2] == *l);

		if (base[0] + 2 >= base[1])
		{
			base[1] *= 2;
			base = my_realloc(base, base[1] * sizeof(*base));
			*l = &base[2];
		}
	}

	base[0]++;
	for (i = base[0]+1; i > 2; i--)
		base[i] = base[i-1];
	base[2] = n;
}


#if 0

/*  not tested  */

void
ilist_insert(ilist *l, int pos, int n)
{
	int *base;
	int i;

	if (*l == NULL)
	{
		base = my_malloc(sizeof(**l) * ILIST_ALLOC);
		base[1] = ILIST_ALLOC;

		*l = &base[2];
	}
	else
	{
		base = (*l)-2;
		assert(&base[2] == *l);

		if (base[0] + 2 >= base[1])
		{
			base[1] *= 2;
			base = my_realloc(base, base[1] * sizeof(*base));
			*l = &base[2];
		}
	}

	base[0]++;
	pos += 2;

	for (i = base[0]+1; i > pos; i--)
		base[i] = base[i-1];

	base[pos] = n;
}

#endif


void
ilist_delete(ilist *l, int i)
{
	int *base;
	int j;

	assert(i >= 0 && i < ilist_len(*l));		/* bounds check */
	base = (*l)-2;

	for (j = i+2; j <= base[0]; j++)
		base[j] = base[j+1];

	base[0]--;
}


void
ilist_clear(ilist *l)
{
	int *base;

	if (*l != NULL)
	{
		base = (*l)-2;
		base[0] = 0;
	}
}


void
ilist_reclaim(ilist *l)
{
	int *base;

	if (*l != NULL)
	{
		base = (*l)-2;
		my_free(base);
	}
	*l = NULL;
}


int
ilist_lookup(ilist l, int n)
{
	int i;

	if (l == NULL)
		return -1;

	for (i = 0; i < ilist_len(l); i++)
		if (l[i] == n)
			return i;

	return -1;
}


void
ilist_rem_value(ilist *l, int n)
{
	int i;

	for (i = ilist_len(*l) - 1; i >= 0; i--)
		if ((*l)[i] == n)
			ilist_delete(l, i);
}


void
ilist_rem_value_uniq(ilist *l, int n)
{
	int i;

	for (i = ilist_len(*l) - 1; i >= 0; i--)
		if ((*l)[i] == n)
		{
			ilist_delete(l, i);
			break;
		}
}


#if 1

ilist
ilist_copy(ilist l)
{
	int *base;
	int *copy_base;

	if (l == NULL)
		return NULL;

	base = l-2;
	assert(&base[2] == l);

	copy_base = my_malloc(base[1] * sizeof(*base));
	bcopy(base, copy_base, (base[0] + 2) * sizeof(*base));

	return &copy_base[2];
}

#else

ilist
ilist_copy(ilist l)
{
	ilist new = NULL;
	int i;

	for (i = 0; i < ilist_len(l); i++)
		ilist_append(&new, l[i]);

	return new;
}

#endif


void
ilist_scramble(ilist l)
{
	int i;
	int tmp;
	int one, two;
	int len;

	len = ilist_len(l);

	for (i = 0; i < len * 2; i++)
	{
		one = rnd(0, len-1);
		two = rnd(0, len-1);

		tmp = l[one];
		l[one] = l[two];
		l[two] = tmp;
	}
}


void
ilist_test()
{
	int i;
	ilist x;
	ilist y;

	setbuf(stdout, NULL);
	bzero(&x, sizeof(x));

	printf("len = %d\n", ilist_len(x));

	for (i = 0; i < 100; i++)
		ilist_append(&x, i);

	assert(x[ilist_len(x)-1] == 99);

	printf("len = %d\n", ilist_len(x));
	for (i = 0; i < ilist_len(x); i++)
		printf("%d ", x[i]);
	printf("\n");

	for (i = 900; i < 1000; i++)
	{
		ilist_prepend(&x, i);
		if (x[ilist_len(x)-1] != 99)
			fprintf(stderr, "fail: i = %d\n", i);
	}

	printf("len = %d\n", ilist_len(x));
	for (i = 0; i < ilist_len(x); i++)
		printf("%d ", x[i]);
	printf("\n");

	ilist_delete(&x, 100);

	printf("len = %d\n", ilist_len(x));
	for (i = 0; i < ilist_len(x); i++)
		printf("%d ", x[i]);
	printf("\n");

	printf("len before = %d\n", ilist_len(x));
	ilist_append(&x, 15);
	printf("len after = %d\n", ilist_len(x));
	printf("x[0] = %d\n", x[0]);

	printf("ilist_lookup(998) == %d\n", ilist_lookup(x, 998));

	y = ilist_copy(x);
	assert(ilist_len(x) == ilist_len(y));
	for (i = 0; i < ilist_len(x); i++)
	{
		assert(&x[i] != &y[i]);
		if (x[i] != y[i])
		{
			fprintf(stderr, "[%d] different\n", i);
			assert(FALSE);
		}
	}

	printf("ilist_lookup(998) == %d\n", ilist_lookup(x, 998));

	ilist_clear(&x);
	assert(ilist_len(x) == 0);
}

