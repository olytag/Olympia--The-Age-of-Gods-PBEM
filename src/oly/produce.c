// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


/* region production routines */


#define        MOUNTAIN_STONE    50
#define        POPPY_OPIUM    25


struct terr_struct {
    int terr;        /* terrain type */
    int item;        /* good produced by location */
    int qty;        /* amount produced */
    int max;                      /* maximum qty in location */
};

struct terr_struct terr_prod[] =
        {
                {sub_forest,        item_lumber,       30, 30},
                {sub_sacred_grove,  item_lumber,       5,  5},
                {sub_tree_circle,   item_lumber,       5,  5},

                {sub_mountain,      item_stone, MOUNTAIN_STONE, MOUNTAIN_STONE},
                {sub_rocky_hill,    item_stone, MOUNTAIN_STONE, MOUNTAIN_STONE},
                {sub_desert,        item_stone,        10, 10},

                {sub_cave,          item_farrenstone,  2,  2},
                {sub_ruins,         item_ratspider,    1,  1},
                {sub_plain,         item_wild_horse,   1,  100},
                {sub_pasture,       item_wild_horse,   3,  20},
                {sub_ocean,         item_fish,         50, 50},

                {sub_mallorn_grove, item_avinia_leaf,  1,  4},
                {sub_mallorn_grove, item_mallorn_wood, 2,  2},

                {sub_bog,           item_spiny_root,   2,  2},
                {sub_pits,          item_spiny_root,   2,  2},
                {sub_swamp,         item_spiny_root,   1,  1},

                {sub_yew_grove,     item_yew,          5,  5},
                {sub_graveyard,     item_corpse,       30, 30},
                {sub_tree_circle,   item_lana_bark,    3,  30},
                {sub_sand_pit,      item_pretus_bones, 1,  1},
                {sub_poppy_field,   item_opium, POPPY_OPIUM,    POPPY_OPIUM},

                /*
                 *  Sun Dec  1 10:31:53 1996 -- Scott Turner
                 *
                 *  Peasant production is now a special case.
                 *
                {sub_forest, 	item_peasant, 	10, 10000},
                {sub_mountain, 	item_peasant, 	10, 10000},
                {sub_plain, 	item_peasant, 	10, 10000},
                {sub_city, 	item_peasant, 	10, 10000},
                */

                {0, 0,                                 0,  0}
        };

/*
 *  Mon Sep 16 11:41:04 1996 -- Scott Turner
 *
 *  Terrain production for places that contain something, e.g., provinces
 *  that contain temples produce postulants.
 *
 */
struct terr_struct terr_prod2[] =
        {
                {sub_temple, item_postulant, 5, 5},

                {0, 0,                       0}
        };


/*
 *  Tue Jul  7 09:59:59 1998 -- Scott Turner
 *
 *  Replenish up to "qty" but not more than max total.
 *
 *  Tue Jul  7 10:31:23 1998 -- Scott Turner
 *
 *  For "population" type production, driving the population to zero
 *  is a bad idea.  max != qty in the terr_prod table indicates a
 *  population type resource.
 *
 */
static void
replenish(int where, int item, int qty, int max) {
    int n;

    n = has_item(where, item);
    if (n == 0 && max != qty) {
        if (rnd(1, 100) > 50) {
            gen_item(where, item, 1);
        }
    } else if (n < max) {
        int m = max - n;
        if (m > qty) { m = qty; }
        gen_item(where, item, m);
    };
}

