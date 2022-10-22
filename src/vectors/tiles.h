// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTOR_TILES_H
#define OLYTAG_VECTOR_TILES_H

typedef struct tile **tiles_list;

int tiles_list_len(tiles_list l);
void tiles_list_append(tiles_list *l, struct tile *n);
tiles_list tiles_list_copy(tiles_list l);
void tiles_list_reclaim(tiles_list *l);
void tiles_list_scramble(tiles_list l);

#endif //OLYTAG_VECTOR_TILES_H
