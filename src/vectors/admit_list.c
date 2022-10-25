// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "admit_list.h"
#include "vlist.h"

int admit_list_cap(admit_list_t l) {
    return vlist_cap((vlist) l);
}

int admit_list_len(admit_list_t l) {
    return vlist_len((vlist) l);
}

admit_list_t *admit_list_add(admit_list_t *l, admit_ptr p) {
    return (admit_list_t *) vlist_add((vlist *) l, p);
}

admit_list_t *admit_list_append(admit_list_t *l, admit_ptr p) {
    return (admit_list_t *) vlist_append((vlist *) l, p);
}

admit_list_t *admit_list_clear(admit_list_t *l) {
    return (admit_list_t *) vlist_clear((vlist *) l);
}

admit_list_t admit_list_copy(admit_list_t l) {
    return (admit_list_t) vlist_copy((vlist) l);
}

admit_list_t *admit_list_delete(admit_list_t *l, int index) {
    return (admit_list_t *) vlist_delete((vlist *) l, index);
}

int admit_list_lookup(admit_list_t l, admit_ptr p) {
    return vlist_lookup((vlist) l, p);
}

admit_list_t *admit_list_prepend(admit_list_t *l, admit_ptr p) {
    return (admit_list_t *) vlist_prepend((vlist *) l, p);
}

void admit_list_reclaim(admit_list_t *l) {
    vlist_reclaim((vlist *) l);
}

admit_list_t *admit_list_rem_value(admit_list_t *l, admit_ptr p) {
    return (admit_list_t *) vlist_rem_value((vlist *) l, p);
}

admit_list_t *admit_list_rem_value_uniq(admit_list_t *l, admit_ptr p) {
    return (admit_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

admit_list_t admit_list_scramble(admit_list_t l) {
    return (admit_list_t) vlist_scramble((vlist) l);
}

