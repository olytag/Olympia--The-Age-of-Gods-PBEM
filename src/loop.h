
/*
 *  loop.h -- abstracted loops
 *
 *  or, "Too bad this language doesn't have generators"
 *
 *
 *  Q:  Why abstract loops?  These defines are really gross.
 *
 *  A:  We use abstract data types so that we can change the representation
 *      without having to change all of the code that uses the types.
 *
 *      It is easy to abstract add, delete and fetch operations.  But the
 *      most common operation is to iterate over the elements of a collection.
 *  
 *      The method of iteration is almost certain to change when switching
 *      implementations, too.  List-to-tree, tree-to-bit-array, etc.
 *  
 *      Abstracting loops makes the code cleaner and easier to read, and
 *      easier to change.
 */


/*
 *  Loops below should be free of the "delete problem", i.e.
 *
 *	loop_something(i)
 *	{
 *		delete_something(i);
 *	}
 *	next_something;
 *
 *  should work, i.e. it shouldn't core dump because next(i) is no
 *  longer defined, or go over an element twice, or miss an element.
 */

/*
 *  break and continue should work inside these loops, but don't
 *  return out of them.  Return will bypass the end-of-loop cleanup.
 */

/*
 *  Thanks to the X Window system for going ahead of us and making sure
 *  that most vendor's compilers can handle unreasonably large defines.
 */


