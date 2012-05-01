
#include	<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include	<string.h>
#include	<stdlib.h>
#include	"z.h"
#include	"oly.h"


/*
 *  u.c -- the useful function junkyard
 */


/*
 *  Mon Jun 16 11:45:05 1997 -- Scott Turner
 *
 *  True if you have to cross an ocean between the two
 *  locs; cheats right now by simply seeing if they're in
 *  the same region.  This works if you define the map correctly :-)
 *
 */
int
crosses_ocean(int a, int b)
{
  if (region(a) != region(b)) return TRUE;
  return FALSE;
};


void
kill_stack_ocean(int who)
{
	static ilist l = NULL;
	int i;
	int where;

	ilist_clear(&l);

	loop_stack(who, i)
	{
		ilist_append(&l, i);
	}
	next_stack;

	for (i = ilist_len(l) - 1; i >= 0; i--)
	{
		kill_char(l[i], 0, S_body);

		if (kind(l[i]) == T_char)	/* not dead yet! */
		{
			extract_stacked_unit(l[i]);
			where = find_nearest_land(province(l[i]));

			out(l[i], "%s washed ashore at %s.",
						box_name(l[i]),
						box_name(where));

			log_output(LOG_SPECIAL, "kill_stack_ocean, swam "
				"ashore, who=%s", box_code_less(l[i]));
			move_stack(l[i], where);
		}
	}
}


int
survive_fatal(int who)
{

	if (forget_skill(who, sk_survive_fatal) ||
	    forget_skill(who, sk_survive_fatal2))
	{
		wout(who, "%s would have died, but survived a fatal wound!",
						box_name(who));
		wout(who, "Forgot %s.", box_code(sk_survive_fatal));
		wout(who, "Health is now 100.");

		p_char(who)->health = 100;
		p_char(who)->sick = FALSE;

		if (options.survive_np && skill_np_req(sk_survive_fatal)) {
		  wout(who, "Received back %d noble points.",
		       skill_np_req(sk_survive_fatal));
		  add_np(player(who), skill_np_req(sk_survive_fatal));
		};

		return TRUE;
	}

	return FALSE;
}


void
char_reclaim(int who)
{

	p_char(who)->melt_me = TRUE;
#if 0
	kill_char(who, MATES);
#else
	kill_char(who, 0, S_body);	/* QUIT shouldn't give items to stackmates */
#endif
}


int
v_reclaim(struct command *c)
{
	char *what;

	if (numargs(c) < 1 || c->parse[1] == NULL || *(c->parse[1]) == '\0')
		what = "disperses.";
	else
		what = c->parse[1];

	wout(subloc(c->who), "%s %s", box_name(c->who), what);
	char_reclaim(c->who);
	return TRUE;
}


int
new_char(int sk, int ni, int where, int health, int pl,
					int loy_kind, int loy_lev, char *name)
{
	int new;
	struct entity_char *p;

	new = new_ent(T_char, sk);

	if (new < 0)
		return -1;

	if (name && *name)
		set_name(new, name);
	p = p_char(new);
	p->health = health;
	p->unit_item = ni;
	p->break_point = 50;

	if (ni && (item_attack(ni) || item_defense(ni))) {
	  p_char(new)->attack = item_attack(ni);
	  rp_char(new)->defense = item_defense(ni);
	} else if (sk == sub_garrison) {
	  p_char(new)->attack = 0;
	  rp_char(new)->defense = 0;
	} else {
	  p_char(new)->attack = 60;
	  rp_char(new)->defense = 60;
	};

	/*
	 *  Set NPC program?
	 *
	 */
	if (ni && item_prog(ni)) {
	  rp_char(new)->npc_prog = item_prog(ni);
	};

	if (is_loc_or_ship(where))
		set_where(new, where);
	else
		set_where(new, subloc(where));

	set_lord(new, pl, loy_kind, loy_lev);

	if (kind(where) == T_char)
		join_stack(new, where);

	if (beast_capturable(new) || is_npc(new))
		p->break_point = 0;

	return new;
}


int
loc_depth(n)
{

	switch (subkind(n))
	{
	case sub_region:
				return LOC_region;

	case sub_ocean:
	case sub_forest:
	case sub_plain:
	case sub_mountain:
	case sub_desert:
	case sub_swamp:
	case sub_under:
	case sub_cloud:
	case sub_mine_shaft:
				return LOC_province;

	case sub_island:
	case sub_stone_cir:
	case sub_mallorn_grove:
	case sub_bog:
	case sub_cave:
	case sub_city:
	case sub_city_notdone:
	case sub_lair:
	case sub_graveyard:
	case sub_ruins:
	case sub_battlefield:
	case sub_ench_forest:
	case sub_rocky_hill:
	case sub_tree_circle:
	case sub_pits:
	case sub_pasture:
	case sub_oasis:
	case sub_yew_grove:
	case sub_sand_pit:
	case sub_sacred_grove:
	case sub_poppy_field:
	case sub_faery_hill:
	case sub_hades_pit:
	case sub_mine:
	case sub_mine_notdone:
	case sub_mine_collapsed:
				return LOC_subloc;

	case sub_guild:
	case sub_temple:
	case sub_galley:
	case sub_roundship:
	case sub_ship:
	case sub_castle:
	case sub_galley_notdone:
	case sub_roundship_notdone:
	case sub_ship_notdone:
	case sub_ghost_ship:
	case sub_temple_notdone:
	case sub_inn:
	case sub_inn_notdone:
	case sub_castle_notdone:
	case sub_tower:
	case sub_tower_notdone:
	case sub_mine_shaft_notdone:
	case sub_orc_stronghold:
	case sub_orc_stronghold_notdone:
				return LOC_build;

	default:
		fprintf(stderr, "subkind is %d\n", subkind(n));
		assert(FALSE);
	}
}


