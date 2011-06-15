#include	<stdio.h>
#include	<unistd.h>
#include	"z.h"
#include	"oly.h"

/*
 *  Trading-related skills and functions.
 *
 */
struct trade *find_trade(int who, int kind, int item);
struct trade *new_trade(int who, int kind, int item);

/*
 *  Dedicate Tower
 *  Fri Nov 15 13:44:30 1996 -- Scott Turner
 *
 *  Turn a generic tower into one for your guild.
 *
 */
int
v_dedicate_tower(struct command *c)
{
  int where = subloc(c->who);
  int school = c->a;
  int i;
  struct entity_skill *p;

  /*
   *  School should be legitimate.
   *
   */
  if (!valid_box(school) || kind(school) != T_skill ||
      skill_school(school) != school) {
    wout(c->who,"To dedicate a tower, you must specify the skill school.");
    return FALSE;
  };

  /*
   *  You need to already know all the learnable and researchable
   *  skills in this school.
   *
   */
  p = rp_skill(school);
  if (p == NULL) return FALSE;
  
  for (i = 0; i < ilist_len(p->offered); i++)
    if (!has_skill(c->who, p->offered[i])) {
      wout(c->who,"You must master all available skills before creating a guild.");
      return FALSE;
    };
  for (i = 0; i < ilist_len(p->research); i++)
    if (!has_skill(c->who, p->research[i])) {
      wout(c->who,"You must master all available skills before creating a guild.");
      return FALSE;
    };

  /*
   *  You can't dedicate a tower if you're already in a guild and this
   *  isn't that guild.
   *
   */
  if (guild_member(c->who) &&
      guild_member(c->who) != school) {
    wout(c->who, "You cannot dedicate this tower, because you're already");
    wout(c->who, "a member of the %s Guild.", box_name(guild_member(c->who)));
    return FALSE;
  };

  /*
   *  Have to be in a tower.
   *
   */
  if (subkind(where) != sub_tower) {
    wout(c->who,"To dedicate a tower, you must be inside the tower.");
    return FALSE;
  };
  
  /*
   *  Have to be top dog.
   *
   *  Mon Oct 26 09:34:36 1998 -- Scott Turner
   *
   *  Maybe you really have to be alone, so you can't grandfather non-guild
   *  members into a guild.
   *
   */
  if (ilist_len(rp_loc_info(where)->here_list) != 1) {
    wout(c->who, "Must be alone in a tower to dedicate.");
    return FALSE;
  }

  /*
   *  Need some gold for the dedication ceremony.
   *
   */
  if (has_item(c->who, item_gold) < 250) {
    wout(c->who, "Dedicating a tower requires 250 gold.");
    return FALSE;
  };

  /*
   *  Must be in a city.
   *
   */
  if (subkind(loc(where)) != sub_city) {
    wout(c->who, "You may only create a guild in a city.");
    return FALSE;
  };

  /*
   *  Can't be any other guilds of the same sort here.
   *
   */
  loop_here(loc(where),i) {
    if (subkind(i) == sub_guild &&
	rp_subloc(i) &&
	rp_subloc(i)->guild == school) {
      wout(c->who, "There is already a guild for %s in this city.",
	   box_name(school));
      return FALSE;
    };
  } next_here;

  /*
   *  Tue Dec 29 11:33:45 1998 -- Scott Turner
   *
   *  Have to be a guild member already or ready to join the guild.
   *
   */
  if (guild_member(c->who) != school &&
      !can_join_guild(c->who, school)) {
    wout(c->who, "You must be a guild member or ready to join to "
	 "dedicate a guild.");
    return FALSE;
  };

  return TRUE;
};

/*
 * Tue Apr 20 17:21:52 1999 -- Scott Turner
 *
 *  Encapsulated here because it is also called in seed_city.
 *
 */
void
make_tower_guild(int where, int school)
{
  struct entity_subloc *p = p_subloc(where);
  
  change_box_subkind(where, sub_guild);
  p->guild = school;
  if (options.guild_teaching) ilist_append(&p->teaches, school);
  
};

