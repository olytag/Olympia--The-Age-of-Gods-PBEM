// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include "random.h"
#include "sfc32.h"

// rnd replaces the rnd() from z.c.
// it returns a psuedo-random value in the range of low...high
int rnd(int low, int high) {
    static sfc32 *state = 0;
    if (state == 0) {
        state = sfc32_init(0, 12345, 0, 1);
    }
    int n = (int)(sfc32_next(state));
    if (n < 0) {
        n = -n;
    }
    n = n % (high - low + 1) + low;
    //printf("rnd(%d,%d) %d\n", low, high, n);
    return n;
}
