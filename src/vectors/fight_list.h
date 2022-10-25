// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_FIGHT_LIST_H
#define OLYTAG_VECTORS_FIGHT_LIST_H

typedef struct fight *fight_ptr;
typedef fight_ptr *fight_list_t;

int        fight_list_cap(fight_list_t);
int        fight_list_len(fight_list_t);
int        fight_list_lookup(fight_list_t, int);
void       fight_list_reclaim(fight_list_t*);
fight_list_t *fight_list_add(fight_list_t*, fight_ptr);
fight_list_t *fight_list_append(fight_list_t*, fight_ptr);
fight_list_t *fight_list_clear(fight_list_t*);
fight_list_t  fight_list_copy(fight_list_t);
fight_list_t *fight_list_delete(fight_list_t*, int index);
fight_list_t *fight_list_prepend(fight_list_t*, fight_ptr);
fight_list_t *fight_list_rem_value(fight_list_t*, fight_ptr);
fight_list_t *fight_list_rem_value_uniq(fight_list_t*, fight_ptr);
fight_list_t  fight_list_scramble(fight_list_t);

#endif //OLYTAG_VECTORS_FIGHT_LIST_H
