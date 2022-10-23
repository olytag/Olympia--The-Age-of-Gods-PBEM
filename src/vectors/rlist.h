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
rlist *rlist_append(rlist *l, struct road *n);

#endif //OLYTAG_VECTOR_RLIST_H