void
location_production() {
    int where;
    int i, enclosed;
    int terr, encl_terr;
    int has_city;
    float pop_grow = 0.0;
    int pop_limit = 200, pop, dpop = 0;

    loop_loc(where)
            {
                terr = subkind(where);
                has_city = 0;

                for (i = 0; terr_prod[i].terr; i++) {
                    if (terr_prod[i].terr == terr) {
                        replenish(where, terr_prod[i].item, terr_prod[i].qty,
                                  terr_prod[i].max);
                    }
                }

                /*
                 *  Mon Sep 16 11:42:22 1996 -- Scott Turner
                 *
                 *  Now check for production from enclosed locations...
                 *
                 */
                loop_here(where, enclosed)
                        {
                            encl_terr = subkind(enclosed);
                            if (encl_terr == sub_city) { has_city = 1; }
                            for (i = 0; terr_prod2[i].terr; i++) {
                                if (terr_prod2[i].terr == encl_terr) {
                                    replenish(where, terr_prod2[i].item, terr_prod2[i].qty,
                                              terr_prod2[i].max);
                                }
                            };
                        }next_here;

                /*
             *  First limit poppy fields to normal production level.
             *  Then double opium if poppy field was specially tended.
             */
                if (terr == sub_poppy_field) {
                    int n;

                    n = has_item(where, item_opium);
                    if (n > POPPY_OPIUM) {
                        consume_item(where, item_opium, n - POPPY_OPIUM);
                    }

                    if (rp_misc(where) && rp_misc(where)->opium_double) {
                        rp_misc(where)->opium_double = FALSE;
                        gen_item(where, item_opium, has_item(where, item_opium));
                    }
                }

                if (terr == sub_island ||
                    (loc_depth(where) == LOC_province && has_ocean_access(where))) {
                    replenish(where, item_flotsam, 30, 30);
                }

                /*
                 *  Sun Dec  1 10:34:41 1996 -- Scott Turner
                 *
                 *  Peasant production.  Depends upon the location (and
                 *  whether it contains a city).
                 *
                 *  Has_city is set up above...
                 *
                 *  Tue Sep 22 13:20:18 1998 -- Scott Turner
                 *
                 *  Faery ought not have peasants.  It should have (I guess)
                 *  elf peasants, although what you can do with those is
                 *  open to conjecture :-)
                 *
                 */
                if (pop = has_item(where, item_peasant)) {
                    if (has_city) {
                        pop_grow = 0.03;
                        pop_limit = 10000;
                    } else {
                        switch (terr) {
                            case sub_plain:
                            case sub_forest:
                                pop_grow = 0.01;
                                pop_limit = 1000;
                                break;
                            case sub_mountain:
                            case sub_swamp:
                                pop_grow = 0.005;
                                pop_limit = 1000;
                                break;
                            default:
                                pop_grow = 0.000;
                                pop_limit = 500;
                                break;
                        };
                    };

                    /*
                     *  Might be an effect here.
                     *
                     */
                    if (get_effect(where, ef_grow, 0, 0)) {
                        wout(where, "The peasants seem particularly happy this month.");
                        pop_grow += 0.02;
                    };

                    dpop = pop * pop_grow;
                    if (pop_grow > 0.0 && dpop < 1) { dpop = 1; }
                    /*
                     *  Lose population at a reasonable rate.
                     *
                     */
                    if (pop > pop_limit) { dpop = -(pop - pop_limit) / 10; }

                    if (p_subloc(province(where))->loot && dpop > 0) {
                        wout(where, "Pillaging traumatizes the population and no growth occurs.");
                    } else if (dpop > 0) {
                        if (pop > 100) {
                            wout(where, "The population grows by %s peasant%s.",
                                 nice_num(dpop), add_s(dpop));
                        }
                        gen_item(where, item_peasant, dpop);
                    } else {
                        if (pop > 100) {
                            wout(where, "Overcrowding causes %s peasant death%s.",
                                 nice_num(-dpop), add_s(-dpop));
                        }
                        consume_item(where, item_peasant, -dpop);
                    };
                };

                /*
                 *  Sat Apr 18 16:57:53 1998 -- Scott Turner
                 *
                 *  Special case for gold production from peasants.
                 *  They generate 1 gold per 20 peasants (1/10 in cities)
                 *  which accumulates to be removed by various means.
                 *
                 *  Only in civilized (> 100) provinces
                 */
                if ((pop = has_item(where, item_peasant)) > 100) {
                    if (has_city) {
                        dpop = pop * 0.10;
                    } else {
                        dpop = pop * 0.05;
                    };
                    gen_item(where, item_gold, dpop);
                };
            }next_loc;
}


static int
item_gen_here(int where, int item) {
    int i, enclosed, encl_terr;

    for (i = 0; terr_prod[i].terr; i++) {
        if (terr_prod[i].terr == subkind(where) &&
            terr_prod[i].item == item) {
            return TRUE;
        }
    }

    /*
     *  Might be generated by something here.
     *
     */
    loop_here(where, enclosed)
            {
                encl_terr = subkind(enclosed);
                for (i = 0; terr_prod2[i].terr; i++) {
                    if (terr_prod2[i].terr == encl_terr &&
                        terr_prod2[i].item == item) {
                        return TRUE;
                    }
                };
            }next_here;

    return FALSE;
}