int
d_dedicate_tower(struct command *c)
{
  int where = subloc(c->who), i;
  struct entity_subloc *p = rp_subloc(where);
  int school = c->a;

  /*
   *  Have to be in a tower.
   *
   */
  if (subkind(where) != sub_tower) {
    wout(c->who,"To dedicate a tower, you must be inside the tower.");
    return FALSE;
  };
  
  /*
   *  Have to be top dog.
   *
   *  Mon Oct 26 09:34:36 1998 -- Scott Turner
   *
   *  Maybe you really have to be alone, so you can't grandfather non-guild
   *  members into a guild.
   *
   */
  if (ilist_len(rp_loc_info(where)->here_list) != 1) {
    wout(c->who, "Must be alone in a tower to dedicate.");
    return FALSE;
  }

  /*
   *  Must be in a city.
   *
   */
  if (subkind(loc(where)) != sub_city) {
    wout(c->who, "You may only create a guild in a city.");
    return FALSE;
  };

  /*
   *  Can't be any other guilds of the same sort here.
   *
   */
  loop_here(loc(where),i) {
    if (subkind(i) == sub_guild &&
	rp_subloc(i) &&
	rp_subloc(i)->guild == school) {
      wout(c->who, "There is already a guild for %s in this city.",
	   box_name(school));
      return FALSE;
    };
  } next_here;

  /*
   *  Tue Dec 29 11:33:45 1998 -- Scott Turner
   *
   *  Have to be a guild member already or ready to join the guild.
   *
   */
  if (guild_member(c->who) != school &&
      !can_join_guild(c->who, school)) {
    wout(c->who, "You must be a guild member or ready to join to "
	 "dedicate a guild.");
    return FALSE;
  };

  /*
   *  Need some gold for the dedication ceremony.
   *
   */
  if (!charge(c->who, 250)) {
    wout(c->who, "Dedicating a tower requires 250 gold.");
    return FALSE;
  };

  /*
   *  Make it a tower.
   *
   */
  if (p == NULL) {
    wout(c->who,"For some reason, you cannot dedicate this tower.");
    return FALSE;
  } else {
    make_tower_guild(where, school);
    wout(c->who,"%s now dedicated to %s.",
	 box_name(where),
	 just_name(school));
    (void) join_guild(c->who, school);
    /*
     *  Have to "touch" the location so it generates a location report.
     *
     */
     touch_loc(c->who);
  };
  return TRUE;
};

/*
 *  RANDOM_TRADE_GOOD
 *  Fri Nov 15 16:16:45 1996 -- Scott Turner
 *
 *  Select a trade good at random, based on the relative weights.
 *
 */
int random_trade_good()
{
  int t, sofar = 0, selected = 0;
  
  loop_subkind(sub_trade_good, t) {
    if (rp_item(t)) {
      sofar += rp_item(t)->trade_good;
      if (rnd(1,sofar) <= rp_item(t)->trade_good) selected = t;
    };
  } next_subkind;
  
  return selected;
};
 

/*
 *  ADD_TRADING_PRODUCTION
 *  Fri Nov 15 16:08:24 1996 -- Scott Turner
 *
 *  Add a production of a trading good to this market.  Don't pick
 *  one that is already there.
 *
 */
void add_trade(int where, int type)
{
  int i, found, new_tg;
  int other = (type == PRODUCE) ? CONSUME : PRODUCE;
  struct trade *t;
  
  do {
    new_tg = random_trade_good();
  } while (find_trade(where, type, new_tg) != NULL ||
	   find_trade(where, other, new_tg) != NULL);
  
  /*
   *  Create a new trade and add it to the market.
   *
   */
  t = new_trade(where, type, new_tg);
  t->qty = rp_item(new_tg)->trade_good;
  t->cost = rp_item(new_tg)->base_price;

  /*
   *  Mon Feb  7 08:16:06 2000 -- Scott Turner
   *  Put in the buy/sell as well.
   *
   *  Wed Feb  9 06:51:01 2000 -- Scott Turner
   *
   *  Whoops, only 1/8 the qty.
   *
   */
  other = (type == PRODUCE) ? SELL : BUY;
  t = new_trade(where, other, new_tg);
  t->qty = rp_item(new_tg)->trade_good/NUM_MONTHS;
  t->cost = rp_item(new_tg)->base_price;
};

/*
 *  Big city trades.
 *
 */
static void
update_big_city_trades(int where)
{
  /*
   *  Remove small city trades.
   *
   */
  delete_city_trade(where, item_lumber);
  delete_city_trade(where, item_stone);
  /*
   *  Update (or add) big city trades.
   *
   */
  update_city_trade(where, CONSUME, item_lana_bark, rnd(1,5),
		    rp_item(item_lana_bark)->base_price, 0);
  update_city_trade(where, CONSUME, item_pretus_bones, rnd(1,5),
		    rp_item(item_pretus_bones)->base_price, 0);
  update_city_trade(where, CONSUME, item_mallorn_wood, rnd(1,5),
		    rp_item(item_mallorn_wood)->base_price, 0);
  update_city_trade(where, CONSUME, item_yew, rnd(1,5),
		    rp_item(item_yew)->base_price, 0);
  update_city_trade(where, CONSUME, item_farrenstone, rnd(1,5),
		    rp_item(item_farrenstone)->base_price, 0);
  update_city_trade(where, CONSUME, item_spiny_root, rnd(1,5),
		    rp_item(item_spiny_root)->base_price, 0);
  update_city_trade(where, CONSUME, item_avinia_leaf, rnd(1,5),
		    rp_item(item_avinia_leaf)->base_price, 0);
}

/*
 *  Small city trades.
 *
 */
