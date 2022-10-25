// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_CS_LIST_H
#define OLYTAG_VECTORS_CS_LIST_H

typedef char **cs_list_t;

int        cs_list_cap(cs_list_t l);
int        cs_list_len(cs_list_t l);
int        cs_list_lookup(cs_list_t l, char *p);
void       cs_list_reclaim(cs_list_t *l);
cs_list_t *cs_list_add(cs_list_t *l, char *p);
cs_list_t *cs_list_append(cs_list_t *l, char *p);
cs_list_t *cs_list_clear(cs_list_t *l);
cs_list_t  cs_list_copy(cs_list_t l);
cs_list_t *cs_list_delete(cs_list_t *l, int index);
cs_list_t *cs_list_prepend(cs_list_t *l, char *p);
cs_list_t *cs_list_rem_value(cs_list_t *l, char *p);
cs_list_t *cs_list_rem_value_uniq(cs_list_t *l, char *p);
cs_list_t  cs_list_scramble(cs_list_t l);

#endif //OLYTAG_VECTORS_CS_LIST_H
