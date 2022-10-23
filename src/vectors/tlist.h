// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTOR_TLIST_H
#define OLYTAG_VECTOR_TLIST_H

#include "mapgen/tile.h"

typedef struct tile **tlist;

int tlist_cap(tlist l);
int tlist_len(tlist l);

tlist *tlist_add(tlist *l, tile_t *t);
tlist *tlist_append(tlist *l, tile_t *t);
tlist *tlist_clear(tlist *l);
tlist tlist_copy(tlist l);
// tlist_delete removes the i'th entry from the list
tlist *tlist_delete(tlist *l, int index);
int tlist_lookup(tlist l, tile_t *t);
tlist *tlist_prepend(tlist *l, tile_t *t);
void tlist_reclaim(tlist *l);
tlist *tlist_rem_value(tlist *l, tile_t *t);
tlist *tlist_rem_value_uniq(tlist *l, tile_t *t);
tlist tlist_scramble(tlist l);

#endif //OLYTAG_VECTOR_TLIST_H
