// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <stdio.h>
#include "prng.h"
#include "sfc32_test.h"

int test_prng(void) {
    // set stdout to flush like stderr
    setbuf(stdout, NULL);

    int errors = 0;

    sfc32 *s = sfc32_init(0, 12345, 0, 1);
    uint32_t expect1[10] = {
            235160590, 2967261163, 116171463, 2882324903,
            362604721, 4227106926, 1933307004, 1608300071,
            2256615412, 2701957640,
    };
    for (int i = 0; i < 10; i++) {
        uint32_t got = sfc32_next(s);
        if (got != expect1[i]) {
            printf("sfc32: fail: expect %10u: got %10u\n", expect1[i], got);
            errors++;
        }
    }
    if (errors != 0) {
        printf("test_random: test 1 failed\n");
        return 2;
    }

    return 0;
}
