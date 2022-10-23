// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include "tlist.h"
#include "random/random.h"


#define TLIST_ALLOC 6


typedef struct tiles_vector {
    int capacity;
    int length;
    tile_t *list[1];
} tiles_vector_t;


// tlist_cap returns the capacity of the list.
int tlist_cap(tlist l) {
    if (l == 0) {
        return 0;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) l) - offsetof(tiles_vector_t, list));
    return base->capacity;
}


// tlist_len returns the length of the list.
int tlist_len(tlist l) {
    if (l == 0) {
        return 0;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) l) - offsetof(tiles_vector_t, list));
    return base->length;
}


tlist *tlist_add(tlist *l, tile_t *t) {
    if (tlist_lookup(*l, t) == -1) {
        return tlist_append(l, t);
    }
    return l;
};


tlist *tlist_append(tlist *l, tile_t *t) {
    tiles_vector_t *base;
    if (*l == 0) {
        base = calloc(1, sizeof(tiles_vector_t) + TLIST_ALLOC * sizeof(tile_t *));
        assert(base != 0);
        base->capacity = TLIST_ALLOC;
        *l = base->list;
    }
    base = (tiles_vector_t *) (((char *) (*l)) - offsetof(tiles_vector_t, list));
    if (base->length + 2 >= base->capacity) {
        base->capacity *= 2;
        base = realloc(base, sizeof(tiles_vector_t) + base->capacity * sizeof(tile_t *));
        assert(base != 0);
        *l = base->list;
    }

    base->list[base->length] = t;
    base->length = base->length + 1;

    return l;
}


tlist *tlist_clear(tlist *l) {
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


tlist tlist_copy(tlist l) {
    if (l == 0) {
        return 0;
    }

    tiles_vector_t *base = (tiles_vector_t *) (((char *) l) - offsetof(tiles_vector_t, list));

    tiles_vector_t *cp = calloc(1, sizeof(tiles_vector_t) + base->capacity * sizeof(tile_t *));
    assert(cp != 0);

    cp->capacity = base->capacity;
    cp->length = base->length;
    for (int i = 0; i < base->length; i++) {
        cp->list[i] = base->list[i];
    }

    return cp->list;
}


tlist *tlist_delete(tlist *l, int index) {
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


int tlist_lookup(tlist l, tile_t *t) {
    if (l == 0) {
        return -1;
    }

    tiles_vector_t *base = (tiles_vector_t *) (((char *) l) - offsetof(tiles_vector_t, list));

    for (int i = 0; i < base->length; ++i) {
        if (base->list[i] == t) {
            return i;
        }
    }

    return -1 * base->length;
}


tlist *tlist_prepend(tlist *l, tile_t *t) {
    tiles_vector_t *base;
    if (*l == 0) {
        base = calloc(1, sizeof(tiles_vector_t) + TLIST_ALLOC * sizeof(tile_t *));
        assert(base != 0);
        base->capacity = TLIST_ALLOC;
        *l = base->list;
    }
    base = (tiles_vector_t *) (((char *) (*l)) - offsetof(tiles_vector_t, list));
    if (base->length + 2 >= base->capacity) {
        base->capacity *= 2;
        base = realloc(base, sizeof(tiles_vector_t) + base->capacity * sizeof(tile_t *));
        assert(base != 0);
        *l = base->list;
    }

    base->length = base->length + 1;
    for (tile_t **p = base->list + base->length; p != base->list; p--) {
        *p = *(p-1);
    }
    base->list[0] = t;

    return l;
}


void tlist_reclaim(tlist *l) {
    if (l == 0 || *l == 0) {
        return;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) (*l)) - offsetof(tiles_vector_t, list));
    *l = 0;
    free(base);
}


tlist *tlist_rem_value(tlist *l, tile_t *t) {
    if (l == 0 || *l == 0) {
        return l;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) (*l)) - offsetof(tiles_vector_t, list));
    for (int i = base->length - 1; i >= 0; i--) {
        if (base->list[i] == t) {
            tlist_delete(l, i);
        }
    }
    return l;
}


tlist *tlist_rem_value_uniq(tlist *l, tile_t *t) {
    if (l == 0 || *l == 0) {
        return l;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) (*l)) - offsetof(tiles_vector_t, list));
    for (int i = base->length - 1; i >= 0; i--) {
        if (base->list[i] == t) {
            return tlist_delete(l, i);
        }
    }
    return l;
}


// Knuth, The Art of Computer Programming, Vol. 2 (Addison Wesley).
tlist tlist_scramble(tlist l) {
    if (l == 0) {
        return l;
    }
    tiles_vector_t *base = (tiles_vector_t *) (((char *) l) - offsetof(tiles_vector_t, list));
    for (int i = 0; i < base->length-1; i++) {
        int r = rnd(i, base->length-1);
        if (r != i) {
            tile_t *tmp = base->list[i];
            base->list[i] = base->list[r];
            base->list[r] = tmp;
        }
    }
    return l;
}