/*
 *  First try to give items to someone below, then to someone above.
 */

int
stackmate_inheritor(int who)
{
	int i;
	int target = 0;

	loop_here(who, i)
	{
		if (kind(i) == T_char && !is_prisoner(i))
		{
			target = i;
			break;
		}
	}
	next_here;

	if (target)
		return target;

	return stack_parent(who);
}


void
take_unit_items(int from, int inherit, int how_many)
{
	int to;
	struct item_ent *e;
	int first = TRUE;
	int qty;
	int silent;
	int i;
	extern int gold_combat;
	extern int gold_combat_indep;

	switch (inherit)
	{
	case 0:
		to = 0;
		silent = TRUE;
		break;

	case MATES:
		to = stackmate_inheritor(from);
		silent = FALSE;
		break;

	case MATES_SILENT:
		to = stackmate_inheritor(from);
		silent = TRUE;
		break;

	default:
		to = inherit;
		silent = FALSE;
	}

	if (how_many == TAKE_NI)
		gen_item(from, noble_item(from), 1);

	loop_inv(from, e)
	{
	  /*
	   *  Thu Mar 29 12:30:23 2001 -- Scott Turner
	   *
	   *  An auraculum should stay with its maker if he dies.
	   *
	   */
	  if (how_many == TAKE_SOME &&
	      e->item == char_auraculum(from)) continue;

	  /*
	   *  Ungiveable items can't be taken this way.
	   *
	   *  Mon May  3 08:49:42 1999 -- Scott Turner
	   *
	   *  We can't just continue here; we need to get rid
	   *  of the ungiveables.
	   *
	   */
	  if (rp_item(e->item) &&
	      rp_item(e->item)->ungiveable) {
	    move_item(from, 0, e->item, e->qty);
	    continue;
	  };

	  /*
	   *  Wed Mar 12 11:56:30 1997 -- Scott Turner
	   *
	   *  No beast transfers this way?
	   *
	   *  Mon May  3 08:51:30 1999 -- Scott Turner
	   *
	   *  Turn these into monster stacks.
	   *
	   */
	  if (item_capturable(e->item) && e->item != noble_item(from)) {
	    create_monster_stack(e->item, e->qty, subloc(from));
	    move_item(from, 0, e->item, e->qty);
	    continue;
	  };
	  
	  /*
	   *  Wed Mar 12 11:58:54 1997 -- Scott Turner
	   *
	   *  Only some trained men will come over.  att+def = 10
	   *  is the 50/50 point, so the chance to come over is:
	   *
	   *               5/(att+def)
	   *
	   */
	  if (item_attack(e->item) || item_defense(e->item)) {
	    qty = 0;
	    for(i=0;i<e->qty;i++)
	      if (rnd(1,100) <
		  (500/(item_attack(e->item) + item_defense(e->item))))
		qty++;
	  } else {
	    qty = e->qty;
	  };

	  /*
	   *  Don't let beasts grab men this way.
	   *
	   */
	  if (man_item(e->item) && subkind(to) == sub_ni) qty = 0;

	  if (how_many == TAKE_SOME && rnd(1,2) == 1)
			qty = rnd(qty/2, qty);

/*
 *  Don't let unique items get dropped this way
 */
	  if (qty == 0 && item_unique(e->item))
			qty = 1;

	  if (qty > 0 && !silent && valid_box(to))
		{
			if (first)
			{
				first = FALSE;
				wout(to, "Taken from %s:", box_name(from));
				indent += 3;
			}

			wout(to, "%s", box_name_qty(e->item, qty));
		}

	  move_item(from, to, e->item, qty);

	  if (e->item == item_gold && player(from) != player(to) &&
	      player(to) > 1000)
		{
			if (player(from) < 1000)
				gold_combat_indep += qty;
			else 
				gold_combat += qty;
		}

	  if (qty != e->qty)
			move_item(from, 0, e->item, e->qty - qty);
	}
	next_inv;


/*
 *  Now give prisoners too
 */

	loop_here(from, i)
	{
		if (kind(i) == T_char && is_prisoner(i))
		{
			if (to > 0)
			{
				if (first && !silent)
				{
				    wout(to, "Taken from %s:", box_name(from));
				    indent += 3;
				    first = FALSE;
				}

				move_prisoner(from, to, i);

				if (!silent)
					wout(to, "%s", liner_desc(i));

				if (player(i) == player(to))
					p_char(i)->prisoner = FALSE;
			}
			else
			{
				p_magic(i)->swear_on_release = FALSE;
				drop_stack(from, i);
			}
		}
	}
	next_here;

	if (!first && !silent)
		indent -= 3;
}


void
add_char_damage(int who, int amount, int inherit)
{
	struct entity_char *p;

	if (amount <= 0)
		return;

	p = p_char(who);

	if (p->health == -1)
	{
		if (amount >= 50)
			kill_char(who, inherit, S_body);
		return;
	}

	if (p->health > 0)
	{
		if (amount > p->health)
			amount = p->health;

		p->health -= amount;
		assert(p->health >= 0);

		wout(who, "%s is wounded.  Health is now %d.",
					box_name(who), p->health);
	}

	if (p->health <= 0)
	{
		kill_char(who, inherit, S_body);
	}
	/*
	 *  Wed Nov 25 12:51:14 1998 -- Scott Turner
	 *
	 *  Hero skill "Avoid Illness" can help negate this.
	 *
	 */
	else if (!p->sick &&
		 rnd(1,100) > p->health &&
#ifdef HERO
		 rnd(1,100) > min((2*skill_exp(who, sk_avoid_illness)), 80) &&
#endif // HERO		 
		 !has_artifact(who, ART_SICKNESS, 0, 0, 0)) 
	{
		p->sick = TRUE;
		wout(who, "%s has fallen ill.", box_name(who));
	}
}