#define	loop_all_here(where, i) \
{ int ll_i; \
  int ll_check = 2; \
  ilist ll_l = NULL; \
	all_here(where, &ll_l); \
	for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) \
	{ \
		i = ll_l[ll_i];

#define	next_all_here		} assert(ll_check == 2); ilist_reclaim(&ll_l); }


#define	loop_char_here(where, i) \
{ int ll_i; \
  int ll_check = 13; \
  ilist ll_l = NULL; \
	all_char_here(where, &ll_l); \
	for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) \
	{ \
		i = ll_l[ll_i];

#define	exit_char_here	{ assert(ll_check == 13); ilist_reclaim(&ll_l); }
#define	next_char_here	} assert(ll_check == 13); ilist_reclaim(&ll_l); }



#define	loop_stack(who, i) \
{ int ll_i; \
  int ll_check = 20; \
  ilist ll_l = NULL; \
	all_stack(who, &ll_l); \
	for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) \
	{ \
		i = ll_l[ll_i];

#define	next_stack	} assert(ll_check == 20); ilist_reclaim(&ll_l); }


#define	loop_known(kn, i) \
{ int ll_i; \
  int ll_check = 3; \
  extern int int_comp(); \
	qsort(kn, ilist_len(kn), sizeof(int), int_comp); \
	for (ll_i = 0; ll_i < ilist_len(kn); ll_i++) { \
	    i = kn[ll_i];

#define	next_known	} assert(ll_check == 3); }

/*
 *  Iterate over all valid boxes.  i is instantiated with the entity
 *  numbers.
 */

#define	loop_boxes(i) \
{ int ll_i; \
  int ll_check = 4; \
	for (ll_i = 1; ll_i < MAX_BOXES; ll_i++) \
	    if (kind(ll_i) != T_deleted) \
		{ \
			i = ll_i;

#define	next_box	} assert(ll_check == 4); }


#define	loop_kind(kind, i) \
{ int ll_i, ll_next; \
  int ll_check = 5; \
	ll_next = kind_first(kind); \
	while ((ll_i = ll_next) > 0) { \
		ll_next = kind_next(ll_i); \
		i = ll_i;

#define	next_kind	} assert(ll_check == 5); }


#define	loop_char(i)	loop_kind(T_char, i)
#define	next_char	next_kind

#define	loop_player(i)	loop_kind(T_player, i)
#define	next_player	next_kind

#define loop_nation(i) loop_kind(T_nation, i)
#define next_nation next_kind

#define	loop_loc(i)	loop_kind(T_loc, i)
#define	next_loc	next_kind

#define	loop_item(i)	loop_kind(T_item, i)
#define	next_item	next_kind

#define	loop_exit(i)	loop_kind(T_exit, i)
#define	next_exit	next_kind

#define	loop_skill(i)	loop_kind(T_skill, i)
#define	next_skill	next_kind

#define	loop_gate(i)	loop_kind(T_gate, i)
#define	next_gate	next_kind

#define	loop_ship(i)	loop_kind(T_ship, i)
#define	next_ship	next_kind

#define	loop_post(i)	loop_kind(T_post, i)
#define	next_post	next_kind

#define	loop_storm(i)	loop_kind(T_storm, i)
#define	next_storm	next_kind


#define	loop_subkind(sk, i) \
{ int ll_i, ll_next; \
  int ll_check = 26; \
	ll_next = sub_first(sk); \
	while ((ll_i = ll_next) > 0) { \
		ll_next = sub_next(ll_i); \
		i = ll_i;

#define	next_subkind	} assert(ll_check == 26); }

#define	loop_garrison(i)	loop_subkind(sub_garrison, i)
#define	next_garrison		next_subkind

#define	loop_city(i)		loop_subkind(sub_city, i)
#define	next_city		next_subkind

#define	loop_guild(i)		loop_subkind(sub_guild, i)
#define	next_guild		next_subkind

#define	loop_mountain(i)	loop_subkind(sub_mountain, i)
#define	next_mountain		next_subkind

#define	loop_inn(i)		loop_subkind(sub_inn, i)
#define	next_inn		next_subkind

#define	loop_temple(i)		loop_subkind(sub_temple, i)
#define	next_temple		next_subkind

#define	loop_collapsed_mine(i)	loop_subkind(sub_mine_collapsed, i)
#define	next_collapsed_mine	next_subkind

#define	loop_dead_body(i)	loop_subkind(sub_dead_body, i)
#define	next_dead_body		next_subkind

#define	loop_lost_soul(i)	loop_subkind(sub_lost_soul, i)
#define	next_lost_soul		next_subkind

#define	loop_castle(i)		loop_subkind(sub_castle, i)
#define	next_castle		next_subkind

#define	loop_pl_regular(i)	loop_subkind(sub_pl_regular, i)
#define	next_pl_regular		next_subkind

#define	loop_artifact(i)	loop_subkind(sub_magic_artifact, i)
#define	next_artifact		next_subkind


#define	loop_loc_or_ship(i) \
{ int ll_i; \
  int ll_check = 17; \
  int ll_state = 1; \
	ll_i = kind_first(T_ship); \
	if (ll_i <= 0) { ll_i = kind_first(T_loc); ll_state = 0; } \
	while (ll_i > 0) { \
	    i = ll_i;

#define	next_loc_or_ship \
	ll_i = kind_next(ll_i); \
	if (ll_i <= 0 && ll_state) \
	 { ll_i = kind_first(T_loc); ll_state = 0; } \
    } assert(ll_check == 17); }


#define	loop_province(i) \
{ int ll_i; \
  int ll_check = 6; \
	for (ll_i = kind_first(T_loc); ll_i > 0; ll_i = kind_next(ll_i)) \
	    if (loc_depth(ll_i) == LOC_province) { \
	        i = ll_i;

#define	next_province	} assert(ll_check == 6); }


#define	loop_loc_teach(where, i) \
{ int ll_i; \
  int ll_check = 7; \
  ilist ll_l = NULL; \
	assert(valid_box(where)); \
	if (rp_subloc(where)) \
	    ll_l = ilist_copy(rp_subloc(where)->teaches); \
		for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) { \
		    i = ll_l[ll_i];

#define	next_loc_teach	} assert(ll_check == 7); ilist_reclaim(&ll_l); }


#define	loop_units(pl, i) \
{ int ll_i; \
  ilist ll_l = NULL; \
  int ll_check = 21; \
	if (rp_player(pl)) \
	    ll_l = ilist_copy(rp_player(pl)->units); \
		for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) { \
		    i = ll_l[ll_i];

#define	next_unit	} assert(ll_check == 21); ilist_reclaim(&ll_l); }


#define	loop_here(where, i) \
{ int ll_i; \
  ilist ll_l = NULL; \
  int ll_check = 8; \
	assert(valid_box(where)); \
	if (rp_loc_info(where)) \
	    ll_l = ilist_copy(rp_loc_info(where)->here_list); \
		for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) { \
		    i = ll_l[ll_i];

#define exit_here	{ assert(ll_check == 8); ilist_reclaim(&ll_l); }
#define	next_here	} assert(ll_check == 8); ilist_reclaim(&ll_l); }


#define	loop_gates_here(where, i) \
{ int ll_i; \
  ilist ll_l = NULL; \
  int ll_check = 18; \
	assert(valid_box(where)); \
	if (rp_loc_info(where)) \
	    ll_l = ilist_copy(rp_loc_info(where)->here_list); \
		for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) \
		    if (kind(ll_l[ll_i]) == T_gate) { \
			    i = ll_l[ll_i];

#define	next_gate_here	} assert(ll_check == 18); ilist_reclaim(&ll_l); }


#define	loop_exits_here(where, i) \
{ int ll_i; \
  int ll_check = 10; \
  ilist ll_l = NULL; \
	assert(valid_box(where)); \
	if (rp_loc(where)) \
	    ll_l = ilist_copy(rp_loc(where)->exits_here); \
		for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) { \
		    i = ll_l[ll_i];

#define	next_exit_here	} assert(ll_check == 10); ilist_reclaim(&ll_l); }


/*
 *  Iterate struct item_ent *e over who's inventory
 */

#define	loop_inv(who, e) \
{ int ll_i; \
  int ll_check = 11; \
  struct item_ent ll_copy; \
  struct item_ent **ll_l = NULL; \
    assert(valid_box(who)); \
    ll_l = (struct item_ent **) ilist_copy((ilist) bx[who]->items); \
	for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) \
	    if (valid_box(ll_l[ll_i]->item) && ll_l[ll_i]->qty > 0) { \
		    ll_copy = *ll_l[ll_i]; \
		    e = &ll_copy;

#define	next_inv   } assert(ll_check == 11); ilist_reclaim((ilist *) &ll_l); }



#define	loop_char_skill(who, e) \
{ int ll_i; \
  int ll_check = 15; \
  struct skill_ent **ll_l = NULL; \
    assert(valid_box(who)); \
    if (rp_char(who)) \
       ll_l = (struct skill_ent **) ilist_copy((ilist) rp_char(who)->skills); \
       for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) { \
	     e = ll_l[ll_i];

#define	next_char_skill \
		} assert(ll_check == 15); ilist_reclaim((ilist *) &ll_l); }



#define	loop_char_skill_known(who, e) \
{ int ll_i; \
  int ll_check = 16; \
  struct skill_ent **ll_l = NULL; \
    assert(valid_box(who)); \
    if (rp_char(who)) \
       ll_l = (struct skill_ent **) ilist_copy((ilist) rp_char(who)->skills); \
       for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) \
          if (ll_l[ll_i]->know == SKILL_know) { \
		e = ll_l[ll_i];

#define	exit_char_skill_known \
		{ assert(ll_check == 16); ilist_reclaim((ilist *) &ll_l); }
#define	next_char_skill_known \
		} assert(ll_check == 16); ilist_reclaim((ilist *) &ll_l); }


#define	loop_trade(who, e) \
{ int ll_i; \
  int ll_check = 19; \
  struct trade **ll_l = NULL; \
    assert(valid_box(who)); \
    ll_l = (struct trade **) ilist_copy((ilist) bx[who]->trades); \
	for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) \
	    if (valid_box(ll_l[ll_i]->item) && ll_l[ll_i]->qty > 0) { \
		    e = ll_l[ll_i];

#define	next_trade  } assert(ll_check == 19); ilist_reclaim((ilist *) &ll_l); }


#define	loop_prov_dest(where, i) \
{ int ll_i; \
  int ll_check = 23; \
  struct entity_loc *ll_p; \
	assert(loc_depth(where) == LOC_province); \
	ll_p = rp_loc(where); \
	assert(ll_p); \
	for (ll_i = 0; ll_i < ilist_len(ll_p->prov_dest); ll_i++) \
	{ \
		i = ll_p->prov_dest[ll_i];

#define	next_prov_dest		} assert(ll_check == 23); }


#if 0
#define	loop_loc_owner(where, i) \
{ int ll_i, ll_next; \
  int ll_check = 25; \
	ll_next = province_admin(where); \
	while ((ll_i = ll_next) > 0) { \
		ll_next = char_pledge(ll_i); \
		i = ll_i;

#define	next_loc_owner	} assert(ll_check == 25); }
#endif

/*
 *  This loop sorts the player's inventory according to the
 *  attack && defense values, so that we pick up the toughest
 *  fighters first when going through the inventory.
 *
 *  Note that it really belongs in loop.h.
 *
 */
#define	loop_sorted_inv(who, e) \
{ int ll_i; \
  int ll_check = 27; \
  extern int combat_comp(); \
  struct item_ent ll_copy; \
  struct item_ent **ll_l = NULL; \
    assert(valid_box(who)); \
    ll_l = (struct item_ent **) ilist_copy((ilist) bx[who]->items); \
    qsort(ll_l, ilist_len(ll_l), sizeof(int), combat_comp); \
	for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) \
	    if (valid_box(ll_l[ll_i]->item) && ll_l[ll_i]->qty > 0) { \
		    ll_copy = *ll_l[ll_i]; \
		    e = &ll_copy;

#define	next_sorted_inv   } assert(ll_check == 27); ilist_reclaim((ilist *) &ll_l); }

/*
 *  Loop over a priest's followers.
 *
 */
#define	loop_followers(priest, i) \
{ int ll_i; \
  ilist ll_l = NULL; \
  int ll_check = 28; \
	if (rp_char(priest) && is_priest(priest)) \
	    ll_l = ilist_copy(rp_char(priest)->religion.followers); \
		for (ll_i = 0; ll_i < ilist_len(ll_l); ll_i++) { \
		    i = ll_l[ll_i];

#define	next_follower	} assert(ll_check == 28); ilist_reclaim(&ll_l); }

