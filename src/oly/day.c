
// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    <string.h>
#include <stdlib.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


extern double pow(double, double);

static void reseed_monster_provinces(void);

/*
 *  0	not on the ocean
 *  1	near a coast
 *  2	near a dangerous coast
 *  3	not near a coast
 */

int
near_rocky_coast(int where) {
    struct exit_view **l;
    int i;
    int ret = 3;

    if (subkind(where) != sub_ocean) {
        return 0;
    }

    l = exits_from_loc_nsew(0, where);

    for (i = 0; i < ilist_len(l); i++) {
        if (subkind(l[i]->destination) != sub_ocean) {
            if (subkind(l[i]->destination == sub_mountain)) {
                return 2;
            }
            ret = 1;
        }
    }

    return ret;
}


static void
ship_coastal_damage() {
    int ship;
    int n;
    int damage, reduction;
    struct entity_ship *s;

    loop_ship(ship)
            {
                if (!is_ship(ship)) {        /* not completed */
                    continue;
                }

                s = rp_ship(ship);
                n = near_rocky_coast(subloc(ship));

                switch (n) {
                    case 0:
                        break;

                    case 1:
                        if (rnd(1, 75) == 1) {
                            wout(ship, "%s struck a coastal reef.  ", box_name(ship));
                            damage = rnd(3, 5);
                            if (s && s->keels) {
                                reduction = (int)
                                        (damage * (s->keels / (float) s->hulls));
                                if (reduction == damage) {
                                    wout(ship, "The ship's keel easily absorbs the impact.");
                                } else if (reduction) {
                                    wout(ship, "The ship's keel absorbs some of the impact.");
                                };
                                damage -= reduction;
                            };
                            if (damage) {
                                wout(ship, "There is minor damage to the ship.");
                                add_structure_damage(ship, damage);
                            };
                        }
                        break;

                    case 2:
                        if (rnd(1, 50) == 1) {
                            wout(ship, "%s struck some submerged rocks.  ", box_name(ship));
                            damage = rnd(6, 10);
                            if (s && s->keels) {
                                reduction = (int)
                                        (damage * (s->keels / (float) s->hulls));
                                if (reduction == damage) {
                                    wout(ship, "The ship's keel easily absorbs the impact.");
                                } else if (reduction) {
                                    wout(ship, "The ship's keel absorbs some of the impact.");
                                };
                                damage -= reduction;
                            };
                            if (damage) {
                                wout(ship, "There is minor damage to the ship.");
                                add_structure_damage(ship, damage);
                            };
                        }
                        break;

                    case 3:
                        switch (rnd(1, 200)) {
                            case 1:
                            case 2:
                                wout(ship, "Hungry looking birds circle overhead.");
                                break;

                            case 3:
                                wout(ship, "Sharks circle in the water a short "
                                           "distance from the ship.");
                                break;
                        }
                        break;

                    default:
                        assert(FALSE);
                }
            }
    next_ship;
}

static void
give_stack_losses(int who, int evac_levels) {
    struct item_ent *e;
    int i, j;
    int loss_pct = 0, losses;

    vector_stack(who, TRUE);
    wout(VECT, "You must evacuate through %s level%s of collapsing mine shaft!",
         nice_num(evac_levels),
         evac_levels == 1 ? "" : "s");
    loop_stack(who, i)
            {
                loop_inv(i, e)
                            {
                                /*
                                 *  1-20 % per level evacuated.
                                 *
                                 */
                                loss_pct = 0;
                                for (j = 0; j < evac_levels; j++) { loss_pct += rnd(1, 20); }
                                losses = (e->qty * loss_pct) / 100;
                                /*
                                 *  No losses?
                                 *
                                 */
                                if (!losses) { continue; }
                                /*
                                 *  Can't lose more than you have.
                                 *
                                 */
                                if (losses > e->qty) { losses = e->qty; }
                                /*
                                 *  Tell him.
                                 *
                                 */
                                if (rp_item(e->item)->is_man_item || rp_item(e->item)->animal) {
                                    wout(i, "%s %s %s killed in the evacuation.",
                                         cap(nice_num(losses)),
                                         plural_item_name(e->item, losses),
                                         losses == 1 ? "was" : "were");
                                } else {
                                    wout(i, "%s %s %s lost in the evacuation.",
                                         cap(nice_num(losses)),
                                         plural_item_name(e->item, losses),
                                         losses == 1 ? "was" : "were");
                                };
                                /*
                                 *  Delete the item.
                                 *
                                 */
                                consume_item(i, e->item, losses);
                            }next_inv;
                /*
                 *  An evacuating noble takes 1-20 points damage as well.
                 *
                 */
                losses = 0;
                for (j = 0; j < evac_levels; j++) { losses += rnd(1, 20); }
                wout(i, "You take %d points damage evacuating.", losses);
                add_char_damage(i, losses, MATES);
            }next_stack;
};

static void
evacuate(int where, int exit, int evac_levels) {
    int i;
    struct item_ent *e;

    /*
     *  Evacuate all nobles at this level.
     *
     */
    vector_clear();
    vector_char_here(where);
    wout(VECT, "%s collapses and you must evacuate to %s!",
         box_name(where), box_name(exit));

    loop_here(where, i)
            {
                /*
                 *  Get each stack...
                 *
                 */
                if (kind(i) == T_char) {
                    give_stack_losses(i, evac_levels);
                    if (kind(i) == T_char) {
                        move_stack(i, exit);
                    } else {
                        /* He's turned into a dead body */
                        move_item(where, exit, i, 1);
                    };
                    continue;
                }
                /*
                 *  An unfinished mine shaft needs to be evacuated and removed.
                 *
                 */
                if (kind(i) == T_loc) {
                    evacuate(i, exit, evac_levels);
                    remove_from_here_list(where, i);
                    delete_box(i);
                    continue;
                };
                /*
                 *  What's left here?  We need to get rid of it.
                 *
                 */
                wout(gm_player, "Removing %s from collapsing mine?", i);
                delete_box(i);
            }next_here;

    /*
     *  Wed Jul 12 20:27:12 2000 -- Scott Turner
     *
     *  In the course of handing out damage, we may have created
     *  some dead bodies here.
     *
     */
    loop_here(where, i)
            {
                wout(gm_player, "Rescuing %s from %s.", box_name(i), box_name(where));
                if (kind(i) == T_char) {
                    move_stack(i, exit);
                } else {
                    move_item(where, exit, i, 1);
                };
            }next_here;

    /*
     *  Wed Jul 12 21:23:51 2000 -- Scott Turner
     *
     *  Jeez, there can be stuff in the inventory!
     *
     */
    loop_inv(where, e)
                {
                    move_item(where, exit, e->item, e->qty);
                }next_inv;

};

extern void unconnect_location(int loc1);

static void
collapse_mine(int where) {
    int evac_levels = 1;
    int exit = location_direction(where, DIR_UP);
    int i, next, prev;
    int depth = mine_depth(where);
    struct entity_mine *mi = get_mine_info(where);

    prev = exit;
    assert(subkind(where) == sub_mine_shaft);
    assert(prev && valid_box(prev));

    do {
        next = 0;
        /*
         *  Evacuate this level.
         *
         */
        evacuate(where, exit, evac_levels);
        /*
         *  Wipe out shoring on this level!
         *
         */
        mi->shoring[depth] = 0;
        /*
         *  Now figure out where, if anywhere, we collapse next.
         *
         */
        next = location_direction(where, DIR_DOWN);
        /*
         *  Disconnect this location.
         *
         */
        unconnect_location(where);
        /*
         *  Now delete this location?
         *
         */
        set_where(where, 0);
        delete_box(where);
        /*
         *  If there's a next, use it for the next iteration.
         *
         */
        where = next;
        evac_levels++;
        depth++;
    } while (where);
};

static void
check_mine_calamity(int mine) {
    int where = mine, i, depth = 0, prev = 0;
    int found, collapsing = 0, collapse;

    /*
     * Thu Feb 13 15:49:59 1997 -- Scott Turner
     *
     *  Creates it if necessary.
     */
    struct entity_mine *mi = get_mine_info(where);

    /*
     *  Check this level for a collapse.
     *
     */
    depth = mine_depth(where);
    collapse = 10 * (depth + 1) - 70 * mi->shoring[depth];
    if (collapse < 6) { collapse = 6; }
    if (collapse > 0 && rnd(1, 100) < collapse) {
        collapse_mine(where);
        return;
    };

    /*
     *  At deep levels, we start to uncover balrogs...
     *
     */
    if (depth > 15 && rnd(1, 100) < 25) {
        int new;
        int item;

        new = create_monster_stack(item_balrog, rnd(3, 8), where);
        if (new < 0) { return; }
        wout(where, "%s come out of hiding.", box_name(new));
    };
}

static void
inn_calamity(int where) {
    int own;
    int dam;
    char buf[LEN];

    own = building_owner(where);
    dam = rnd(5, 15);

    switch (rnd(1, 6)) {
        case 1:
            strcpy(buf, "Some customers ");
            break;
        case 2:
            strcpy(buf, "Some patrons ");
            break;
        case 3:
            strcpy(buf, "An irate customer ");
            break;
        case 4:
            strcpy(buf, "Two large, angry men ");
            break;
        case 5:
            strcpy(buf, "A surly local ");
            break;
        case 6:
            strcpy(buf, "A party of traveling entertainers ");
            break;
    }

    switch (rnd(1, 5)) {
        case 1:
            strcat(buf, "got drunk, ");
            break;
        case 2:
            strcat(buf, "started a fight, ");
            break;
        case 3:
            strcat(buf, "got drunk and started a fight, ");
            break;
        case 4:
            strcat(buf, "insulted the chef, ");
            break;
        case 5:
            strcat(buf, "refused to pay, ");
            break;
    }

    switch (rnd(1, 7)) {
        case 1:
            strcat(buf, "and broke some furniture");
            break;
        case 2:
            strcat(buf, "and damaged a wall");
            break;
        case 3:
            strcat(buf, "and kicked in the door");
            break;
        case 4:
            strcat(buf, "and knocked over a keg of beer");
            break;
        case 5:
            strcat(buf, "and set a fire in the closet");
            break;
        case 6:
            strcat(buf, "and knocked over the smokehouse");
            break;
        case 7:
            strcat(buf, "and broke some chairs");
            break;
    }

    if (own) {
        wout(own, "%s:  %s, causing %d points of damage.",
             box_name(where), buf, dam);
    }

    add_structure_damage(where, dam);
}

static int mine_collapse_day;

