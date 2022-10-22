// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <assert.h>
#include <malloc.h>
#include "roads.h"


#define ROAD_LIST_ALLOC 6


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

