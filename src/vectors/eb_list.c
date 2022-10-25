// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "eb_list.h"
#include "vlist.h"

int eb_list_cap(eb_list_t l) {
    return vlist_cap((vlist) l);
}

int eb_list_len(eb_list_t l) {
    return vlist_len((vlist) l);
}

eb_list_t *eb_list_add(eb_list_t *l, eb_ptr p) {
    return (eb_list_t *) vlist_add((vlist *) l, p);
}

eb_list_t *eb_list_append(eb_list_t *l, eb_ptr p) {
    return (eb_list_t *) vlist_append((vlist *) l, p);
}

eb_list_t *eb_list_clear(eb_list_t *l) {
    return (eb_list_t *) vlist_clear((vlist *) l);
}

eb_list_t eb_list_copy(eb_list_t l) {
    return (eb_list_t) vlist_copy((vlist) l);
}

eb_list_t *eb_list_delete(eb_list_t *l, int index) {
    return (eb_list_t *) vlist_delete((vlist *) l, index);
}

int eb_list_lookup(eb_list_t l, eb_ptr p) {
    return vlist_lookup((vlist) l, p);
}

eb_list_t *eb_list_prepend(eb_list_t *l, eb_ptr p) {
    return (eb_list_t *) vlist_prepend((vlist *) l, p);
}

void eb_list_reclaim(eb_list_t *l) {
    vlist_reclaim((vlist *) l);
}

eb_list_t *eb_list_rem_value(eb_list_t *l, eb_ptr p) {
    return (eb_list_t *) vlist_rem_value((vlist *) l, p);
}

eb_list_t *eb_list_rem_value_uniq(eb_list_t *l, eb_ptr p) {
    return (eb_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

eb_list_t eb_list_scramble(eb_list_t l) {
    return (eb_list_t) vlist_scramble((vlist) l);
}