static void
random_loc_damage() {
    int where;
    int depth;

    loop_loc(where)
            {
                switch (subkind(where)) {
                    case sub_mine_shaft:
                        /*
                         *  Only check for collapse 1 day in 30;
                         *  Mines don't collapse if protected.
                         *
                         *  Mon Nov 23 07:55:56 1998 -- Scott Turner
                         *
                         *  Make sure we only check at most 1 day per month.
                         *  We do this by randomly picking a day at the
                         *  beginning of the month to check for a collapse.
                         *
                         */
                        if (sysclock.day == mine_collapse_day &&
                            valid_box(where) &&
                            rnd(1, 30) == 1 &&
                            !get_effect(where, ef_protect_mine, 0, 0)) {
                            check_mine_calamity(where);
                        }
                        break;

                    case sub_inn:
                        if (rnd(1, 100) == 1) {
                            inn_calamity(where);
                        }
                        break;
                }
            }
    next_loc;
}


static void
heal_char_sup(int who) {
    int h;
    int inn = FALSE;
    int chance;
    int amount;
    int a;

    h = char_health(who);

    if (h >= 100) {
        return;
    }

    if (subkind(subloc(who)) == sub_inn) {
        inn = TRUE;
    }

    chance = 5;
    if (inn) {
        chance = 10;
    }

#ifdef HERO
    /*
     *  Wed Nov 25 13:01:02 1998 -- Scott Turner
     *
     *  Improved Recovery can help!
     *
     */
    chance += skill_exp(who, sk_improved_recovery);
#endif // HERO	

    if (char_sick(who) && rnd(1, 100) <= chance) {
        wout(who, "%s defeated illness and is now recovering.",
             box_name(who));

        p_char(who)->sick = FALSE;
    }

    amount = rnd(3, 15);

    if (char_sick(who)) {
        if (h - amount < 0) {
            amount = h;
        }

        wout(who, "%s lost %s health.",
             box_name(who), nice_num(amount));

        p_char(who)->health -= amount;

        if (char_health(who) <= 0) {
            kill_char(who, MATES, S_body);
        }
    } else {
        if (h + amount > 100) {
            amount = 100 - h;
        }

        wout(who, "%s gained %s health.",
             box_name(who), nice_num(amount));

        p_char(who)->health += amount;

        if (h + amount >= 100) {
            wout(who, "%s is fully healed.", box_name(who));
        }
    }

    if (kind(who) && char_health(who) < 100 &&
        (a = best_artifact(who, ART_HEALING, 0, 0))) {
        p_char(who)->health += rp_item_artifact(a)->param1;
        if (p_char(who)->health > 100) p_char(who)->health = 100;
        wout(who, "Your artifact provides %s points of healing power.",
             nice_num(rp_item_artifact(a)->param1));
    };
}


static void
heal_characters() {
    int who;
    int n;

    loop_char(who)
            {
                n = char_health(who);

                if (n >= 0 && n < 100) {
                    heal_char_sup(who);
                }
            }
    next_char;
}


static void
add_noble_points() {
    int pl;

    loop_player(pl)
            {
                switch (subkind(pl)) {
                    case sub_pl_regular:
                        if (next_np_turn(pl) == 0) {
                            add_np(pl, 1);
                        }
                        break;
                }
            }
    next_player;
}

static void
add_claim_gold() {
    int pl;

    loop_player(pl)
            {
                switch (subkind(pl)) {
                    case sub_pl_regular:
                        wout(pl, "Added 25 gold to CLAIM.");
                        gen_item(pl, item_gold, 25);
                        break;
                }
            }
    next_player;
}


void
add_unformed_sup(int pl) {
    struct entity_player *p;
    int new;
    extern int new_ent_prime;        /* allocate short numbers */

    p = rp_player(pl);
    if (p == NULL) {
        return;
    }

    new_ent_prime = TRUE;

    while (ilist_len(p->unformed) < 5) {
        new = new_ent(T_unform, 0);
        if (new <= 0) {
            break;
        }

        ilist_append(&p->unformed, new);
    }

    new_ent_prime = FALSE;
}


static void
add_unformed() {
    int pl;

    loop_player(pl)
            {
                add_unformed_sup(pl);

            }
    next_player;
}


static void
increment_current_aura() {
    int who;
    struct char_magic *p;
    int ac;                    /* auraculum */
    int ma;                    /* max aura */
    int total = 0;

    loop_char(who)
            {
                total = 0;

                if (!is_magician(who)) {
                    continue;
                }

                ac = has_auraculum(who);
                ma = max_eff_aura(who);

                p = p_magic(who);

                if (p->cur_aura < ma) {
                    add_aura(who, 1);
                    total++;
                };
                if (p->cur_aura < ma) {
                    add_aura(who, 1);
                    total++;
                };

                if (ac)            /* auraculum grants two more points */
                {
                    if (p->cur_aura < ma) {
                        add_aura(who, 1);
                        total++;
                    };
                    if (p->cur_aura < ma) {
                        add_aura(who, 1);
                        total++;
                    };
                }

                {
                    struct item_ent *e;
                    int n, staff_bonus;

                    loop_inv(who, e)
                                {
                                    if (n = item_aura_bonus(e->item)) {
                                        if (p->cur_aura < ma) {
                                            add_aura(who, 1);
                                            total++;
                                        }
                                    };
                                }
                    next_inv;

                    /*
                     *  If he's holding some pieces of the Sun Staff, he
                     *  may get some piety bonus.
                     *
                     */
                    staff_bonus = 0;
                    loop_inv(who, e)
                                {
                                    if (subkind(e->item) == sub_special_staff) {
                                        ++staff_bonus;
                                    }
                                }next_inv;
                    if (staff_bonus) {
                        for (n = 0; n < pow(2, staff_bonus); n++) {
                            if (p->cur_aura < ma) {
                                add_aura(who, 1);
                                total++;
                            } else {
                                break;
                            }
                        }
                    };
                }
                if (total) {
                    wout(who, "You regenerate %s point%s of mana.",
                         nice_num(total), total == 1 ? "" : "s");
                }
            }next_char;
}

/*
 *  Lose_Elves
 *  Fri Oct 18 15:41:34 1996 -- Scott Turner
 *  
 *  Elves in stacks wander off...
 *
 *  Fri Nov 24 12:11:10 2000 -- Scott Turner
 *
 *  Updated to handle any monster types...
 *
 */
static void
lose_monsters(int mtype, char *msg, int max_loss, int chance) {
    int who, num, i, loss;

    loop_char(who)
            {
                /*
                 *  Don't leave a monster stack of all your kind
                 *
                 */
                if (is_npc(who) && noble_item(who) == mtype) { continue; }

                /*
                 *  And if you don't have any mtype, it doesn't matter.
                 *
                 */
                num = has_item(who, mtype);
                if (!num) { continue; }

                /*
                 *  Otherwise, up to "max" can wander off.
                 *
                 */
                loss = 0;
                for (i = 0; i < num && loss < max_loss; i++) {
                    if (rnd(1, 100) < chance) { loss++; }
                }

                /*
                 *  Maybe no one left?
                 *
                 */
                if (!loss) { continue; }

                /*
                 *  Otherwise inform him.
                 *
                 */
                consume_item(who, mtype, loss);
                wout(who, msg, cap(nice_num(loss)), plural_item_name(mtype, loss), (loss > 1) ? "" : "s");

            }next_char;
}

static void reseed_monster_provinces(void) {
    int where, who, found, item;
    int provinces = 0, monsters = 0;
    int create_new_beasts(int where, int sk);

    loop_province(where)
                {
                    /*
                     *  Reasonable place to create a monster?
                     *
                     */
                    if (subkind(where) == sub_ocean ||
                        subkind(where) == sub_mine_shaft ||
                        subkind(where) == sub_mine_shaft_notdone ||
                        has_item(where, item_peasant) > 100 ||
                        in_hades(where) ||
                        in_faery(where) ||
                        in_clouds(where)) {
                        continue;
                    }
                    provinces++;
                }next_province;

    monsters = ilist_len(rp_player(indep_player)->units);

    if (monsters < provinces / 2) {
        loop_province(where)
                    {
                        /*
                         *  Reasonable place to create a monster?
                         *
                         */
                        if (subkind(where) == sub_ocean ||
                            subkind(where) == sub_mine_shaft ||
                            subkind(where) == sub_mine_shaft_notdone ||
                            has_item(where, item_peasant) > 100 ||
                            in_hades(where) ||
                            in_faery(where) ||
                            in_clouds(where)) {
                            continue;
                        }
                        /*
                         *  Already a beast stack here?
                         *
                         */
                        found = 0;
                        loop_here(where, who)
                                {
                                    item = noble_item(who);
                                    if (is_npc(who) && beast_wild(who) && item &&
                                        item_attack(item) + item_defense(item)) {
                                        found = 1;
                                        break;
                                    };
                                }next_here;
                        /*
                         *  Shall we create something?
                         *
                         */
                        if (!found && rnd(1, 100) == 1) {
                            wout(gm_player, "Creating new random beast stack in %s.",
                                 box_name(where));
                            (void) create_new_beasts(where, 0);
                        };
                    }next_province;
    } else {
        log_output(LOG_MISC, "No need to reseed monsters in provinces.");
    };
};

/*
 *  Fri Feb 28 11:21:30 1997 -- Scott Turner
 *  
 *  New beasts appear on the map -- only at the province level.
 *
 *  Thu Sep 24 13:41:49 1998 -- Scott Turner
 *
 *  Added reseeding of sublocs.
 *
 *  Thu Sep 16 07:12:43 1999 -- Scott Turner
 *
 *  There should be no more than 1 beast stack for every 2 wilderness
 *  provinces.
 *
 */
static void
reseed_monsters() {
    reseed_monster_provinces();
    reseed_monster_sublocs();
};

int
place_here(int where, int who) {
    int i;
    int first = 0;

    loop_char_here(where, i)
            {
                if (i == stack_leader(who)) {
                    exit_char_here;
                    return first;
                };
                if (player(i) == player(who)) { first++; }
            }next_char_here;

    assert(FALSE);
    return -1;

}

/*
 *  Fri Nov 26 07:21:29 1999 -- Scott Turner
 *
 *  Go through the world and weaken any beasts who are not
 *  top dog in their location.
 *
 */
static void
weaken_monsters() {
    int i, place;
    loop_units(indep_player, i)
            {
                /*
                 *  Is it a monster stack?
                 *
                 */
                if (noble_item(i)) {
                    place = place_here(subloc(i), i);
                    /*
                     *  If you're more than 4 down in the province, die.
                     *
                     */
                    if (place > 4) {
                        wout(i, "Killed off for excess.");
                        kill_char(i, 0, S_nothing);
                    } else if (place > 1) {
                        /*
                         *  If you're not first or second, weaken yourself.  Die if
                         *  you're just a noble item.
                         *
                         */
                        wout(i, "Weakening.");
                        if (has_item(i, noble_item(i))) {
                            sub_item(i, noble_item(i), 1);
                        } else {
                            wout(i, "Overcrowding suicide.");
                            kill_char(i, 0, S_nothing);
                        };
                    };
                };
            }next_unit;
};

