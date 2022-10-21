// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    <unistd.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"

/*
 *  Mining-related skills and functions.
 *
 */
#define MINE_PRODUCTS 4

int mine_products[MINE_PRODUCTS] = {item_iron, item_gold, item_mithril,
                                    item_gate_crystal};

struct {
    int mins[MINE_PRODUCTS];
    int maxs[MINE_PRODUCTS];
} mine_qties[MINE_MAX + 1] = {
/*        iron  gold  mithril gate_crystal  */
/*        ----  ----  ------- ------------  */
/* 0 */   {{0,  0,   0, 0},
                  {0,  0,    0,  0}},
/* 1 */
          {{0,  0,   0, 0},
                  {10, 0,    0,  0}},
/* 2 */
          {{10, 0,   0, 0},
                  {20, 0,    0,  0}},
/* 3 */
          {{10, 0,   0, 0},
                  {30, 0,    0,  0}},
/* 4 */
          {{10, 0,   0, 0},
                  {40, 0,    0,  0}},
/* 5 */
          {{10, 0,   0, 0},
                  {30, 100,  0,  0}},
/* 6 */
          {{0,  0,   0, 0},
                  {20, 300,  0,  0}},
/* 7 */
          {{0,  100, 0, 0},
                  {20, 450,  0,  0}},
/* 8 */
          {{0,  120, 0, 0},
                  {20, 600,  0,  0}},
/* 9 */
          {{0,  140, 0, 0},
                  {20, 750,  0,  0}},
/*10 */
          {{0,  160, 0, 0},
                  {20, 900,  1,  0}},
/*11 */
          {{0,  180, 0, 0},
                  {0,  1000, 3,  0}},
/*12 */
          {{0,  200, 0, 0},
                  {0,  1200, 6,  0}},
/*13 */
          {{0,  220, 0, 0},
                  {0,  1300, 9,  0}},
/*14 */
          {{0,  240, 0, 0},
                  {0,  1500, 12, 0}},
/*15 */
          {{0,  200, 0, 0},
                  {0,  1200, 15, 1}},
/*16 */
          {{0,  160, 0, 0},
                  {0,  900,  10, 1}},
/*17 */
          {{0,  120, 0, 0},
                  {0,  600,  5,  1}},
/*18 */
          {{0,  80,  0, 0},
                  {0,  300,  0,  1}},
/*19 */
          {{0,  20,  0, 1},
                  {0,  100,  0,  1}},
/*20 */
          {{0,  0,   0, 1},
                  {0,  100,  0,  3}}
};

/*
 *  Mine_Chance
 *  Fri Jan 24 12:51:08 1997 -- Scott Turner
 *
 *  This function returns the % chance per day that a particular
 *  miner finds a particular item.  The chance depends upon:
 * 
 *   (1) the miner's experience (skill_exp == months practiced)
 *   (2) the item being sought
 *   (3) the number of workers
 *
 *  According to the following formula:
 *
 *   (1) Initial chance from item.
 *   (2) +2% for each month experience (max +50%)
 *   (3) +0.1% for each worker (max +10%)
 *
 *  Per week of mining, so that we divide by 7 at the end.
 *
 */
static float
mine_chance(int item, int who, int skill) {
    float initial_chance = 0.0, exp_bonus = 0.0, worker_bonus = 0.0;

    switch (item) {
        case item_iron:
            initial_chance = 70.0;
            break;
        case item_gold:
            initial_chance = 50.0;
            break;
        case item_mithril:
            initial_chance = 30.0;
            break;
        case item_gate_crystal:
            initial_chance = 10.0;
            break;
        default:
            return 0;
    };

    /*
     *  Add experience bonus.
     *
     */
    exp_bonus = skill_exp(who, skill) * 5.0;
    if (exp_bonus > 50) { exp_bonus = 50.0; }

    /*
     *  Worker bonus
     *
     */
    worker_bonus = effective_workers(who) * 0.20;
    if (worker_bonus > 20.0) { worker_bonus = 20.0; }

    /*
     *  Add it up and return the daily %.
     *
     */
    return (initial_chance + exp_bonus + worker_bonus) / 7.0;

};

