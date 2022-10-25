// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "effect_list.h"
#include "vlist.h"

int effect_list_cap(effect_list_t l) {
    return vlist_cap((vlist) l);
}

int effect_list_len(effect_list_t l) {
    return vlist_len((vlist) l);
}

effect_list_t *effect_list_add(effect_list_t *l, effect_ptr p) {
    return (effect_list_t *) vlist_add((vlist *) l, p);
}

effect_list_t *effect_list_append(effect_list_t *l, effect_ptr p) {
    return (effect_list_t *) vlist_append((vlist *) l, p);
}

effect_list_t *effect_list_clear(effect_list_t *l) {
    return (effect_list_t *) vlist_clear((vlist *) l);
}

effect_list_t effect_list_copy(effect_list_t l) {
    return (effect_list_t) vlist_copy((vlist) l);
}

effect_list_t *effect_list_delete(effect_list_t *l, int index) {
    return (effect_list_t *) vlist_delete((vlist *) l, index);
}

int effect_list_lookup(effect_list_t l, effect_ptr p) {
    return vlist_lookup((vlist) l, p);
}

effect_list_t *effect_list_prepend(effect_list_t *l, effect_ptr p) {
    return (effect_list_t *) vlist_prepend((vlist *) l, p);
}

void effect_list_reclaim(effect_list_t *l) {
    vlist_reclaim((vlist *) l);
}

effect_list_t *effect_list_rem_value(effect_list_t *l, effect_ptr p) {
    return (effect_list_t *) vlist_rem_value((vlist *) l, p);
}

effect_list_t *effect_list_rem_value_uniq(effect_list_t *l, effect_ptr p) {
    return (effect_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

effect_list_t effect_list_scramble(effect_list_t l) {
    return (effect_list_t) vlist_scramble((vlist) l);
}