static void
update_small_city_trades(int where)
{
  int amount = (10000 - has_item(province(where), item_peasant))/400;
  update_city_trade(where, CONSUME, item_lumber, amount/2,
		    rp_item(item_lumber)->base_price, 0);
  update_city_trade(where, CONSUME, item_stone, amount,
		    rp_item(item_stone)->base_price, 0);
};

/*
 *  Override causes cities which only produce a good once per year
 *  to produce it now anyway.  This is useful for epoch city trade
 *  seeding.
 *
 *  Thu Dec  2 05:52:22 1999 -- Scott Turner
 *
 *  Renamed from loc_trade_sup to do_production.
 *
 */
void
do_production(int where, int override)
{
  struct trade *t;
  struct trade *new;

  loop_trade(where, t) {
    int okay = TRUE;
    int next_month;

    if (t->month_prod && !override) {
      int this_month = oly_month(sysclock) - 1;
      int next_month = (this_month + 1) % NUM_MONTHS;
      int prod_month = t->month_prod - 1;

      if (next_month != prod_month)
	okay = FALSE;
    }

    if (t->kind == PRODUCE && okay) {
      new = new_trade(where, SELL, t->item);

      if (new->qty < t->qty)
	new->qty = t->qty;

      new->cost = t->cost;
      new->cloak = t->cloak;
    } else if (t->kind == CONSUME) {
      new = new_trade(where, BUY, t->item);
      if (new->qty < t->qty)
	new->qty = t->qty;

      new->cost = t->cost;
      new->cloak = t->cloak;
    }
  } next_trade;
}

/*
 *  Update_market
 *  Tue Jan 18 08:43:27 2000 -- Scott Turner
 *
 *  Update one market.
 *
 *  Tue Jan 18 10:16:25 2000 -- Scott Turner
 *
 *  Refactor BUY/SELL.  Modify "change" to negative for SELL transactions.
 *  Check to see if the item has been sold or bought with a != instead
 *  of a >= or a <=.  Everything else should be the same.
 *
 */
static void
update_market(int where)
{
  struct trade *t, *new;
  int change, i, other, bp;

  loop_trade(where, t) {
    /*
     *  Ignore opium!
     *
     */
    if (t->item == item_opium) continue;
    
    /*
     * Calculate what the change in price, if any, will be.
     *
     */
    change = (t->cost * (9 + rnd(1,11))) / 100;
    if (change < 1) change = 1;
    
    /*
     *  If this isn't a sell or buy, skip over it.
     *
     */
    if (t->kind != SELL && t->kind != BUY) continue;
    
    /*
     *  other is the corresponding other part of the
     *  transaction, i.e., SELL-PRODUCE and BUY-CONSUME.
     *
     */
    other = CONSUME;
    
    /*
     *  If we're a SELL we switch the other and the "polarity"
     *  of the price change.
     *
     */
    if (t->kind == SELL) {
      change = -change;
      other = PRODUCE;
    };

    /*
     *  Find the corresponding other.
     */
    new = new_trade(where, other, t->item);
    assert(new != NULL);
    
    /*  
     *  Add in the price delta if quantity unchanged from last month;
     *  otherwise subtract it.
     *
     *  Tue Jan 18 11:47:12 2000 -- Scott Turner
     *
     *  City goods don't always have an "old_qty" at first; we'll assume
     *  those are unsold.
     *
     */
    if (!t->old_qty || t->old_qty == t->qty) {
      t->counter++;
      if (rp_item(t->item)) {
	t->cost += change;
	new->cost += change;
      };
    } else {
      t->counter = 0;
      if (rp_item(t->item)) {
	t->cost -= change;
	new->cost -= change;
      };
    };

    /*
     *  If a good has reached the end of its "counter", then
     *  delete the two trades.
     *
     *  Tue Jan 18 12:46:59 2000 -- Scott Turner
     *
     *  This is a bad thing for the "constant" city goods, like
     *  fish, etc.  It's a good thing for trade goods, unique items, etc.
     *  Hmm.
     */
    if ((rp_item(t->item)->trade_good || item_unique(t->item)) &&
	t->counter > options.market_age) {
      wout(gm_player, "Deleting good %s from %s.",
	   box_name(t->item),
	   box_name(where));
      ilist_rem_value((ilist *) &bx[where]->trades, (int) t);
      ilist_rem_value((ilist *) &bx[where]->trades, (int) new);
      continue;
    };

    /*
     *  Make sure our price is legitimate.
     *
     *  Tue Jan 18 11:11:20 2000 -- Scott Turner
     *
     *  Some items may not have a base price; in which case
     *  we'll only insist that the price stay positive.
     *
     *  Thu Jun 15 18:58:46 2000 -- Scott Turner
     *
     *  Uh, let's keep the price under 50 if it doesn't have
     *  a bp.
     *
     */
    bp = rp_item(t->item)->base_price;

    if (bp && t->cost > bp * 2) {
      t->cost = bp * 2;
      new->cost = t->cost;
    };

    if (bp && t->cost < bp / 2) {
      t->cost = bp / 2;
      new->cost = t->cost;
    };

    if (!bp && t->cost < 1) {
      t->cost = 1;
      new->cost = 1;
    };

    if (!bp && t->cost > 50) { 
      t->cost = 50;
      new->cost = 50;
    };
    
    /*
     *  Now save the current quantity for next month.
     *
     */
    t->old_qty = t->qty;
	
  } next_trade;

  /*
   *   Tue Jan 18 10:51:16 2000 -- Scott Turner
   *
   *   Add in test for prod_month (unused?).
   *
   *   Tue Jan 18 10:53:32 2000 -- Scott Turner
   *
   *   In the trade guilds, qty is the yearly production, and
   *   you can't exceed that (generated qty/8 per month).  Unfortunately,
   *   in the city markets, qty is the monthly production *and* the limit.
   *   So we need a special case test in here (or we could modify the
   *   databases :-().
   *
   */
  loop_trade(where, t) {
    int other = SELL;

    if (t->kind != CONSUME && t->kind != PRODUCE) continue;
    if (t->kind == CONSUME) other = BUY;

    /*
     *  A production or consumption with no qty can be deleted
     *  and skipped.
     *
     */
    if (t->qty == 0) {
      ilist_rem_value((ilist *) &bx[where]->trades, (int) t);
      continue;
    };
    
    /*
     *  Some productions only happen 1x year.
     *
     */
    if (t->kind == PRODUCE && t->month_prod) {
      int this_month = oly_month(sysclock) - 1;
      int next_month = (this_month + 1) % NUM_MONTHS;
      int prod_month = t->month_prod - 1;

      if (next_month != prod_month) continue;
    };

    /*
     *  Produce or consume the monthly amount, up to the
     *  yearly limit.
     *
     */
    new = new_trade(where, other, t->item);
    if (new->qty < t->qty) {
      if (rp_item(t->item)->trade_good) {
	new->qty += (t->qty/8);  /* per month */
      } else {
	new->qty = t->qty;
      };
      new->old_qty = new->qty;
    };
    new->cost = t->cost;
    new->cloak = t->cloak;
  } next_trade;
};