#ifndef OLY_FORWARD_struct_harvest
#define OLY_FORWARD_struct_harvest
struct harvest {
    int item;
    int skill;
    int worker;
    int chance;       /* chance to get one each day, if nonzero */
    char *got_em;
    char *none_now;
    char *none_ever;
    char *task_desc;
    int public;       /* 3rd party view, yes/no */
    int piety;        /* Does it use piety? */
};
#endif //OLY_FORWARD_struct_harvest


struct harvest harv_tbl[] = {
        {
                item_peasant,
                0,
                0,
                0,
                "recruited",
                "There are no more peasants here to recruit.",
                "There are no peasants here to recruit.",
                "recruit peasants",
                TRUE,
                0
        },
        {
                item_postulant,
                sk_proselytise,
                0,
                0,
                "recruited",
                "There are no more postulants here to recruit.",
                "Postulants must be recruited near a temple.",
                "recruit postulants",
                TRUE,
                3
        },
        {
                item_postulant,
                sk_proselytise_b,
                0,
                0,
                "recruited",
                "There are no more postulants here to recruit.",
                "Postulants must be recruited near a temple.",
                "recruit postulants",
                TRUE,
                3
        },
        {
                item_corpse,
                0,
                0,
                0,
                "raised",
                "There are no more corpses here to raise.",
                "Corpses are found in graveyards.",
                "raise corpses",
                FALSE,
                0
        },
        {
                item_mallorn_wood,
                sk_harvest_mallorn,
                0,
                20,
                "cut",
                "All mallorn wood ready this month has been cut here.",
                "Mallorn wood is found only in mallorn groves.",
                "cut mallorn wood",
                TRUE,
                0
        },
        {
                item_opium,
                sk_harvest_opium,
                0,
                0,
                "harvested",
                "All opium  ready this month has been harvested.",
                "Opium is harvested only in poppy fields.",
                "harvest opium",
                TRUE,
                0
        },
        {
                item_stone,
                sk_quarry_stone,
                item_worker,
                0,
                "quarried",
                "No further stone may be quarried here this month.",
                "Stone must be quarried in mountain provinces.",
                "quarry stone",
                TRUE,
                0
        },
        {
                item_fish,
                sk_fishing,
                item_sailor,
                50,
                "caught",
                "No further fish may be caught here this month.",
                "Fish must be caught in ocean provinces.",
                "catch fish",
                TRUE,
                0
        },
        {
                item_lumber,
                sk_harvest_lumber,
                item_worker,
                0,
                "cut",
                "All ready timber has already been cut this month.",
                "Wood must be cut in forest provinces.",
                "cut timber",
                TRUE,
                0
        },
        {
                item_yew,
                sk_harvest_yew,
                item_worker,
                0,
                "cut",
                "All yew available this month has already been cut.",
                "Yew must be cut in yew groves",
                "cut yew",
                TRUE,
                0
        },
        {
                item_wild_horse,
                sk_catch_horse,
                0,
                25,
                "caught",
                "No wild horses can be found roaming here now.",
                "Wild horses are found on the plains and in pastures.",
                "catch horses",
                TRUE,
                0
        },
        {
                item_avinia_leaf,
                sk_collect_foliage,
                0,
                20,
                "collected",
                "All of the avinia leaves here have been collected.",
                "Avinia leaves are found in mallorn groves.",
                "collect avinia leaves",
                TRUE,
                0
        },
        {
                item_spiny_root,
                sk_collect_foliage,
                0,
                25,
                "collected",
                "All of the spiny roots here have been collected.",
                "Spiny roots are found in swamps, pits and bogs.",
                "collect spiny roots",
                TRUE,
                0
        },
        {
                item_lana_bark,
                sk_collect_foliage,
                0,
                50,
                "collected",
                "All of the lana bark here has been collected.",
                "Lana bark is found in circles of trees.",
                "collect lana bark",
                TRUE,
                0
        },
        {
                item_farrenstone,
                sk_collect_elem,
                0,
                25,
                "collected",
                "This cave's supply of farrenstone for this month has been exhausted.",
                "Farrenstone is found in caves.",
                "collect farrenstone",
                TRUE,
                0
        },
        {
                item_pretus_bones,
                sk_collect_elem,
                0,
                25,
                "collected",
                "No pretus bones can be found.",
                "Pretus bones are found in sand pits.",
                "collect pretus bones",
                TRUE,
                0
        },
        {
                item_ratspider,
                sk_collect_elem,
                0,
                1,
                "collected",
                "No ratspiders can be found.",
                "Ratspiders are found in ruins.",
                "collect ratspiders",
                TRUE,
                0
        },
        {
                0,
                0,
                0,
                0,
                NULL,
                NULL,
                NULL,
                NULL,
                0,
                0
        }
};


