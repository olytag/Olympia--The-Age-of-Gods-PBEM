// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "rlist.h"
#include "mapgen/road.h"


int main(void) {
    // set stdout to flush like stderr
    setbuf(stdout, NULL);

    int errors = 0;

    rlist x;
    memset(&x, 0, sizeof(x));
    if (0 != rlist_len(x)) {
        printf("rlist len: expected %d, got %d\n", 0, rlist_len(x));
        errors++;
    }
    if (errors != 0) {
        printf("rlist: test 1 failed\n");
        return 2;
    }

    road_t *input2[100];
    for (int i = 0; i < 100; i++) {
        input2[i] = calloc(1, sizeof(road_t));
        assert(input2[i] != 0);
    }
    road_t *expectedValue2[100];
    for (int i = 0; i < 100; i++) {
        expectedValue2[i] = input2[i];
        assert(expectedValue2[i] = input2[i]);
    }
    for (int i = 0; i < 100; i++) {
        rlist_append(&x, input2[i]);
    }
    if (100 != rlist_len(x)) {
        printf("rlist: len: expected %d, got %d\n", 100, rlist_len(x));
        errors++;
    } else {
        for (int i = 0; i < 100; i++) {
            road_t *got = x[i];
            if (input2[i] != got) {
                printf("rlist: %3d: expected %p, got %p\n", i, input2[i], got);
                errors++;
            }
        }
    }
    if (errors != 0) {
        printf("rlist: test 2 failed\n");
        return 2;
    }

    road_t *input3[100];
    int expectedLength3[100];
    road_t *expectedValue3[100];
    for (int i = 0; i < 100; i++) {
        input3[i] = calloc(1, sizeof(road_t));
        assert(input3[i] != 0);
        expectedLength3[i] = 101 + i;
        expectedValue3[i] = input3[i];
        assert(expectedValue3[i] = input3[i]);
    }
    for (int i = 0; i < 100; i++) {
        rlist_append(&x, input3[i]);
        int gotLength = rlist_len(x);
        if (gotLength != expectedLength3[i]) {
            printf("rlist: append: %3d: expected length %3d, got %3d\n", 101+i, expectedLength3[i], gotLength);
            errors++;
        }
    }
    if (errors == 0) {
        road_t *gotValue = x[0];
        if (expectedValue2[0] != gotValue) {
            printf("rlist: append: %3d: expected value %p, got %p\n", 0, expectedValue2[0], gotValue);
            errors++;
        }
    }
    if (errors == 0) {
        for (int i = 0; i < 100; i++) {
            road_t *gotValue = x[100+i];
            if (expectedValue3[i] != gotValue) {
                printf("rlist: %3d: append: expected value %p, got %p\n", 100+i, expectedValue3[i], gotValue);
                errors++;
            }
        }
    }
    if (errors != 0) {
        printf("rlist: test 3 failed\n");
        return 2;
    }

    printf("rlist: passed\n");
    return 0;
}
