// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <stdio.h>
#include "prng.h"

int main(void) {
    sfc32 *s = sfc32_init(0, 12345, 0, 1);
    uint32_t expect[11] = {
            235160590,
            2967261163,
            116171463,
            2882324903,
            362604721,
            4227106926,
            1933307004,
            1608300071,
            2256615412,
            2701957640,
            0,
    };

    int errors = 0;
    for (int i = 0; expect[i] != 0; i++) {
        uint32_t got = sfc32_next(s);
        if (got != expect[i]) {
            printf("sfc32: fail: expect %10u: got %10u\n", expect[i], got);
            errors++;
        }
    }
    if (errors > 0) {
        printf("sfc32: test failed\n");
        return 2;
    }

    printf("sfc32: test passed\n");
    return 0;
}