static void
add_trade_goods(int where)
{
  struct trade *t;
  int produce = 0, consume = 0, i;
  
  loop_trade(where, t) {
    /*
     *  Track how many of each; we may need to add some
     *  at the end.
     *
     *  Produce or consume the monthly amount, up to the
     *  yearly limit.
     *
     */
    if (t->kind == PRODUCE) {
      produce++;
    };
	
    if (t->kind == CONSUME) {
      consume++;
    };
  } next_trade;

  /*
   *  We need to have 3 productions and 3 consumptions.
   *
   */
  if (produce < 3) {
    /*
     *  Add a production.
     *
     */
    for(i = 0; i < (3 - produce); i++) 
      add_trade(where, PRODUCE);
  };
  if (consume < 3) {
    /*
     *  Add a consumption.
     *
     */
    for(i = 0; i < (3 - consume); i++) 
      add_trade(where, CONSUME);
  };
};

/*
 *  Update_Markets
 *  Tue Nov 12 12:33:30 1996 -- Scott Turner
 *
 *  Fri Sep 25 08:20:12 1998 -- Scott Turner
 *
 *  Split the buy/sell price updates and the consume/produce
 *  quantity updates into two parts.
 *
 *  Tue Jan 18 08:41:30 2000 -- Scott Turner
 *
 *  Extend this to cover all markets; need to add special code to
 *  distinguish trade good special cases.  Non-trade goods should not
 *  be "cycled".
 *
 */
void
update_markets()
{
  struct trade *t, *new;
  int where, produce = 0, consume = 0, change, i;
  stage("update_markets()");

  loop_guild(where) {
    if (is_guild(where) == sk_trading) {
      update_market(where);
      add_trade_goods(where);
    };    
  } next_guild;

  /*
   *  Wed Sep 15 17:39:39 1999 -- Scott Turner
   *
   *  Now adjust city markets for common goods.
   *
   *  Harbor city: buys fish
   *  City < 10000: buys 10 wood per (10k-pop)
   *                at bp = ?
   *              : buys 10 stone per (10k-pop)
   *  City == 10K : buys 1-5 lana bark, avinia,
   *                spiny root, farrenstone, yew,
   *                mallor, pretus bones
   *  All cities  : buys 1-5 wild horses
   *  add_city_trade(int where, int kind, int item,
   *                 int qty, int cost, int month)
   *  delete_city_trade(int where, int item)
   *
   *  Wed Nov 10 08:23:13 1999 -- Scott Turner
   *
   *  We should do these in the same manner as fish, etc.
   *  (e.g., with PRODUCE/CONSUME) to make everything consistent.
   *
   *  Thu Dec  2 05:28:53 1999 -- Scott Turner
   *
   *  Folded in the calls from location_trades.
   *
   */
  loop_city(where) {
    /*
     *  City-size specific trades.
     *
     */
    if (has_item(where,item_peasant) > 9500) {
      update_big_city_trades(where);
    } else {
      update_small_city_trades(where);
    };

    /*
     *  Everyone buys wild horses.
     *
     */
    update_city_trade(where, CONSUME, item_wild_horse, rnd(1,10),
		     rp_item(item_wild_horse)->base_price, 0);

    /*
     *  Update the markets.
     *
     */
    update_market(where);
    
    /*
     *  
     * Opium
     *
     */
    opium_market_delta(where);

    if (in_faery(where) || in_clouds(where)) {
      trade_suffuse_ring(where);
    };

    if (!in_faery(where) &&
	!in_clouds(where) &&
	!in_hades(where))
      add_scrolls(where);

  } next_city;
}

