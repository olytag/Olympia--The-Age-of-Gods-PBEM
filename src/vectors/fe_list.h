// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_FE_LIST_H
#define OLYTAG_VECTORS_FE_LIST_H

typedef struct flag_ent *fe_ptr;
typedef fe_ptr *fe_list_t;

int        fe_list_cap(fe_list_t);
int        fe_list_len(fe_list_t);
int        fe_list_lookup(fe_list_t, fe_ptr);
void       fe_list_reclaim(fe_list_t*);
fe_list_t *fe_list_add(fe_list_t*, fe_ptr);
fe_list_t *fe_list_append(fe_list_t*, fe_ptr);
fe_list_t *fe_list_clear(fe_list_t*);
fe_list_t  fe_list_copy(fe_list_t);
fe_list_t *fe_list_delete(fe_list_t*, int index);
fe_list_t *fe_list_prepend(fe_list_t*, fe_ptr);
fe_list_t *fe_list_rem_value(fe_list_t*, fe_ptr);
fe_list_t *fe_list_rem_value_uniq(fe_list_t*, fe_ptr);
fe_list_t  fe_list_scramble(fe_list_t);

#endif //OLYTAG_VECTORS_FE_LIST_H
