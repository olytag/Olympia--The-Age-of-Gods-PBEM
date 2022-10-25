// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "req_ent_list.h"
#include "vlist.h"

int req_ent_list_cap(req_ent_list_t l) {
    return vlist_cap((vlist) l);
}

int req_ent_list_len(req_ent_list_t l) {
    return vlist_len((vlist) l);
}

req_ent_list_t *req_ent_list_add(req_ent_list_t *l, req_ent_ptr p) {
    return (req_ent_list_t *) vlist_add((vlist *) l, p);
}

req_ent_list_t *req_ent_list_append(req_ent_list_t *l, req_ent_ptr p) {
    return (req_ent_list_t *) vlist_append((vlist *) l, p);
}

req_ent_list_t *req_ent_list_clear(req_ent_list_t *l) {
    return (req_ent_list_t *) vlist_clear((vlist *) l);
}

req_ent_list_t req_ent_list_copy(req_ent_list_t l) {
    return (req_ent_list_t) vlist_copy((vlist) l);
}

req_ent_list_t *req_ent_list_delete(req_ent_list_t *l, int index) {
    return (req_ent_list_t *) vlist_delete((vlist *) l, index);
}

int req_ent_list_lookup(req_ent_list_t l, req_ent_ptr p) {
    return vlist_lookup((vlist) l, p);
}

req_ent_list_t *req_ent_list_prepend(req_ent_list_t *l, req_ent_ptr p) {
    return (req_ent_list_t *) vlist_prepend((vlist *) l, p);
}

void req_ent_list_reclaim(req_ent_list_t *l) {
    vlist_reclaim((vlist *) l);
}

req_ent_list_t *req_ent_list_rem_value(req_ent_list_t *l, req_ent_ptr p) {
    return (req_ent_list_t *) vlist_rem_value((vlist *) l, p);
}

req_ent_list_t *req_ent_list_rem_value_uniq(req_ent_list_t *l, req_ent_ptr p) {
    return (req_ent_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

req_ent_list_t req_ent_list_scramble(req_ent_list_t l) {
    return (req_ent_list_t) vlist_scramble((vlist) l);
}

