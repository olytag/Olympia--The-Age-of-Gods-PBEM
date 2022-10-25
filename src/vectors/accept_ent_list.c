// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "accept_ent_list.h"
#include "vlist.h"

int ae_list_cap(ae_list_t l) {
    return vlist_cap((vlist) l);
}

int ae_list_len(ae_list_t l) {
    return vlist_len((vlist) l);
}

ae_list_t *ae_list_add(ae_list_t *l, ae_ptr p) {
    return (ae_list_t *) vlist_add((vlist *) l, p);
}

ae_list_t *ae_list_append(ae_list_t *l, ae_ptr p) {
    return (ae_list_t *) vlist_append((vlist *) l, p);
}

ae_list_t *ae_list_clear(ae_list_t *l) {
    return (ae_list_t *) vlist_clear((vlist *) l);
}

ae_list_t ae_list_copy(ae_list_t l) {
    return (ae_list_t) vlist_copy((vlist) l);
}

ae_list_t *ae_list_delete(ae_list_t *l, int index) {
    return (ae_list_t *) vlist_delete((vlist *) l, index);
}

int ae_list_lookup(ae_list_t l, ae_ptr p) {
    return vlist_lookup((vlist) l, p);
}

ae_list_t *ae_list_prepend(ae_list_t *l, ae_ptr p) {
    return (ae_list_t *) vlist_prepend((vlist *) l, p);
}

void ae_list_reclaim(ae_list_t *l) {
    vlist_reclaim((vlist *) l);
}

ae_list_t *ae_list_rem_value(ae_list_t *l, ae_ptr p) {
    return (ae_list_t *) vlist_rem_value((vlist *) l, p);
}

ae_list_t *ae_list_rem_value_uniq(ae_list_t *l, ae_ptr p) {
    return (ae_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

ae_list_t ae_list_scramble(ae_list_t l) {
    return (ae_list_t) vlist_scramble((vlist) l);
}

