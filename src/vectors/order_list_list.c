// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "order_list_list.h"
#include "vlist.h"

int order_list_list_cap(order_list_list_t l) {
    return vlist_cap((vlist) l);
}

int order_list_list_len(order_list_list_t l) {
    return vlist_len((vlist) l);
}

order_list_list_t *order_list_list_add(order_list_list_t *l, order_list_ptr p) {
    return (order_list_list_t *) vlist_add((vlist *) l, p);
}

order_list_list_t *order_list_list_append(order_list_list_t *l, order_list_ptr p) {
    return (order_list_list_t *) vlist_append((vlist *) l, p);
}

order_list_list_t *order_list_list_clear(order_list_list_t *l) {
    return (order_list_list_t *) vlist_clear((vlist *) l);
}

order_list_list_t order_list_list_copy(order_list_list_t l) {
    return (order_list_list_t) vlist_copy((vlist) l);
}

order_list_list_t *order_list_list_delete(order_list_list_t *l, int index) {
    return (order_list_list_t *) vlist_delete((vlist *) l, index);
}

int order_list_list_lookup(order_list_list_t l, order_list_ptr p) {
    return vlist_lookup((vlist) l, p);
}

order_list_list_t *order_list_list_prepend(order_list_list_t *l, order_list_ptr p) {
    return (order_list_list_t *) vlist_prepend((vlist *) l, p);
}

void order_list_list_reclaim(order_list_list_t *l) {
    vlist_reclaim((vlist *) l);
}

order_list_list_t *order_list_list_rem_value(order_list_list_t *l, order_list_ptr p) {
    return (order_list_list_t *) vlist_rem_value((vlist *) l, p);
}

order_list_list_t *order_list_list_rem_value_uniq(order_list_list_t *l, order_list_ptr p) {
    return (order_list_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

order_list_list_t order_list_list_scramble(order_list_list_t l) {
    return (order_list_list_t) vlist_scramble((vlist) l);
}

