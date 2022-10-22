// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_RANDOM_SFC32_H
#define OLYTAG_RANDOM_SFC32_H

// mdhender: a simple random number generator for testing

#include <stdint.h>

// SFC32 is from https://simblob.blogspot.com/2022/05/upgrading-prng.html#more
//
// Example
//
//   sfc32 *s = sfc32_init(0, 12345, 0, 1);
//   printf("  %10u\n", sfc32_next(s)); //  235160590
//   printf("  %10u\n", sfc32_next(s)); // 2967261163
//   printf("  %10u\n", sfc32_next(s)); //  116171463
//   printf("  %10u\n", sfc32_next(s)); // 2882324903
//   printf("  %10u\n", sfc32_next(s)); //  362604721
//   printf("  %10u\n", sfc32_next(s)); // 4227106926
//   printf("  %10u\n", sfc32_next(s)); // 1933307004
//   printf("  %10u\n", sfc32_next(s)); // 1608300071
//   printf("  %10u\n", sfc32_next(s)); // 2256615412
//   printf("  %10u\n", sfc32_next(s)); // 2701957640

// sfc32_state holds the state for our PRNG.
typedef struct sfc32_state {
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
} sfc32;

// sfc32_seed seeds a PRNG.
sfc32 *sfc32_seed(sfc32 *state, uint32_t a, uint32_t b, uint32_t c, uint32_t d);

// sfc32_next returns the next value from the PRNG.
uint32_t sfc32_next(sfc32 *state);

// sfc32_init returns an initialized PRNG.
sfc32 *sfc32_init(uint32_t a, uint32_t b, uint32_t c, uint32_t d);

#endif //OLYTAG_SFC32_H
