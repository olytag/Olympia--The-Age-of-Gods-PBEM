// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

/*
 *  Reallocing array handler
 *
 *  Length is stored in ilist[0], maximum in ilist[1].
 *  The user-visible ilist is shifted to &ilist[2], so
 *  that iterations can proceed from index 0.
 */

// todo: when we use malloc instead of my_alloc we get segfaults.

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ilist.h"
#include "z.h"
#include "random.h"


#define ILIST_ALLOC 6 // doubles with each realloc


/*
 *  Mon Apr  3 11:47:28 2000 -- Scott Turner
 *
 *  Add w/o duplication.
 *
 */
void ilist_add(ilist *l, int n) {
    if (ilist_lookup(*l, n) == -1) {
        ilist_append(l, n);
    }
};


void ilist_append(ilist *l, int n) {
    int *base;

    if (*l == NULL) {
        base = my_malloc(sizeof(**l) * ILIST_ALLOC);
        base[1] = ILIST_ALLOC;

        *l = &base[2];
    } else {
        base = (*l) - 2;
        assert(&base[2] == *l);

        if (base[0] + 2 >= base[1]) {
            base[1] *= 2;
            base = my_realloc(base, base[1] * sizeof(*base));
            *l = &base[2];
        }
    }

    base[base[0] + 2] = n;
    base[0]++;
}


void ilist_clear(ilist *l) {
    int *base;

    if (*l != NULL) {
        base = (*l) - 2;
        base[0] = 0;
    }
}


ilist ilist_copy(ilist l) {
    int *base;
    int *copy_base;

    if (l == NULL) {
        return NULL;
    }

    base = l - 2;
    assert(&base[2] == l);

    copy_base = my_malloc(base[1] * sizeof(*base));
    memcpy(copy_base, base, (size_t) ((base[0] + 2) * sizeof(*base)));

    return &copy_base[2];
}


void ilist_delete(ilist *l, int i) {
    int *base;
    int j;

    assert(i >= 0 && i < ilist_len(*l));        /* bounds check */
    base = (*l) - 2;

    for (j = i + 2; j <= base[0]; j++) {
        base[j] = base[j + 1];
    }

    base[0]--;
}


//void ilist_insert(ilist *l, int pos, int n) {
//    int *base;
//    int i;
//
//    if (*l == NULL)
//    {
//        base = my_malloc(sizeof(**l) * ILIST_ALLOC);
//        base[1] = ILIST_ALLOC;
//
//        *l = &base[2];
//    }
//    else
//    {
//        base = (*l)-2;
//        assert(&base[2] == *l);
//
//        if (base[0] + 2 >= base[1])
//        {
//            base[1] *= 2;
//            base = my_realloc(base, base[1] * sizeof(*base));
//            *l = &base[2];
//        }
//    }
//
//    base[0]++;
//    pos += 2;
//
//    for (i = base[0]+1; i > pos; i--)
//        base[i] = base[i-1];
//
//    base[pos] = n;
//}


int ilist_lookup(ilist l, int n) {
    if (l == NULL) { return -1; }

    int end = ((int *) (l))[-2];
    for (int i = 0; i < end; ++i) {
        if (*(l++) == n) { return i; }
    }

    return -1;
}


void ilist_prepend(ilist *l, int n) {
    int *base;

    if (*l == NULL) {
        base = my_malloc(sizeof(**l) * ILIST_ALLOC);
        base[1] = ILIST_ALLOC;

        *l = &base[2];
    } else {
        base = (*l) - 2;
        assert(&base[2] == *l);

        if (base[0] + 2 >= base[1]) {
            base[1] *= 2;
            base = my_realloc(base, base[1] * sizeof(*base));
            *l = &base[2];
        }
    }

    base[0]++;
    for (int i = base[0] + 1; i > 2; i--) {
        base[i] = base[i - 1];
    }
    base[2] = n;
}


void ilist_reclaim(ilist *l) {
    if (*l != NULL) {
        int *base = (*l) - 2;
        my_free(base);
    }
    *l = NULL;
}


void ilist_rem_value(ilist *l, int n) {
    for (int i = ilist_len(*l) - 1; i >= 0; i--) {
        if ((*l)[i] == n) {
            ilist_delete(l, i);
        }
    }
}


void ilist_rem_value_uniq(ilist *l, int n) {
    for (int i = ilist_len(*l) - 1; i >= 0; i--) {
        if ((*l)[i] == n) {
            ilist_delete(l, i);
            break;
        }
    }
}


/*
 *  Knuth, The Art of Computer Programming, Vol. 2 (Addison Wesley).
 *  Essentially, to shuffle A[1]...A[N]:
 *  1) put I = 1;
 *  2) generate a random number R in the range I..N;
 *  3) if R is not I, swap A[R] and A[I];
 *  4) I <- I+1;
 *  5) if I is less than N go to step 2
 */


void ilist_scramble(ilist l) {
    int len = ilist_len(l) - 1;

    for (int i = 0; i < len; i++) {
        int r = rnd(i, len);
        if (r != i) {
            int tmp = l[i];
            l[i] = l[r];
            l[r] = tmp;
        }
    }
}