/*
 *  Thu Dec  2 18:25:49 1999 -- Scott Turner
 *
 *  Add new monsters, weaken existing monsters, etc.
 *
 *  Fri Nov 24 12:09:57 2000 -- Scott Turner
 *
 *  Doesn't appear to be any decaying of undead.
 *
 */
static void
adjust_monsters() {
    reseed_monsters();
    weaken_monsters();
    lose_monsters(item_elf, "%s %s wander%s off into the wilderness.", 100, 25);
    lose_monsters(item_corpse, "%s %s crumble%s into dust!", 2, 25);
};

/*
 *  Tue Oct 29 11:47:39 1996 -- Scott Turner
 *
 *  Comparison function for sort in increment_current_piety.
 *
 */
static int
followers_comp(a, b)
        int *a;
        int *b;
{
    int fa = ilist_len(rp_char(*b)->religion.followers);
    int fb = ilist_len(rp_char(*a)->religion.followers);

    if (fa == fb) {
        return (char_piety(*a) - char_piety(*b));
    }
    return (fa - fb);

}

/*
 *  Mon Aug 19 11:59:49 1996 -- Scott Turner
 *  
 *  Raise piety for priests, analogous to aura for magicians.
 *
 *  * based on # of followers
 *    -> top priest, 12 piety per month
 *	+ 10 angels
 *    -> next two, 6 piety per month
 *	+ 5 angels
 *    -> top half of rest, 3 piety per month
 *    -> the rest, none.
 *
 *  Thu Mar 29 12:48:12 2001 -- Scott Turner
 *
 *  Include piety limits.
 *
 */
static void
increment_current_piety() {
    int who, religion, i, staff_bonus;
    ilist priests = NULL;
    struct item_ent *e;
    int max_piety;

    /*
     *  Remove all angels from inventories.
     *
     */
    loop_char(who)
            {
                loop_inv(who, e)
                            {
                                if (e->item == item_angel) {
                                    consume_item(who, e->item, e->qty);
                                    wout(who, "Your angels return to the spiritual realm.");
                                };
                            }next_inv;
            }next_char;

    /*
     *  Go through each skill
     *
     */
    loop_skill(religion)
            {
                /*
                 *  Skip non-religion skills
                 *
                 */
                if (skill_school(religion) != religion) { continue; }
                if (!religion_skill(religion)) { continue; }
                if (religion == sk_basic_religion) { continue; }
                /*
                 *  Go through all the nobles and collect those that are priests
                 *  of this religion.
                 *
                 */
                ilist_clear(&priests);
                loop_char(who)
                        {
                            /*
                             *  Skip non-priests
                             *
                             */
                            if (is_priest(who) != religion) { continue; }
                            /*
                             *  Wed Feb  3 13:25:20 1999 -- Scott Turner
                             *
                             *  Only add him to the list if he's alive and not a
                             *  prisoner.
                             *
                             */
                            if (alive(who) && !is_prisoner(who)) {
                                ilist_append(&priests, who);
                            }
                            /*
                             *  If he's holding some pieces of the Sun Staff, he
                             *  may get some piety bonus.
                             *
                             */
                            staff_bonus = 0;
                            loop_inv(who, e)
                                        {
                                            if (subkind(e->item) == sub_special_staff) { ++staff_bonus; }
                                        }next_inv;
                            if (staff_bonus) {
                                wout(who, "Your god rewards you with %s piety for the parts "
                                          "of the Staff of the Sun you hold.", nice_num(pow(2, staff_bonus)));
                                add_piety(who, pow(2, staff_bonus), 0); // mdhender: added 0 to compile
                            };
                        }next_char;
                /*
                 *  If we have any, sort them by how many followers they have.
                 *
                 */
                if (!ilist_len(priests)) { continue; }

                qsort(priests, ilist_len(priests), sizeof(int), followers_comp);

                /*
                 *  Top guy gets 12 piety plus 10 angels.
                 *
                 */
                wout(priests[0],
                     "%s honors you as High Priest of his worshippers!",
                     god_name(religion));
                wout(priests[0],
                     "%s blesses you with great piety and the protection of angels.",
                     god_name(religion));
                add_piety(priests[0], options.top_piety, 1);
                wout(priests[0], "You gain %s piety.", nice_num(options.top_piety));
                gen_item(priests[0], item_angel, 10);
                wout(priests[0], "%s sends you 10 angels.",
                     god_name(religion));
                rp_relig_skill(religion)->high_priest = priests[0];

                /*
                 *  Next two get 6 piety plus 5 angels.
                 *
                 */
                for (i = 1; i < ilist_len(priests) && i < 3; i++) {
                    wout(priests[i],
                         "%s honors you as a great Bishop of the faithful!",
                         god_name(religion));
                    wout(priests[i],
                         "%s blesses you with great piety and the protection of angels.",
                         god_name(religion));
                    add_piety(priests[i], options.middle_piety, 1);
                    wout(priests[i], "You gain %s piety.", nice_num(options.middle_piety));
                    gen_item(priests[i], item_angel, 5);
                    wout(priests[i], "%s sends you 5 angels.",
                         god_name(religion));
                    rp_relig_skill(religion)->bishops[i - 1] = priests[i];
                };

                /*
                 *  Half the remainder get 3 piety.
                 *
                 */
                for (i = 3; i < (3 + (ilist_len(priests) - 2) / 2); i++) {
                    wout(priests[i], "As a faithful priest, %s blesses you with %s piety.",
                         god_name(religion),
                         nice_num(options.bottom_piety));
                    add_piety(priests[i], options.bottom_piety, 1);
                };

                /*
                 *  The rest get nothing.
                 *
                 */
                for (i = (3 + (ilist_len(priests) - 2) / 2); i < ilist_len(priests); i++) {
                    wout(priests[i], "%s is disappointed that you haven't brought more faithful to the church.",
                         god_name(religion));
                    wout(priests[i], "%s grants you no benefits this month.",
                         god_name(religion));
                };

            }next_skill;

    ilist_reclaim(&priests);
}

/*
 *  Wed Jul  2 11:47:55 1997 -- Scott Turner
 *
 *  Crown a high king of Pen.
 *
 */
static void
do_pen_crown() {
    int pl, top_pen = 0, top_money = 0;
    int i, pen_nation = 0, pen_crown = 0, pen_noble = 0;

    /*
     *  Is there a Pen nation?
     *
     */
    pen_nation = find_nation("Pen");
    if (!pen_nation) { return; }

    /*
     *  Find the richest Pen player.
     *
     */
    clear_temps(T_player);

    loop_char(i)
            {
                bx[player(i)]->temp += has_item(i, item_gold);
            }next_char;

    loop_player(pl)
            {
                if (nation(pl) == pen_nation
                    && bx[pl]->temp > top_money) {
                    top_money = bx[pl]->temp;
                    top_pen = pl;
                };
            }next_player;

    if (!top_pen) { return; }

    /*
     *  Pick a noble from the Top Pen to receive the crown.
     *
     */
    loop_units(top_pen, i)
            {
                pen_noble = i;
                break;
            }next_unit;

    if (!pen_noble) { return; }

    /*
     *  Find the Pen Crown, or create it.
     *
     */
    loop_subkind(sub_magic_artifact, i)
            {
                if (p_item_artifact(i)->type == ART_PEN) {
                    pen_crown = i;
                    break;
                };
            }next_subkind;

    wout(gm_player, "Pen Crown awarded to %s.", box_name(pen_noble));
    if (!pen_crown) {
        pen_crown = create_unique_item(pen_noble, sub_magic_artifact);
        set_name(pen_crown, "Pen Crown");
        rp_item(pen_crown)->weight = 5;
        p_item_artifact(pen_crown)->type = ART_PEN;
        wout(pen_noble, "The Gods of the Pen select you as the new king.");
    } else {
        if (item_unique(pen_crown) != pen_noble) {
            wout(item_unique(pen_crown), "The Gods of the Pen pass the crown to a new king.");
            wout(pen_noble, "The Gods of the Pen select you as the new king.");
        };
        move_item(item_unique(pen_crown), pen_noble, pen_crown, 1);
    };
    p_item_artifact(pen_crown)->param1 = 100;
    rp_item_artifact(pen_crown)->param2 = 100;
    rp_item_artifact(pen_crown)->uses = rnd(10, 20);
};

static void
increment_stone_ring_aura() {
    int who;
    struct char_magic *p;
    int ma;                    /* max aura */

    loop_char(who)
            {
                if (!is_magician(who) || subkind(subloc(who)) != sub_stone_cir) {
                    continue;
                }

                ma = max_eff_aura(who);

                p = p_magic(who);

                if (p->cur_aura < ma) {
                    add_aura(who, 1);
                    wout(who, "Current aura is now %d.", p->cur_aura);
                }
            }
    next_char;
}


static void
decrement_ability_shroud() {
    int who;
    struct char_magic *p;

    loop_char(who)
            {
                p = rp_magic(who);

                if (p && p->ability_shroud > 0) {
                    p->ability_shroud--;
                }
            }
    next_char;
}


#if 0
static void
decrement_loc_barrier()
{
    int where;
    struct entity_loc *p;

    loop_loc(where)
    {
        p = rp_loc(where);

#if 1
        /* start to clean up old illegal loc barriers */

        if (p && p->barrier < 0 && loc_depth(where) == LOC_build)
        {
            assert(sysclock.turn == 50);
            wout(where, "The barrier over %s will dissipate at the end of turn 57.",
                box_name(where));
            p->barrier = 8;
        }
#endif

        if (p && p->barrier > 0)
        {
            p->barrier--;
            if (p->barrier == 0)
            wout(where, "The barrier over %s has dissipated.",
                            box_name(where));
        }
    }
    next_loc;
}
#endif

static void
decrement_region_shroud() {
    int where;
    struct entity_loc *p;

    loop_loc(where)
            {
                p = rp_loc(where);

                if (p && p->shroud > 0) {
                    p->shroud--;
                    notify_loc_shroud(where);
                }
            }
    next_loc;
}


static void
decrement_meditation_hinder() {
    int who;
    struct char_magic *p;

    loop_char(who)
            {
                p = rp_magic(who);

                if (p && p->hinder_meditation > 0) {
                    p->hinder_meditation--;
                }
            }
    next_char;
}


