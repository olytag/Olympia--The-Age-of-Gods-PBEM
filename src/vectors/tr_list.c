// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "tr_list.h"
#include "vlist.h"

int tr_list_cap(tr_list_t l) {
    return vlist_cap((vlist) l);
}

int tr_list_len(tr_list_t l) {
    return vlist_len((vlist) l);
}

tr_list_t *tr_list_add(tr_list_t *l, tr_ptr p) {
    return (tr_list_t *) vlist_add((vlist *) l, p);
}

tr_list_t *tr_list_append(tr_list_t *l, tr_ptr p) {
    return (tr_list_t *) vlist_append((vlist *) l, p);
}

tr_list_t *tr_list_clear(tr_list_t *l) {
    return (tr_list_t *) vlist_clear((vlist *) l);
}

tr_list_t tr_list_copy(tr_list_t l) {
    return (tr_list_t) vlist_copy((vlist) l);
}

tr_list_t *tr_list_delete(tr_list_t *l, int index) {
    return (tr_list_t *) vlist_delete((vlist *) l, index);
}

int tr_list_lookup(tr_list_t l, tr_ptr p) {
    return vlist_lookup((vlist) l, p);
}

tr_list_t *tr_list_prepend(tr_list_t *l, tr_ptr p) {
    return (tr_list_t *) vlist_prepend((vlist *) l, p);
}

void tr_list_reclaim(tr_list_t *l) {
    vlist_reclaim((vlist *) l);
}

tr_list_t *tr_list_rem_value(tr_list_t *l, tr_ptr p) {
    return (tr_list_t *) vlist_rem_value((vlist *) l, p);
}

tr_list_t *tr_list_rem_value_uniq(tr_list_t *l, tr_ptr p) {
    return (tr_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

tr_list_t tr_list_scramble(tr_list_t l) {
    return (tr_list_t) vlist_scramble((vlist) l);
}

