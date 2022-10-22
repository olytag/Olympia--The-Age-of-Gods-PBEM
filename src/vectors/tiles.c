// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <assert.h>
#include <malloc.h>
#include <string.h>
#include "tiles.h"


#define TILES_LIST_ALLOC 6


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