static void
noncreator_curse_erode() {
    int who;
    struct item_ent *e;
    struct item_magic *im;
    int creator;
    int curse;

    loop_char(who)
            {
                loop_inv(who, e)
                            {
                                im = rp_item_magic(e->item);
                                if (im == NULL) {
                                    continue;
                                }

                                if (im->curse_loyalty == 0) {
                                    continue;
                                }

                                if (im->creator == who) {
                                    continue;
                                }

#if 0
                                if (kind(im->creator) != T_char)	/* lazy cleanup */
                                {
                                    log_output(LOG_CODE, "noncreator_curse_erode: "
                                        "lazy cleanup (creat=%d,item=%d,"
                                        "curse=%d)",
                                        im->creator,
                                        e->item,
                                        im->curse_loyalty);

                                    im->creator = 0;
                                    im->curse_loyalty = 0;
                                }
#endif

                                if (loyal_kind(who) == LOY_oath) {
                                    continue;
                                }
/*
 *  NOTYET:  must fix
 */


#if 1
                                log_output(LOG_CODE, "noncreator_curse_erode: NOTYET!");
#else
                                delta_loyalty(who, -(im->curse_loyalty), TRUE);

                                log_output(LOG_SPECIAL, "%s loses %d loyalty from a "
                                    "curse on %s.",
                                    box_name(who), im->curse_loyalty,
                                    box_name(e->item));
#endif
                            }
                next_inv;
            }
    next_char;
}


#if 0
/*
 *  Print the end-of-turn message at the end of each monthly events log.
 *
 *  It is important to generate some output for every character, so that
 *  even if nothing happened to a unit, it will still have a marker set
 *  saying that we have some output for it.  Presence of these markers is
 *  used to drive unit's inclusion into the master turn report.
 */

static void
announce_month_end(char *msg)
{
    int i;

    loop_loc_or_ship(i)
    {
        if (loc_depth(i) > LOC_region)
            out(i, msg);
    }
    next_loc_or_ship;

    loop_char(i)
    {
        out(i, msg);
    }
    next_char;

    loop_player(i)
    {
        out(i, msg);
    }
    next_player;
}
#endif


/*
 *  Decay unit loyalties at the end of the turn.
 */

static void
loyalty_decay() {
    int who;
    int amount;
    struct entity_char *p;

    loop_char(who)
            {
                p = rp_char(who);
                if (p == NULL || p->fresh_hire) {
                    continue;
                }

                if (p->loy_kind == LOY_unsworn ||
                    p->loy_kind == LOY_oath ||
                    p->loy_kind == LOY_npc) {
                    continue;
                }        /* no decay */

                /*
                 *  Need to ignore lost souls...
                 *
                 */
                if (subkind(who) == sub_lost_soul) continue;

                switch (p->loy_kind) {

                    case LOY_summon:
                        p->loy_rate--;
                        if (p->loy_rate <= 0 &&
                            !get_effect(who, ef_guard_loyalty, 0, 0)) {
                            leave_stack(who);
                            set_loyal(who, LOY_npc, 0);    /* redundant */
                            continue;
                        }
                        break;

                    case LOY_fear:
                        p->loy_rate--;
                        if (p->loy_rate <= 0 && rnd(1, 2) == 1 &&
                            !get_effect(who, ef_guard_loyalty, 0, 0)) {
                            log_output(LOG_DEATH, "%s deserts, %s", box_name(who), loyal_s(who));
                            (void) unit_deserts(who, deserted_player, TRUE, LOY_unsworn, 0);
                        }
                        break;

                    case LOY_contract:
                        p->loy_rate -= max(50, p->loy_rate / 10);
                        amount = max(50, 1 - p->loy_rate);
                        if (p->loy_rate <= 0 &&
                            !get_effect(who, ef_guard_loyalty, 0, 0) &&
                            has_item(who, item_gold) < amount) {
                            /*
                              *  Claim some gold"?
                              *
                              */
                            int pl = player(who);
                            if (has_item(pl, item_gold) >=
                                amount - has_item(who, item_gold)) {
                                wout(who, "CLAIMing %s gold to honor myself.",
                                     nice_num(amount - has_item(who, item_gold)));
                                move_item(pl, who, item_gold, amount - has_item(who, item_gold));
                            };
                        };

                        if (p->loy_rate <= 0 &&
                            !get_effect(who, ef_guard_loyalty, 0, 0) &&
                            charge(who, amount)) {
                            wout(who, "%s honors himself with %s gold.",
                                 box_name(who), nice_num(amount));
                            p->loy_rate += amount;
                        };
                        if (p->loy_rate <= 0 &&
                            rnd(1, 2) == 1 &&
                            !get_effect(who, ef_guard_loyalty, 0, 0)) {
                            log_output(LOG_DEATH, "%s deserts, %s", box_name(who), loyal_s(who));
                            (void) unit_deserts(who, deserted_player, TRUE, LOY_unsworn, 0);
                            continue;
                        }

                        break;

                    default:
                        assert(FALSE);
                }

                if (p->loy_rate < 0)
                    p->loy_rate = 0;
            }
    next_char;
}


static void
pillage_decay() {
    int where;

    loop_loc(where)
            {
                if (loc_pillage(where)) {
                    /* if (!recent_pillage(where)) */
                    p_subloc(where)->loot--;
                }
            }
    next_loc;
}

static void
auto_drop() {
    int pl;
    struct entity_player *p;

    loop_pl_regular(pl)
            {
                p = p_player(pl);

                if (sysclock.turn - p->last_order_turn >= 4) {
                    char *s = "";
                    char *email = "";

                    if (rp_player(pl)) {
                        if (rp_player(pl)->email && *rp_player(pl)->email) {
                            email = rp_player(pl)->email;
                        }

                        if (rp_player(pl)->full_name &&
                            *rp_player(pl)->full_name) {
                            s = rp_player(pl)->full_name;
                        }
                    }

                    queue(pl, "quit");
                    log_output(LOG_SPECIAL, "Queued drop for %s", box_name(pl));
                    log_output(LOG_SPECIAL, "    %s <%s>", s, email);
                }
            }
    next_pl_regular;
}

static void
turn_limit_drop(int turns) {
    int pl;
    struct entity_player *p;

    loop_pl_regular(pl)
            {
                p = p_player(pl);

                if (sysclock.turn - p->first_turn > turns) {
                    char *s = "";
                    char *email = "";

                    if (rp_player(pl)) {
                        if (rp_player(pl)->email && *rp_player(pl)->email) {
                            email = rp_player(pl)->email;
                        }

                        if (rp_player(pl)->full_name &&
                            *rp_player(pl)->full_name) {
                            s = rp_player(pl)->full_name;
                        }
                    }

                    wout(pl, "You are being dropped because "
                             "you have reached the turn limit for this game.");
                    wout(pl, "We hope you've enjoyed your Olympia experience "
                             "and will join us again in another game!");
                    queue(pl, "quit");
                    log_output(LOG_SPECIAL, "Queued drop for %s", box_name(pl));
                    log_output(LOG_SPECIAL, "    %s <%s>", s, email);
                }
            }
    next_pl_regular;
}

static int
maint_cost(int item, int who) {
    return rp_item(item) ? rp_item(item)->maintenance : 0;

    /*
switch (item)
{
case item_peasant:		return 1;

case item_postulant:
  if (is_priest(who)) return 0;
  else return 1;

case item_worker:
case item_soldier:
case item_sailor:
case item_angry_peasant:
case item_crossbowman:		return 2;

case item_blessed_soldier:
case item_pikeman:
case item_swordsman:
case item_pirate:
case item_archer:		return 3;

case item_fanatic:
  if (is_priest(who)) return 0;
  else return 3;

case item_knight:
case item_elite_arch:		return 4;

case item_elite_guard:		return 5;
}

return 0;
*/
}


static void
men_starve(int who, int have) {
    static ilist item = NULL;
    static ilist qty = NULL;
    static ilist cost = NULL;
    static ilist starve = NULL;
    int failcheck = 0;
    struct item_ent *e;
    int npaid = 0;
    int gold;
    int nstarve;
    int nmen = 0;
    int n;
    int i;
    char *s;
    int hit_one;

    ilist_clear(&item);
    ilist_clear(&qty);
    ilist_clear(&cost);
    ilist_clear(&starve);

    loop_inv(who, e)
                {
                    if (n = maint_cost(e->item, who)) {
                        ilist_append(&item, e->item);
                        ilist_append(&qty, e->qty);
                        ilist_append(&cost, n);
                        ilist_append(&starve, 0);

                        nmen += e->qty;
                    }
                }
    next_inv;

    gold = have;

    do {
        hit_one = FALSE;

        for (i = 0; i < ilist_len(item); i++) {
            if (qty[i] > 0 && have >= cost[i]) {
                have -= cost[i];
                qty[i]--;
                npaid++;
                hit_one = TRUE;
            }
        }
    } while (hit_one && have > 0);

    gold -= have;
    nstarve = nmen - npaid;
    nstarve = (nstarve + 2) / 3;

    assert(nstarve > 0);

    i = 0;
    while (nstarve > 0) {
        assert(failcheck++ < 10000);

        if (qty[i]) {
            nstarve--;
            starve[i]++;
            qty[i]--;
        }

        if (++i >= ilist_len(item)) {
            i = 0;
        }
    }

    autocharge(who, gold);

    for (i = 0; i < ilist_len(item); i++) {
        if (starve[i]) {
            if (item[i] == item_peasant) {
                s = "starved";
            } else {
                if (rnd(1, 2) == 1) {
                    s = "left service";
                } else {
                    s = "deserted";
                }
            }

            wout(who, "%s %s.",
                 cap(just_name_qty(item[i], starve[i])), s);
            consume_item(who, item[i], starve[i]);

            if (item[i] == item_sailor || item[i] == item_pirate) {
                check_captain_loses_sailors(starve[i], who, 0);
            }
        }
    }
}

/*
 *  Mon Apr 16 13:25:47 2001 -- Scott Turner
 *
 *  A very hacky fix.  This ought to just return the cost, but it's
 *  convenient to have it also print the modifiers to the appropriate
 *  log file.  That's the purpose of "towho".  It can be set to zero
 *  to not print out the modifiers.
 *
 */
