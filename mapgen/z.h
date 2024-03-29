// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_Z_H
#define OLYTAG_Z_H

#include <stdio.h>

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
extern long i_strncmp(char *s, char *t, long n);
extern long i_strcmp(char *s, char *t);
extern long fuzzy_strcmp(char *, char *);


///*
// *  Assertion verifier
// */
//
//extern void asfail(char *file, long line, char *cond);
//
//#ifdef __STDC__
//#define	assert(p)	if(!(p)) asfail(__FILE__, __LINE__, #p);
//#else
//#define	assert(p)	if(!(p)) asfail(__FILE__, __LINE__, "p");
//#endif


extern long readfile(char *path);
extern char *readlin();
extern char *readlin_ew();

#endif //OLYTAG_Z_H
