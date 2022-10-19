// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

// a collection of reallocation lists
// length is stored in list[0], maximum in list[1].
// the user-visible list entries are shifted to &list[2], so that iterations can start from index 0.

#include <malloc.h>
#include <assert.h>
#include <strings.h>
#include "lists.h"

#define ILIST_ALLOC 6
#define ROAD_LIST_ALLOC 6
#define TILES_LIST_ALLOC 6

// ilist_len returns the current length of the list.
int ilist_len(ilist l) {
    if (l == 0) {
        return 0;
    }

    int *base = l - 2;
    assert(&base[2] == l);

    int *header = (int *) base;
    return header[0];
}


// ilist_append appends the value to the list, growing it if needed.
void ilist_append(ilist *l, int n) {
    int *base;
    if (*l == 0) {
        base = calloc(ILIST_ALLOC, sizeof(int));
        assert(base != 0);

        int *header = (int *) base;
        header[0] = 0;
        header[1] = ILIST_ALLOC;
        *l = &base[2];
    } else {
        base = (*l) - 2;
        assert(&base[2] == *l);

        int *header = (int *) base;
        if (header[0] + 2 >= header[1]) {
            header[1] *= 2;
            base = realloc(base, header[1] * sizeof(int));
            assert(base != 0);
            *l = &base[2];
        }
    }
    assert(&base[2] == *l);

    int *header = (int *) base;
    base[header[0] + 2] = n;
    header[0]++;
}


void ilist_prepend(ilist *l, int n) {
    int *base;
    if (*l == 0) {
        base = calloc(ILIST_ALLOC, sizeof(int));
        assert(base != 0);

        int *header = (int *) base;
        header[0] = 0;
        header[1] = ILIST_ALLOC;
        *l = &base[2];
    } else {
        base = (*l) - 2;
        assert(&base[2] == *l);

        int *header = (int *) base;
        if (header[0] + 2 >= header[1]) {
            header[1] *= 2;
            base = realloc(base, header[1] * sizeof(int));
            assert(base != 0);
            *l = &base[2];
        }
    }
    assert(&base[2] == *l);

    int *header = (int *) base;
    header[0]++;
    for (int i = header[0] + 1; i > 2; i--) {
        base[i] = base[i - 1];
    }
    base[2] = n;
}


// ilist_delete removes the nth entry from the list by shifting the remaining entries left.
void ilist_delete(ilist *l, int index) {
    assert(index >= 0 && index < ilist_len(*l)); /* bounds check */

    int *base = (*l) - 2;
    assert(&base[2] == *l);

    int *header = (int *) base;
    for (int j = index + 2; j <= header[0]; j++) {
        base[j] = base[j + 1];
    }
    header[0]--;
}


// ilist_clear clears out the list without freeing any memory.
void ilist_clear(ilist *l) {
    if (*l != 0) {
        int *base = (*l) - 2;
        assert(&base[2] == *l);

        int *header = (int *) base;
        header[0] = 0;
    }
}


void ilist_reclaim(ilist *l) {
    if (*l != 0) {
        int *base = (*l) - 2;
        free(base);
    }
    *l = 0;
}


// ilist_lookup returns the index of the first entry in the list that matches n,
// or -1 if it can't find a match.
int ilist_lookup(ilist l, int n) {
    if (l == 0) {
        return -1;
    }

    int *base = l - 2;
    assert(&base[2] == l);

    int *header = (int *) base;
    int end = header[0];
    for (int i = 0; i < end; ++i) {
        if (*(l++) == n) {
            return i;
        }
    }
    return -1;
}


/*
 *  Mon Apr  3 11:47:28 2000 -- Scott Turner
 *
 *  Add w/o duplication.
 *
 */
void ilist_add(ilist *l, int n) {
    if (ilist_lookup(*l, n) == -1) {
        ilist_append(l, n);
    }
};


void ilist_rem_value(ilist *l, int n) {
    for (int i = ilist_len(*l) - 1; i >= 0; i--) {
        if ((*l)[i] == n) {
            ilist_delete(l, i);
        }
    }
}


void ilist_rem_value_uniq(ilist *l, int n) {
    for (int i = ilist_len(*l) - 1; i >= 0; i--) {
        if ((*l)[i] == n) {
            ilist_delete(l, i);
            return;
        }
    }
}


