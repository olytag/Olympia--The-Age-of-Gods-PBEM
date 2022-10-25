// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_REQ_ENT_LIST_H
#define OLYTAG_VECTORS_REQ_ENT_LIST_H

typedef struct req_ent *req_ent_ptr;
typedef req_ent_ptr *req_ent_list_t;

int        req_ent_list_cap(req_ent_list_t);
int        req_ent_list_len(req_ent_list_t);
int        req_ent_list_lookup(req_ent_list_t, req_ent_ptr);
void       req_ent_list_reclaim(req_ent_list_t*);
req_ent_list_t *req_ent_list_add(req_ent_list_t*, req_ent_ptr);
req_ent_list_t *req_ent_list_append(req_ent_list_t*, req_ent_ptr);
req_ent_list_t *req_ent_list_clear(req_ent_list_t*);
req_ent_list_t  req_ent_list_copy(req_ent_list_t);
req_ent_list_t *req_ent_list_delete(req_ent_list_t*, int index);
req_ent_list_t *req_ent_list_prepend(req_ent_list_t*, req_ent_ptr);
req_ent_list_t *req_ent_list_rem_value(req_ent_list_t*, req_ent_ptr);
req_ent_list_t *req_ent_list_rem_value_uniq(req_ent_list_t*, req_ent_ptr);
req_ent_list_t  req_ent_list_scramble(req_ent_list_t);

#endif //OLYTAG_VECTORS_REQ_ENT_LIST_H
