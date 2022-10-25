// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "entity_player_list.h"
#include "vlist.h"

int entity_player_list_cap(entity_player_list_t l) {
    return vlist_cap((vlist) l);
}

int entity_player_list_len(entity_player_list_t l) {
    return vlist_len((vlist) l);
}

entity_player_list_t *entity_player_list_add(entity_player_list_t *l, entity_player_ptr p) {
    return (entity_player_list_t *) vlist_add((vlist *) l, p);
}

entity_player_list_t *entity_player_list_append(entity_player_list_t *l, entity_player_ptr p) {
    return (entity_player_list_t *) vlist_append((vlist *) l, p);
}

entity_player_list_t *entity_player_list_clear(entity_player_list_t *l) {
    return (entity_player_list_t *) vlist_clear((vlist *) l);
}

entity_player_list_t entity_player_list_copy(entity_player_list_t l) {
    return (entity_player_list_t) vlist_copy((vlist) l);
}

entity_player_list_t *entity_player_list_delete(entity_player_list_t *l, int index) {
    return (entity_player_list_t *) vlist_delete((vlist *) l, index);
}

int entity_player_list_lookup(entity_player_list_t l, entity_player_ptr p) {
    return vlist_lookup((vlist) l, p);
}

entity_player_list_t *entity_player_list_prepend(entity_player_list_t *l, entity_player_ptr p) {
    return (entity_player_list_t *) vlist_prepend((vlist *) l, p);
}

void entity_player_list_reclaim(entity_player_list_t *l) {
    vlist_reclaim((vlist *) l);
}

entity_player_list_t *entity_player_list_rem_value(entity_player_list_t *l, entity_player_ptr p) {
    return (entity_player_list_t *) vlist_rem_value((vlist *) l, p);
}

entity_player_list_t *entity_player_list_rem_value_uniq(entity_player_list_t *l, entity_player_ptr p) {
    return (entity_player_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

entity_player_list_t entity_player_list_scramble(entity_player_list_t l) {
    return (entity_player_list_t) vlist_scramble((vlist) l);
}

