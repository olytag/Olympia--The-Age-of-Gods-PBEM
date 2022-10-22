// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include <stdlib.h>
#include    <string.h>
#include <unistd.h> // mdhender: added for open(), close(), read()
#include    "z.h"
#include "forward.h"

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
#define N    16
#define MASK    ((unsigned)(1 << (N - 1)) + (1 << (N - 1)) - 1)
#define LOW(x)    ((unsigned)(x) & MASK)
#define HIGH(x)    LOW((x) >> N)
#define MUL(x, y, z)    { long l = (long)(x) * (long)(y); \
        (z)[0] = LOW(l); (z)[1] = HIGH(l); }
#define CARRY(x, y)    ((long)(x) + (long)(y) > MASK)
#define ADDEQU(x, y, z)    (z = CARRY(x, (y)), x = LOW(x + (y)))
#define X0    0x330E
#define X1    0xABCD
#define X2    0x1234
#define A0    0xE66D
#define A1    0xDEEC
#define A2    0x5
#define C    0xB
#define SET3(x, x0, x1, x2)    ((x)[0] = (x0), (x)[1] = (x1), (x)[2] = (x2))
#define SETLOW(x, y, n) SET3(x, LOW((y)[n]), LOW((y)[(n)+1]), LOW((y)[(n)+2]))
#define SEED(x0, x1, x2) (SET3(x, x0, x1, x2), SET3(a, A0, A1, A2), c = C)
#define REST(v)    for (i = 0; i < 3; i++) { xsubi[i] = x[i]; x[i] = temp[i]; } \
        return (v);
#define NEST(TYPE, f, F)    TYPE f(xsubi) register unsigned short *xsubi; { \
    register int i; register TYPE v; unsigned temp[3]; \
    for (i = 0; i < 3; i++) { temp[i] = x[i]; x[i] = LOW(xsubi[i]); }  \
    v = F(); REST(v); }
#define HI_BIT    (1L << (2 * N - 1))

static unsigned x[3] = {X0, X1, X2}, a[3] = {A0, A1, A2}, c = C;
static unsigned short lastx[3];

static void next();

#if HAVEFP

