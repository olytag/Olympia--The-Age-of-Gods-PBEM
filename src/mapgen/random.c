// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <stdio.h>
#include "random.h"
#include "prng.h"

static sfc32 *state = 0;
static sfc32 privateState;
static uint32_t defaultSeed[4] = {0, 12345, 0, 1};


// rnd replaces the rnd() from z.c.
// it returns a psuedo-random value in the range of low...high
int rnd(int low, int high) {
    if (state == 0) {
        state = &privateState;
        state = sfc32_seed(state, defaultSeed[0], defaultSeed[1], defaultSeed[2], defaultSeed[3]);
    }
    int n = (int)(sfc32_next(state));
    if (n < 0) {
        n = -n;
    }
    n = n % (high - low + 1) + low;
    //printf("rnd(%d,%d) %d\n", low, high, n);
    return n;
}


void load_seed(char *seedFileName) {
    if (state == 0) {
        state = &privateState;
        state = sfc32_seed(state, defaultSeed[0], defaultSeed[1], defaultSeed[2], defaultSeed[3]);
    }
    FILE *fp = fopen(seedFileName, "rb");
    if (fp == 0) { // silently ignore errors!
        return;
    }
    fread(state, sizeof(sfc32), 1, fp);
    fclose(fp);
}


void save_seed(char *seedFileName) {
    if (state == 0) {
        state = &privateState;
        state = sfc32_seed(state, defaultSeed[0], defaultSeed[1], defaultSeed[2], defaultSeed[3]);
    }
    FILE *fp = fopen(seedFileName, "wb");
    if (fp == 0) { // silently ignore errors!
        return;
    }
    fwrite(state, sizeof(sfc32), 1, fp);
    fclose(fp);
}




