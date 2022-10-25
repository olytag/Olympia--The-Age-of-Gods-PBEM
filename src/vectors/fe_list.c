// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "fe_list.h"
#include "vlist.h"

int fe_list_cap(fe_list_t l) {
    return vlist_cap((vlist) l);
}

int fe_list_len(fe_list_t l) {
    return vlist_len((vlist) l);
}

fe_list_t *fe_list_add(fe_list_t *l, fe_ptr p) {
    return (fe_list_t *) vlist_add((vlist *) l, p);
}

fe_list_t *fe_list_append(fe_list_t *l, fe_ptr p) {
    return (fe_list_t *) vlist_append((vlist *) l, p);
}

fe_list_t *fe_list_clear(fe_list_t *l) {
    return (fe_list_t *) vlist_clear((vlist *) l);
}

fe_list_t fe_list_copy(fe_list_t l) {
    return (fe_list_t) vlist_copy((vlist) l);
}

fe_list_t *fe_list_delete(fe_list_t *l, int index) {
    return (fe_list_t *) vlist_delete((vlist *) l, index);
}

int fe_list_lookup(fe_list_t l, fe_ptr p) {
    return vlist_lookup((vlist) l, p);
}

fe_list_t *fe_list_prepend(fe_list_t *l, fe_ptr p) {
    return (fe_list_t *) vlist_prepend((vlist *) l, p);
}

void fe_list_reclaim(fe_list_t *l) {
    vlist_reclaim((vlist *) l);
}

fe_list_t *fe_list_rem_value(fe_list_t *l, fe_ptr p) {
    return (fe_list_t *) vlist_rem_value((vlist *) l, p);
}

fe_list_t *fe_list_rem_value_uniq(fe_list_t *l, fe_ptr p) {
    return (fe_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

fe_list_t fe_list_scramble(fe_list_t l) {
    return (fe_list_t) vlist_scramble((vlist) l);
}

