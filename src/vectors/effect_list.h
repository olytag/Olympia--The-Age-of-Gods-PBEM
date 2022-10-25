// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_EFFECT_LIST_H
#define OLYTAG_VECTORS_EFFECT_LIST_H

typedef struct effect *effect_ptr;
typedef effect_ptr *effect_list_t;

int        effect_list_cap(effect_list_t);
int        effect_list_len(effect_list_t);
int        effect_list_lookup(effect_list_t, effect_ptr);
void       effect_list_reclaim(effect_list_t*);
effect_list_t *effect_list_add(effect_list_t*, effect_ptr);
effect_list_t *effect_list_append(effect_list_t*, effect_ptr);
effect_list_t *effect_list_clear(effect_list_t*);
effect_list_t  effect_list_copy(effect_list_t);
effect_list_t *effect_list_delete(effect_list_t*, int index);
effect_list_t *effect_list_prepend(effect_list_t*, effect_ptr);
effect_list_t *effect_list_rem_value(effect_list_t*, effect_ptr);
effect_list_t *effect_list_rem_value_uniq(effect_list_t*, effect_ptr);
effect_list_t  effect_list_scramble(effect_list_t);

#endif //OLYTAG_VECTORS_EFFECT_LIST_H
