// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include "prng.h"

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



// sfc32_seed seeds a PRNG.
sfc32 *sfc32_seed(sfc32 *state, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    if (state == 0) {
        state = malloc(sizeof(sfc32));
        if (state == 0) {
            fprintf(stderr, "sfc32: out of memory\n");
            exit(2);
        }
    }
    state->a = a;
    state->b = b;
    state->c = c;
    state->d = d;

    // source recommends running 12 iterations before using state
    for (int i = 0; i < 12; i++) {
        sfc32_next(state);
    }

    return state;
}


// sfc32_next returns the next value from the PRNG.
uint32_t sfc32_next(sfc32 *state) {
    state->a |= 0;
    state->b |= 0;
    state->c |= 0;
    state->d |= 0;
    uint32_t t = (state->a + state->b | 0) + state->d | 0;
    state->d = state->d + 1 | 0;
    state->a = state->b ^ state->b >> 9;
    state->b = state->c + (state->c << 3) | 0;
    state->c = state->c << 21 | state->c >> 11;
    state->c = state->c + t | 0;
    return (t >> 0);
}


// sfc32_init returns an initialized PRNG.
// it is equivalent to calling sfc32_seed(0, a, b, c, d).
// i don't know why i included it.
sfc32 *sfc32_init(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    return sfc32_seed(0, a, b, c, d);
}

