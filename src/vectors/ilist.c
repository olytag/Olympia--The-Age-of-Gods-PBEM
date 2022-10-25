// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.


#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include "ilist.h"
#include "random/random.h"


#define ILIST_ALLOC 6


typedef struct tiles_vector {
    int capacity;
    int length;
    int list[1];
} tiles_vector_t;


// ilist_cap returns the capacity of the list.
int ilist_cap(ilist l) {
    if (l == 0) {
        return 0;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) l) - offsetof(tiles_vector_t, list));
    return base->capacity;
}


// ilist_len returns the length of the list.
int ilist_len(ilist l) {
    if (l == 0) {
        return 0;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) l) - offsetof(tiles_vector_t, list));
    return base->length;
}


ilist *ilist_add(ilist *l, int n) {
    if (ilist_lookup(*l, n) == -1) {
        return ilist_append(l, n);
    }
    return l;
};


ilist *ilist_append(ilist *l, int n) {
    tiles_vector_t *base;
    if (*l == 0) {
        base = calloc(1, sizeof(tiles_vector_t) + ILIST_ALLOC * sizeof(int));
        assert(base != 0);
        base->capacity = ILIST_ALLOC;
        *l = base->list;
    }
    base = (tiles_vector_t *) (((char *) (*l)) - offsetof(tiles_vector_t, list));
    if (base->length + 2 >= base->capacity) {
        base->capacity *= 2;
        base = realloc(base, sizeof(tiles_vector_t) + base->capacity * sizeof(int));
        assert(base != 0);
        *l = base->list;
    }

    base->list[base->length] = n;
    base->length = base->length + 1;

    return l;
}


ilist *ilist_clear(ilist *l) {
    if (l == 0 || *l == 0) {
        return l;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) (*l)) - offsetof(tiles_vector_t, list));
    base->length = 0;
    for (int i = 0; i < base->capacity; i++) {
        base->list[i] = 0;
    }

    return l;
}


ilist ilist_copy(ilist l) {
    if (l == 0) {
        return 0;
    }

    tiles_vector_t *base = (tiles_vector_t *) (((char *) l) - offsetof(tiles_vector_t, list));

    tiles_vector_t *cp = calloc(1, sizeof(tiles_vector_t) + base->capacity * sizeof(int));
    assert(cp != 0);

    cp->capacity = base->capacity;
    cp->length = base->length;
    for (int i = 0; i < base->length; i++) {
        cp->list[i] = base->list[i];
    }

    return cp->list;
}


ilist *ilist_delete(ilist *l, int index) {
    if (l == 0 || *l == 0) {
        return l;
    }

    tiles_vector_t *base = (tiles_vector_t *) (((char *) (*l)) - offsetof(tiles_vector_t, list));
    assert(0 <= index && index < base->length);        /* bounds check */

    for (int i = index; i < base->length; i++) {
        base->list[i] = base->list[i + 1];
    }
    base->list[base->length] = 0;
    base->length = base->length - 1;

    return l;
}


int ilist_lookup(ilist l, int n) {
    if (l == 0) {
        return -1;
    }

    tiles_vector_t *base = (tiles_vector_t *) (((char *) l) - offsetof(tiles_vector_t, list));

    for (int i = 0; i < base->length; ++i) {
        if (base->list[i] == n) {
            return i;
        }
    }

    return -1 * base->length;
}


ilist *ilist_prepend(ilist *l, int n) {
    tiles_vector_t *base;
    if (*l == 0) {
        base = calloc(1, sizeof(tiles_vector_t) + ILIST_ALLOC * sizeof(int));
        assert(base != 0);
        base->capacity = ILIST_ALLOC;
        *l = base->list;
    }
    base = (tiles_vector_t *) (((char *) (*l)) - offsetof(tiles_vector_t, list));
    if (base->length + 2 >= base->capacity) {
        base->capacity *= 2;
        base = realloc(base, sizeof(tiles_vector_t) + base->capacity * sizeof(int));
        assert(base != 0);
        *l = base->list;
    }

    base->length = base->length + 1;
    for (int*p = base->list + base->length; p != base->list; p--) {
        *p = *(p-1);
    }
    base->list[0] = n;

    return l;
}


void ilist_reclaim(ilist *l) {
    if (l == 0 || *l == 0) {
        return;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) (*l)) - offsetof(tiles_vector_t, list));
    *l = 0;
    free(base);
}


ilist *ilist_rem_value(ilist *l, int n) {
    if (l == 0 || *l == 0) {
        return l;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) (*l)) - offsetof(tiles_vector_t, list));
    for (int i = base->length - 1; i >= 0; i--) {
        if (base->list[i] == n) {
            ilist_delete(l, i);
        }
    }
    return l;
}


ilist *ilist_rem_value_uniq(ilist *l, int n) {
    if (l == 0 || *l == 0) {
        return l;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) (*l)) - offsetof(tiles_vector_t, list));
    for (int i = base->length - 1; i >= 0; i--) {
        if (base->list[i] == n) {
            return ilist_delete(l, i);
        }
    }
    return l;
}


// Knuth, The Art of Computer Programming, Vol. 2 (Addison Wesley).
ilist ilist_scramble(ilist l) {
    if (l == 0) {
        return l;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) l) - offsetof(tiles_vector_t, list));
    for (int i = 0; i < base->length-1; i++) {
        int r = rnd(i, base->length-1);
        if (r != i) {
            int tmp = base->list[i];
            base->list[i] = base->list[r];
            base->list[r] = tmp;
        }
    }
    return l;
}