/*
 *  Mon Jan 20 11:54:43 1997 -- Scott Turner
 *
 *  Mine depth is now how deep underground your mine shaft location
 *  is (nested).
 *
 *  Wed May  7 12:22:12 1997 -- Scott Turner
 *
 *  Mine shafts are no longer nested -they are up/down.  So you want
 *  to follow the "up link" to determine how deep you are.  Ugh.
 *
 */
int
mine_depth(int where) {
    int depth = -1;

    while (where > 0 && subkind(where) == sub_mine_shaft) {
        where = location_direction(where, DIR_UP);
        depth++;
    };

    return depth;

};

/*
 *  Sat Jan 25 11:20:37 1997 -- Scott Turner
 *
 *  Get the mine that heads up this mine shaft.
 *
 *  Wed May 13 19:43:34 1998 -- Scott Turner
 *
 *  Ugghhhhhhhh.  This is considerably uglier, since mine shafts are no
 *  longer sublocs.  We need to go "up" until we find a mine_info.
 */
struct entity_mine
*get_mine_info(int where) {
    int up;

    if (subkind(where) != sub_mine_shaft) { return NULL; }

    while (where > 0 && subkind(where) == sub_mine_shaft) {
        up = location_direction(where, DIR_UP);
        where = up;
    };

    assert(where);

    if (!p_loc(where)->mine_info) { create_mine_info(where); }

    return p_loc(where)->mine_info;

};

/*
 *  Fri Jan 31 12:47:13 1997 -- Scott Turner
 *
 *  Create mine info when first accessed.
 *
 */
void
create_mine_info(int mine) {
    int i, j;
    ilist *el = NULL;
    struct item_ent *new;

    assert(!p_loc(mine)->mine_info);

    /*
     *  Allocate the entity_mine.
     *
     */
    p_loc(mine)->mine_info = my_malloc(sizeof(*p_loc(mine)->mine_info));

    /*
     *  Now go through each level of the mine and add in all
     *  the goods for that level...
     *
     */
    for (i = 0; i < MINE_MAX; i++) {
        el = (ilist *) &(p_loc(mine)->mine_info->mc[i].items);
        p_loc(mine)->mine_info->shoring[i] = NO_SHORING;
        for (j = 0; j < MINE_PRODUCTS; j++) {
            int qty = rnd(mine_qties[i].mins[j], mine_qties[i].maxs[j]);
            if (qty) {
                new = my_malloc(sizeof(*new));
                new->item = mine_products[j];
                new->qty = qty;
                ilist_append(el, (int) new);
            };
        };
    };
};

/*
 *  Sat Jan 25 11:30:04 1997 -- Scott Turner
 *
 *  Return the amount of some item at some mine level.
 *
 */
static int
mine_has_item(int mine, int depth, int item) {
    int i;
    struct entity_mine *mi = get_mine_info(mine);

    if (!mi) { return 0; }

    assert(depth >= 0);
    if (depth >= MINE_MAX) { depth = MINE_MAX - 1; }

    for (i = 0; i < ilist_len(mi->mc[depth].items); i++) {
        if (mi->mc[depth].items[i]->item == item) {
            return mi->mc[depth].items[i]->qty;
        }
    }

    return 0;
};

/*
 *  Sat Jan 25 11:30:04 1997 -- Scott Turner
 *
 *  Decrement the amount of some item at some mine level.
 *
 */
static void
mine_sub_item(int mine, int depth, int item, int amount) {
    int i;
    struct entity_mine *mi = get_mine_info(mine);

    if (!mi) { return; }

    assert(depth >= 0);
    if (depth >= MINE_MAX) { depth = MINE_MAX - 1; }

    for (i = 0; i < ilist_len(mi->mc[depth].items); i++) {
        if (mi->mc[depth].items[i]->item == item) {
            mi->mc[depth].items[i]->qty -= amount;
            return;
        }
    };

    return;
};

