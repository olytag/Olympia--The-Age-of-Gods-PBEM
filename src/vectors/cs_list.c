// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "cs_list.h"
#include "vlist.h"

int cs_list_cap(cs_list_t l) {
    return vlist_cap((vlist) l);
}

int cs_list_len(cs_list_t l) {
    return vlist_len((vlist) l);
}

cs_list_t *cs_list_add(cs_list_t *l, char *p) {
    return (cs_list_t *) vlist_add((vlist *) l, p);
}

cs_list_t *cs_list_append(cs_list_t *l, char *p) {
    return (cs_list_t *) vlist_append((vlist *) l, p);
}

cs_list_t *cs_list_clear(cs_list_t *l) {
    return (cs_list_t *) vlist_clear((vlist *) l);
}

cs_list_t cs_list_copy(cs_list_t l) {
    return (cs_list_t) vlist_copy((vlist) l);
}

cs_list_t *cs_list_delete(cs_list_t *l, int index) {
    return (cs_list_t *) vlist_delete((vlist *) l, index);
}

int cs_list_lookup(cs_list_t l, char *p) {
    return vlist_lookup((vlist) l, p);
}

cs_list_t *cs_list_prepend(cs_list_t *l, char *p) {
    return (cs_list_t *) vlist_prepend((vlist *) l, p);
}

void cs_list_reclaim(cs_list_t *l) {
    vlist_reclaim((vlist *) l);
}

cs_list_t *cs_list_rem_value(cs_list_t *l, char *p) {
    return (cs_list_t *) vlist_rem_value((vlist *) l, p);
}

cs_list_t *cs_list_rem_value_uniq(cs_list_t *l, char *p) {
    return (cs_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

cs_list_t cs_list_scramble(cs_list_t l) {
    return (cs_list_t) vlist_scramble((vlist) l);
}

