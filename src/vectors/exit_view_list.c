// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "exit_view_list.h"
#include "vlist.h"

int ev_list_cap(ev_list_t l) {
    return vlist_cap((vlist) l);
}

int ev_list_len(ev_list_t l) {
    return vlist_len((vlist) l);
}

ev_list_t *ev_list_add(ev_list_t *l, ev_ptr p) {
    return (ev_list_t *) vlist_add((vlist *) l, p);
}

ev_list_t *ev_list_append(ev_list_t *l, ev_ptr p) {
    return (ev_list_t *) vlist_append((vlist *) l, p);
}

ev_list_t *ev_list_clear(ev_list_t *l) {
    return (ev_list_t *) vlist_clear((vlist *) l);
}

ev_list_t ev_list_copy(ev_list_t l) {
    return (ev_list_t) vlist_copy((vlist) l);
}

ev_list_t *ev_list_delete(ev_list_t *l, int index) {
    return (ev_list_t *) vlist_delete((vlist *) l, index);
}

int ev_list_lookup(ev_list_t l, ev_ptr p) {
    return vlist_lookup((vlist) l, p);
}

ev_list_t *ev_list_prepend(ev_list_t *l, ev_ptr p) {
    return (ev_list_t *) vlist_prepend((vlist *) l, p);
}

void ev_list_reclaim(ev_list_t *l) {
    vlist_reclaim((vlist *) l);
}

ev_list_t *ev_list_rem_value(ev_list_t *l, ev_ptr p) {
    return (ev_list_t *) vlist_rem_value((vlist *) l, p);
}

ev_list_t *ev_list_rem_value_uniq(ev_list_t *l, ev_ptr p) {
    return (ev_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

ev_list_t ev_list_scramble(ev_list_t l) {
    return (ev_list_t) vlist_scramble((vlist) l);
}