int
start_generic_mine(struct command *c, int item, int skill) {
    int where = subloc(c->who);
    int days = c->a ? c->a : 7;
    int nworkers;

    if (subkind(where) != sub_mine_shaft) {
        wout(c->who, "Must be in a mine shaft to extract %s.",
             just_name(item));
        return FALSE;
    }

    nworkers = effective_workers(c->who);
    if (nworkers < 10) {
        wout(c->who, "Mining requires at least ten workers.");
        return FALSE;
    }

    c->wait = days;

    wout(c->who, "Will mine %s for the next %s days.",
         just_name(item),
         nice_num(c->wait));

    return TRUE;
}


int
finish_generic_mine(struct command *c, int item, int skill) {
    int where = subloc(c->who);
    int has;
    int nworkers;
    int qty;
    int depth;
    float chance;
    int lode;

    if (subkind(where) != sub_mine_shaft) {
        wout(c->who, "%s is no longer in a mine shaft.", box_name(c->who));
        return FALSE;
    };

    nworkers = effective_workers(c->who);
    if (nworkers < 10) {
        wout(c->who, "%s no longer has ten workers.",
             box_name(c->who));
        return FALSE;
    };


    /*
     *  Calculate the chance that we found some of whatever we're
     *  looking for...
     *
     */
    chance = mine_chance(item, c->who, skill) * c->days_executing;

    if (chance < rnd(1, 100)) {
        wout(c->who, "Mining yielded no %s.", just_name(item));
        return FALSE;
    };

    /*
     *  Now see if there's any of that left here at this level.
     *
     */
    depth = mine_depth(where);
    has = mine_has_item(where, depth, item);
    if (!has) {
        wout(c->who, "Mining yielded no %s.", just_name(item));
        if (rnd(1, 100) < 70) {
            wout(c->who, "This level appears to be mined out.");
        };
        return FALSE;
    };

    /*
     *  How much of this do we actually get?  We can recover from
     *  40-60% of this, depending upon how many men we have as workers.
     *
     */
    if (nworkers > 100) { nworkers = 100; }
    qty = (has * rnd(40, 40 + (nworkers / 5))) / 100;

    /*
     *  Guarantee at least 1 for gate crystals, mithril, etc.
     *
     */
    if (!qty) { qty = 1; }

    /*
     *  Remove that much from the mine; any bonuses don't affect
     *  the actual amount in the mine.
     *
     */
    mine_sub_item(where, depth, item, qty);

    /*
     *  This mine might be blessed.
     *
     */
    if (get_effect(where, ef_improve_mine, 0, 0)) {
        qty += ((float) qty * 0.50 + 0.50);
        wout(c->who, "%s is unusually productive.", box_name(where));
    };

    /*
     *  We might have hit a rich lode.
     *
     */
    lode = rnd(1, 5000);
    if (lode < c->days_executing) {
        wout(c->who, "You hit an incredibly rich lode of %s!", just_name(item));
        qty *= 100;
    } else if (lode < c->days_executing * 10) {
        wout(c->who, "You hit a very rich lode of %s!", just_name(item));
        qty *= 10;
    } else if (lode < c->days_executing * 100) {
        wout(c->who, "You hit a rich lode of %s!", just_name(item));
        qty *= 2;
    };

    gen_item(c->who, item, qty);

    wout(c->who, "Mining yielded %s.", box_name_qty(item, qty));
    return TRUE;
}


int
v_mine_iron(struct command *c) {

    return start_generic_mine(c, item_iron, sk_mine_iron);
}


int
d_mine_iron(struct command *c) {

    return finish_generic_mine(c, item_iron, sk_mine_iron);
}


int
v_mine_gold(struct command *c) {

    return start_generic_mine(c, item_gold, sk_mine_gold);
}


int
d_mine_gold(struct command *c) {

    return finish_generic_mine(c, item_gold, sk_mine_gold);
}


int
v_mine_mithril(struct command *c) {

    return start_generic_mine(c, item_mithril, sk_mine_mithril);
}