/*
 *  Mon Nov 25 11:41:48 1996 -- Scott Turner
 *
 *  Smuggle goods hangs (or removes) a "smuggling" effect on the user.
 *  Then, based on how much smuggling experience he has, he may avoid
 *  the costs to enter a city.
 *
 */
int
v_smuggle_goods(struct command *c)
{
  int flag = c->a;

  if (flag && get_effect(c->who, ef_smuggle_goods, 0, 0)) {
    wout(c->who,"You are already prepared to smuggle goods.");
    return FALSE;
  };

  if (!flag && !get_effect(c->who, ef_smuggle_goods, 0, 0)) {
    wout(c->who,"You are not smuggling at this time.");
    return FALSE;
  };

  return TRUE;
};

int
d_smuggle_goods(struct command *c)
{
  int flag = c->a;

  if (flag) {
    if (get_effect(c->who, ef_smuggle_goods, 0, 0)) {
      wout(c->who,"You are already prepared to smuggle goods.");
      return FALSE;
    };
    add_effect(c->who, ef_smuggle_goods, 0, -1, 1);
    wout(c->who,"You are now prepared to smuggle goods.");
    return TRUE;
  };

  if (!flag) {
    if (!get_effect(c->who, ef_smuggle_goods, 0, 0)) {
      wout(c->who,"You are not smuggling at this time.");
      return FALSE;
    };
    delete_effect(c->who, ef_smuggle_goods, 0);
    wout(c->who,"You are no longer smuggling goods.");
    return TRUE;
  };
};

/*
 *
 */
int
v_smuggle_men(struct command *c)
{
  int flag = c->a;

  if (flag && get_effect(c->who, ef_smuggle_men, 0, 0)) {
    wout(c->who,"You are already prepared to smuggle goods.");
    return FALSE;
  };

  if (!flag && !get_effect(c->who, ef_smuggle_men, 0, 0)) {
    wout(c->who,"You are not smuggling at this time.");
    return FALSE;
  };

  return TRUE;
};

int
d_smuggle_men(struct command *c)
{
  int flag = c->a;

  if (flag) {
    if (get_effect(c->who, ef_smuggle_men, 0, 0)) {
      wout(c->who,"You are already prepared to smuggle goods.");
      return FALSE;
    };
    add_effect(c->who, ef_smuggle_men, 0, -1, 1);
    wout(c->who,"You are now prepared to smuggle goods.");
    return TRUE;
  };

  if (!flag) {
    if (!get_effect(c->who, ef_smuggle_men, 0, 0)) {
      wout(c->who,"You are not smuggling at this time.");
      return FALSE;
    };
    delete_effect(c->who, ef_smuggle_men, 0);
    wout(c->who,"You are no longer smuggling goods.");
    return TRUE;
  };

  return TRUE;
};

/*
 *  Wed Nov 27 12:25:46 1996 -- Scott Turner
 *
 *  Build wagons is just a production skill, so everything necessary
 *  is encoded in lib/skill
 *
 */

/*
 *  Mon Nov 25 10:50:57 1996 -- Scott Turner
 *
 *  Is a good traded by this place?
 *
 */
struct trade *traded_here(int where, int good)
{
  struct trade *t;

  /*
   *  Fri Nov 13 12:52:55 1998 -- Scott Turner
   *
   *  Loop_trade only runs through the trades hung on "where";
   *  it doesn't pick up any from the traders in where.
   *
   */
  loop_trade(where, t) {
    if (t->item == good &&
	(t->kind == SELL || t->kind == BUY)) return t;
  } next_trade;
  return NULL;
};

/*
 *  Mon Nov 25 10:50:57 1996 -- Scott Turner
 *
 *  Is a good traded by this place?
 *
 */
struct trade *produced_here(int where, int good)
{
  struct trade *t;

  /*
   *  Fri Nov 13 12:52:55 1998 -- Scott Turner
   *
   *  Loop_trade only runs through the trades hung on "where";
   *  it doesn't pick up any from the traders in where.
   *
   */
  loop_trade(where, t) {
    if (t->item == good &&
	(t->kind == CONSUME || t->kind == PRODUCE)) return t;
  } next_trade;
  return NULL;
};