void
put_back_cookie(int who)
{
	struct entity_misc *p;

	p = rp_misc(who);

	if (p == NULL || p->npc_home == 0)
		return;

	gen_item(p->npc_home, p->npc_cookie, 1);
}



/*
 *  Has a contacted b || has b found a?
 */

int
contacted(int a, int b)
{
	struct entity_char *p;

	p = p_char(a);

	if (ilist_lookup(p->contact, b) >= 0)
		return TRUE;

	if (ilist_lookup(p->contact, player(b)) >= 0)
		return TRUE;

	return FALSE;
}


int
char_here(int who, int target)
{
	int where = subloc(who);
	int pl;

	if (where != subloc(target))
		return FALSE;

	if (char_really_hidden(target))
	{
		pl = player(who);
		if (pl == player(target))
			return TRUE;

		if (contacted(target, who))
			return TRUE;

		return FALSE;
	}

	return TRUE;
}


int
check_char_here(int who, int target)
{

	if (target == garrison_magic)
	{
		wout(who, "There is no garrison here.");
		return FALSE;
	}

	if (kind(target) != T_char || !char_here(who, target))
	{
		wout(who, "%s is not here.", box_code(target));
		return FALSE;
	}

	return TRUE;
}


int
check_char_gone(int who, int target)
{

	if (target == garrison_magic)
	{
		wout(who, "There is no garrison here.");
		return FALSE;
	}

	if (kind(target) != T_char)
	{
		wout(who, "%s is not a character.", box_code(target));
		return FALSE;
	}

	if (!char_here(who, target))
	{
		wout(who, "%s can not be seen here.", box_code(target));
		return FALSE;
	}

	if (char_gone(target))
	{
		wout(who, "%s has left.", box_name(target));
		return FALSE;
	}

	return TRUE;
}


int
check_still_here(int who, int target)
{

	if (target == garrison_magic)
	{
		wout(who, "There is no garrison here.");
		return FALSE;
	}

	if (kind(target) != T_char)
	{
		wout(who, "%s is not a character.", box_code(target));
		return FALSE;
	}

	if (!char_here(who, target))
	{
		wout(who, "%s can no longer be seen here.", box_name(target));
		return FALSE;
	}

#if 0
	if (char_gone(target))
	{
		wout(who, "%s is no longer here.", box_name(target));
		return FALSE;
	}
#endif

	return TRUE;
}


int
check_skill(int who, int skill)
{

	if (has_skill(who, skill) < 1)
	{
		wout(who, "Requires %s.", box_name(skill));
		return FALSE;
	}

	return TRUE;
}


static void
sink_ship(int ship)
{
	int who;
	struct entity_subloc *p;
	int i;
	int storm;
	int where = subloc(ship);

	log_output(LOG_SPECIAL, "%s has sunk in %s.", box_name(ship),
					box_name(subloc(ship)));

	wout(ship, "%s has sunk!", box_name(ship));
	wout(subloc(ship), "%s has sunk!", box_name(ship));

	if (subkind(where) == sub_ocean)
	{
		loop_here(ship, who)
		{
			if (kind(who) == T_char)
				kill_stack_ocean(who);
		}
		next_here;
	}
	else
	{
		loop_here(ship, who)
		{
			if (kind(who) == T_char)
				move_stack(who, where);
			else
				set_where(who, where);
		}
		next_here;
	}

#if 0
/*
 *  Unbind any storms bound to this ship
 */

	p = rp_subloc(ship);
	if (p)
	{
		for (i = 0; i < ilist_len(p->bound_storms); i++)
		{
			storm = p->bound_storms[i];
			if (kind(storm) == T_storm)
				p_misc(storm)->bind_storm = 0;
		}

		ilist_clear(&p->bound_storms);
	}
#endif

	set_where(ship, 0);
	delete_box(ship);
}


void
get_rid_of_collapsed_mine(int fort)
{
	int who;
	int where = subloc(fort);

	assert(subkind(fort) == sub_mine_collapsed);

/*
 *  Move anything inside, out, just in case
 */

	loop_here(fort, who)
	{
		if (kind(who) == T_char)
			move_stack(who, where);
		else
			set_where(who, where);
	}
	next_here;

	set_where(fort, 0);
	delete_box(fort);
}


static void
building_collapses(int fort)
{
	int who;
	int where = subloc(fort);

	log_output(LOG_SPECIAL, "%s collapsed in %s.", box_name(fort),
					box_name(where));

	vector_char_here(fort);
	vector_add(where);
	wout(VECT, "%s collapses!", box_name(fort));

	loop_here(fort, who)
	{
		if (kind(who) == T_char)
			move_stack(who, where);
		else
			set_where(who, where);
	}
	next_here;

	if (subkind(fort) == sub_mine)
	{
		change_box_subkind(fort, sub_mine_collapsed);
		p_misc(fort)->mine_delay = 8;
		return;
	}

	if (subkind(fort) == sub_castle)
	{
		int i;

		loop_garrison(i)
		{
			if (garrison_castle(i) == fort)
				p_misc(i)->garr_castle = 0;
		}
		next_garrison;
	}

	set_where(fort, 0);
	delete_box(fort);
}


int
add_structure_damage(int fort, int damage)
{
	struct entity_subloc *p;

	assert(damage >= 0);

	p = p_subloc(fort);

	if (p->damage + damage > 100)
		p->damage = 100;
	else
		p->damage += damage;

	if (p->damage < 100)
		return FALSE;

/*
 *  Completely destroyed
 */

	if (is_ship(fort))
		sink_ship(fort);
	else
		building_collapses(fort);

	return TRUE;
}

/*
 *  Wed Dec  1 18:36:53 1999 -- Scott Turner
 *
 *  Counting function.  Count the items in a unit/stack that
 *  meet the accepatance function.
 *
 */
