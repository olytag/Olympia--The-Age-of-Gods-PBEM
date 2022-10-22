// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

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

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "memory.h"


int my_malloc_size = 0;
int my_realloc_size = 0;


void *my_malloc(unsigned size) {
    char *p, *np;
    int i;

    size += sizeof(int);
    my_malloc_size += size;

    p = (char *) malloc(size + sizeof(char));

    if (p == NULL) {
        fprintf(stderr, "my_malloc: out of memory (can't malloc %d bytes)\n", size);
        exit(1);
    }

    memset(p, 0, size);

    *((int *) p) = size;

    *(p + size) = 'X';

    return p + sizeof(int);
}


void *my_realloc(void *ptr, unsigned size) {
    char *p = ptr;

    if (p == NULL) {
        return my_malloc(size);
    }

    size += sizeof(int);
    my_realloc_size += size;
    p -= sizeof(int);

    if (*((char *) (p + *(int *) p)) != 'X') {
        assert(*((char *) (p + *(int *) p)) == 'X');
        /* NOT REACHED */
        exit(2);
    }

    p = (char *) realloc(p, size + sizeof(char));

    *((int *) p) = size;
    *(p + size) = 'X';

    if (p == NULL) {
        fprintf(stderr, "my_realloc: out of memory (can't realloc %d bytes)\n", size);
        exit(1);
    }

    return p + sizeof(int);
}


void my_free(void *ptr) {
    char *p = ptr;

    p -= sizeof(int);

    assert(*((char *) (p + *(int *) p)) == 'X');
    *((char *) (p + *(int *) p)) = 0;
    *((int *) p) = 0;

    free(p);
}
