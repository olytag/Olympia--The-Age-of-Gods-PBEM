// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_EB_LIST_H
#define OLYTAG_VECTORS_EB_LIST_H

typedef struct entity_build *eb_ptr;
typedef eb_ptr *eb_list_t;

int        eb_list_cap(eb_list_t);
int        eb_list_len(eb_list_t);
int        eb_list_lookup(eb_list_t, eb_ptr);
void       eb_list_reclaim(eb_list_t*);
eb_list_t *eb_list_add(eb_list_t*, eb_ptr);
eb_list_t *eb_list_append(eb_list_t*, eb_ptr);
eb_list_t *eb_list_clear(eb_list_t*);
eb_list_t  eb_list_copy(eb_list_t);
eb_list_t *eb_list_delete(eb_list_t*, int index);
eb_list_t *eb_list_prepend(eb_list_t*, eb_ptr);
eb_list_t *eb_list_rem_value(eb_list_t*, eb_ptr);
eb_list_t *eb_list_rem_value_uniq(eb_list_t*, eb_ptr);
eb_list_t  eb_list_scramble(eb_list_t);

#endif //OLYTAG_VECTORS_EB_LIST_H