// ilist_copy returns a copy of the list.
ilist ilist_copy(ilist l) {
    if (l == 0) {
        return 0;
    }

    int *base = l - 2;
    assert(&base[2] == l);

    int *header = (int *) base;
    int *copy_base = calloc(header[1], sizeof(int));
    assert(copy_base != 0);

    memcpy(copy_base, base, header[1] * sizeof(int));

    return &copy_base[2];
}


/*
 *  Knuth, The Art of Computer Programming, Vol. 2 (Addison Wesley).
 *  Essentially, to shuffle A[1]...A[N]:
 *  1) put I = 1;
 *  2) generate a random number R in the range I..N;
 *  3) if R is not I, swap A[R] and A[I];
 *  4) I <- I+1;
 *  5) if I is less than N go to step 2
 */
void ilist_scramble(ilist l) {
    extern long rnd(long low, long high);

    int len = ilist_len(l) - 1;
    for (int i = 0; i < len; i++) {
        int r = rnd(i, len);
        if (r != 1) {
            int tmp = l[i];
            l[i] = l[r];
            l[r] = tmp;
        }
    }
}


// road_list_len returns the length of the list.
int road_list_len(road_list l) {
    if (l == 0) {
        return 0;
    }

    struct road **base = l - 2;
    assert(&base[2] == l);

    int *header = (int *) base;
    return header[0];
}


// road_list_append appends the value to the list, growing it if needed.
void road_list_append(road_list *l, struct road *n) {
    struct road **base;
    if (*l == 0) {
        base = calloc(ROAD_LIST_ALLOC, sizeof(struct road *));
        assert(base != 0);

        int *header = (int *) base;
        header[0] = 0;
        header[1] = ROAD_LIST_ALLOC;
        *l = &base[2];
    } else {
        base = (*l) - 2;
        assert(&base[2] == *l);

        int *header = (int *) base;
        if (header[0] + 2 >= header[1]) {
            header[1] *= 2;
            base = realloc(base, header[1] * sizeof(struct road *));
            assert(base != 0);
            *l = &base[2];
        }
    }
    assert(&base[2] == *l);

    int *header = (int *) base;
    base[header[0] + 2] = n;
    header[0]++;
}


// tiles_list_len returns the length of the list.
int tiles_list_len(tiles_list l) {
    if (l == 0) {
        return 0;
    }

    struct tile **base = l - 2;
    assert(&base[2] == l);

    int *header = (int *) base;
    return header[0];
}


// tiles_list_append appends the value to the list, growing it if needed.
void tiles_list_append(tiles_list *l, struct tile *n) {
    struct tile **base;
    if (*l == 0) {
        base = calloc(TILES_LIST_ALLOC, sizeof(struct tile *));
        assert(base != 0);

        int *header = (int *) base;
        header[0] = 0;
        header[1] = TILES_LIST_ALLOC;
        *l = &base[2];
    } else {
        base = (*l) - 2;
        assert(&base[2] == *l);

        int *header = (int *) base;
        if (header[0] + 2 >= header[1]) {
            header[1] *= 2;
            base = realloc(base, header[1] * sizeof(struct tile *));
            assert(base != 0);
            *l = &base[2];
        }
    }
    assert(&base[2] == *l);

    int *header = (int *) base;
    base[header[0] + 2] = n;
    header[0]++;
}


// tiles_list_copy returns a copy of the list.
tiles_list tiles_list_copy(tiles_list l) {
    if (l == 0) {
        return 0;
    }

    struct tile **base = l - 2;
    assert(&base[2] == l);

    int *header = (int *) base;
    struct tile **copy_base = calloc(header[1], sizeof(struct tile *));
    assert(copy_base != 0);

    memcpy(copy_base, base, header[1] * sizeof(struct tile *));

    return &copy_base[2];
}


void tiles_list_reclaim(tiles_list *l) {
    if (*l != 0) {
        struct tile **base = (*l) - 2;
        free(base);
    }
    *l = 0;
}


void tiles_list_scramble(tiles_list l) {
    extern long rnd(long low, long high);

    int len = tiles_list_len(l) - 1;
    for (int i = 0; i < len; i++) {
        int r = rnd(i, len);
        if (r != 1) {
            struct tile *tmp = l[i];
            l[i] = l[r];
            l[r] = tmp;
        }
    }
}