int
count_generic(int who, int stack, int (*func)(int))
{
  int sum = 0, i;
  struct item_ent *e;
  
  if (stack) {
    loop_char_here(who, i) {
      sum += count_generic(who, 0, func);
    } next_char_here;
  } else {
    sum += func(who);
    loop_inv(who, e) {
      if (func(e->item))
	sum += e->qty;
    } next_inv;
  };
  return sum;
};

static
int is_man_item(int item)
{
  return man_item(item);
};

int
count_man_items(int who)
{
	struct item_ent *e;
	int sum = 0;

#if 1
	if (subkind(who) == sub_garrison)
		sum = 0;
#endif


#if 0
	loop_inv(who, e)
	{
		if (man_item(e->item))
			sum += e->qty;
	}
	next_inv;

	return sum;
#endif

	return count_generic(who, 0, is_man_item);
}


int
count_stack_units(int who)
{
	int i;
	int sum = 1;

	loop_char_here(who, i)
	{
		sum++;
	}
	next_char_here;

	return sum;
}


int
count_stack_figures(int who)
{
	int i;
	int sum = 0;

	loop_stack(who, i)
	{
		sum += count_man_items(i);
	}
	next_stack;

	return sum;
}

int
count_fighters_2(int who, int attack_min)
{
  struct item_ent *e;
  int beasts = 0, men = 0;
  int man_limit = calc_man_limit(who, FALSE);
  int beast_limit = calc_beast_limit(who, FALSE);

  loop_inv(who, e) {
    if (item_attack(e->item) >= attack_min)
      if (item_animal(e->item))
	beasts += e->qty;
      else
	men += e->qty;
  } next_inv;

  if (beasts > beast_limit) beasts = beast_limit;
  if (men > man_limit) men = man_limit;

  return beasts + men;
}

int
count_fighters(int who, int attack_min)
{
	int i;
	int sum = 0;

	loop_stack(who, i)
	{
		sum += count_fighters_2(i, attack_min);
	}
	next_stack;

	return sum;
}
/*
 *  Mon Oct 28 16:16:54 1996 -- Scott Turner
 *
 *  Ninjas don't "count" :-).  Neither do angels.
 *
 *  Mon Jan  4 08:16:19 1999 -- Scott Turner
 *
 *  Or golems :-).
 *
 */
int
count_any_real(int who, int ignore_ninjas, int ignore_angels)
{
	struct item_ent *e;
	int sum = 1;

#if 1
	if (subkind(who) == sub_garrison)
		sum = 0;
#endif

	loop_inv(who, e)
	{
	  if ((ignore_ninjas && e->item == item_ninja) ||
	      (ignore_angels &&
	       e->item == item_angel ||
	       e->item == item_dirt_golem ||
	       e->item == item_flesh_golem ||
	       e->item == item_iron_golem)) continue;

	  if (man_item(e->item) || is_fighter(e->item))
	    sum += e->qty;
	}
	next_inv;

	return sum;
}

int
count_any(int who)
{
  return count_any_real(who, TRUE, TRUE);
};

int
count_stack_any_real(int who, int ignore_ninjas, int ignore_angels)
{
	int i;
	int sum = 0;

	loop_stack(who, i)
	{
		sum += count_any_real(i, ignore_ninjas, ignore_angels);
	}
	next_stack;

	return sum;
}

int
count_stack_any(int who)
{
  return count_stack_any_real(who, TRUE, TRUE);
};

int
count_loc_char_item(int where, int item)
{
	int i;
	int sum = 0;

	loop_char_here(where, i)
	{
		sum += has_item(i, item);
	}
	next_char_here;

	return sum;
}


void
clear_temps(int kind)
{
	int i;

	loop_kind(kind, i)
	{
		bx[i]->temp = 0;
	}
	next_kind;
}


void
olytime_increment(olytime *p)
{

	p->days_since_epoch++;
	p->day++;
}


/*
 *  Ready counter for next turn
 *  Must be followed by an olytime_increment
 */

void
olytime_turn_change(olytime *p)
{

	p->day = 0;
	p->turn++;
}


int
max(int a, int b)
{

	if (a > b)
		return a;
	return b;
}


int
min(int a, int b)
{

	if (a < b)
		return a;
	return b;
}


/*
 *  Olympian weight system
 *
 *	Each item has three fields related to weight and carrying
 *	capacity:
 *
 *		weight			fetch with item_weight(item)
 *		land capacity		fetch with land_cap(item)
 *		ride capacity		fetch with ride_cap(item)
 *
 *	Weight is the complete weight of the item, such as 100 for
 *	men, or 1,000 for oxen.
 *
 *	land capacity is how much the item can carry walking,
 *	not counting its own weight.
 *
 *	ride capacity is how much the item can carry on horseback,
 *	not counting its own weight.
 *
 *	if the item can carry itself riding or walking, but can not
 *	carry any extra goods, set the capacity to -1.  This is because
 *	0 represents "not set" instead of a value.
 *
 *	For example, a wild horse can walk and ride, but cannot be laden
 *	with rider or inventory.  Therefore, its land_cap is -1, and its
 *	ride_cap is -1.
 *
 *	An ox can carry great loads: land_cap 1500.  Perhaps it can trot
 *	alongside horses, but can carry no inventory if doing so.
 *	ride_cap -1.
 *
 */

/*
 *  Fri Oct  9 18:45:42 1998 -- Scott Turner
 *
 *  item_ride_cap and item_fly_cap can no longer be macros because
 *  we need to factor in artifacts.  They are also being changed to
 *  take T_char as well as T_item.
 *
 */
int
item_ride_cap(int who)
{
  int a;
  int base = who;
  int cap = 0;

  if (kind(who) == T_char) base = noble_item(who);
  if (!base) base = item_peasant;
  cap = rp_item(base) ? rp_item(base)->ride_cap : 0;
  
  if (a=best_artifact(who, ART_RIDING, 0, 0)) {
    cap += rp_item_artifact(a)->param1;
  };
  return cap;
};