int
unit_maint_cost(int who, int towho) {
    struct item_ent *e;
    int a;
    int cost = 0;
    int num = 0, food = 0;

    loop_inv(who, e)
                {
                    if (e->item != noble_item(who)) {   /* don't charge ni beasts */
                        cost += maint_cost(e->item, who) * e->qty;
                    }
                    num += e->qty;
                }
    next_inv;

    /*
     *  Reduction for found food.
     *
     */
    food = get_effect(who, ef_food_found, 0, 0);
    delete_effect(who, ef_food_found, 0);
    if (food && num) {
        if (food > num) {
            food = num;
        };
        if (towho) {
            wout(towho, "Foraging reduces maintenance costs by %s.",
                 gold_s(food));
        }
        cost -= food;
    };

    /*
     *  Reduction for artifact.
     *
     */
    if (a = best_artifact(who, ART_MAINTENANCE, 0, 0)) {
        food = (cost * rp_item_artifact(a)->param1) / 100;
        if (food > cost) { food = cost; }
        if (food > 0) {
            if (towho) {
                wout(towho, "Artifact reduces maintenance costs by %s.",
                     gold_s(food));
            }
            cost -= food;
        };
    };

    /*
     *  Mon Jun 16 12:32:46 1997 -- Scott Turner
     *
     *  Reduction in cost by a certain amount from the populace
     *  for garrisons.
     *
     */
    if (subkind(who) == sub_garrison) {
        int reduction = has_item(province(who), item_peasant) / 20;
        if (reduction > 50) { reduction = 50; }
        if (reduction > cost) { reduction = cost; }
        if (reduction > 0) {
            if (towho) {
                wout(towho,
                     "Local citizens contribute %s toward garrison costs.",
                     gold_s(reduction));
            }
            cost -= reduction;
        };
    };

    return cost;
}


void
charge_maint_sup(int who) {
    int cost;
    int have;

    cost = unit_maint_cost(who, who);

    if (cost < 1) {
        return;
    }

    if (autocharge(who, cost)) {
        wout(who, "Paid maintenance of %s.", gold_s(cost));
        return;
    }

    have = stack_has_item(who, item_gold);

    wout(who, "Maintenance costs are %s, can afford %s.",
         gold_s(cost), gold_s(have));

    men_starve(who, have);
}


static void
charge_maint_costs() {
    int who;
    int garr;

    /*
     *  Do garrisons first.
     *
     */
    loop_garrison(garr)
            {
                /*
                 *
                 *  Sun Dec  1 18:59:06 1996 -- Scott Turner
                 *
                 *  Do maintenance.
                 */
                charge_maint_sup(garr);
                /*
                 *  Mon Nov 23 08:27:19 1998 -- Scott Turner
                 *
                 *  A garrison should probably disband when the last
                 *  man starves.
                 *
                 */
                if (count_man_items(garr) == 0) {
                    vector_clear();
                    vector_add(garr);
                    vector_add(province(garr));
                    wout(VECT, "%s disbands.", box_name(garr));
                    p_misc(garr)->garr_castle = 0;
                    kill_char(garr, 0, S_nothing);
                };

            }next_garrison;

    loop_char(who)
            {
#if 0
                if (loyal_kind(who) == LOY_unsworn)
                    continue;
#endif

                if (subkind(player(who)) != sub_pl_regular) {
                    continue;
                }

                charge_maint_sup(who);
            }
    next_char;
}

/*
 *  Fri Sep  6 13:26:30 1996 -- Scott Turner
 *
 */
static void
update_all_effects() {
    int who;

    loop_boxes(who)
                {
                    update_effects(who);
                }next_box;
}

static void
inn_income() {
    int i;            /* variable to iterate over inns */
    int owner;        /* owner of inn */
    int n_inns;        /* number of inns sharing this province */
    int where;        /* where is the inn */
    int base;        /* base of money inn will get */
    int pil;        /* location pillage severity */
    int a;
    extern int gold_inn;

    loop_inn(i)
            {
                owner = building_owner(i);

                if (owner == 0) {
                    continue;
                }

                where = subloc(i);
                n_inns = count_loc_structures(where, sub_inn, 0);

                base = rnd(50, 75);

                if (pil = loc_pillage(where)) {
                    base /= pil;
                }

                base /= n_inns;

                if (pil == 0 && rnd(1, 8) == 1) {
                    int bonus = rnd(5, 13) * 10;

                    wout(owner, "A rich traveller stayed in %s this "
                                "month, spending %s.",
                         box_name(i), gold_s(bonus));

                    base += bonus;
                }

                /*
                 *  Artifact possibility.
                 *
                 */
                if (a = best_artifact(owner, ART_INCOME, sub_inn, 0)) {
                    base += (base * rp_item_artifact(a)->param1) / 100;
                };

                gen_item(owner, item_gold, base);
                gold_inn += base;
                wout(owner, "%s yielded %s in income.",
                     box_name(i), gold_s(base));

                if (pil) {
                    switch (rnd(1, 3)) {
                        case 1:
                            wout(owner, "Patrons were scared away by "
                                        "recent looting in the province.");
                            break;
                        case 2:
                            wout(owner, "Profits were hurt by pillaging "
                                        "in the area.");
                            break;
                        case 3:
                            wout(owner, "Recent pillaging in the area "
                                        "lowered profits.");
                            break;
                    }
                }
            }
    next_inn;
}


static void
temple_income() {
    int i, j;        /* variable to iterate over temples */
    int owner;        /* owner of inn */
    extern int gold_temple;
    int income, mu;
    int a;

    loop_temple(i)
            {
                owner = building_owner(i);

                if (owner == 0) { continue; }
                if (!is_priest(owner) ||
                    !is_temple(i) ||
                    is_priest(owner) != is_temple(i)) {
                    wout(owner, "Only a priest of the %s may collect offerings in %s.",
                         box_name(is_temple(i)), box_name(i));
                    continue;
                };

                if (options.mp_antipathy) {
                    /*
                     *  Is there a magician present?
                     *
                     */
                    mu = 0;
                    loop_char_here(i, j)
                            {
                                if (contains_mu_undead(j)) {
                                    wout(owner, "No peasants make offerings this month.");
                                    mu = 1;
                                    break;
                                };
                            }next_char_here;
                    if (mu) { continue; }
                };

                income = has_item(province(i), item_peasant) / 10;
                if (income > 100) { income = 100; }

                /*
                 *  Artifact possibility.
                 *
                 */
                if (a = best_artifact(owner, ART_INCOME, sub_temple, 0)) {
                    income += (income * rp_item_artifact(a)->param1) / 100;
                };

                gen_item(owner, item_gold, income);
                gold_temple += income;
                wout(owner, "%s collected offerings of %s.",
                     box_name(i), gold_s(income));
            }
    next_temple;
}


static void
collapsed_mine_decay() {
    int i;
    struct entity_misc *p;

    loop_collapsed_mine(i)
            {
                p = p_misc(i);

                p->mine_delay--;
                if (p->mine_delay < 0) {
                    p->mine_delay = 0;
                }

                if (p->mine_delay == 0) {
                    get_rid_of_collapsed_mine(i);
                }
            }
    next_collapsed_mine;
}


/*
 *  1 ghost warrior evaporates at the end of each turn
 */

static void
ghost_warrior_decay() {
    int i;
    int has;

    loop_char(i)
            {
                if (is_npc(i)) {
                    continue;
                }

                has = has_item(i, item_ghost_warrior);
                if (has <= 0) {
                    continue;
                }

                has = 1;

                if (has) {
                    wout(i, "%s evaporated.",
                         cap(box_name_qty(item_ghost_warrior, has)));
                    consume_item(i, item_ghost_warrior, has);
                }
            }
    next_char;
}


/*
 *  0-1-2 corpses decompose at the end of each month
 */

static void
corpse_decay() {
    int i;
    int has;

    loop_char(i)
            {
                if (is_npc(i)) {
                    continue;
                }

                has = has_item(i, item_corpse);
                if (has <= 0) {
                    continue;
                }

                has = min(has, rnd(0, 2));

                if (has) {
                    wout(i, "%s decomposed.",
                         cap(box_name_qty(item_corpse, has)));
                    consume_item(i, item_corpse, has);
                }
            }
    next_char;
}


/*
 *  Bodies of dead nobles rot after one year
 *
 *  Thu May 29 12:31:27 1997 -- Scott Turner
 *
 *  1/6 chance per month for a dead body to decay to a lost soul.
 *  1/20 chance per month for a lost soul to perish.
 *
 *  Tue May 18 07:13:14 1999 -- Scott Turner
 *
 *  Only bodies rot -- after 12 months.
 *
 */

static void
dead_body_rot() {
    int i;
    int owner;
    int t;

#if 0
    loop_lost_soul(i) {
      owner = body_old_lord(i);

      /*
       *  Thu May 28 18:21:40 1998 -- Scott Turner
       *
       *  A soul may have lost its old lord if the player quit.
       *
       */
      if (!owner) {
        kill_char(i, MATES, S_nothing);
        continue;
      };

      if (rnd(1,10) == 1) {
        if (kind(owner) == T_player)
      wout(owner, "The soul of %s has transcended the mortal plain.",
           rp_misc(i)->save_name);
        /*
         *  Make it disappear.
         *
         */
        kill_char(i, MATES, S_nothing);
      };
    } next_lost_soul
#endif

    loop_dead_body(i)
            {
                owner = item_unique(i);
                assert(owner);

                if (sysclock.turn - p_char(i)->death_time.turn > 12) {
                    if (kind(owner) == T_char) {
                        wout(owner, "The body of %s decomposed.",
                             rp_misc(i)->save_name);
                    }
                    kill_char(i, MATES, S_nothing);
                };
            }next_dead_body;
}


static void
storm_decay() {
    int i;
    struct entity_misc *p;

    loop_storm(i)
            {
                p = p_misc(i);

                p->storm_str--;
                if (p->storm_str > 0) {
                    continue;
                }

                p->storm_str = 0;

                dissipate_storm(i, TRUE);
            }
    next_storm;
}


static void
storm_owner_touch_loc() {
    int i;
    int owner;
    int where;
    int pl;

    loop_storm(i)
            {
                owner = npc_summoner(i);

                if (owner) {
                    where = subloc(i);
                    pl = player(owner);
                    if (pl) {
                        touch_loc_pl(pl, where);
                    }
                }
            }
    next_storm;
}


static void
storm_move() {
    int i;
    struct entity_misc *p;
    int owner;
    int pl;

    loop_storm(i)
            {
                p = p_misc(i);

                if (!p->npc_dir) {
                    continue;
                }

                assert(loc_depth(p->storm_move) == LOC_province);

                wout(province(i), "It has stopped raining.");
                wout(p->storm_move, "It has begun to rain.");
                set_where(i, p->storm_move);

                owner = npc_summoner(i);
                if (owner) {
                    set_known(owner, p->storm_move);
                }

                pl = player(owner);
                if (pl) {
                    touch_loc_pl(pl, p->storm_move);
                }

                p->npc_dir = 0;
                p->storm_move = 0;
            }
    next_storm;
}


#if 0
static void
collect_taxes()
{
    int fort;
    int prov;
    int amount;
    int owner;
    extern int gold_taxes;

    loop_castle(fort)
    {
        prov = province(fort);
        owner = building_owner(fort);

        if (owner == 0)
        {
            fprintf(stderr, "  castle %s, no owner\n", box_code_less(fort));
            continue;		/* no one to collect taxes */
        }

        amount = has_item(prov, item_tax_cookie);

        consume_item(prov, item_tax_cookie, amount);
        gold_taxes += amount;
        gen_item(owner, item_gold, amount);

        wout(owner, "Collected %s in taxes.", gold_s(amount));
#if 0
        fprintf(stderr, "  castle %s, %s gets %d\n", box_code_less(fort), box_code_less(owner), amount);
#endif
    }
    next_castle;
}
#endif

