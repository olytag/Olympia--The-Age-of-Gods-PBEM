// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information. 

#include "fight_list.h"
#include "vlist.h"

int fight_list_cap(fight_list_t l) {
    return vlist_cap((vlist) l);
}

int fight_list_len(fight_list_t l) {
    return vlist_len((vlist) l);
}

fight_list_t *fight_list_add(fight_list_t *l, fight_ptr p) {
    return (fight_list_t *) vlist_add((vlist *) l, p);
}

fight_list_t *fight_list_append(fight_list_t *l, fight_ptr p) {
    return (fight_list_t *) vlist_append((vlist *) l, p);
}

fight_list_t *fight_list_clear(fight_list_t *l) {
    return (fight_list_t *) vlist_clear((vlist *) l);
}

fight_list_t fight_list_copy(fight_list_t l) {
    return (fight_list_t) vlist_copy((vlist) l);
}

fight_list_t *fight_list_delete(fight_list_t *l, int index) {
    return (fight_list_t *) vlist_delete((vlist *) l, index);
}

int fight_list_lookup(fight_list_t l, int p) {
    if (l == 0) {
        return -1;
    }
    int length = fight_list_len(l);
    for (int i = 0; i < length; i++) {
        if (l[i] == p) {
            return i;
        }
    }
    return -1;
}

fight_list_t *fight_list_prepend(fight_list_t *l, fight_ptr p) {
    return (fight_list_t *) vlist_prepend((vlist *) l, p);
}

void fight_list_reclaim(fight_list_t *l) {
    vlist_reclaim((vlist *) l);
}

fight_list_t *fight_list_rem_value(fight_list_t *l, fight_ptr p) {
    return (fight_list_t *) vlist_rem_value((vlist *) l, p);
}

fight_list_t *fight_list_rem_value_uniq(fight_list_t *l, fight_ptr p) {
    return (fight_list_t *) vlist_rem_value_uniq((vlist *) l, p);
}

fight_list_t fight_list_scramble(fight_list_t l) {
    return (fight_list_t) vlist_scramble((vlist) l);
}