int
d_mine_mithril(struct command *c) {

    return finish_generic_mine(c, item_mithril, sk_mine_mithril);
}

int
v_mine_gate_crystal(struct command *c) {

    return start_generic_mine(c, item_gate_crystal, sk_mine_crystal);
}


int
d_mine_gate_crystal(struct command *c) {

    return finish_generic_mine(c, item_gate_crystal, sk_mine_crystal);
}


/*
 *  Add_Wooden_Shoring
 *  Tue Feb  4 08:45:40 1997 -- Scott Turner
 *
 *  Modify the appropriate level of a mine shaft with wooden shoring.
 *
 */
int
d_add_wooden_shoring(struct command *c) {
    int depth;
    struct entity_mine *mi;
    int where = subloc(c->who);

    if (subkind(where) != sub_mine_shaft) {
        wout(c->who, "You must be in a mine shaft to add wooden shoring.");
        return FALSE;
    };

    /*
     *  Maybe already shored?
     *
     */
    mi = get_mine_info(where);
    depth = mine_depth(where);
    assert(mi);

    if (mi->shoring[depth] >= WOODEN_SHORING) {
        wout(c->who, "This mine shaft already has wooden shoring.");
        return FALSE;
    }

    mi->shoring[depth] = WOODEN_SHORING;
    wout(c->who, "Wooden shoring added to %s.", box_name(where));

    return TRUE;
}

int
v_add_wooden_shoring(struct command *c) {
    int depth;
    struct entity_mine *mi;
    int where = subloc(c->who);

    if (subkind(where) != sub_mine_shaft) {
        wout(c->who, "You must be in a mine shaft to add wooden shoring.");
        return FALSE;
    };

    /*
     *  Maybe already shored?
     *
     */
    mi = get_mine_info(where);
    depth = mine_depth(where);
    assert(mi);

    if (mi->shoring[depth] >= WOODEN_SHORING) {
        wout(c->who, "This mine shaft already has wooden shoring.");
        return FALSE;
    }

    /*
     *  Needs wood.
     *
     */
    if (has_item(c->who, item_lumber) < 25) {
        wout(c->who, "You do not have sufficient wood to shore this shaft.");
        return FALSE;
    };

    return TRUE;
}

/*
 *  Add_Iron_Shoring
 *  Tue Feb  4 08:45:40 1997 -- Scott Turner
 *
 *  Modify the appropriate level of a mine shaft with iron shoring.
 *
 */
int
d_add_iron_shoring(struct command *c) {
    int depth;
    struct entity_mine *mi;
    int where = subloc(c->who);

    if (subkind(where) != sub_mine_shaft) {
        wout(c->who, "You must be in a mine shaft to add iron shoring.");
        return FALSE;
    };

    /*
     *  Maybe already shored?
     *
     */
    mi = get_mine_info(where);
    depth = mine_depth(where);
    assert(mi);

    if (mi->shoring[depth] >= IRON_SHORING) {
        wout(c->who, "This mine shaft already has iron shoring.");
        return FALSE;
    }

    mi->shoring[depth] = IRON_SHORING;
    wout(c->who, "Iron shoring added to %s.", box_name(where));

    return TRUE;
}

int
v_add_iron_shoring(struct command *c) {
    int depth;
    struct entity_mine *mi;
    int where = subloc(c->who);

    if (subkind(where) != sub_mine_shaft) {
        wout(c->who, "You must be in a mine shaft to add iron shoring.");
        return FALSE;
    };

    /*
     *  Maybe already shored?
     *
     */
    mi = get_mine_info(where);
    depth = mine_depth(where);
    assert(mi);

    if (mi->shoring[depth] >= IRON_SHORING) {
        wout(c->who, "This mine shaft already has iron shoring.");
        return FALSE;
    }

    /*
     *  Needs iron.
     *
     */
    if (has_item(c->who, item_iron) < 5) {
        wout(c->who, "You do not have sufficient iron to shore this shaft.");
        return FALSE;
    };

    return TRUE;
}

