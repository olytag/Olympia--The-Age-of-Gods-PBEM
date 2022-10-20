// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_LISTS_H
#define OLYTAG_LISTS_H

// a collection of reallocation lists

typedef int *ilist;


//#define ilist_len(a)		(((int *)(a)) == NULL ? 0 : ((int *)(a))[-2])

void ilist_add(ilist *l, int n);
void ilist_append(ilist *l, int n);
void ilist_clear(ilist *l);
ilist ilist_copy(ilist l);
void ilist_delete(ilist *l, int index);
void ilist_insert(ilist *l, int pos, int n);
int ilist_len(ilist l);
int ilist_lookup(ilist l, int n);
void ilist_prepend(ilist *l, int n);
void ilist_reclaim(ilist *l);
void ilist_rem_value(ilist *l, int n);
void ilist_rem_value_uniq(ilist *l, int n);
void ilist_scramble(ilist l);

typedef struct road **road_list;

int road_list_len(road_list l);
void road_list_append(road_list *l, struct road *n);

typedef struct tile **tiles_list;

int tiles_list_len(tiles_list l);
void tiles_list_append(tiles_list *l, struct tile *n);
tiles_list tiles_list_copy(tiles_list l);
void tiles_list_reclaim(tiles_list *l);
void tiles_list_scramble(tiles_list l);

#endif //OLYTAG_LISTS_H
