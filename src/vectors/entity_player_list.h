// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_ENTITY_PLAYER_LIST_H
#define OLYTAG_VECTORS_ENTITY_PLAYER_LIST_H

typedef struct entity_player *entity_player_ptr;
typedef entity_player_ptr *entity_player_list_t;

int        entity_player_list_cap(entity_player_list_t);
int        entity_player_list_len(entity_player_list_t);
int        entity_player_list_lookup(entity_player_list_t, entity_player_ptr);
void       entity_player_list_reclaim(entity_player_list_t*);
entity_player_list_t *entity_player_list_add(entity_player_list_t*, entity_player_ptr);
entity_player_list_t *entity_player_list_append(entity_player_list_t*, entity_player_ptr);
entity_player_list_t *entity_player_list_clear(entity_player_list_t*);
entity_player_list_t  entity_player_list_copy(entity_player_list_t);
entity_player_list_t *entity_player_list_delete(entity_player_list_t*, int index);
entity_player_list_t *entity_player_list_prepend(entity_player_list_t*, entity_player_ptr);
entity_player_list_t *entity_player_list_rem_value(entity_player_list_t*, entity_player_ptr);
entity_player_list_t *entity_player_list_rem_value_uniq(entity_player_list_t*, entity_player_ptr);
entity_player_list_t  entity_player_list_scramble(entity_player_list_t);

#endif //OLYTAG_VECTORS_ENTITY_PLAYER_LIST_H