/*
 *  Mon Nov 25 10:41:35 1996 -- Scott Turner
 *
 *  Increase the demand for a good -- raise it's price 5-8%.  Note that
 *  you can't raise a price over base*2.
 *
 */
int
v_increase_demand(struct command *c)
{
  int where = subloc(c->who);
  int good = c->a;

  /*
   *  You need to be in the guild where the good is being traded.
   *
   *  Fri Nov 13 12:49:25 1998 -- Scott Turner
   *
   *  Now works in any market, but only on goods sold/bought by the
   *  market.
   */
  if (!market_here(where)) {
    wout(c->who,"You must be in a market to use this skill.");
    return FALSE;
  };

  /*
   *  Needs to be a good traded in this guild (by the guild).
   *
   */
  if (!good || !traded_here(where,good)) {
    wout(c->who,"That good is not traded here.");
    return FALSE;
  };

  return TRUE;
};

int
d_increase_demand(struct command *c)
{
  int where = subloc(c->who);
  int good = c->a, change;
  struct trade *t;

  /*
   *  You need to be in the guild where the good is being traded.
   *
   *  Fri Nov 13 12:49:25 1998 -- Scott Turner
   *
   *  Now works in any market, but only on goods sold/bought by the
   *  market.
   */
  if (!market_here(where)) {
    wout(c->who,"You must be in a market to use this skill.");
    return FALSE;
  };

  /*
   *  Needs to be a good traded in this guild (by the guild).
   *
   *  Fri Nov 13 12:54:20 1998 -- Scott Turner
   *
   *  Note that all these functions implicitly assume that a loc
   *  has only one BUY or SELL for a particular good.
   *
   */
  if (!good || !(t = traded_here(where,good))) {
    wout(c->who,"That good is not traded here.");
    return FALSE;
  };

  /*
   *  Maybe a problem?
   *
   */
  if (!rp_item(t->item)) {
    wout(c->who,"I'm confused about that trade good, tell the GM.");
    return FALSE;
  };

  /*
   *  No more than 2*base_price
   *
   */
  if (rp_item(t->item) && rp_item(t->item)->base_price*2 <= t->cost) {
    wout(c->who, "The demand for %s is straining the market and cannot be further increased.", box_name(t->item));
    return FALSE;
  };
    
  /*
   *  Up the price, etc.
   *
   */
  change = (t->cost * (5 + rnd(1,3))) / 100;
  if (change < 1) change = 1;
  t->cost += change;
  wout(c->who,"The demand for %s increases; the new price is %s.",
       box_name(t->item),
       gold_s(t->cost));

  /*
   *  Also modify the consume/produce price, as necessary.
   *
   */
  if ((t = produced_here(where,good))) {
    t->cost +=change;
  };

  return TRUE;
};

/*
 *
 */
int
v_decrease_demand(struct command *c)
{
  int where = subloc(c->who);
  int good = c->a;

  /*
   *  You need to be in the guild where the good is being traded.
   *
   *  Fri Nov 13 12:49:25 1998 -- Scott Turner
   *
   *  Now works in any market, but only on goods sold/bought by the
   *  market.
   */
  if (!market_here(where)) {
    wout(c->who,"You must be in a market to use this skill.");
    return FALSE;
  };

  /*
   *  Needs to be a good traded in this guild (by the guild).
   *
   */
  if (!good || !traded_here(where,good)) {
    wout(c->who,"That good is not traded here.");
    return FALSE;
  };

  return TRUE;
};

int
d_decrease_demand(struct command *c)
{
  int where = subloc(c->who);
  int good = c->a, change;
  struct trade *t;

  /*
   *  You need to be in the guild where the good is being traded.
   *
   *  Fri Nov 13 12:49:25 1998 -- Scott Turner
   *
   *  Now works in any market, but only on goods sold/bought by the
   *  market.
   */
  if (!market_here(where)) {
    wout(c->who,"You must be in a market to use this skill.");
    return FALSE;
  };

  /*
   *  Needs to be a good traded in this guild (by the guild).
   *
   */
  if (!good || !(t = traded_here(where,good))) {
    wout(c->who,"That good is not traded here.");
    return FALSE;
  };

  /*
   *  Maybe a problem?
   *
   */
  if (!rp_item(t->item)) {
    wout(c->who,"I'm confused about that trade good, tell the GM.");
    return FALSE;
  };

  /*
   *  No more than 2*base_price
   *
   */
  if (rp_item(t->item) && rp_item(t->item)->base_price/2 >= t->cost) {
    wout(c->who, "The demand for %s has bottomed out and cannot be further decreased.", box_name(t->item));
    return FALSE;
  };
    
  /*
   *  Up the price, etc.
   *
   */
  change = (t->cost * (5 + rnd(1,3))) / 100;
  if (change < 1) change = 1;
  t->cost -= change;
  wout(c->who,"The demand for %s decreases; the new price is %s.",
       box_name(t->item),
       gold_s(t->cost));

  /*
   *  Also modify the consume/produce price, as necessary.
   *
   */
  if ((t = produced_here(where,good))) {
    t->cost -= change;
  };

  return TRUE;
};

