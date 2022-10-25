// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_EFFECT_H
#define OLYTAG_EFFECT_H

/*
 *  EFFECT.H
 *  Tue Aug  6 12:09:42 1996 -- Scott Turner
 *
 */
extern int get_all_effects(int what, int t, int st, int v);

extern int get_effect(int what, int t, int st, int v);

extern void update_effects(int what);

extern int add_effect(int what, int t, int st, int duration, int value);

extern void delete_effect(int what, int type, int st);

/*
 *  Loop over the effects on something.
 *
 *  What = int
 *  e = struct effect *;
 *
  if (rp_char(what)) { \
    el = (ilist) rp_char(what)->effects; \
  } else if (rp_loc(what)) { \
    el = (ilist) rp_loc(what)->effects; \
  } else if (rp_subloc(what)) { \
    el = (ilist) rp_subloc(what)->effects; \
  }; \
 * 
 */
#define    loop_effects(what, e) \
{ int ll_check = 29; \
  effect_list_t el = effects(what); \
  effect_list_t ll_l = effect_list_copy(el); \
  for (int ll_i = 0; ll_i < effect_list_len(ll_l); ll_i++) {  \
    (e) = ll_l[ll_i];

#define    next_effect    } assert(ll_check == 29); effect_list_reclaim(&ll_l); }

#endif //OLYTAG_EFFECT_H
