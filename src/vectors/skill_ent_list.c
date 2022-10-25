// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "skill_ent_list.h"
#include "vlist.h"

int skill_ent_list_cap(skill_ent_list_t l) {
    return vlist_cap((vlist) l);
}

int skill_ent_list_len(skill_ent_list_t l) {
    return vlist_len((vlist) l);
}

skill_ent_list_t *skill_ent_list_add(skill_ent_list_t *l, skill_ent_ptr p) {
    return (skill_ent_list_t *) vlist_add((vlist *) l, p);
}

skill_ent_list_t *skill_ent_list_append(skill_ent_list_t *l, skill_ent_ptr p) {
    return (skill_ent_list_t *) vlist_append((vlist *) l, p);
}

skill_ent_list_t *skill_ent_list_clear(skill_ent_list_t *l) {
    return (skill_ent_list_t *) vlist_clear((vlist *) l);
}

skill_ent_list_t skill_ent_list_copy(skill_ent_list_t l) {
    return (skill_ent_list_t) vlist_copy((vlist) l);
}

skill_ent_list_t *skill_ent_list_delete(skill_ent_list_t *l, int index) {
    return (skill_ent_list_t *) vlist_delete((vlist *) l, index);
}

int skill_ent_list_lookup(skill_ent_list_t l, skill_ent_ptr p) {
    return vlist_lookup((vlist) l, p);
}

skill_ent_list_t *skill_ent_list_prepend(skill_ent_list_t *l, skill_ent_ptr p) {
    return (skill_ent_list_t *) vlist_prepend((vlist *) l, p);
}

void skill_ent_list_reclaim(skill_ent_list_t *l) {
    vlist_reclaim((vlist *) l);
}

skill_ent_list_t *skill_ent_list_rem_value(skill_ent_list_t *l, skill_ent_ptr p) {
    return (skill_ent_list_t *) vlist_rem_value((vlist *) l, p);
}

skill_ent_list_t *skill_ent_list_rem_value_uniq(skill_ent_list_t *l, skill_ent_ptr p) {
    return (skill_ent_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

skill_ent_list_t skill_ent_list_scramble(skill_ent_list_t l) {
    return (skill_ent_list_t) vlist_scramble((vlist) l);
}

