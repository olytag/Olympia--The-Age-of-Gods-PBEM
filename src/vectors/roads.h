// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTOR_ROADS_H
#define OLYTAG_VECTOR_ROADS_H

#include "mapgen/road.h"

typedef struct roads_vector {
    road_t **list;
    int capacity;
    int length;
} roads_vector_t;

typedef struct road **road_list;

int road_list_len(road_list l);
void road_list_append(road_list *l, struct road *n);

int roadsv_cap(roads_vector_t *base);
int roadsv_len(roads_vector_t *base);
roads_vector_t *roadsv_append(roads_vector_t *base, road_t *r);

#endif //OLYTAG_VECTOR_ROADS_H
