// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_WA_LIST_H
#define OLYTAG_VECTORS_WA_LIST_H

typedef struct wait_arg *wa_ptr;
typedef wa_ptr *wa_list_t;

int        wa_list_cap(wa_list_t);
int        wa_list_len(wa_list_t);
int        wa_list_lookup(wa_list_t, wa_ptr);
void       wa_list_reclaim(wa_list_t*);
wa_list_t *wa_list_add(wa_list_t*, wa_ptr);
wa_list_t *wa_list_append(wa_list_t*, wa_ptr);
wa_list_t *wa_list_clear(wa_list_t*);
wa_list_t  wa_list_copy(wa_list_t);
wa_list_t *wa_list_delete(wa_list_t*, int index);
wa_list_t *wa_list_prepend(wa_list_t*, wa_ptr);
wa_list_t *wa_list_rem_value(wa_list_t*, wa_ptr);
wa_list_t *wa_list_rem_value_uniq(wa_list_t*, wa_ptr);
wa_list_t  wa_list_scramble(wa_list_t);

#endif //OLYTAG_VECTORS_WA_LIST_H
