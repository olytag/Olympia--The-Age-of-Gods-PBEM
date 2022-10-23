// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_ILIST_H
#define OLYTAG_VECTORS_ILIST_H

typedef int *ilist;

int ilist_cap(ilist l);
int ilist_len(ilist l);

ilist *ilist_add(ilist *l, int n);
ilist *ilist_append(ilist *l, int n);
ilist *ilist_clear(ilist *l);
ilist ilist_copy(ilist l);
// ilist_delete removes the i'th entry from the list
ilist *ilist_delete(ilist *l, int index);
int ilist_lookup(ilist l, int n);
ilist *ilist_prepend(ilist *l, int n);
void ilist_reclaim(ilist *l);
ilist *ilist_rem_value(ilist *l, int n);
ilist *ilist_rem_value_uniq(ilist *l, int n);
ilist ilist_scramble(ilist l);

#endif //OLYTAG_VECTORS_ILIST_H
