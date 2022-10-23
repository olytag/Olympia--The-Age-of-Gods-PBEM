// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_ILIST_H
#define OLYTAG_VECTORS_ILIST_H

/*
 *  'ilist' reallocing array definitions
 */

typedef int *ilist;

#define ilist_len(a)        (((int *)(a)) == NULL ? 0 : ((int *)(a))[-2])

void ilist_add(ilist *l, int n);
void ilist_append(ilist *l, int n);
void ilist_clear(ilist *l);
ilist ilist_copy(ilist l);
// ilist_delete removes the i'th entry from the list
void ilist_delete(ilist *l, int i);
void ilist_insert(ilist *l, int pos, int n);
int ilist_lookup(ilist l, int n);
void ilist_prepend(ilist *l, int n);
void ilist_reclaim(ilist *l);
void ilist_rem_value(ilist *l, int n);
void ilist_rem_value_uniq(ilist *l, int n);
void ilist_scramble(ilist l);

int test_ilist(void);

#endif //OLYTAG_ILIST_H
