// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "wa_list.h"
#include "vlist.h"

int wa_list_cap(wa_list_t l) {
    return vlist_cap((vlist) l);
}

int wa_list_len(wa_list_t l) {
    return vlist_len((vlist) l);
}

wa_list_t *wa_list_add(wa_list_t *l, wa_ptr p) {
    return (wa_list_t *) vlist_add((vlist *) l, p);
}

wa_list_t *wa_list_append(wa_list_t *l, wa_ptr p) {
    return (wa_list_t *) vlist_append((vlist *) l, p);
}

wa_list_t *wa_list_clear(wa_list_t *l) {
    return (wa_list_t *) vlist_clear((vlist *) l);
}

wa_list_t wa_list_copy(wa_list_t l) {
    return (wa_list_t) vlist_copy((vlist) l);
}

wa_list_t *wa_list_delete(wa_list_t *l, int index) {
    return (wa_list_t *) vlist_delete((vlist *) l, index);
}

int wa_list_lookup(wa_list_t l, wa_ptr p) {
    return vlist_lookup((vlist) l, p);
}

wa_list_t *wa_list_prepend(wa_list_t *l, wa_ptr p) {
    return (wa_list_t *) vlist_prepend((vlist *) l, p);
}

void wa_list_reclaim(wa_list_t *l) {
    vlist_reclaim((vlist *) l);
}

wa_list_t *wa_list_rem_value(wa_list_t *l, wa_ptr p) {
    return (wa_list_t *) vlist_rem_value((vlist *) l, p);
}

wa_list_t *wa_list_rem_value_uniq(wa_list_t *l, wa_ptr p) {
    return (wa_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

wa_list_t wa_list_scramble(wa_list_t l) {
    return (wa_list_t) vlist_scramble((vlist) l);
}

