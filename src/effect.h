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
#define	loop_effects(what, e) \
{ int ll_i; \
  ilist ll_l = NULL, el = NULL; \
  int ll_check = 29; \
  el = (ilist) effects(what); \
  ll_l = ilist_copy(el); \
  for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) {  \
    e = (struct effect *) ll_l[ll_i];

#define	next_effect	} assert(ll_check == 29); ilist_reclaim(&ll_l); }
