// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include "vlist.h"
#include "random/random.h"


#define VLIST_ALLOC 6


typedef struct void_vector {
    int capacity;
    int length;
    void *list[1];
} void_vector_t;


// vlist_cap returns the capacity of the list.
int vlist_cap(vlist l) {
    if (l == 0) {
        return 0;
    }
    void_vector_t *base = (void_vector_t *) (((char *) l) - offsetof(void_vector_t, list));
    return base->capacity;
}


// vlist_len returns the length of the list.
int vlist_len(vlist l) {
    if (l == 0) {
        return 0;
    }
    void_vector_t *base = (void_vector_t *) (((char *) l) - offsetof(void_vector_t, list));
    return base->length;
}


vlist *vlist_add(vlist *l, void *p) {
    if (vlist_lookup(*l, p) == -1) {
        return vlist_append(l, p);
    }
    return l;
};


vlist *vlist_append(vlist *l, void *p) {
    void_vector_t *base;
    if (*l == 0) {
        base = calloc(1, sizeof(void_vector_t) + VLIST_ALLOC * sizeof(void *));
        assert(base != 0);
        base->capacity = VLIST_ALLOC;
        *l = base->list;
    }
    base = (void_vector_t *) (((char *) (*l)) - offsetof(void_vector_t, list));
    if (base->length + 2 >= base->capacity) {
        base->capacity *= 2;
        base = realloc(base, sizeof(void_vector_t) + base->capacity * sizeof(void *));
        assert(base != 0);
        *l = base->list;
    }

    base->list[base->length] = p;
    base->length = base->length + 1;

    return l;
}


vlist *vlist_clear(vlist *l) {
    if (l == 0 || *l == 0) {
        return l;
    }
    void_vector_t *base = (void_vector_t *) (((char *) (*l)) - offsetof(void_vector_t, list));
    base->length = 0;
    for (int i = 0; i < base->capacity; i++) {
        base->list[i] = 0;
    }

    return l;
}


vlist vlist_copy(vlist l) {
    if (l == 0) {
        return 0;
    }

    void_vector_t *base = (void_vector_t *) (((char *) l) - offsetof(void_vector_t, list));

    void_vector_t *cp = calloc(1, sizeof(void_vector_t) + base->capacity * sizeof(void *));
    assert(cp != 0);

    cp->capacity = base->capacity;
    cp->length = base->length;
    for (int i = 0; i < base->length; i++) {
        cp->list[i] = base->list[i];
    }

    return cp->list;
}


vlist *vlist_delete(vlist *l, int index) {
    if (l == 0 || *l == 0) {
        return l;
    }

    void_vector_t *base = (void_vector_t *) (((char *) (*l)) - offsetof(void_vector_t, list));
    assert(0 <= index && index < base->length);        /* bounds check */

    for (int i = index; i < base->length; i++) {
        base->list[i] = base->list[i + 1];
    }
    base->list[base->length] = 0;
    base->length = base->length - 1;

    return l;
}


int vlist_lookup(vlist l, void *p) {
    if (l == 0) {
        return -1;
    }

    void_vector_t *base = (void_vector_t *) (((char *) l) - offsetof(void_vector_t, list));

    for (int i = 0; i < base->length; ++i) {
        if (base->list[i] == p) {
            return i;
        }
    }

    return -1;
}


vlist *vlist_prepend(vlist *l, void *p) {
    void_vector_t *base;
    if (*l == 0) {
        base = calloc(1, sizeof(void_vector_t) + VLIST_ALLOC * sizeof(void *));
        assert(base != 0);
        base->capacity = VLIST_ALLOC;
        *l = base->list;
    }
    base = (void_vector_t *) (((char *) (*l)) - offsetof(void_vector_t, list));
    if (base->length + 2 >= base->capacity) {
        base->capacity *= 2;
        base = realloc(base, sizeof(void_vector_t) + base->capacity * sizeof(void *));
        assert(base != 0);
        *l = base->list;
    }

    base->length = base->length + 1;
    for (void **bp = base->list + base->length; bp != base->list; bp--) {
        *bp = *(bp - 1);
    }
    base->list[0] = p;

    return l;
}


void vlist_reclaim(vlist *l) {
    if (l == 0 || *l == 0) {
        return;
    }
    void_vector_t *base = (void_vector_t *) (((char *) (*l)) - offsetof(void_vector_t, list));
    *l = 0;
    free(base);
}


vlist *vlist_rem_value(vlist *l, void *p) {
    if (l == 0 || *l == 0) {
        return l;
    }
    void_vector_t *base = (void_vector_t *) (((char *) (*l)) - offsetof(void_vector_t, list));
    for (int i = base->length - 1; i >= 0; i--) {
        if (base->list[i] == p) {
            vlist_delete(l, i);
        }
    }
    return l;
}


vlist *vlist_rem_value_uniq(vlist *l, void *p) {
    if (l == 0 || *l == 0) {
        return l;
    }
    void_vector_t *base = (void_vector_t *) (((char *) (*l)) - offsetof(void_vector_t, list));
    for (int i = base->length - 1; i >= 0; i--) {
        if (base->list[i] == p) {
            return vlist_delete(l, i);
        }
    }
    return l;
}


// Knuth, The Art of Computer Programming, Vol. 2 (Addison Wesley).
vlist vlist_scramble(vlist l) {
    if (l == 0) {
        return l;
    }
    void_vector_t *base = (void_vector_t *) (((char *) l) - offsetof(void_vector_t, list));
    for (int i = 0; i < base->length-1; i++) {
        int r = rnd(i, base->length-1);
        if (r != i) {
            void *tmp = base->list[i];
            base->list[i] = base->list[r];
            base->list[r] = tmp;
        }
    }
    return l;
}
