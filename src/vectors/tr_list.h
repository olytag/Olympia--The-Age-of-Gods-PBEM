// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_TR_LIST_H
#define OLYTAG_VECTORS_TR_LIST_H

typedef struct trade *tr_ptr;
typedef tr_ptr *tr_list_t;

int        tr_list_cap(tr_list_t);
int        tr_list_len(tr_list_t);
int        tr_list_lookup(tr_list_t, tr_ptr);
void       tr_list_reclaim(tr_list_t*);
tr_list_t *tr_list_add(tr_list_t*, tr_ptr);
tr_list_t *tr_list_append(tr_list_t*, tr_ptr);
tr_list_t *tr_list_clear(tr_list_t*);
tr_list_t  tr_list_copy(tr_list_t);
tr_list_t *tr_list_delete(tr_list_t*, int index);
tr_list_t *tr_list_prepend(tr_list_t*, tr_ptr);
tr_list_t *tr_list_rem_value(tr_list_t*, tr_ptr);
tr_list_t *tr_list_rem_value_uniq(tr_list_t*, tr_ptr);
tr_list_t  tr_list_scramble(tr_list_t);

#endif //OLYTAG_VECTORS_TR_LIST_H