/*
 *  Mon Nov 25 11:01:03 1996 -- Scott Turner
 *
 *  Increase the # of a good demanded or offered.
 */
int
v_increase_supply(struct command *c)
{
  int where = subloc(c->who);
  int good = c->a;

  /*
   *  You need to be in the guild where the good is being traded.
   *
   *  Fri Nov 13 12:49:25 1998 -- Scott Turner
   *
   *  Now works in any market, but only on goods sold/bought by the
   *  market.
   */
  if (!market_here(where)) {
    wout(c->who,"You must be in a market to use this skill.");
    return FALSE;
  };

  /*
   *  Needs to be a good traded in this guild (by the guild).
   *
   */
  if (!good || !traded_here(where,good) || !produced_here(where,good)) {
    wout(c->who,"That good is not traded here.");
    return FALSE;
  };

  return TRUE;
};

int
d_increase_supply(struct command *c)
{
  int where = subloc(c->who);
  int good = c->a;
  float change;
  struct trade *t, *p;

  /*
   *  You need to be in the guild where the good is being traded.
   *
   *  Fri Nov 13 12:49:25 1998 -- Scott Turner
   *
   *  Now works in any market, but only on goods sold/bought by the
   *  market.
   */
  if (!market_here(where)) {
    wout(c->who,"You must be in a market to use this skill.");
    return FALSE;
  };

  /*
   *  Needs to be a good produced in this guild (by the guild).
   *
   */
  if (!good || !(t = traded_here(where, good)) ||
      !(p = produced_here(where,good))) {
    wout(c->who,"That good is not produced here.");
    return FALSE;
  };

  /*
   *  Maybe a problem?
   *
   */
  if (!rp_item(t->item)) {
    wout(c->who,"I'm confused about that trade good, tell the GM.");
    return FALSE;
  };

  /*
   *  No more than the year's supply
   *
   */
  if (rp_item(t->item) && rp_item(t->item)->trade_good <= t->qty) {
    wout(c->who, "The supply of %s is exhausted and cannot be further increased.", box_name(t->item));
    return FALSE;
  };
    
  /*
   *  Add up to 15-33% more of the good; handle amounts < 1 as
   *  chance...
   *
   */
  change = ((p->qty/8) * (14 + rnd(1,19))) / 100.0;
  if (change < 1 && rnd(1,100) < change*100) change = 1.0;
  t->qty += change;
  wout(c->who,"The supply of %s increases; the new amount available is %d.",
       box_name(t->item), t->qty);

  return TRUE;
};

/*
 *
 */
int
v_decrease_supply(struct command *c)
{
  int where = subloc(c->who);
  int good = c->a;

  /*
   *  You need to be in the guild where the good is being traded.
   *
   *  Fri Nov 13 12:49:25 1998 -- Scott Turner
   *
   *  Now works in any market, but only on goods sold/bought by the
   *  market.
   */
  if (!market_here(where)) {
    wout(c->who,"You must be in a market to use this skill.");
    return FALSE;
  };

  /*
   *  Needs to be a good traded in this guild (by the guild).
   *
   */
  if (!good || !traded_here(where,good) || !produced_here(where,good)) {
    wout(c->who,"That good is not traded here.");
    return FALSE;
  };

  return TRUE;
};

int
d_decrease_supply(struct command *c)
{
  int where = subloc(c->who);
  int good = c->a;
  float change;
  struct trade *t, *p;

  /*
   *  You need to be in the guild where the good is being traded.
   *
   *  Fri Nov 13 12:49:25 1998 -- Scott Turner
   *
   *  Now works in any market, but only on goods sold/bought by the
   *  market.
   */
  if (!market_here(where)) {
    wout(c->who,"You must be in a market to use this skill.");
    return FALSE;
  };

  /*
   *  Needs to be a good traded in this guild (by the guild).
   *
   */
  if (!good ||
      !(t = traded_here(where, good)) ||
      !(p = produced_here(where,good))) {
    wout(c->who,"That good is not traded here.");
    return FALSE;
  };

  /*
   *  Maybe a problem?
   *
   */
  if (!rp_item(t->item)) {
    wout(c->who,"I'm confused about that trade good, tell the GM.");
    return FALSE;
  };

  /*
   *  No more than the year's supply
   *
   */
  if (t->qty < 1) {
    wout(c->who, "The supply of %s cannot be further decreased.", box_name(t->item));
    return FALSE;
  };
    
  /*
   *  Add up to 15-33% more of the good; handle amounts < 1 as
   *  chance...
   *
   */
  change = ((p->qty/8) * (14 + rnd(1,19))) / 100.0;
  if (change < 1 && rnd(1,100) < change*100) change = 1.0;
  t->qty -= change;
  if (t->qty < 0) t->qty = 0;
  wout(c->who,"The supply of %s decreases; the new amount available is %d.",
       box_name(t->item), t->qty);

  return TRUE;
};