static struct harvest *
find_harv(int k, int who) {
    int i;

    for (i = 0; harv_tbl[i].item; i++) {
        if (harv_tbl[i].item == k &&
            (!harv_tbl[i].skill || has_skill(who, harv_tbl[i].skill))) {
            return &harv_tbl[i];
        }
    }

    return NULL;
}


static ilist collectors = NULL;


void
init_collect_list() {
    int i;
    struct command *c;
    int cmd_collect;

    cmd_collect = find_command("collect");
    assert(cmd_collect > 0);

    loop_char(i)
            {
                c = rp_command(i);

                if (c && c->state == RUN && c->cmd == cmd_collect) {
                    ilist_append(&collectors, i);
                }
            }
    next_char;
}


static void
bump_other_collectors(int where, struct harvest *t) {
    int i;
    struct command *c;
    int wh2;
    ilist l;

    l = ilist_copy(collectors);

    for (i = 0; i < ilist_len(l); i++) {
        c = rp_command(l[i]);
        assert(c);

        if (c->a != t->item) {
            continue;
        }

        wh2 = subloc(c->who);

        if (t->item == item_fish && is_ship(wh2)) {
            wh2 = loc(wh2);
        }

        if (where != wh2) {
            continue;
        }

        interrupt_order(c->who);
    }

    ilist_reclaim(&l);
}

/*
 *  Sun Dec  1 10:28:53 1996 -- Scott Turner
 *
 *  Special case for peasants...
 *
 */
static int
item_avail(int where, int item) {
    int num = has_item(where, item);
    if (item == item_peasant) {
        if (subkind(where) == sub_city) {
            where = province(where);
            num = has_item(where, item);
        };
        if (num < (has_city(where) ? 1000 : 100)) { return 0; }
        return (has_item(where, item) * 0.05) - rp_loc(where)->recruited;
    } else {
        return has_item(where, item);
    };
};

int
v_generic_harvest(struct command *c, int number, int days, struct harvest *t) {
    int where = subloc(c->who);
    int workers;
    int avail;

    if (t->item == item_fish && is_ship(where)) {
        where = loc(where);
    }

#if 1
    if (where == 13240 || where == 13241) {
        fprintf(stderr, "\n%d: begin fish for %s\n",
                sysclock.day,
                box_code_less(c->who));
    }
#endif

    if (t->skill && has_skill(c->who, t->skill) < 1) {
        wout(c->who, "Requires %s.", box_name(t->skill));
        return FALSE;
    }

    if (t->piety && !use_piety(c->who, t->piety)) {
        wout(c->who, "You do not have the piety for that prayer.");
        return FALSE;
    }

    if (days < 1) {
        days = -1;
    }    /* as long as it takes to get number */

    c->c = number;        /* number desired; 0 means all possible */
    c->d = 0;        /* number we have obtained so far */

    /*
     *  Special case for peasants...
     *
     */
    avail = item_avail(where, t->item);

    if (avail <= 0) {
        return i_generic_harvest(c, t);
    }

    if (t->worker) {
        workers = has_item(c->who, t->worker);

        if (workers < 1) {
            wout(c->who, "Need at least one %s to %s.",
                 box_name(t->worker),
                 t->task_desc);
            return FALSE;
        }
    }

    ilist_append(&collectors, c->who);

    c->wait = days;
    return TRUE;
}


/*
 *  c->d holds how much we've collected "so far"
 *
 */

/*
 *  Tue Oct 15 16:16:35 1996 -- Scott Turner
 *
 *  If this location is blessed for improved collection, then we want
 *  to reward the collection unit with an extra 50% of the item.  Note
 *  that this extra stuff does not come out of the location's stockpile.
 *
 *  If an item gets a % chance, then blessing should work by increasing
 *  that chance.
 *
 */

