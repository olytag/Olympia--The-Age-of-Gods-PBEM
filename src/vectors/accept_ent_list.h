// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_AE_LIST_H
#define OLYTAG_VECTORS_AE_LIST_H

typedef struct accept_ent *ae_ptr;
typedef ae_ptr *ae_list_t;

int        ae_list_cap(ae_list_t);
int        ae_list_len(ae_list_t);
int        ae_list_lookup(ae_list_t, ae_ptr);
void       ae_list_reclaim(ae_list_t*);
ae_list_t *ae_list_add(ae_list_t*, ae_ptr);
ae_list_t *ae_list_append(ae_list_t*, ae_ptr);
ae_list_t *ae_list_clear(ae_list_t*);
ae_list_t  ae_list_copy(ae_list_t);
ae_list_t *ae_list_delete(ae_list_t*, int index);
ae_list_t *ae_list_prepend(ae_list_t*, ae_ptr);
ae_list_t *ae_list_rem_value(ae_list_t*, ae_ptr);
ae_list_t *ae_list_rem_value_uniq(ae_list_t*, ae_ptr);
ae_list_t  ae_list_scramble(ae_list_t);

#endif //OLYTAG_VECTORS_AE_LIST_H