/*
 *  Sat Nov 23 10:38:41 1996 -- Scott Turner
 *
 *  Implement this by hanging an effect on the guy.  If hiding nothing,
 *  then unhide the hidden item.
 */
int
v_hide_item(struct command *c)
{
  int item=c->a;

  /*
   *  Is he unhiding?
   *
   */
  if (!item && !get_effect(c->who, ef_hide_item, 0, 0)) {
    wout(c->who,"You don't have anything to unhide.");
    return FALSE;
  };

  /*
   *  Is it an item?
   *
   */
  if (item && (!valid_box(item) || kind(item) != T_item)) {
    wout(c->who, "You can't hide such a thing.");
    return FALSE;
  };

  /*
   *  Does he have the item?
   *
   */
  if (item && !has_item(c->who, item)) {
    wout(c->who,"You do not have that item to hide.");
    return FALSE;
  };
  
  return TRUE;
};

/*
 *  Tue Nov 26 16:02:34 1996 -- Scott Turner
 *
 *  For a unique item, I guess we want to move it onto some
 *  appropriate entity while it is "hidden".
 *
 */
int
d_hide_item(struct command *c)
{
  int item=c->a;
  int what = get_effect(c->who, ef_hide_item, 0, 0);
    
  /*
   *  Is he unhiding?
   *
   */
  if (!item) {
    if (!what) {
      wout(c->who,"You don't have anything to unhide.");
      return FALSE;
    };
    /*
     *  Otherwise unhide it.
     *
     */
    delete_effect(c->who, ef_hide_item, 0);
    if (item_unique(what)) {
      move_item(indep_player, c->who, what, 1);
    } else {
      gen_item(c->who, what, 1);
    };
    wout(c->who,"You unhide one %s.", box_name(what));
    return TRUE;
  };

  /*
   *  Is it an item?
   *
   */
  if (!valid_box(item) || kind(item) != T_item) {
    wout(c->who, "You can't hide such a thing.");
    return FALSE;
  };

  /*
   *  Does he have the item?
   *
   */
  if (!has_item(c->who, item)) {
    wout(c->who,"You do not have that item to hide.");
    return FALSE;
  };

  /*
   *  Possibly unhide.
   *
   */
  if (what) {
    delete_effect(c->who, ef_hide_item, 0);
    if (item_unique(what)) {
      move_item(indep_player, c->who, what, 1);
    } else {
      gen_item(c->who, what, 1);
    };
    wout(c->who,"You unhide one %s.", box_name(what));
  };
  
  /*
   *  Hang the effect and delete the item from his possession.
   *
   */
  if (!item_unique(item)) {
    consume_item(c->who, item, 1);
  } else {
      move_item(c->who, indep_player, item, 1);
  };
  add_effect(c->who, ef_hide_item, 0, 1, item);
  wout(c->who,"You hide one %s.",box_name(item));
  
  return TRUE;
};

/*
 *  Sun Nov 24 11:29:12 1996 -- Scott Turner
 *
 *  Similar, but put it in the hidden money slot.
 *
 *  The argument is the total amount of money hidden.
 *
 */
int
v_hide_money(struct command *c)
{
  int amount=c->a;
  int what = get_effect(c->who, ef_hide_money, 0, 0);

  /*
   *  None to unhide.
   *
   */
  if (!amount && !what) {
    wout(c->who, "You don't have any money to unhide.");
    return FALSE;
  };
    
  return TRUE;
};

int
d_hide_money(struct command *c)
{
  int amount=c->a;
  int what = get_effect(c->who, ef_hide_money, 0, 0);

  /*
   *  None to unhide.
   *
   */
  if (!amount && !what) {
    wout(c->who, "You don't have any money to unhide.");
    return FALSE;
  };
    
  /*
   *  Does he have enough money?
   *
   */
  if (what + has_item(c->who, item_gold) < amount) {
    wout(c->who,"You don't have %s to hide, hiding %s.",
	 gold_s(amount-what), has_item(c->who, item_gold));
    what = has_item(c->who, item_gold);
  };

  /*
   *  Adjust the levels.
   *
   */
  if (amount-what > 0) {
    charge(c->who, amount-what);
    wout(c->who,"Adding %s to the amount in hiding.",gold_s(amount-what));
  } else {
    gen_item(c->who, item_gold, what-amount);
    wout(c->who,"Removing %s from the amount in hiding.",
	 gold_s(what-amount));
  };
  delete_effect(c->who, ef_hide_money, 0);
  add_effect(c->who, ef_hide_money, 0, 1, amount);
  wout(c->who,"You now have %s in hiding.",gold_s(amount));
  
  return TRUE;
};
