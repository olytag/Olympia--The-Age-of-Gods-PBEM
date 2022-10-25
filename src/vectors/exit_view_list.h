// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_EXIT_VIEW_LIST_H
#define OLYTAG_VECTORS_EXIT_VIEW_LIST_H

typedef struct exit_view *ev_ptr;
typedef ev_ptr *ev_list_t;

int        ev_list_cap(ev_list_t);
int        ev_list_len(ev_list_t);
int        ev_list_lookup(ev_list_t, ev_ptr);
void       ev_list_reclaim(ev_list_t*);
ev_list_t *ev_list_add(ev_list_t*, ev_ptr);
ev_list_t *ev_list_append(ev_list_t*, ev_ptr);
ev_list_t *ev_list_clear(ev_list_t*);
ev_list_t  ev_list_copy(ev_list_t);
ev_list_t *ev_list_delete(ev_list_t*, int index);
ev_list_t *ev_list_prepend(ev_list_t*, ev_ptr);
ev_list_t *ev_list_rem_value(ev_list_t*, ev_ptr);
ev_list_t *ev_list_rem_value_uniq(ev_list_t*, ev_ptr);
ev_list_t  ev_list_scramble(ev_list_t);

#endif //OLYTAG_VECTORS_EXIT_VIEW_LIST_H