int
item_fly_cap(int who)
{
  int a;
  int base = who;
  int cap = 0;

  if (kind(who) == T_char) base = noble_item(who);
  if (!base) base = item_peasant;
  cap = rp_item(base) ? rp_item(base)->fly_cap : 0;
  
  if (a=best_artifact(who, ART_FLYING, 0, 0)) {
    cap += rp_item_artifact(a)->param1;
  };
  return cap;
};

int
item_land_cap(int who)
{
  int a;
  int base = who;
  int cap = 0;

  if (kind(who) == T_char) base = noble_item(who);
  if (!base) base = item_peasant;
  cap = rp_item(base) ? rp_item(base)->land_cap : 0;
  
  if (a=best_artifact(who, ART_CARRY, 0, 0)) {
    cap += rp_item_artifact(a)->param1;
  };
  return cap;
};

int
item_weight(int who)
{
  int a;
  int base = who;
  int cap = 0;

  if (kind(who) == T_char) base = noble_item(who);
  if (!base) base = item_peasant;
  cap = rp_item(base) ? rp_item(base)->weight : 0;
  
  /*
   *  Sun Feb 16 22:18:31 1997 -- Scott Turner
   *
   *  Check for a potion of weightlessness.
   *
   */
  if (get_effect(who, ef_weightlessness, 0, 0)) {
    cap -= 500;
  };
  
  /*
   *  And an artifact of weightlessness.
   *
   */
  if (is_artifact(who) &&
      rp_item_artifact(who)->type == ART_WEIGHTLESS) {
    cap -= rp_item_artifact(who)->param1;
  };
  
  return cap;
};

static void
add_item_weight(int item, int qty, struct weights *w, int mountains)
{
  int wt, lc, rc, fc;

  wt = item_weight(item) * qty;
  lc = item_land_cap(item);
  rc = item_ride_cap(item);
  fc = item_fly_cap(item);

  if (lc)
    w->land_cap += max(lc, 0) * qty;
  else
    w->land_weight += wt;

  /*
   *  Tue Dec  8 17:56:36 1998 -- Scott Turner
   *
   *  New wagons don't "ride" in the mountains.
   *
   */
  if (rc && (item == item_new_wagon || item == item_war_wagon) &&
      mountains)
    rc = 0;

  if (rc)
    w->ride_cap += max(rc, 0) * qty;
  else
    w->ride_weight += wt;

  if (fc)
    w->fly_cap += max(fc, 0) * qty;
  else
    w->fly_weight += wt;

  w->total_weight += wt;

  if (item_animal(item))
    w->animals += qty;
}


void
determine_unit_weights(int who, struct weights *w, int mountains)
{
  struct item_ent *e;
  int unit_base;

  assert(kind(who) == T_char);

  bzero(w, sizeof(*w));

  /*
  unit_base = noble_item(who);
  if (unit_base == 0)
    unit_base = item_peasant;
  */

  add_item_weight(who, 1, w, mountains);

  loop_inv(who, e)
    {
      add_item_weight(e->item, e->qty, w, mountains);
    }
  next_inv;

}


void
determine_stack_weights(int who, struct weights *w, int mountains)
{
	struct weights v;
	int i;

	determine_unit_weights(who, w, mountains);

	loop_all_here(who, i) {
		determine_unit_weights(i, &v, mountains);
		w->total_weight += v.total_weight;
		w->land_cap += v.land_cap;
		w->ride_cap += v.ride_cap;
		w->fly_cap += v.fly_cap;
		w->land_weight += v.land_weight;
		w->ride_weight += v.ride_weight;
		w->fly_weight += v.fly_weight;
		w->animals += v.animals;
	} next_all_here;
}


int
ship_weight(int ship)
{
	int i;
	int sum = 0;
	struct weights w;

	assert(kind(ship) == T_ship);

	loop_char_here(ship, i)
	{
		determine_unit_weights(i, &w, FALSE);
		sum += w.total_weight;
	}
	next_char_here;

	return sum;
}


int
lookup(char *table[], char *s)
{
	int i = 0;

	while (table[i] != NULL)
		if (i_strcmp(s, table[i]) == 0)
			return i;
		else
			i++;

	return -1;
}


char *
loyal_s(int who)
{
	char *s;

	switch (loyal_kind(who))
	{
	case 0:
		s = "unsworn";
		break;

	case LOY_contract:
		s = "contract";
		break;

	case LOY_oath:
		s = "oath";
		break;

	case LOY_fear:
		s = "fear";
		break;

	case LOY_npc:
		s = "npc";
		break;

	case LOY_summon:
		s = "summon";
		break;

	default:
		assert(FALSE);
	}

	return sout("%s-%d", s, loyal_rate(who));
}


char *
gold_s(int n)
{

	return sout("%s~gold", comma_num(n));
}


char *
weeks(int n)
{

	if (n == 0)
		return "0~days";

	if (n % 7 == 0)
	{
		n = n / 7;
		return sout("%s~week%s", nice_num(n), add_s(n));
	}

	return sout("%s~day%s", nice_num(n), add_s(n));
}


char *
more_weeks(int n)
{

	if (n == 0)
		return "0~more days";

	if (n % 7 == 0)
	{
		n = n / 7;
		return sout("%d~more week%s", n, add_s(n));
	}

	return sout("%d~more day%s", n, add_s(n));
}


char *
comma_num(int n)
{
	int ones;
	int thousands;
	int millions;
	int further;

	further = n / 1000000000;
	n = n % 1000000000;

	millions = n / 1000000;
	n = n % 1000000;

	thousands = n / 1000;
	ones = n % 1000;

	if (further == 0 && millions == 0 && thousands == 0)
		return sout("%d", ones);
	else if (further == 0 && millions == 0)
		return sout("%d,%.3d", thousands, ones);
	else if (further == 0)
		return sout("%d,%.3d,%.3d", millions, thousands, ones);
	else
		return sout("%d,%.3d,%.3d,%.3d", further, millions,
						thousands, ones);
}


