// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <stdio.h>
#include <string.h>
#include "ilist.h"
#include "ilist_test.h"

// test_ilist runs the test suite for ilist.
// it returns non-zero if there were errors.
int test_ilist(void) {
    // set stdout to flush like stderr
    setbuf(stdout, NULL);

    int errors = 0;

    ilist x;
    memset(&x, 0, sizeof(x));
    if (0 != ilist_len(x)) {
        printf("test_ilist: ilist_len: expected %d, got %d\n", 0, ilist_len(x));
        errors++;
    }
    if (errors != 0) {
        printf("test_ilist: test 0 failed\n");
        return 2;
    }

    int input1[100] = {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
            20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
            30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
            40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
            50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
            60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
            70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
            80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
            90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
    };
    for (int i = 0; i < 100; i++) {
        ilist_append(&x, input1[i]);
    }
    if (100 != ilist_len(x)) {
        printf("test_ilist: ilist_len: expected %d, got %d\n", 100, ilist_len(x));
        errors++;
    } else {
        for (int i = 0; i < 100; i++) {
            int got = x[i];
            if (got != input1[i]) {
                printf("test_ilist: %3d: expected %3d, got %3d\n", i, input1[i], got);
                errors++;
            }
        }
    }
    if (errors != 0) {
        printf("test_ilist: test 1 failed\n");
        return 2;
    }

    int input2[100] = {
            900, 901, 902, 903, 904, 905, 906, 907, 908, 909,
            910, 911, 912, 913, 914, 915, 916, 917, 918, 919,
            920, 921, 922, 923, 924, 925, 926, 927, 928, 929,
            930, 931, 932, 933, 934, 935, 936, 937, 938, 939,
            940, 941, 942, 943, 944, 945, 946, 947, 948, 949,
            950, 951, 952, 953, 954, 955, 956, 957, 958, 959,
            960, 961, 962, 963, 964, 965, 966, 967, 968, 969,
            970, 971, 972, 973, 974, 975, 976, 977, 978, 979,
            980, 981, 982, 983, 984, 985, 986, 987, 988, 989,
            990, 991, 992, 993, 994, 995, 996, 997, 998, 999,
    };
    int expectedLength2[100] = {
            101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
            111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
            121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
            131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
            141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
            151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
            161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
            171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
            181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
            191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
    };
    int expectedValue2[100] = {
            900, 901, 902, 903, 904, 905, 906, 907, 908, 909,
            910, 911, 912, 913, 914, 915, 916, 917, 918, 919,
            920, 921, 922, 923, 924, 925, 926, 927, 928, 929,
            930, 931, 932, 933, 934, 935, 936, 937, 938, 939,
            940, 941, 942, 943, 944, 945, 946, 947, 948, 949,
            950, 951, 952, 953, 954, 955, 956, 957, 958, 959,
            960, 961, 962, 963, 964, 965, 966, 967, 968, 969,
            970, 971, 972, 973, 974, 975, 976, 977, 978, 979,
            980, 981, 982, 983, 984, 985, 986, 987, 988, 989,
            990, 991, 992, 993, 994, 995, 996, 997, 998, 999,
    };
    for (int i = 0; i < 100; i++) {
        ilist_prepend(&x, input2[i]);
        int gotLength = ilist_len(x);
        if (gotLength != expectedLength2[i]) {
            printf("test_ilist: prepend: %3d: expected length %3d, got %3d\n", i, expectedLength2[i], gotLength);
            errors++;
        } else {
            int gotValue = x[0];
            if (gotValue != expectedValue2[i]) {
                printf("test_ilist: prepend: %3d: expected value %3d, got %3d\n", i, expectedValue2[i], gotValue);
                errors++;
            } else {
                if (x[gotLength - 1] != 99) {
                    printf("test_ilist: %3d: prepend: expected value %3d, got %3d\n", i, 99, x[gotLength - 1]);
                    errors++;
                }
            }
        }
    }

    if (ilist_len(x) != 200) {
        printf("test_ilist: delete: pre: expected value %3d, got %3d\n", 200, ilist_len(x));
        errors++;
    }
    ilist_delete(&x, 100);
    if (ilist_len(x) != 199) {
        printf("test_ilist: delete: expected value %3d, got %3d\n", 199, ilist_len(x));
        errors++;
    }
    if (errors != 0) {
        printf("test_ilist: test 3 failed\n");
        return 2;
    }

    if (ilist_len(x) != 199) {
        printf("test_ilist: append: pre: expected value %3d, got %3d\n", 199, ilist_len(x));
        errors++;
    }
    if (x[0] != 999) {
        printf("test_ilist: append: pre: expected value %3d, got %3d\n", 999, x[0]);
        errors++;
    }
    if (x[ilist_len(x) - 1] != 99) {
        printf("test_ilist: append: pre: expected value %3d, got %3d\n", 99, x[ilist_len(x) - 1]);
        errors++;
    }
    ilist_append(&x, 15);
    if (ilist_len(x) != 200) {
        printf("test_ilist: append: expected value %3d, got %3d\n", 200, ilist_len(x));
        errors++;
    }
    if (x[0] != 999) {
        printf("test_ilist: append: expected value %3d, got %3d\n", 999, x[0]);
        errors++;
    }
    if (x[ilist_len(x) - 2] != 99) {
        printf("test_ilist: append: expected value %3d, got %3d\n", 99, x[ilist_len(x) - 1]);
        errors++;
    }
    if (x[ilist_len(x) - 1] != 15) {
        printf("test_ilist: append: expected value %3d, got %3d\n", 15, x[ilist_len(x) - 1]);
        errors++;
    }
    if (errors != 0) {
        printf("test_ilist: test 4 failed\n");
        return 2;
    }

    int input5[4] = {999, 998, 1, 15};
    int expected5[4] = {0, 1, 100, 114};
    for (int i = 0; i < 4; i++) {
        int got = ilist_lookup(x, input5[i]);
        if (got != expected5[i]) {
            printf("test_ilist: lookup: %3d: expected value %3d, got %3d\n", i, expected5[i], got);
            errors++;
        }
    }
    if (errors != 0) {
        printf("test_ilist: test 5 failed\n");
        return 2;
    }

    ilist y = ilist_copy(x);
    if (ilist_len(x) != ilist_len(y)) {
        printf("test_ilist: copy: expected length %3d, got %3d\n", ilist_len(x), ilist_len(y));
        errors++;
    } else {
        for (int i = 0; i < ilist_len(x); i++) {
            if (&x[i] == &y[i]) {
                printf("test_ilist: copy: expected address x[%3d] != address y[%3d]\n", i, i);
                errors++;
            } else if (x[i] != y[i]) {
                printf("test_ilist: copy: expected value %3d, got %3d\n", x[i], y[i]);
                errors++;
            }
        }
    }
    if (errors != 0) {
        printf("test_ilist: test 6 failed\n");
        return 2;
    }

    ilist_clear(&x);
    if (0 != ilist_len(x)) {
        printf("test_ilist: clear: x: expected length %3d, got %3d\n", 0, ilist_len(x));
        errors++;
    }
    if (200 != ilist_len(y)) {
        printf("test_ilist: clear: y: expected length %3d, got %3d\n", 0, ilist_len(y));
        errors++;
    }
    if (errors != 0) {
        printf("test_ilist: test 7 failed\n");
        return 2;
    }

    return 0;
}