int
has_city(int where) {
    int enclosed;
    loop_here(where, enclosed)
            {
                if (subkind(enclosed) == sub_city) {
                    /*
                     *  Causes a memory leak!
                     *
                     */
                    exit_here;
                    return enclosed;
                };
            }next_here;
    return FALSE;
};

/*
 *  Sun Dec  1 19:02:10 1996 -- Scott Turner
 *
 *  New version.  Collect tax rate in peasants from taxed provinces and
 *  put the money in the garrisons.
 *
 *  Sat Apr 18 17:04:32 1998 -- Scott Turner
 *
 *  New new version :-0.  Take money first out of the province, then
 *  out of the hides of peasants.
 *
 */
static void
collect_taxes() {
    int where, garr, amount, city, treasury;
    extern int gold_taxes;

    stage("collect_taxes()");

    loop_loc(where)
            {
                if (province(where) != where) { continue; }
                garr = garrison_here(where);
                city = has_city(where);
                /*
                 *  Tax here if:
                 *   + garrison exists
                 *   + someone in castle
                 *   + tax rate is set
                 *   + sufficient pop
                 *
                 */
                if (garr &&
                    province_admin(where) &&
                    rp_loc(where)->tax_rate &&
                    has_item(where, item_peasant) > (city ? 1000 : 100)) {
                    /*
                     *  Figure out how many we're going to convert.
                     *
                     */
                    amount = (rp_loc(where)->tax_rate * has_item(where, item_peasant)) / 100.0;
                    if (amount < 1) { amount = 1; }
                    /*
                     *  Tax first from the treasury...
                     *
                     */
                    if (has_item(where, item_gold) > amount) {
                        treasury = amount;
                    } else {
                        treasury = has_item(where, item_gold);
                    };
                    amount -= treasury;
                    /*
                     *  Might have to consume peasants.
                     *
                     */
                    assert(amount >= 0);
                    /*
                     *  Remove peasants; add gold.
                     *
                     */
                    assert(consume_item(where, item_peasant, amount));
                    /*
                     *  Improve_Taxes doubles the base tax.
                     *
                     */
                    if (get_effect(where, ef_improve_taxes, 0, 0)) {
                        amount += amount + treasury;
                    };
                    /*
                     *  Consume out of the province.
                     *
                     */
                    assert(move_item(where, garr, item_gold, treasury));
                    gold_taxes += treasury;
                    gen_item(garr, item_gold, amount);
                    gold_taxes += amount;
                    rp_misc(garr)->garr_tax = amount + treasury;
                    wout(garr, "Collected %s in taxes.", gold_s(amount + treasury));
                };
            }next_loc;
};

#if 0
void
compute_civ_levels()
{
    int where;
    int flag;
    int i;
    int dest_civ;

    stage("compute_civ_levels()");

    clear_temps(T_loc);

    loop_loc(i)
    {
        if (loc_depth(i) < LOC_province)
            continue;

        switch (subkind(i))
        {
        case sub_castle:
            bx[province(i)]->temp += 3;
            break;

        case sub_tower:
        case sub_temple:
            bx[province(i)]->temp += 2;
            break;

        case sub_inn:
        case sub_mine:
            bx[province(i)]->temp += 1;
            break;

        case sub_city:
            if (safe_haven(i) || major_city(i))
                bx[province(i)]->temp += 4;
            else
                bx[province(i)]->temp += 2;
            break;
        }

        if (bx[province(i)]->temp > 12)
            bx[province(i)]->temp = 12;
    }
    next_loc;

    do
    {
        flag = FALSE;

        loop_province(where)
        {
            if (subkind(where) == sub_ocean)
                continue;

            loop_prov_dest(where, i)
            {
                if (i == 0)
                    continue;

                assert(bx[i]->temp <= 12);

                dest_civ = bx[i]->temp / 2;

                if (dest_civ > bx[where]->temp)
                {
                    bx[where]->temp = dest_civ;
                    flag = TRUE;
                }
            }
            next_prov_dest;
        }
        next_province;
    }
    while (flag);

    loop_province(where)
    {
        p_loc(where)->civ = bx[where]->temp / 2;
    }
    next_province;
}
#endif

/*
 *  Add_bounty
 *  Fri Sep 11 10:27:03 1998 -- Scott Turner
 *
 *  Add a random animal part bounty to a city.
 *
 */
struct trade *
new_trade(int who, int kind, int item);

static void
add_bounty(int where) {
    int i, sum = 0, choice = 0, found, count = 0, qty;
    struct trade *t;
    /*
     *  Select in one pass a random animal part.
     *  Avoid anything that is already being bought.
     *
     *  Tue Apr 27 06:36:40 1999 -- Scott Turner
     *
     *  Make the selection of animal parts correspond to the frequency
     *  of selection of animals for the wilderness.  This is a little more
     *  difficult, because we must loop through the animals rather than
     *  the animal_part.
     *
     */
    do {
        /*
         *  Pick a random beast.
         *
         */
        choice = random_beast(0);
        /*
         *  Now make sure this would not be a duplicate of a part
         *  already being offered at this market.
         *
         */
        found = 0;
        loop_trade(where, t)
                    {
                        if (t->item == i && t->kind == BUY) {
                            found = 1;
                            break;
                        };
                    }next_trade;
        assert(count++ < 1000); /* Just in case! */
    } while (found || !item_animal_part(choice));

    choice = item_animal_part(choice);
    assert(choice && valid_box(choice));
    /*
     *  Now add that trade.
     *
     */
    t = new_trade(where, CONSUME, choice);
    qty = rnd(5, 20);
    t->old_qty = qty;
    t->qty = qty;
    t->cost = rp_item(choice)->base_price;
    t = new_trade(where, BUY, choice);
    t->old_qty = qty;
    t->qty = qty;
    t->cost = rp_item(choice)->base_price;
    wout(gm_player, "Added a bounty for %s at %s.",
         plural_item_name(choice, 2),
         box_name(where));
};

/*
 *  Update_Bounties
 *  Fri Sep 11 10:20:06 1998 -- Scott Turner
 *
 *  Add/adjust monster bounties in cities.
 *
 *  Tue Jan 18 11:59:52 2000 -- Scott Turner
 *
 *  Bounties should have a CONSUME.
 *
 *  Tue Jan 18 12:00:29 2000 -- Scott Turner
 *
 *   Bounty pricing updated in update_markets.
 *
 */
void
update_bounties() {
    struct trade *t, *new;
    int where, count = 0;

    stage("update_bounties()");
    loop_city(where)
            {
                if (region(where) == faery_region ||
                    region(where) == hades_region ||
                    region(where) == cloud_region) {
                    continue;
                }
                count = 0;
                loop_trade(where, t)
                            {
                                if (t->kind == CONSUME &&
                                    subkind(t->item) == sub_animal_part) {
                                    count++;
                                    /*
                                     *  Tue Jun  6 13:12:19 2000 -- Scott Turner
                                     *
                                     *  If more than 3; delete extras.
                                     *
                                     */
                                    if (count > 3) {
                                        wout(gm_player, "Deleting excess bounty for %s in %s.",
                                             box_name(t->item), box_name(where));
                                        ilist_rem_value((ilist *) &bx[where]->trades, (int) t);
                                    };
                                };
                            }next_trade;
                /*
                 *  Possibly add some bounties.
                 *
                 */
                for (; count < 3; count++) { add_bounty(where); }
            }next_city;
};

void
post_production() {

#if 0
    compute_civ_levels();
#endif
    location_production();
    /*
     *  Thu Dec  2 05:26:58 1999 -- Scott Turner
     *
     *  Merge update_markets and location_trades
     *
     */
    update_markets();
    /* location_trades(); */
    update_bounties();
    /*
     *  Collect taxes after production to give rulers first
     *  shot at peasant $$.
     *
     */
    collect_taxes();
    seed_taxes();

    post_has_been_run = TRUE;
}


static void
hide_mage_decay() {
    int i;
    struct char_magic *p;

    loop_char(i)
            {
                p = rp_magic(i);
                if (p && p->hide_mage > 0) {
                    p->hide_mage--;
                }
            }
    next_char;
}


#if 0
/*
 *  Mon Dec  9 12:31:41 1996 -- Scott Turner
 *
 *  This stuff is no longer used, since Faery operates differently
 *  now.
 *
 */
static void
link_decay()
{
  int i;
  struct entity_subloc *p;

  loop_loc(i)
  {
      p = rp_subloc(i);

      if (p == NULL || ilist_len(p->link_to) < 1)
          continue;

      if (p->link_open > 0)
          p->link_open--;

      if (p->link_when == oly_month(sysclock))
      {
          if (p->link_open < 2 && p->link_open >= 0)
              p->link_open = 2;
      }
  }
  next_loc;
}
#endif

/*
 *  Mon Dec  9 13:05:54 1996 -- Scott Turner
 *
 *  Close a faery hill by removing all of it's exits and removing it
 *  from whatever location it is in...
 *
 */
void
close_faery_hill(int where) {
    int i, dest;
    struct entity_subloc *sl;

    assert(subkind(where) == sub_faery_hill);

    /*
     *  Remove from it's enclosing location.
     *
     *  Fri Dec 20 11:49:55 1996 -- Scott Turner
     *
     *  Better to just stick it in the faery_region as a floater.
     */
    set_where(where, faery_region);

    /*
     *  And break all of its connections to other locations.
     *
     */
    sl = p_subloc(where);
    for (i = 0; i < ilist_len(sl->link_to); i++) {
        dest = sl->link_to[i];
        assert(valid_box(dest));
        ilist_rem_value(&(p_subloc(dest)->link_from), where);
        wout(dest, "%s disappears.", box_name(where));
    };
    ilist_reclaim(&sl->link_to);
};

/*
 *  Mon Dec  9 13:19:33 1996 -- Scott Turner
 *
 *  Open a faery hill by plunking it into faery and connecting it up 
 *  to the real world.
 *
 */
