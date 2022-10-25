// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_SKILL_ENT_LIST_H
#define OLYTAG_VECTORS_SKILL_ENT_LIST_H

typedef struct skill_ent *skill_ent_ptr;
typedef skill_ent_ptr *skill_ent_list_t;

int        skill_ent_list_cap(skill_ent_list_t);
int        skill_ent_list_len(skill_ent_list_t);
int        skill_ent_list_lookup(skill_ent_list_t, skill_ent_ptr);
void       skill_ent_list_reclaim(skill_ent_list_t*);
skill_ent_list_t *skill_ent_list_add(skill_ent_list_t*, skill_ent_ptr);
skill_ent_list_t *skill_ent_list_append(skill_ent_list_t*, skill_ent_ptr);
skill_ent_list_t *skill_ent_list_clear(skill_ent_list_t*);
skill_ent_list_t  skill_ent_list_copy(skill_ent_list_t);
skill_ent_list_t *skill_ent_list_delete(skill_ent_list_t*, int index);
skill_ent_list_t *skill_ent_list_prepend(skill_ent_list_t*, skill_ent_ptr);
skill_ent_list_t *skill_ent_list_rem_value(skill_ent_list_t*, skill_ent_ptr);
skill_ent_list_t *skill_ent_list_rem_value_uniq(skill_ent_list_t*, skill_ent_ptr);
skill_ent_list_t  skill_ent_list_scramble(skill_ent_list_t);

#endif //OLYTAG_VECTORS_SKILL_ENT_LIST_H
