// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTOR_ROADS_H
#define OLYTAG_VECTOR_ROADS_H

typedef struct road **road_list;

int road_list_len(road_list l);
void road_list_append(road_list *l, struct road *n);

#endif //OLYTAG_VECTOR_ROADS_H