void
open_faery_hill(int where) {
    int i, dest = 0, sum = 0, dest2 = 0;
    struct entity_subloc *sl;

    assert(subkind(where) == sub_faery_hill);

    /*
     *  Select a location from all the provinces in faery; don't pick
     *  one that already has a hill or a city in it.
     *
     */
    loop_province(i)
                {
                    if (!in_faery(i)) { continue; }
                    if (subkind(i) < sub_forest || subkind(i) > sub_swamp) { continue; }
                    if (province_subloc(i, sub_faery_hill)) { continue; }
                    if (province_subloc(i, sub_city)) { continue; }
                    sum++;
                    if (rnd(1, sum) == 1) {
                        dest = i;
                    };
                }next_province;
    assert(dest);
    set_where(where, dest);  /* It's now somewhere in faery... */

    /*
     *  Now create a route into the real world.  No other faery hills
     *  can be here, and it must be "wilderness".
     *
     */
    sum = 0;
    dest2 = 0;
    loop_province(i)
                {
                    if (in_faery(i) || in_hades(i) || in_clouds(i)) { continue; }
                    if (subkind(i) < sub_forest || subkind(i) > sub_swamp) { continue; }
                    if (province_subloc(i, sub_faery_hill)) { continue; }
                    if (has_item(i, item_peasant) > 100) { continue; }
                    if (province_subloc(i, sub_city)) { continue; }
                    sum++;
                    if (rnd(1, sum) == 1) {
                        dest2 = i;
                    };
                }next_province;
    assert(dest2);

    sl = p_subloc(where);
    ilist_append(&sl->link_to, dest2);
    sl = p_subloc(dest2);
    ilist_append(&sl->link_from, where);
    wout(dest2, "%s appears here.", box_name(where));

    /*
     *  A chance for the Wild Hunt to make a visit.
     *
     */
    if (rnd(1, 20) == 20) {
        wout(where, "Hollow laughter and enchanting music fill the air!");
        wout(gm_player, "The wild hunt is riding in %s.", box_name(where));
        do_wild_hunt(where);
    };

    /*
    fprintf(stderr,"Placed %s into %s with link to %s.\n",
        box_name(where),
        box_name(dest),
        box_name(dest2));
    */

};


/*
 *  Mon Dec  9 13:01:03 1996 -- Scott Turner
 *
 *  Shuffle around the faery hills.
 *
 */
static void
change_faery_hills() {
    int i;
    int closed = 0, opened = 0, total_open = 0, total_closed = 0;
    struct exit_view **l;

    loop_subkind(sub_faery_hill, i)
            {
                /*
                 *  If this faery hill is "open", then possibly close it.
                 *
                 */
                l = exits_from_loc(0, i);
                if (ilist_len(l) > 0) {
                    total_open++;
                } else {
                    total_closed++;
                }

                if (ilist_len(l) > 0 && rnd(1, 2) == 1) {
                    close_faery_hill(i);
                    closed++;
                };

                /*
                 *  If this faery hill is "closed", then possibly open it.  It's closed
                 *  if it has only the connection out to Faery...
                 *
                 */
                l = exits_from_loc(0, i);
                if (ilist_len(l) == 0 && rnd(1, 2) == 1) {
                    open_faery_hill(i);
                    opened++;
                };
            }next_subkind;

    wout(gm_player, "%s faery hills were open and %s closed.",
         nice_num(total_open), nice_num(total_closed));
    wout(gm_player, "Closed %s faery hills and opened %s.",
         nice_num(closed), nice_num(opened));
};

#if 0
/*
 *  Move tax base from cities out to province level, so it
 *  can be collected
 *
 *  Sun Dec  1 18:54:33 1996 -- Scott Turner
 *
 *  No more taxes from cities.
 */

static void
move_city_gold()
{
    int i;
    int prov;
    int has;

    loop_city(i)
    {
        prov = province(i);
        has = has_item(i, item_tax_cookie);

        move_item(i, prov, item_tax_cookie, has);
    }
    next_city;
}
#endif


#if 0
/*
 *       Season  Month   Name
 *       ------  -----   ----
 *
 *       Spring    1     Fierce winds
 *       Spring    2     Snowmelt
 *       Summer    3     Blossom bloom
 *       Summer    4     Sunsear
 *       Fall      5     Thunder and rain
 *       Fall      6     Harvest
 *       Winter    7     Waning days
 *       Winter    8     Dark night
 *
 *
 *	Uldim pass and Summerbridge are open during months 3-4-5-6
 *	At the end of month 2, issue the "now open" message.
 *	At the end of month 6, issue the "now closed" message.
 */

static void
special_locs_open()
{
    int i;

    loop_province(i)
    {
        if (summerbridge(i) == 1)
        {
            log_output(LOG_CODE, "%s open to the north.", box_name(i));
            wout(i, "The swamps of Summerbridge have dried "
                    "enough to permit passage north.");
        }
        else if (summerbridge(i) == 2)
        {
            log_output(LOG_CODE, "%s open to the south.", box_name(i));
            wout(i, "The swamps of Summerbridge have dried "
                    "enough to permit passage south.");
        }
        else if (uldim(i) == 3)
        {
            log_output(LOG_CODE, "%s open to the south.", box_name(i));
            wout(i, "The snows blocking Uldim pass to the south "
                "have melted.");
        }
        else if (uldim(i) == 4)
        {
            log_output(LOG_CODE, "%s open to the north.", box_name(i));
            wout(i, "The snows blocking Uldim pass to the north "
                "have melted.");
        }
    }
    next_province;
}


static void
special_locs_close()
{
    int i;

    loop_province(i)
    {
        if (summerbridge(i) == 1)
        {
            log_output(LOG_CODE, "%s closed to the north.", box_name(i));
            wout(i, "Seasonal rains have made Summerbridge an "
                "impassable bog to the north.");
        }
        else if (summerbridge(i) == 2)
        {
            log_output(LOG_CODE, "%s closed to the south.", box_name(i));
            wout(i, "Seasonal rains have made Summerbridge an "
                "impassable bog to the south.");
        }
        else if (uldim(i) == 3)
        {
            log_output(LOG_CODE, "%s closed to the south.", box_name(i));
            wout(i, "Falling snow blocks Uldim pass to the south "
                "for the winter.");
        }
        else if (uldim(i) == 4)
        {
            log_output(LOG_CODE, "%s closed to the north.", box_name(i));
            wout(i, "Falling snow blocks Uldim pass to the north "
                "for the winter.");
        }
    }
    next_province;
}
#endif

static void
clear_orders_sent() {
    int pl;
    struct entity_player *p;

    loop_player(pl)
            {
                p = rp_player(pl);

                if (p) {
                    p->sent_orders = 0;
                }
            }
    next_player;
}

/*
 *  Sun Dec  1 18:50:04 1996 -- Scott Turner
 *
 *  Recruited keeps track of how many recruited in a loc.
 *
 */
void
reset_recruited() {
    int where;

    loop_loc(where)
            {
                if (!rp_loc(where)) { continue; }
                rp_loc(where)->recruited = 0;
            }next_loc;

};

/*
 *  Wed Apr 16 12:08:09 1997 -- Scott Turner
 *
 *  Put a new piece of staff somewhere.
 *
 */
static int
add_staff_piece() {
    int sum, dest2, i, beasts, piece;

    sum = 0;
    dest2 = 0;
    loop_province(i)
                {
                    if (in_faery(i) || in_hades(i) || in_clouds(i)) { continue; }
                    if (subkind(i) < sub_forest || subkind(i) > sub_swamp) { continue; }
                    if (has_item(i, item_peasant) > 100) { continue; }
                    if (province_subloc(i, sub_city)) { continue; }
                    sum++;
                    if (rnd(1, sum) == 1) {
                        dest2 = i;
                    };
                }next_province;
    assert(dest2);

    beasts = create_new_beasts(dest2, 0);  /* New beast stack. */
    piece = create_unique_item(beasts, sub_special_staff);
    set_name(piece, "Staff of the Sun fragment");
    rp_item(piece)->weight = 5;

    fprintf(stderr, "Created new piece of Sun Staff at %s in %s.\n",
            box_name(dest2), box_name(beasts));

    return TRUE;
};

static void
destroy_all_priests() {
    int i;
    loop_char(i)
            {
                if (is_priest(i)) {
                    /*
                     *  This *really* kills them!
                     *
                     */
                    (void) forget_skill(i, sk_survive_fatal);
                    (void) forget_skill(i, sk_survive_fatal2);
                    (void) forget_skill(i, sk_transcend_death);
                    kill_char(i, 0, S_nothing);
                };
            }next_char;
};

static void
destroy_all_mus() {
    int i;
    loop_char(i)
            {
                if (is_magician(i)) {
                    /*
                     *  This *really* kills them!
                     *
                     */
                    (void) forget_skill(i, sk_survive_fatal);
                    (void) forget_skill(i, sk_survive_fatal2);
                    (void) forget_skill(i, sk_transcend_death);
                    kill_char(i, 0, S_nothing);
                };
            }next_char;
};

static void
destroy_staff() {
    int i;
    loop_subkind(sub_special_staff, i)
            {
                destroy_unique_item(item_unique(i), i);
            }next_subkind;
};


/*
 *  Wed Apr 16 11:50:12 1997 -- Scott Turner
 *
 *  Staff of the Sun.
 *
 *  This not only checks for the win, it takes care of inserting
 *  a new piece of the staff into the game if necessary.
 *
 */
#define STAFF_NUM_PIECES 8
#define STAFF_INITIAL_TURN 4
#define STAFF_PERIOD 4

static void
do_staff() {
    int i, num_pieces = 0, next_piece, priests = 0, mus = 0, n = 0;

    /*
     *  How many pieces of staff are out there?
     *
     */
    loop_subkind(sub_special_staff, i)
            {
                ++n;
                if (item_unique(i) && kind(item_unique(i)) == T_char) {
                    if (is_priest(item_unique(i))) { priests++; }
                    if (is_magician(item_unique(i))) { mus++; }
                };
            }next_subkind;

    /*
     *  Time for another piece?
     *
     */
    next_piece = STAFF_INITIAL_TURN + n * STAFF_PERIOD;

    if (n < STAFF_NUM_PIECES && sysclock.turn >= next_piece) {
        add_staff_piece();
    }

    /*
     *  Now check to see if anyone has won the game.
     *
     */
    if (priests == STAFF_NUM_PIECES) {
        destroy_all_mus();
        destroy_staff();
    };
    if (mus == STAFF_NUM_PIECES) {
        destroy_all_priests();
        destroy_staff();
    };
};

/*
 *  This fixes all the fees to enter locations to their new
 *  values.
 *
 */
