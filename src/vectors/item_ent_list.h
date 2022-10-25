// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_ITEM_ENT_LIST_H
#define OLYTAG_VECTORS_ITEM_ENT_LIST_H

typedef struct item_ent *ie_ptr;
typedef ie_ptr *ie_list_t;

int        ie_list_cap(ie_list_t);
int        ie_list_len(ie_list_t);
int        ie_list_lookup(ie_list_t, ie_ptr);
void       ie_list_reclaim(ie_list_t*);
ie_list_t *ie_list_add(ie_list_t*, ie_ptr);
ie_list_t *ie_list_append(ie_list_t*, ie_ptr);
ie_list_t *ie_list_clear(ie_list_t*);
ie_list_t  ie_list_copy(ie_list_t);
ie_list_t *ie_list_delete(ie_list_t*, int index);
ie_list_t *ie_list_prepend(ie_list_t*, ie_ptr);
ie_list_t *ie_list_rem_value(ie_list_t*, ie_ptr);
ie_list_t *ie_list_rem_value_uniq(ie_list_t*, ie_ptr);
ie_list_t  ie_list_scramble(ie_list_t);

#endif //OLYTAG_VECTORS_ITEM_ENT_LIST_H
