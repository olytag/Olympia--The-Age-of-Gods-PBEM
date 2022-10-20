// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "lists.h"

int main(int argc, char **argv) {
    int i;
    ilist x;
    ilist y;

    setbuf(stdout, NULL);
    memset(&x, 0, sizeof(x));

    printf("len = %d\n", ilist_len(x));

    for (i = 0; i < 100; i++) {
        ilist_append(&x, i);
    }

    printf("x[ilist_len(x) - 1] == x[%d - 1] == %d\n", ilist_len(x), x[ilist_len(x) - 1]);
    assert(x[ilist_len(x) - 1] == 99);

    printf("len = %d\n", ilist_len(x));
    for (i = 0; i < ilist_len(x); i++) {
        printf("%d ", x[i]);
    }
    printf("\n");

    for (i = 900; i < 1000; i++) {
        ilist_prepend(&x, i);
        if (x[ilist_len(x) - 1] != 99) {
            fprintf(stderr, "fail: i = %d\n", i);
        }
    }

    printf("len = %d\n", ilist_len(x));
    for (i = 0; i < ilist_len(x); i++) {
        printf("%d ", x[i]);
    }
    printf("\n");

    ilist_delete(&x, 100);

    printf("len = %d\n", ilist_len(x));
    for (i = 0; i < ilist_len(x); i++) {
        printf("%d ", x[i]);
    }
    printf("\n");

    printf("len before = %d\n", ilist_len(x));
    ilist_append(&x, 15);
    printf("len after = %d\n", ilist_len(x));
    printf("x[0] = %d\n", x[0]);

    printf("ilist_lookup(998) == %d\n", ilist_lookup(x, 998));

    y = ilist_copy(x);
    assert(ilist_len(x) == ilist_len(y));
    for (i = 0; i < ilist_len(x); i++) {
        assert(&x[i] != &y[i]);
        if (x[i] != y[i]) {
            fprintf(stderr, "[%d] different\n", i);
            assert(x[i] == y[i]);
        }
    }

    printf("ilist_lookup(998) == %d\n", ilist_lookup(x, 998));

    ilist_clear(&x);
    assert(ilist_len(x) == 0);
}