void
update_loc_fees() {
    int i;

    loop_loc(i)
            {
                if (loc_depth(i) == LOC_province && rp_loc(i)) {
                    if (rp_loc(i)->control.nobles != rp_loc(i)->control2.nobles) {
                        rp_loc(i)->control.nobles = rp_loc(i)->control2.nobles;
                        wout(i, "New cost to enter %s is %d per noble.",
                             box_name(i), rp_loc(i)->control.nobles);
                    };
                    if (rp_loc(i)->control.nobles != rp_loc(i)->control2.nobles) {
                        rp_loc(i)->control.men = rp_loc(i)->control2.men;
                        wout(i, "New cost to enter %s is %d per 100 men.",
                             box_name(i), rp_loc(i)->control.men);
                    };
                    if (rp_loc(i)->control.nobles != rp_loc(i)->control2.nobles) {
                        rp_loc(i)->control.weight = rp_loc(i)->control2.weight;
                        wout(i, "New cost to enter %s is %d per 1000 weight.",
                             box_name(i), rp_loc(i)->control.weight);
                    };
                } else if (rp_subloc(i)) {
                    if (rp_subloc(i)->control.nobles != rp_subloc(i)->control2.nobles) {
                        rp_subloc(i)->control.nobles = rp_subloc(i)->control2.nobles;
                        wout(province(i), "New cost to enter %s is %d per noble.",
                             box_name(i), rp_subloc(i)->control.nobles);
                    };
                    if (rp_subloc(i)->control.nobles != rp_subloc(i)->control2.nobles) {
                        rp_subloc(i)->control.men = rp_subloc(i)->control2.men;
                        wout(province(i), "New cost to enter %s is %d per 100 men.",
                             box_name(i), rp_subloc(i)->control.men);
                    };
                    if (rp_subloc(i)->control.nobles != rp_subloc(i)->control2.nobles) {
                        rp_subloc(i)->control.weight = rp_subloc(i)->control2.weight;
                        wout(province(i), "New cost to enter %s is %d per 1000 weight.",
                             box_name(i), rp_subloc(i)->control.weight);
                    };
                };
                /*
                 *  Update market taxes.
                 *
                 */
                if (rp_subloc(i) &&
                    rp_subloc(i)->tax_market != rp_subloc(i)->tax_market2) {
                    rp_subloc(i)->tax_market = rp_subloc(i)->tax_market2;
                    wout(i, "New market tax in %s is %d%%.",
                         box_name(i), rp_subloc(i)->tax_market);
                };
            }next_loc;
    /*
     *  Mon Feb  8 09:51:05 1999 -- Scott Turner
     *
     *  We need to do ships, too.
     *
     */
    loop_ship(i)
            {
                if (rp_subloc(i)) {
                    if (rp_subloc(i)->control.nobles != rp_subloc(i)->control2.nobles) {
                        rp_subloc(i)->control.nobles = rp_subloc(i)->control2.nobles;
                        wout(i, "New cost to enter %s is %d per noble.",
                             box_name(i), rp_subloc(i)->control.nobles);
                    };
                    if (rp_subloc(i)->control.nobles != rp_subloc(i)->control2.nobles) {
                        rp_subloc(i)->control.men = rp_subloc(i)->control2.men;
                        wout(i, "New cost to enter %s is %d per 100 men.",
                             box_name(i), rp_subloc(i)->control.men);
                    };
                    if (rp_subloc(i)->control.nobles != rp_subloc(i)->control2.nobles) {
                        rp_subloc(i)->control.weight = rp_subloc(i)->control2.weight;
                        wout(i, "New cost to enter %s is %d per 1000 weight.",
                             box_name(i), rp_subloc(i)->control.weight);
                    };
                };
            }next_ship;
};

/*
 *  This initializes the fees for all locations.
 *
 */
void
initialize_loc_fees() {
    int i;

    loop_loc(i)
            {
                if (loc_depth(i) == LOC_province && rp_loc(i)) {
                    rp_loc(i)->control2.nobles = rp_loc(i)->control.nobles;
                    rp_loc(i)->control2.men = rp_loc(i)->control.men;
                    rp_loc(i)->control2.weight = rp_loc(i)->control.weight;
                } else if (rp_subloc(i)) {
                    rp_subloc(i)->control2.nobles = rp_subloc(i)->control.nobles;
                    rp_subloc(i)->control2.men = rp_subloc(i)->control.men;
                    rp_subloc(i)->control2.weight = rp_subloc(i)->control.weight;
                };

                if (rp_subloc(i)) {
                    rp_subloc(i)->tax_market2 = rp_subloc(i)->tax_market;
                };

            }next_loc;
};

/*
 *  Sun Dec  1 18:49:36 1996 -- Scott Turner
 *
 *  Stuff to do at the beginning of the month.
 *
 *  Fri May 21 08:10:37 1999 -- Scott Turner
 *
 *  Add experience if you're concealing yourself.
 *
 *  Fri Jun 30 19:07:45 2000 -- Scott Turner
 *
 *  Should get experience only by practice.
 */
void
pre_month() {
    int target;

    /* temp fix */
    determine_noble_ranks();

    initialize_loc_fees();
    reset_recruited();
    mine_collapse_day = rnd(1, MONTH_DAYS);

};

void
post_month() {
    stage("post_month()");
    clear_orders_sent();
    update_loc_fees();

    /*
     *  Adjust monster levels.
     *
     */
    adjust_monsters();

    /*
     *  Nobles & loyalties.
     *
     */
    add_noble_points();
    add_unformed();
    loyalty_decay();

    /*
     *  Adjust magic levels.
     *
     */
    increment_current_aura();
    increment_current_piety();
    decrement_ability_shroud();
    decrement_region_shroud();
    decrement_meditation_hinder();
    hide_mage_decay();
    if (options.mp_antipathy) { do_staff(); }

    /*
     *  Other decays.
     *
     */
    ghost_warrior_decay();
    dead_body_rot();
    storm_decay();
    storm_move();
    collapsed_mine_decay();
    pillage_decay();

    /*
     *  Income properties.
     *
     */
    inn_income();
    temple_income();

    /*
     *  Wed Jul  2 11:46:56 1997 -- Scott Turner
     *
     *  Check for the crowning of the High King of Pen
     *
     */
    if (oly_month(sysclock) == 6) {
        do_pen_crown();
    }

    post_production();

    /*
     *  Don't charge maintenance until after taxes.
     *
     */
    charge_maint_costs();
    if (options.auto_drop) { auto_drop(); }
    if (options.turn_limit) { turn_limit_drop(options.turn_limit); }

    /*
     *  Mutable map stuff.
     *
     */
    float_cloudlands();
    change_faery_hills();
    update_faery();
#if 0
    swap_region_locs(hades_region);
    swap_region_locs(faery_region);
#endif

    /*
     *  Miscellaneous
     *
     */
    determine_noble_ranks();

    post_has_been_run = TRUE;
}

/*
 *  Sun Jun  1 11:12:04 1997 -- Scott Turner
 *
 *  Drain mana/piety and kill those who get to 0.
 *
 *  Mon Jun  9 12:03:32 1997 -- Scott Turner
 *
 *  Don't drain undead...
 *
 */
static void
hades_corruption() {
    int where, i, loss;

    loop_loc(where)
            {
                if (region(where) == hades_region) {
                    loop_char_here(where, i)
                            {
                                if (subkind(i) == sub_undead) { continue; }
                                if (subkind(i) == sub_demon_lord) { continue; }
                                if (subkind(i) == sub_lost_soul) { continue; }
                                if (noble_item(i) && subkind(noble_item(i)) == sub_undead) { continue; }
                                if (noble_item(i) && subkind(noble_item(i)) == sub_demon_lord) { continue; }
                                if (noble_item(i) && subkind(noble_item(i)) == sub_lost_soul) { continue; }
                                if (has_skill(i, sk_transcend_death)) { continue; }
                                loss = rnd(1, 10);
                                if (is_priest(i)) {
                                    if (!use_piety(i, loss)) {
                                        wout(i, "The evil corruption of Hades overcomes your spirit!");
                                        kill_char(i, MATES, S_soul);
                                    }
                                    wout(i, "The evil corruption of Hades drains %s piety!",
                                         nice_num(loss));
                                } else if (!deduct_aura(i, loss)) {
                                    if (char_cur_aura(i))
                                        rp_magic(i)->cur_aura = 0;
                                    wout(i, "The unshielded magical energies overcome your magical powers!");
                                    kill_char(i, MATES, S_soul);
                                } else {
                                    wout(i, "The unshielded magical energies of Hades drain %s aura!",
                                         nice_num(loss));
                                };
                            }next_char_here;
                };
            }next_loc;
};

void
daily_events() {
    static int curse_erode_day = 0;
    static ilist weather_days = NULL;
    static int wday_index = 0;
#if 0
    static int faery_day = 0;
#endif

#ifdef NEW_TRADE
    /*
     *  Run the markets.
     *
     */
    match_all_trades();
#endif

    if (curse_erode_day == 0) {
        curse_erode_day = rnd(1, MONTH_DAYS);
    }

#if 0
    if (faery_day == 0)
        faery_day = rnd(MONTH_DAYS/2, MONTH_DAYS);
#endif

    if (ilist_len(weather_days) == 0) {
        int i;
        extern int int_comp();

        for (i = 1; i <= MONTH_DAYS; i++) {
            ilist_append(&weather_days, i);
        }

        assert(4 <= MONTH_DAYS);

        ilist_scramble(weather_days);
        qsort(weather_days, 4, sizeof(int), int_comp);
    }

    ship_coastal_damage();
    random_loc_damage();
    check_ocean_chars();

    if (sysclock.day % 7 == 0) {
        heal_characters();
        weekly_prisoner_escape_check();
        hades_corruption();
    }

    if (sysclock.day == curse_erode_day) {
        noncreator_curse_erode();
    }

    if (sysclock.day == 15) {
        increment_stone_ring_aura();
    }

    if (sysclock.day == weather_days[wday_index]) {
        wday_index++;

        natural_weather();
    }

#if 0
    if (sysclock.day == faery_day)
        auto_faery();
#endif

    update_all_effects();
}


void
touch_loc_pl(int pl, int where) {
    struct entity_player *p;
    int inside;

    p = p_player(pl);

    /*
     *  Mon Jan  6 12:10:46 1997 -- Scott Turner
     *
     *  Special case for ships?
     *
     */
    if (subkind(where) == sub_ship) { set_bit(&p->locs, where); }

    where = viewloc(where);

    set_bit(&p->locs, where);

    if (loc_depth(where) > LOC_province) {
        inside = loc(where);
        /*
         *  Thu Apr 16 07:10:59 1998 -- Scott Turner
         *
         *  Faery hills "float" in Faery, which can cause
         *  a problem here.
         *
         */
        if (loc_depth(inside) > LOC_region) {
            set_bit(&p->locs, inside);
        }
    }
}


void
touch_loc(int who) {
    int where = subloc(who);

    touch_loc_pl(player(who), where);
}


void
init_locs_touched() {
    int who;

    loop_char(who)
            {
                if (!is_prisoner(who)) {
                    touch_loc(who);
                }
            }
    next_char;

    storm_owner_touch_loc();
}

