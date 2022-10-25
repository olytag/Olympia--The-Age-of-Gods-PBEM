// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_VECTORS_ORDER_LIST_LIST_H
#define OLYTAG_VECTORS_ORDER_LIST_LIST_H

typedef struct order_list *order_list_ptr;
typedef order_list_ptr *order_list_list_t;

int        order_list_list_cap(order_list_list_t);
int        order_list_list_len(order_list_list_t);
int        order_list_list_lookup(order_list_list_t, order_list_ptr);
void       order_list_list_reclaim(order_list_list_t*);
order_list_list_t *order_list_list_add(order_list_list_t*, order_list_ptr);
order_list_list_t *order_list_list_append(order_list_list_t*, order_list_ptr);
order_list_list_t *order_list_list_clear(order_list_list_t*);
order_list_list_t  order_list_list_copy(order_list_list_t);
order_list_list_t *order_list_list_delete(order_list_list_t*, int index);
order_list_list_t *order_list_list_prepend(order_list_list_t*, order_list_ptr);
order_list_list_t *order_list_list_rem_value(order_list_list_t*, order_list_ptr);
order_list_list_t *order_list_list_rem_value_uniq(order_list_list_t*, order_list_ptr);
order_list_list_t  order_list_list_scramble(order_list_list_t);

#endif //OLYTAG_VECTORS_ORDER_LIST_LIST_H
