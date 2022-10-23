// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTOR_RLIST_H
#define OLYTAG_VECTOR_RLIST_H

#include "mapgen/road.h"

typedef struct road **rlist;

int rlist_cap(rlist l);
int rlist_len(rlist l);

rlist *rlist_add(rlist *l, road_t *t);
rlist *rlist_append(rlist *l, road_t *t);
rlist *rlist_clear(rlist *l);
rlist rlist_copy(rlist l);
// rlist_delete removes the i'th entry from the list
rlist *rlist_delete(rlist *l, int index);
int rlist_lookup(rlist l, road_t *t);
rlist *rlist_prepend(rlist *l, road_t *t);
void rlist_reclaim(rlist *l);
rlist *rlist_rem_value(rlist *l, road_t *t);
rlist *rlist_rem_value_uniq(rlist *l, road_t *t);
rlist rlist_scramble(rlist l);

#endif //OLYTAG_VECTOR_RLIST_H