int
d_generic_harvest(struct command *c, struct harvest *t) {
    int where = subloc(c->who);
    int number = c->c;
    int qty;
    int workers;

    if (t->item == item_fish && is_ship(where)) {
        where = loc(where);
    }
    /*
     *  Also special case for recruiting peasants in the city.
     *
     */
    if (t->item == item_peasant && subkind(where) == sub_city) {
        where = province(where);
    }

    /*
     *  Fri Jul  2 09:58:03 1999 -- Scott Turner
     *
     *  If gathering is probablistic, then let's fail out
     *  if we don't succeed rolling the dice.
     *
     */
    if (t->chance && rnd(1, 100) > t->chance) {
        if (c->wait == 0) { return i_generic_harvest(c, t); }
        return TRUE;
    };

    qty = item_avail(where, t->item);

    if (t->worker) {
        workers = has_item(c->who, t->worker);
        qty = min(qty, workers);

        if (number > 0 && (c->d + qty > number)) {
            qty = number - c->d;
        }

        assert(qty >= 0);
    } else {
        qty = min(qty, 1);
    }

    if (qty > 0) {
        move_item(where, c->who, t->item, qty);
        c->d += qty;
        /*
         *  Fri Jul  2 10:03:09 1999 -- Scott Turner
         *
         *  With improved production, we add in a bonus of material.
         *  We need to do something reasonable if qty == 1.
         *
         */
        if (get_effect(where, ef_improve_production, 0, t->item)) {
            if (qty > 1) {
                gen_item(c->who, t->item, ((float) qty * 0.50 + 0.50));
                c->d += ((float) qty * 0.50 + 0.50);
                wout(c->who, "%s is unusually productive.", box_name(where));
            } else if (rnd(1, 100) > 50) {
                gen_item(c->who, t->item, 1);
                c->d += 1;
                wout(c->who, "%s is unusually productive.", box_name(where));
            };
        };

        /*
         *  There's no point spending an extra day to find out that the
         *  resource is depleted.  If there are none left, terminate the
         *  command now, rather than next evening.
         *
         *  We also want to bump any other units collecting out, so they
         *  won't waste an extra evening just finding out that there's no
         *  more to collect.
         */
        if (item_avail(where, t->item) == 0) {
            int ret = i_generic_harvest(c, t);
            bump_other_collectors(where, t);
            return ret;
        }

        if (c->wait != 0 && !(number > 0 && c->d >= number)) {
            return TRUE;
        }        /* not done yet */
    }
    return i_generic_harvest(c, t);
}


int i_generic_harvest(struct command *c, struct harvest *t) {
    int where = subloc(c->who);

    if (t->item == item_fish && is_ship(where)) {
        where = loc(where);
    }

    if (c->d == 0 && item_avail(where, t->item) == 0) {
        if (item_gen_here(where, t->item)) {
            out(c->who, t->none_now);
        } else {
            out(c->who, t->none_ever);
        }
    } else {
        out(c->who, "%s %s.", cap(t->got_em),
            just_name_qty(t->item, c->d));

        if (c->d > 0) {

            if (t->public) {
                show_to_garrison = TRUE;
                out(where, "%s %s %s.",
                    box_name(c->who),
                    t->got_em,
                    just_name_qty(t->item, c->d));
                show_to_garrison = FALSE;
            };

            if (t->skill) {
                add_skill_experience(c->who, t->skill);
            }
        };
    }

    ilist_rem_value(&collectors, c->who);

    c->wait = 0;
    if (c->d > 0 && c->d >= c->c) {
        return TRUE;
    }
    return FALSE;
}

/*
 *  Priests can only collect peasants (recruit) if they're in
 *  a province with a temple of their religion.
 *
 */
int
v_collect(struct command *c) {
    int item = c->a;
    int number = c->b;
    int days = c->c;
    struct harvest *t;

    if (is_priest(c->who) && (item == item_peasant || item == item_postulant)) {
        int temple_found = 0, i;
        loop_all_here(province(subloc(c->who)), i)
                {
                    if (is_temple(i) == is_priest(c->who)) {
                        temple_found = 1;
                        break;
                    };
                }next_all_here;
        if (!temple_found) {
            wout(c->who, "Priest may only recruit in a province with a temple of the priest's religion.");
            return FALSE;
        };
    };

    t = find_harv(item, c->who);

    if (t == NULL) {
        wout(c->who, "Don't know how to collect %s.",
             box_code(item));
        return FALSE;
    };

    return v_generic_harvest(c, number, days, t);
}