static char *num_s[] = { "zero", "one", "two", "three", "four", "five",
				"six", "seven", "eight", "nine", "ten" };


char *
nice_num(int n)
{

	if (n > 10 || n < 0)
		return sout("%s", comma_num(n));
	else
		return num_s[n];
}


char *
knum(int n, int nozero)
{

	if (n == 0 && nozero)
		return "";

	if (n < 9999)
		return sout("%d", n);

	if (n < 1000000)
		return sout("%dk", n/1000);

	return sout("%dM", n/1000000);
}


char *
ordinal(int n)
{

	if (n >= 10 && n <= 19)
		return sout("%sth", comma_num(n));

	switch (n % 10) {
	case 1:		return sout("%sst", comma_num(n));
	case 2:		return sout("%snd", comma_num(n));
	case 3:		return sout("%srd", comma_num(n));
	default:	return sout("%sth", comma_num(n));
	}
}


int
mylog_output(int base, int num)
{
	int power = 1;
	int value = base;

	assert(base > 10);

	num = num * 10;

	while (value < num)
	{
		value = value * base / 10;
		power++;
	}

	return power;
}


int
my_sqrt(int n)
{
	int power = 1;

	while (power * power <= n)
		power++;

	return power - 1;
}


char *
cap(char *s)				/* return a capitalized copy of s */
{
	char *t;

	if (s == NULL || *s == '\0')
		return s;

	t = sout("%s", s);
	*t = toupper(*t);

	return t;
}


int
deduct_np(int pl, int num)
{
	struct entity_player *p;

	assert(kind(pl) == T_player);

	p = p_player(pl);

	if (p->noble_points < num)
		return FALSE;

	p->noble_points -= num;
	p->np_spent += num;
	return TRUE;
}


void
add_np(int pl, int num)
{
	struct entity_player *p;

	assert(kind(pl) == T_player);

	p = p_player(pl);
	p->noble_points += num;
	p->np_gained += num;
}


int
deduct_aura(int who, int amount)
{
	struct char_magic *p;

	p = rp_magic(who);

	if (p == NULL || p->cur_aura < amount)
		return FALSE;

	p->cur_aura -= amount;
	return TRUE;
}


int
charge_aura(int who, int amount)
{

	if (!deduct_aura(who, amount))
	{
		wout(who, "%s aura or piety required, current level is %s.",
			cap(nice_num(amount)), nice_num(char_cur_aura(who)));
		return FALSE;
	}

	return TRUE;
}


int
check_aura(int who, int amount)
{

	if (char_cur_aura(who) < amount)
	{
		wout(who, "%s aura or piety required, current level is %s.",
			cap(nice_num(amount)), nice_num(char_cur_aura(who)));
		return FALSE;
	}

	return TRUE;
}


int
has_item(int who, int item)
{
	int i;

	assert(valid_box(who));
#if 0
	if (!valid_box(item)) {
		fprintf(stderr, "has_item(who=%s, item=%s) failure\n",
				box_code_less(who), box_code_less(item));
		fprintf(stderr, "player(who) = %s\n", box_code_less(player(who)));
		fprintf(stderr, "c->line '%s'\n", bx[who]->cmd->line);
		assert(FALSE);
	}
#endif
	assert(valid_box(item));

	for (i = 0; i < ilist_len(bx[who]->items); i++)
		if (bx[who]->items[i]->item == item)
			return bx[who]->items[i]->qty;

	return 0;
}


static void
add_item(int who, int item, int qty)
{
	struct item_ent *new;
	int i;
	int old;

	assert(valid_box(who));
	assert(valid_box(item));
	assert(qty >= 0);

	{
		int lore;

		lore = item_lore(item);

		if (lore && kind(who) == T_char && !test_known(who, item))
		{
			queue_lore(who, item, FALSE);
		}
	}

	for (i = 0; i < ilist_len(bx[who]->items); i++)
		if (bx[who]->items[i]->item == item)
		{
			old = bx[who]->items[i]->qty;

			bx[who]->items[i]->qty += qty;

#ifndef NEW_TRADE
			investigate_possible_trade(who, item, old);
#endif // NEW_TRADE
			return;
		}

	new = my_malloc(sizeof(*new));
	new->item = item;
	new->qty = qty;

	ilist_append((ilist *) &bx[who]->items, (int) new);

#ifndef NEW_TRADE
	investigate_possible_trade(who, item, 0);
#endif // NEW_TRADE
}


int
sub_item(int who, int item, int qty)
{
	int i;

	assert(valid_box(who));
	assert(valid_box(item));
	assert(qty >= 0);

	for (i = 0; i < ilist_len(bx[who]->items); i++)
		if (bx[who]->items[i]->item == item)
		{
			if (bx[who]->items[i]->qty < qty)
				return FALSE;

			bx[who]->items[i]->qty -= qty;
			return TRUE;
		}

	return FALSE;
}


void
gen_item(int who, int item, int qty)
{

	assert(!item_unique(item));
	add_item(who, item, qty);
}


int
consume_item(int who, int item, int qty)
{

  if (item_unique(item)) {
    wout(gm_player, "Destroying unique item %s via consume_item.",
	 box_name(item));
    destroy_unique_item(who, item);
    return TRUE;
  };
	assert(!item_unique(item));
	return sub_item(who, item, qty);
}


/*
 *  Move item from one unit to another
 *  Destination=0 means discard the items
 */

