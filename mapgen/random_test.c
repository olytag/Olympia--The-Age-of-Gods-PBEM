// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <stdio.h>
#include "random.h"
#include "random_test.h"


int test_random(void) {
    // set stdout to flush like stderr
    setbuf(stdout, NULL);

    int errors = 0;

    int expect2[100] = {
            1, 4, 4, 4, 2, 1, 5, 2, 5, 7,
            7, 10, 5, 8, 5, 2, 2, 3, 5, 1,
            8, 2, 2, 8, 6, 9, 6, 5, 10, 9,
            2, 5, 1, 8, 4, 9, 10, 8, 3, 2,
            4, 7, 7, 1, 10, 4, 2, 1, 2, 10,
            5, 8, 7, 7, 4, 9, 2, 4, 7, 3,
            2, 8, 9, 8, 7, 4, 10, 2, 7, 5,
            5, 8, 2, 10, 2, 10, 8, 10, 1, 8,
            5, 1, 9, 5, 7, 2, 10, 3, 5, 7,
            10, 4, 5, 2, 5, 10, 10, 2, 3, 2,
    };
    for (int i = 0; i < 100; i++) {
        int got = rnd(1, 10);
        if (got != expect2[i]) {
            printf("rnd: fail: %3d: expect %10d: got %10d\n", i, expect2[i], got);
            errors++;
        }
    }
    if (errors != 0) {
        printf("test_random: test 2 failed\n");
        return 2;
    }

    int input3[7] = {-10, -11, -12, -13, -14, -15, -16};
    int expect3[7] = {-3, -3, -2, -1, -2, 2, 3};
    for (int i = 0; i < 7; i++) {
        int got = rnd(-3, input3[i]);
        if (got != expect3[i]) {
            printf("rnd: fail: %3d: input %10d: expect %10d: got %10d\n", i, input3[i], expect3[i], got);
            errors++;
        }
    }
    if (errors != 0) {
        printf("test_random: test 3 failed\n");
        return 2;
    }

    int expect4[100] = {
            3546, 4616, 1587, 8731, 7908, 5140, 4238, 9040, 1974, 4367,
            5463, 7840, 4879, 6826, 4414, 1949, 3683, 5496, 5750, 7056,
            7405, 3598, 9988, 1476, 1967, 5080, 6327, 3633, 6179, 6153,
            1346, 7320, 7723, 3668, 1868, 8597, 9756, 2346, 7862, 2184,
            1791, 1504, 3447, 6717, 5655, 6665, 2593, 4039, 6152, 9370,
            3923, 1722, 4127, 2547, 4877, 9845, 1574, 7164, 5997, 4120,
            7138, 7706, 2506, 8554, 8932, 5550, 3763, 6517, 3232, 1513,
            3750, 7473, 1611, 9645, 9863, 2356, 1800, 1378, 9127, 9065,
            9614, 3637, 1575, 2462, 8987, 1419, 7424, 8028, 8646, 8644,
            6934, 3041, 8074, 8893, 8298, 4685, 1514, 6694, 5142, 2975,
    };
    for (int i = 0; i < 100; i++) {
        int got = rnd(1000, 9999);
        if (got != expect4[i]) {
            printf("rnd: fail: %3d: expect %10d: got %10d\n", i, expect4[i], got);
            errors++;
        }
    }
    if (errors != 0) {
        printf("test_random: test 4 failed\n");
        return 2;
    }

    return 0;
}