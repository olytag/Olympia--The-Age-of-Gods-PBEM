// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "tlist.h"
#include "mapgen/tile.h"


int main(void) {
    // set stdout to flush like stderr
    setbuf(stdout, NULL);

    int errors = 0;

    tlist x;
    memset(&x, 0, sizeof(x));
    if (0 != tlist_len(x)) {
        printf("tlist len: expected %d, got %d\n", 0, tlist_len(x));
        errors++;
    }
    if (errors != 0) {
        printf("tlist: test 1 failed\n");
        return 2;
    }

    tile_t *input2[100];
    for (int i = 0; i < 100; i++) {
        input2[i] = calloc(1, sizeof(tile_t));
        assert(input2[i] != 0);
    }
    tile_t *expectedValue2[100];
    for (int i = 0; i < 100; i++) {
        expectedValue2[i] = input2[i];
        assert(expectedValue2[i] = input2[i]);
    }
    for (int i = 0; i < 100; i++) {
        tlist_append(&x, input2[i]);
    }
    if (100 != tlist_len(x)) {
        printf("tlist: len: expected %d, got %d\n", 100, tlist_len(x));
        errors++;
    } else {
        for (int i = 0; i < 100; i++) {
            tile_t *got = x[i];
            if (input2[i] != got) {
                printf("tlist: %3d: expected %p, got %p\n", i, input2[i], got);
                errors++;
            }
        }
    }
    if (errors != 0) {
        printf("tlist: test 2 failed\n");
        return 2;
    }

    tile_t *input3[100];
    int expectedLength3[100];
    tile_t *expectedValue3[100];
    for (int i = 0; i < 100; i++) {
        input3[i] = calloc(1, sizeof(tile_t));
        assert(input3[i] != 0);
        expectedLength3[i] = 101 + i;
        expectedValue3[i] = input3[i];
        assert(expectedValue3[i] = input3[i]);
    }
    for (int i = 0; i < 100; i++) {
        tlist_prepend(&x, input3[i]);
        int gotLength = tlist_len(x);
        if (gotLength != expectedLength3[i]) {
            printf("tlist: prepend: %3d: expected length %3d, got %3d\n", 101 + i, expectedLength3[i], gotLength);
            errors++;
        } else {
            tile_t *gotValue = x[0];
            if (expectedValue3[i] != gotValue) {
                printf("tlist: %3d: prepend: bol: expected value %p, got %p\n", 100 + i, expectedValue3[i], gotValue);
                errors++;
            } else {
                if (expectedValue2[99] != x[gotLength - 1]) {
                    printf("ilist: %3d: prepend: eol: expected value %p, got %p\n", i, expectedValue2[99], x[gotLength - 1]);
                    errors++;
                }
            }
        }
    }
    if (200 != tlist_len(x)) {
        printf("tlist: prepend: length: expected value %3d, got %3d\n", tlist_len(x), 200);
        errors++;
    }
    if (errors != 0) {
        printf("tlist: test 3 failed\n");
        return 2;
    }

    if (200 != tlist_len(x)) {
        printf("tlist: delete: pre: expected value %3d, got %3d\n", tlist_len(x), 200);
        errors++;
    }
    tlist_delete(&x, 100);
    if (199 != tlist_len(x)) {
        printf("tlist: delete: expected value %3d, got %3d\n", tlist_len(x), 199);
        errors++;
    }
    if (errors != 0) {
        printf("ilist: test 4 failed\n");
        return 2;
    }

    tile_t *input5[4] = {x[22], x[44], x[113], x[151]};
    int expected5[4] = {22, 44, 113, 151};
    for (int i = 0; i < 4; i++) {
        int got = tlist_lookup(x, input5[i]);
        if (expected5[i] != got) {
            printf("tlist: lookup: %3d: expected value %3d, got %3d\n", i, expected5[i], got);
            errors++;
        }
    }
    if (errors != 0) {
        printf("tlist: test 5 failed\n");
        return 2;
    }

    tlist y = tlist_copy(x);
    if (tlist_len(x) != tlist_len(y)) {
        printf("tlist: copy: expected length %3d, got %3d\n", tlist_len(x), tlist_len(y));
        errors++;
    } else {
        for (int i = 0; i < tlist_len(x); i++) {
            if (&x[i] == &y[i]) {
                printf("tlist: copy: expected address x[%3d] != address y[%3d]\n", i, i);
                errors++;
            } else if (x[i] != y[i]) {
                printf("tlist: copy: expected value %p, got %p\n", x[i], y[i]);
                errors++;
            }
        }
    }
    if (errors != 0) {
        printf("tlist: test 6 failed\n");
        return 2;
    }

    tlist_clear(&x);
    if (0 != tlist_len(x)) {
        printf("tlist: clear: x: expected length %3d, got %3d\n", 0, tlist_len(x));
        errors++;
    }
    if (199 != tlist_len(y)) {
        printf("tlist: clear: y: expected length %3d, got %3d\n", 199, tlist_len(y));
        errors++;
    }
    if (errors != 0) {
        printf("tlist: test 7 failed\n");
        return 2;
    }

    printf("tlist: passed\n");
    return 0;
}
