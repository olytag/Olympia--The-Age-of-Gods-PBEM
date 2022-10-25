// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include "rlist.h"
#include "random/random.h"


#define RLIST_ALLOC 6


typedef struct roads_vector {
    int capacity;
    int length;
    road_t *list[1];
} roads_vector_t;


// rlist_cap returns the capacity of the list.
int rlist_cap(rlist l) {
    if (l == 0) {
        return 0;
    }
    roads_vector_t *base = (roads_vector_t *) (((char *) l) - offsetof(roads_vector_t, list));
    return base->capacity;
}


// rlist_len returns the length of the list.
int rlist_len(rlist l) {
    if (l == 0) {
        return 0;
    }
    roads_vector_t *base = (roads_vector_t *) (((char *) l) - offsetof(roads_vector_t, list));
    return base->length;
}


rlist *rlist_add(rlist *l, road_t *t) {
    if (rlist_lookup(*l, t) == -1) {
        return rlist_append(l, t);
    }
    return l;
};


rlist *rlist_append(rlist *l, road_t *t) {
    roads_vector_t *base;
    if (*l == 0) {
        base = calloc(1, sizeof(roads_vector_t) + RLIST_ALLOC * sizeof(road_t *));
        assert(base != 0);
        base->capacity = RLIST_ALLOC;
        *l = base->list;
    }
    base = (roads_vector_t *) (((char *) (*l)) - offsetof(roads_vector_t, list));
    if (base->length + 2 >= base->capacity) {
        base->capacity *= 2;
        base = realloc(base, sizeof(roads_vector_t) + base->capacity * sizeof(road_t *));
        assert(base != 0);
        *l = base->list;
    }

    base->list[base->length] = t;
    base->length = base->length + 1;

    return l;
}


rlist *rlist_clear(rlist *l) {
    if (l == 0 || *l == 0) {
        return l;
    }
    roads_vector_t *base = (roads_vector_t *) (((char *) (*l)) - offsetof(roads_vector_t, list));
    base->length = 0;
    for (int i = 0; i < base->capacity; i++) {
        base->list[i] = 0;
    }

    return l;
}


rlist rlist_copy(rlist l) {
    if (l == 0) {
        return 0;
    }

    roads_vector_t *base = (roads_vector_t *) (((char *) l) - offsetof(roads_vector_t, list));

    roads_vector_t *cp = calloc(1, sizeof(roads_vector_t) + base->capacity * sizeof(road_t *));
    assert(cp != 0);

    cp->capacity = base->capacity;
    cp->length = base->length;
    for (int i = 0; i < base->length; i++) {
        cp->list[i] = base->list[i];
    }

    return cp->list;
}


rlist *rlist_delete(rlist *l, int index) {
    if (l == 0 || *l == 0) {
        return l;
    }

    roads_vector_t *base = (roads_vector_t *) (((char *) (*l)) - offsetof(roads_vector_t, list));
    assert(0 <= index && index < base->length);        /* bounds check */

    for (int i = index; i < base->length; i++) {
        base->list[i] = base->list[i + 1];
    }
    base->list[base->length] = 0;
    base->length = base->length - 1;

    return l;
}


int rlist_lookup(rlist l, road_t *t) {
    if (l == 0) {
        return -1;
    }

    roads_vector_t *base = (roads_vector_t *) (((char *) l) - offsetof(roads_vector_t, list));

    for (int i = 0; i < base->length; ++i) {
        if (base->list[i] == t) {
            return i;
        }
    }

    return -1 * base->length;
}


rlist *rlist_prepend(rlist *l, road_t *t) {
    roads_vector_t *base;
    if (*l == 0) {
        base = calloc(1, sizeof(roads_vector_t) + RLIST_ALLOC * sizeof(road_t *));
        assert(base != 0);
        base->capacity = RLIST_ALLOC;
        *l = base->list;
    }
    base = (roads_vector_t *) (((char *) (*l)) - offsetof(roads_vector_t, list));
    if (base->length + 2 >= base->capacity) {
        base->capacity *= 2;
        base = realloc(base, sizeof(roads_vector_t) + base->capacity * sizeof(road_t *));
        assert(base != 0);
        *l = base->list;
    }

    base->length = base->length + 1;
    for (road_t **p = base->list + base->length; p != base->list; p--) {
        *p = *(p-1);
    }
    base->list[0] = t;

    return l;
}


void rlist_reclaim(rlist *l) {
    if (l == 0 || *l == 0) {
        return;
    }
    roads_vector_t *base = (roads_vector_t *) (((char *) (*l)) - offsetof(roads_vector_t, list));
    *l = 0;
    free(base);
}


rlist *rlist_rem_value(rlist *l, road_t *t) {
    if (l == 0 || *l == 0) {
        return l;
    }
    roads_vector_t *base = (roads_vector_t *) (((char *) (*l)) - offsetof(roads_vector_t, list));
    for (int i = base->length - 1; i >= 0; i--) {
        if (base->list[i] == t) {
            rlist_delete(l, i);
        }
    }
    return l;
}


rlist *rlist_rem_value_uniq(rlist *l, road_t *t) {
    if (l == 0 || *l == 0) {
        return l;
    }
    roads_vector_t *base = (roads_vector_t *) (((char *) (*l)) - offsetof(roads_vector_t, list));
    for (int i = base->length - 1; i >= 0; i--) {
        if (base->list[i] == t) {
            return rlist_delete(l, i);
        }
    }
    return l;
}


// Knuth, The Art of Computer Programming, Vol. 2 (Addison Wesley).
rlist rlist_scramble(rlist l) {
    if (l == 0) {
        return l;
    }
    roads_vector_t *base = (roads_vector_t *) (((char *) l) - offsetof(roads_vector_t, list));
    for (int i = 0; i < base->length; i++) {
        int r = rnd(i, base->length);
        if (r != i) {
            road_t *tmp = base->list[i];
            base->list[i] = base->list[r];
            base->list[r] = tmp;
        }
    }
    return l;
}
