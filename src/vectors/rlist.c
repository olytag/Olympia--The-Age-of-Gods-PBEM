// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stddef.h>
#include "rlist.h"


#define ROAD_LIST_ALLOC 6


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


// roads_list__append appends the value to the list, growing it if needed.
rlist *rlist_append(rlist *l, road_t *r) {
    roads_vector_t *base;
    if (*l == 0) {
        base = calloc(1, sizeof(roads_vector_t) + ROAD_LIST_ALLOC * sizeof(road_t *));
        assert(base != 0);
        base->capacity = ROAD_LIST_ALLOC + 1;
        *l = base->list;
    }
    base = (roads_vector_t *) (((char *) (*l)) - offsetof(roads_vector_t, list));
    if (base->length + 2 >= base->capacity) {
        base->capacity *= 2;
        base = realloc(base, sizeof(roads_vector_t) + base->capacity * sizeof(road_t *));
        assert(base != 0);
        *l = base->list;
    }

    base->list[base->length] = r;
    base->length = base->length + 1;

    return l;
}
