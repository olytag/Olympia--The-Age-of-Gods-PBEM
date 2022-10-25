// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_ADMIT_LIST_H
#define OLYTAG_VECTORS_ADMIT_LIST_H

typedef struct admit *admit_ptr;
typedef admit_ptr *admit_list_t;

int        admit_list_cap(admit_list_t);
int        admit_list_len(admit_list_t);
int        admit_list_lookup(admit_list_t, admit_ptr);
void       admit_list_reclaim(admit_list_t*);
admit_list_t *admit_list_add(admit_list_t*, admit_ptr);
admit_list_t *admit_list_append(admit_list_t*, admit_ptr);
admit_list_t *admit_list_clear(admit_list_t*);
admit_list_t  admit_list_copy(admit_list_t);
admit_list_t *admit_list_delete(admit_list_t*, int index);
admit_list_t *admit_list_prepend(admit_list_t*, admit_ptr);
admit_list_t *admit_list_rem_value(admit_list_t*, admit_ptr);
admit_list_t *admit_list_rem_value_uniq(admit_list_t*, admit_ptr);
admit_list_t  admit_list_scramble(admit_list_t);

#endif //OLYTAG_VECTORS_ADMIT_LIST_H
