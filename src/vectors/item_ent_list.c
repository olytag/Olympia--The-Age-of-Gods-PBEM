// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "item_ent_list.h"
#include "vlist.h"

int ie_list_cap(ie_list_t l) {
    return vlist_cap((vlist) l);
}

int ie_list_len(ie_list_t l) {
    return vlist_len((vlist) l);
}

ie_list_t *ie_list_add(ie_list_t *l, ie_ptr p) {
    return (ie_list_t *) vlist_add((vlist *) l, p);
}

ie_list_t *ie_list_append(ie_list_t *l, ie_ptr p) {
    return (ie_list_t *) vlist_append((vlist *) l, p);
}

ie_list_t *ie_list_clear(ie_list_t *l) {
    return (ie_list_t *) vlist_clear((vlist *) l);
}

ie_list_t ie_list_copy(ie_list_t l) {
    return (ie_list_t) vlist_copy((vlist) l);
}

ie_list_t *ie_list_delete(ie_list_t *l, int index) {
    return (ie_list_t *) vlist_delete((vlist *) l, index);
}

int ie_list_lookup(ie_list_t l, ie_ptr p) {
    return vlist_lookup((vlist) l, p);
}

ie_list_t *ie_list_prepend(ie_list_t *l, ie_ptr p) {
    return (ie_list_t *) vlist_prepend((vlist *) l, p);
}

void ie_list_reclaim(ie_list_t *l) {
    vlist_reclaim((vlist *) l);
}

ie_list_t *ie_list_rem_value(ie_list_t *l, ie_ptr p) {
    return (ie_list_t *) vlist_rem_value((vlist *) l, p);
}

ie_list_t *ie_list_rem_value_uniq(ie_list_t *l, ie_ptr p) {
    return (ie_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

ie_list_t ie_list_scramble(ie_list_t l) {
    return (ie_list_t) vlist_scramble((vlist) l);
}