int
d_collect(struct command *c) {
    int item = c->a;
    struct harvest *t;

    t = find_harv(item, c->who);

    if (t == NULL) {
        out(c->who, "Internal error.");
        log_output(LOG_CODE, "d_collect: t is NULL, who=%d", c->who);
        return FALSE;
    }

    return d_generic_harvest(c, t);
}


int
i_collect(struct command *c) {
    int item = c->a;
    struct harvest *t;

    t = find_harv(item, c->who);

    if (t == NULL) {
        out(c->who, "Internal error.");
        log_output(LOG_CODE, "i_collect: t is NULL, who=%d", c->who);
        return FALSE;
    }

    return i_generic_harvest(c, t);
}


int
v_quarry(struct command *c) {
    int ret;

    ret = oly_parse(c, sout("collect %d %d %d", item_stone, c->a, c->b));
    assert(ret);

    return v_collect(c);
}

int
v_recruit(struct command *c) {
    int ret;

    ret = oly_parse(c, sout("collect %d %d %d", item_peasant, c->a, c->b));
    assert(ret);

    return v_collect(c);
}


int
v_raise_corpses(struct command *c) {
    int ret;

    ret = oly_parse(c, sout("collect %d %d %d", item_corpse, c->a, c->b));
    assert(ret);

    return v_collect(c);
}


int
v_fish(struct command *c) {
    int ret;

    ret = oly_parse(c, sout("collect %d %d %d", item_fish, c->a, c->b));
    assert(ret);

    return v_collect(c);
}


int
v_wood(struct command *c) {
    int ret;

    ret = oly_parse(c, sout("collect %d %d %d", item_lumber, c->a, c->b));
    assert(ret);

    return v_collect(c);
}


int
v_opium(struct command *c) {
    int ret;

    ret = oly_parse(c, sout("collect %d %d %d", item_opium, c->a, c->b));
    assert(ret);

    return v_collect(c);
}


int
v_mallorn(struct command *c) {
    int ret;

    ret = oly_parse(c, sout("collect %d %d %d",
                            item_mallorn_wood, c->a, c->b));
    assert(ret);

    return v_collect(c);
}


int
v_yew(struct command *c) {
    int ret;

    ret = oly_parse(c, sout("collect %d %d %d", item_yew, c->a, c->b));
    assert(ret);

    return v_collect(c);
}


int
v_catch(struct command *c) {
    int ret;

    ret = oly_parse(c,
                    sout("collect %d %d %d", item_wild_horse, c->a, c->b));
    assert(ret);

    return v_collect(c);
}

/*
 *  Mon Dec  2 13:17:10 1996 -- Scott Turner
 *
 *  Persuade more growth from the peasants.
 *
 */
int
v_grow_pop(struct command *c) {
    int where = province(c->who);

    /*
     *  You should actually be in the province.
     *
     */
    if (where != subloc(c->who)) {
        wout(c->who, "You must be in the province to encourage the population to grow.");
        return FALSE;
    };

    /*
     *  Must be some peasants here.
     *
     */
    if (has_item(where, item_peasant) < 100) {
        wout(c->who, "There are not enough peasants here to encourage.");
        return FALSE;
    };

    return TRUE;
};

int
d_grow_pop(struct command *c) {
    int where = province(c->who);

    /*
     *  You should actually be in the province.
     *
     */
    if (where != subloc(c->who)) {
        wout(c->who, "You must be in the province to encourage the population to grow.");
        return FALSE;
    };

    /*
     *  Must be some peasants here.
     *
     */
    if (has_item(where, item_peasant) < 100) {
        wout(c->who, "There are not enough peasants here to encourage.");
        return FALSE;
    };

    /*
     *  Add an effect encouraging growth.
     *
     */
    if (!add_effect(where, ef_grow, 0, 30, 1)) {
        wout(c->who, "For some reason, the peasants ignore you.");
        return FALSE;
    };

    wout(c->who, "You encourage the peasants to procreate more.");
    return TRUE;
};