double
drand48() {
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
lrand48() {
    next();
    return (((long) x[2] << (N - 1)) + (x[1] >> 1));
}

long
mrand48() {
    register long l;

    next();
    /* sign-extend in case length of a long > 32 bits
                        (as on Honeywell) */
    // todo: warning: integer overflow in expression '-2147483648' of type 'long int' results in '-2147483648' [-Woverflow]
    return ((l = ((long) x[2] << N) + x[1]) & HI_BIT ? l | -HI_BIT : l);
}

static void
next() {
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


int malloc_size = 0;
int realloc_size = 0;

/*
 *  malloc safety checks:
 *
 *	Space for two extra ints is is allocated beyond what the client
 *	asks for.  The size of the malloc'd region is stored at the
 *	beginning, and a magic number is placed at the end.  realloc's
 *	and free's check the integrity of these markers.  This protects
 *	against overruns, makes sure that non-malloc'd memory isn't freed,
 *	and that memory isn't freed twice.
 *
 *  Wed Jul 10 13:43:41 1996 -- Scott Turner
 *
 *  Changed the guard to be a "char" since Sun architecture doesn't permit
 *  integers to begin on odd byte boundaries.
 *
 *  Mon Dec 21 11:53:17 1998 -- Scott Turner
 *
 *  Temporary change to use "smartnew".
 *
 */
void *
my_malloc(unsigned size) {
    char *p, *np;
    // extern void *malloc(uint);
    int i;

    size += sizeof(int);
    malloc_size += size;

    p = (char *) malloc(size + sizeof(char));

    if (p == NULL) {
        fprintf(stderr, "my_malloc: out of memory (can't malloc "
                        "%d bytes)\n", size);
        exit(1);
    }

    bzero(p, size);

    *((int *) p) = size;

    *(p + size) = 'X';

    return p + sizeof(int);
}


void *
my_realloc(void *ptr, unsigned size) {
    char *p = ptr;
#ifdef LINUX
    extern void *realloc(void *ptr, size_t size);
#else
    extern void *realloc(uint);
#endif
    // extern void *malloc(uint);

    if (p == NULL) {
        return my_malloc(size);
    }

    size += sizeof(int);
    realloc_size += size;
    p -= sizeof(int);

    assert(*((char *) (p + *(int *) p)) == 'X');

    p = (char *) realloc(p, size + sizeof(char));

    *((int *) p) = size;
    *(p + size) = 'X';

    if (p == NULL) {
        fprintf(stderr, "my_realloc: out of memory (can't realloc "
                        "%d bytes)\n", size);
        exit(1);
    }

    return p + sizeof(int);
}


void
my_free(void *ptr) {
    char *p = ptr;

    p -= sizeof(int);

    assert(*((char *) (p + *(int *) p)) == 'X');
    *((char *) (p + *(int *) p)) = 0;
    *((int *) p) = 0;

    free(p);
}


char *
str_save(char *s) {
    char *p;

    p = my_malloc(strlen(s) + 1);
    strcpy(p, s);

    return p;
}


void
asfail(char *file, int line, char *cond) {
    fprintf(stderr, "assertion failure: %s (%d): %s\n",
            file, line, cond);
    abort();
    exit(1);
}


void
lcase(s)
        char *s;
{

    while (*s) {
        *s = tolower(*s);
        s++;
    }
}


/*
 *  Line reader with no size limits
 *  strips newline off end of line
 */

#define    GETLIN_ALLOC    255

char *
getlin(FILE *fp) {
    static char *buf = NULL;
    static unsigned int size = 0;
    int len;
    int c;

    len = 0;

    while ((c = fgetc(fp)) != EOF) {
        if (len >= size) {
            size += GETLIN_ALLOC;
            buf = my_realloc(buf, size + 1);
        }

        if (c == '\n') {
            buf[len] = '\0';
            return buf;
        }

        buf[len++] = (char) c;
    }

    if (len == 0) {
        return NULL;
    }

    buf[len] = '\0';

    return buf;
}


char *
eat_leading_trailing_whitespace(char *s) {
    char *t;

    while (*s && iswhite(*s)) {
        s++;
    }            /* eat leading whitespace */

    for (t = s; *t; t++);

    t--;
    while (t >= s && iswhite(*t)) {                /* eat trailing whitespace */
        *t = '\0';
        t--;
    }

    return s;
}


/*
 *  Get line, remove leading and trailing whitespace
 */

char *
getlin_ew(FILE *fp) {
    char *line;
    char *p;

    line = getlin(fp);

    if (line) {
        while (*line && iswhite(*line)) {
            line++;
        }            /* eat leading whitespace */

        for (p = line; *p; p++)
            if (*p < 32 || *p == '\t')    /* remove ctrl chars */
                *p = ' ';
        p--;
        while (p >= line && iswhite(*p)) {                /* eat trailing whitespace */
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
readfile(char *path) {

    if (line_fd >= 0) {
        close(line_fd);
    }

    line_fd = open(path, 0);

    if (line_fd < 0) {
        fprintf(stderr, "can't open %s: ", path);
        perror("");
        return FALSE;
    }

    nread = read(line_fd, linebuf, MAX_BUF);
    point = linebuf;

    return TRUE;
}


char *
readlin() {
    static char *buf = NULL;
    static unsigned int size = 0;
    int len;
    int c;

    len = 0;

    while (1) {
        if (point >= &linebuf[nread]) {
            if (nread > 0) {
                nread = read(line_fd, linebuf, MAX_BUF);
            }

            if (nread < 1) {
                break;
            }

            point = linebuf;
        }

        c = *point++;

        if (len >= size) {
            size += GETLIN_ALLOC;
            buf = my_realloc(buf, size + 1);
        }

        if (c == '\n') {
            buf[len] = '\0';
            return buf;
        }

        buf[len++] = (char) c;
    }

    if (len == 0) {
        return NULL;
    }

    buf[len] = '\0';

    return buf;
}


char *
readlin_ew() {
    char *line;
    char *p;

    line = readlin();

    if (line) {
        while (*line && iswhite(*line)) {
            line++;
        }            /* eat leading whitespace */

        for (p = line; *p; p++)
            if (*p < 32 || *p == '\t')    /* remove ctrl chars */
                *p = ' ';
        p--;
        while (p >= line && iswhite(*p)) {                /* eat trailing whitespace */
            *p = '\0';
            p--;
        }
    }

    return line;
}


#define    COPY_LEN    1024

void
copy_fp(a, b)
        FILE *a;
        FILE *b;
{
    char buf[COPY_LEN];

    while (fgets(buf, COPY_LEN, a) != NULL) {
        fputs(buf, b);
    }
}


char lower_array[256];


void
init_lower() {
    int i;

    for (i = 0; i < 256; i++) {
        lower_array[i] = i;
    }

    for (i = 'A'; i <= 'Z'; i++) {
        lower_array[i] = i - 'A' + 'a';
    }
}


int
i_strcmp(char *s, char *t) {
    char a, b;

    do {
        a = tolower(*s);
        b = tolower(*t);
        s++;
        t++;
        if (a != b) {
            return a - b;
        }
    } while (a);

    return 0;
}


int
i_strncmp(char *s, char *t, int n) {
    char a, b;

    do {
        a = tolower(*s);
        b = tolower(*t);
        if (a != b) {
            return a - b;
        }
        s++;
        t++;
        n--;
    } while (a && n > 0);

    return 0;
}


static int
fuzzy_transpose(char *one, char *two, int l1, int l2) {
    int i;
    char buf[LEN];
    char tmp;

    if (l1 != l2) {
        return FALSE;
    }

    strcpy(buf, two);

    for (i = 0; i < l2 - 1; i++) {
        tmp = buf[i];
        buf[i] = buf[i + 1];
        buf[i + 1] = tmp;

        if (i_strcmp(one, buf) == 0) {
            return TRUE;
        }

        tmp = buf[i];
        buf[i] = buf[i + 1];
        buf[i + 1] = tmp;
    }

    return FALSE;
}


static int
fuzzy_one_less(char *one, char *two, int l1, int l2) {
    int count = 0;
    int i, j;

    if (l1 != l2 + 1) {
        return FALSE;
    }

    for (j = 0, i = 0; j < l2; i++, j++) {
        if (tolower(one[i]) != tolower(two[j])) {
            if (count++) {
                return FALSE;
            }
            j--;
        }
    }

    return TRUE;
}


static int
fuzzy_one_extra(char *one, char *two, int l1, int l2) {
    int count = 0;
    int i, j;

    if (l1 != l2 - 1) {
        return FALSE;
    }

    for (j = 0, i = 0; i < l1; i++, j++) {
        if (tolower(one[i]) != tolower(two[j])) {
            if (count++) {
                return FALSE;
            }
            i--;
        }
    }

    return TRUE;
}


static int
fuzzy_one_bad(char *one, char *two, int l1, int l2) {
    int count = 0;
    int i;

    if (l1 != l2) {
        return FALSE;
    }

    for (i = 0; i < l2; i++) {
        if (tolower(one[i]) != tolower(two[i]) && count++) {
            return FALSE;
        }
    }

    return TRUE;
}


int
fuzzy_strcmp(char *one, char *two) {
    int l1 = strlen(one);
    int l2 = strlen(two);

    if (l2 >= 4 && fuzzy_transpose(one, two, l1, l2)) {
        return TRUE;
    }

    if (l2 >= 5 && fuzzy_one_less(one, two, l1, l2)) {
        return TRUE;
    }

    if (l2 >= 5 && fuzzy_one_extra(one, two, l1, l2)) {
        return TRUE;
    }

    if (l2 >= 5 && fuzzy_one_bad(one, two, l1, l2)) {
        return TRUE;
    }

    return FALSE;
}


unsigned short seed[3];

void init_random(void) {
    // mdhender: comment out for test building
    //    long l;
    //    srandom(l);
}