int
move_item(int from, int to, int item, int qty)
{

	if (qty <= 0)
		return TRUE;

	if (to == 0)
		return drop_item(from, item, qty);

	if (sub_item(from, item, qty))
	{
		add_item(to, item, qty);

		if (item_unique(item))
		{
			assert(qty == 1);
			p_item(item)->who_has = to;

#if 0
			if (subkind(item) == sub_npc_token)
				move_token(item, from, to);
#endif // 0
		}

		return TRUE;
	}

	return FALSE;
}


void
hack_unique_item(int item, int owner)
{

	p_item(item)->who_has = owner;
	add_item(owner, item, 1);
}


int
create_unique_item(int who, int sk)
{
	int new;

	new = new_ent(T_item, sk);

	if (new < 0)
		return -1;

	if (who) {
	  p_item(new)->who_has = who;
	  add_item(who, new, 1);
	};

	return new;
}

void
destroy_unique_item(int who, int item)
{
	int ret;

	assert(kind(item) == T_item);
	assert(item_unique(item));

	ret = sub_item(who, item, 1);
	assert(ret);

	delete_box(item);
};

int
find_nearest_land(int where)
{
  int i;
  int orig_where = where;
  int dir;
  int check = 0;
  int ret = 0;
  int try_one, try_two;

  try_two = 100;
  while (try_two-- > 0) {
    dir = rnd(1,4);

    try_one = 1000;
    while (try_one-- > 0)
      {
	if (subkind(where) != sub_ocean)
	  return where;

	loop_here(where, i)
	  {
	    if (subkind(i) == sub_island)
	      {
		assert(kind(i) == T_loc);

		ret = i;
		break;
	      }
	  }
	next_here;

	if (ret)
	  return ret;

	where = location_direction(where, dir);

	while (where == 0)
	  {
	    where = orig_where;
	    dir = (dir % 4) + 1;
	    check++;
	    assert(check <= 4);
	    where = location_direction(where, dir);
	  }
      }

    if (try_two == 99)
      log_output(LOG_CODE, "find_nearest_land: Plan B");
  }

  log_output(LOG_CODE, "find_nearest_land: Plan C");

  {
    ilist l = NULL;
    int i;
    int ret;

    loop_loc(i)
      {
	if (region(i) != region(orig_where))
	  continue;
	if (loc_depth(i) != LOC_province)
	  continue;
	if (subkind(i) == sub_ocean)
	  continue;

	ilist_append(&l, i);
      }
    next_loc;

    if (ilist_len(l) < 1)
      return 0;

    ret = l[rnd(0,ilist_len(l)-1)];

    ilist_reclaim(&l);
    return ret;
  }

  return 0;
}


/*
 *  Simply throw away non-unique items
 *  Put unique items into the province to be found with EXPLORE
 *  If we're at sea, look for a nearby island or shore to move
 *  the item to.
 *
 *  Mon Apr 20 10:00:56 1998 -- Scott Turner
 *
 *  Province should collect peasants & gold.
 */

int
drop_item(int who, int item, int qty)
{
	int who_gets;

	if (!item_unique(item) && item != item_peasant && item != item_gold)
		return consume_item(who, item, qty);

	who_gets = province(who);

	if (subkind(item) == sub_dead_body)
	{
		who_gets = nearby_grave(who_gets);
		if (who_gets == 0)
		{
			destroy_unique_item(who, item);
			return TRUE;
		}
	}

	if (subkind(who_gets) == sub_ocean)
		who_gets = find_nearest_land(who_gets);

	if (who_gets == 0) {
		who_gets = subloc(who);	/* oh well */
	};

	if (item != item_gold && item != item_peasant)
	  log_output(LOG_CODE, "drop_item: %s from %s to %s", box_name(item),
					box_name(subloc(who)),
					box_name(who_gets));

	return move_item(who, who_gets, item, qty);
}


int
can_pay(int who, int amount)
{

	return has_item(who, item_gold) >= amount;
}


int
charge(int who, int amount)
{
	return sub_item(who, item_gold, amount);
}


int
stack_has_item(int who, int item)
{
	int i;
	int sum = 0;
	int head = stack_leader(who);

	loop_stack(head, i)
	{
		if (player(i) != player(who))	/* friendly with us */
			continue;

		sum += has_item(i, item);
	}
	next_stack;

	return sum;
}


int
has_use_key(int who, int key)
{
	struct item_ent *e;
	int ret = 0;
	struct item_magic *p;

	loop_inv(who, e)
	{
		p = rp_item_magic(e->item);

		if (p && p->use_key == key)
			ret = e->item;

		if (ret)
			break;
	}
	next_inv;

	return ret;
}


int
stack_has_use_key(int who, int key)
{
	int i;
	int ret = 0;
	int head = stack_leader(who);

	loop_stack(head, i)
	{
		if (player(i) != player(who))	/* friendly with us */
			continue;

		ret = has_use_key(i, key);
		if (ret)
			break;
	}
	next_stack;

	return ret;
}

/*
 *  Subtract qty of item from a stack
 *  Take it from who first, then take it from
 *  anyone else in the stack who has it, starting from
 *  the stack leader and working down.
 *
 *  Return FALSE if the stack doesn't have qty of item.
 */

int
stack_sub_item(int who, int item, int qty)
{
  int n;
  int head;
  int i;

  if (stack_has_item(who, item) < qty)
    return FALSE;

  n = min(has_item(who, item), qty);

  if (n > 0)
    {
      qty -= n;
      sub_item(who, item, n);
    }

  assert(qty >= 0);

  if (qty == 0)
    return TRUE;

/*
 *  Try to borrow what we need from friendly stackmates
 */

  head = stack_leader(who);

  loop_stack(head, i)
    {
      if (qty <= 0)
	break;

      if (player(i) != player(who))	/* friendly with us */
	continue;

      n = min(has_item(i, item), qty);

      if (n > 0)
	{
	  qty -= n;
	  sub_item(i, item, n);

#if 0
	  if (show_day)
	    {
	      wout(who, "Borrowed %s from %s.",
		   box_item_desc(item, n),
		   box_name(i));

	      wout(i, "%s borrowed %s.",
		   box_name(who),
		   box_item_desc(item, n));
	    }
#endif
	}
    }
  next_stack;

  assert(qty == 0);	/* or else stack_has_item above lied */

  return TRUE;
}


