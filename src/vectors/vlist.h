// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_VLIST_H
#define OLYTAG_VECTORS_VLIST_H

#include "mapgen/tile.h"

typedef void **vlist;

int vlist_cap(vlist l);
int vlist_len(vlist l);

vlist *vlist_add(vlist *l, void *p);
vlist *vlist_append(vlist *l, void *p);
vlist *vlist_clear(vlist *l);
vlist vlist_copy(vlist l);
// vlist_delete removes the i'th entry from the list
vlist *vlist_delete(vlist *l, int index);
int vlist_lookup(vlist l, void *p);
vlist *vlist_prepend(vlist *l, void *p);
void vlist_reclaim(vlist *l);
vlist *vlist_rem_value(vlist *l, void *p);
vlist *vlist_rem_value_uniq(vlist *l, void *p);
vlist vlist_scramble(vlist l);

#endif //OLYTAG_VECTORS_VLIST_H