int
autocharge(int who, int amount)
{

	return stack_sub_item(who, item_gold, amount);
}


int
test_bit(sparse kr, int i)
{

	if (ilist_lookup(kr, i) == -1)
		return FALSE;

	return TRUE;
}


void
set_bit(sparse *kr, int i)
{

	if (ilist_lookup(*kr, i) == -1)
		ilist_append(kr, i);
}


void
clear_know_rec(sparse *kr)
{

	ilist_clear(kr);
}


int
test_known(int who, int i)
{
	struct entity_player *ep;
	int pl;

	if (who == 0)
		return FALSE;

	assert(valid_box(who));
	assert(valid_box(i));

	pl = player(who);
	ep = rp_player(pl);

	if (ep && test_bit(ep->known, i))
		return TRUE;

	return FALSE;
}


void
set_known(int who, int i)
{
	int pl;

	assert(valid_box(who));
	assert(valid_box(i));

	pl = player(who);

	assert(valid_box(pl));

	set_bit(&(p_player(pl)->known), i);
}


static int dot_count = 0;

void
print_dot(int c)
{

	if (dot_count == 0)
	{
		fprintf(stderr, "   ");
		dot_count++;
	}

	if (++dot_count % 60 == 0)
		fprintf(stderr, "\n   ");
	fputc(c, stderr);
	fflush(stderr);
}


int
first_character(int where)
{
	int i;
	int first = 0;

	loop_here(where, i)
	{
		if (kind(i) == T_char)
		{
			first = i;
			break;
		}
	}
	next_here;

	return first;
}


char *
entab(int pl)
{

	if (player_notab(pl))
		return "";

	return "entab | ";
}


int
loc_hidden(int n)
{

#if 0
	if (loc_depth(n) > LOC_province && weather_here(n, sub_fog))
		return TRUE;
#endif

	return (rp_loc(n) ? rp_loc(n)->hidden : 0);
}

int
loc_contains_hidden(int n)
{
  int enclosed;
  loop_here(n, enclosed) {
    if (loc_hidden(enclosed)) return enclosed;
  } next_here;
  return FALSE;
};


char *
rest_name(struct command *c, int a)
{
	char *s;
	int i;

	if (numargs(c) < a)
		return NULL;

	s = c->parse[a];

	for (i = a+1; i <= numargs(c); i++)
	{
		s = sout("%s %s", s, c->parse[i]);
	}

	return s;
}


int
nprovinces()
{
	static int nprov = 0;
	int i;

	if (nprov)
		return nprov;

	loop_province(i)
	{
		nprov++;
	}
	next_province;

	return nprov;
}


int
my_prisoner(int who, int pris)
{

	if (kind(pris) != T_char)
		return FALSE;

	if (!is_prisoner(pris))
		return FALSE;

	if (loc(pris) != who)
		return FALSE;

	return TRUE;
}


int
beast_capturable(int who)
{
	int ni;

	if (subkind(who) != sub_ni)
		return FALSE;

	ni = noble_item(who);

	if (item_capturable(ni))
		return TRUE;

	return FALSE;
}

int
beast_wild(int who)
{
	int ni;

	if (subkind(who) != sub_ni)
		return FALSE;

	ni = noble_item(who);

	if (item_wild(ni))
		return TRUE;

	return FALSE;
}

void
stage(char *s)
{
	extern int time_self;
	static long old = 0;
	static long first = 0;
	long t;

	if (!time_self)
	{
		if (s)
			fprintf(stderr, "%s\n", s);
		return;
	}

	time(&t);

	if (old)
	{
		fprintf(stderr, "\t%d sec\n", t-old);
	}
	else
		first = t;
	old = t;

	if (s)
	{
		fprintf(stderr, "%s", s);
	}
	else
		fprintf(stderr, "%d seconds\n", t - first);
}

/*
 *  Thu Jan  2 13:38:03 1997 -- Scott Turner
 *
 *  Ship capacity is now calculated from the ship's hulls, etc.
 *  For testing's sake, we also do the default values for capacity
 *  for galleys and roundships.
 *
 */

int
ship_cap(int ship)
{
  struct entity_ship *s = rp_ship(ship);
  int sc = 0;
  int dam = loc_damage(ship);

  if (s) {
    sc = (s->hulls*HULL_CAPACITY) -
      (s->forts*FORT_WEIGHT) -
      (s->sails*SAIL_WEIGHT) -
      (s->keels*KEEL_WEIGHT);
    if (sc < 0) sc = 0;
  } else if (subkind(ship) == sub_galley) {
    sc = 5000;
  } else if (subkind(ship) == sub_roundship) {
    sc = 25000;
  } else {
    sc = 0;
  };

  if (loc_hp(ship)) sc -= sc * dam / loc_hp(ship);
  
  return sc;
}

/*
 *  Sat Apr 15 11:56:33 2000 -- Scott Turner
 *
 *  Lock up; prevents multiple TAGS in same Lib
 *
 */
void lock_tag()
{
  int fd;
  int val;

  fd = open(sout("%s/lock", libdir),O_RDONLY | O_CREAT, S_IRUSR);

  if (fd == -1) {
    fprintf(stderr,"Problem opening lock file?");
    exit(-1);
  };
  
  if (flock(fd, LOCK_EX | LOCK_NB)) {
    fprintf(stderr,"TAG already running in this directory!\n");
    exit(-1);
  };
  
};
