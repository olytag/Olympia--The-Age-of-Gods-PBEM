// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"
#include "vectors/exit_view_list.h"

#define        MAX_SAVAGES    200    /* max # of savage units allowed */

static int num_savages = 0;        /* total # of savages in the world */


static int
create_savage(int where) {
    int new = create_monster_stack(item_savage, rnd(4, 26), where);
    if (new < 0) { return -1; }
    gen_item(new, item_drum, 1);

    return new;
}


static int
call_savage(int where, int to_where, int who, int why) {
    int new;

    if (controlled_humans_here(where)) {
        return FALSE;
    }

    new = create_savage(where);
    queue(new, "move %s", box_code_less(to_where));

    switch (why) {
        case 0:            /* battle challenge */
            queue(new, "attack %s", box_code_less(who));
            break;

        case 1:            /* call to arms */
            set_loyal(new, LOY_summon, 3);
            queue(new, "stack %s", box_code_less(who));
            break;

        case 2:            /* move and attack structure */
            queue(new, "use 98 1");
            queue(new, "wait time %d", rnd(35, 50));
            queue(new, "attack %s", box_code_less(who));
            break;
    }

#if 0
    init_load_sup(new);   /* make ready to execute commands immediately */
#endif

    return TRUE;
}


int
v_use_drum(struct command *c) {
    struct exit_view **l;
    int where = subloc(c->who);
    int speed = c->a;
    char *speed_s = "";
    char *s;
    int i;
    int n = 0;

    switch (speed) {
        case 1:
            speed_s = "slow ";
            break;

        case 2:
            speed_s = "fast ";
            break;
    }

    wout(c->who, "%s sounds a %sdrumbeat.", box_name(c->who), speed_s);
    wout(where, "%s sounds a %sdrumbeat.", box_name(c->who), speed_s);

    if (loc_depth(where) != LOC_province) {
        s = sout("%sbeating drums may be heard coming from %s.",
                 speed_s, box_name(where));
        wout(subloc(where), "%s", cap(s));
        return TRUE;
    }

    l = exits_from_loc_nsew_select(c->who, province(where), LAND, RAND);

    for (i = 0; i < ev_list_len(l); i++) {
        struct exit_view *v = l[i];
        int dir = v->direction;

        dir = exit_opposite[dir];

        s = sout("%sbeating drums may be heard to the %s.",
                 speed_s, full_dir_s[dir]);
        wout(v->destination, "%s", cap(s));

        if (num_savages < MAX_SAVAGES &&
            subkind(v->destination) != sub_ocean &&
            (speed == 0 || speed == 1) &&
            n == 0) {
            n = call_savage(v->destination, where, c->who, speed);
        }
    }

    switch (speed) {
        case 0:
            s = "battle challenge";
            break;

        case 1:
            s = "call to arms";
            break;

        default:
            s = "call";
            break;
    }

    if (n == 0 && (speed == 0 || speed == 1)) {
        wout(c->who, "No savages are responding to the %s.", s);
    } else if (n > 0 && (speed == 0 || speed == 1)) {
        wout(c->who, "Savages will surely respond to the %s.", s);
    }

    return TRUE;
}


int
v_summon_savage(struct command *c) {
    int i, num_savages = 0;

    if (has_item(c->who, item_drum) < 1) {
        wout(c->who, "Must first make a drum with MAKE 98 1.");
        return FALSE;
    }

    /*
     *  Fri Sep 18 07:09:04 1998 -- Scott Turner
     *
     *  A limit on how many wild men you can summon, since they
     *  are orthogonal to "control men in battle"?
     *
     */
    loop_stack(c->who, i)
            {
                if (kind(i) == T_char &&
                    subkind(i) == sub_ni &&
                    noble_item(i) == item_savage) {
                    num_savages++;
                }
            }next_stack;

    if (num_savages > 2) {
        wout(c->who, "You may only summon 3 savage stacks at a time.");
        return FALSE;
    }

    c->a = 1;    /* speed = summon */

    return v_use_drum(c);
}


static int
keep_savage_check(struct command *c) {
    int target = c->a;

    if (kind(target) != T_char || noble_item(target) != item_savage) {
        wout(c->who, "%s is not a group of savages.", box_code(target));
        return FALSE;
    }

    if (subloc(target) != subloc(c->who)) {
        wout(c->who, "%s is not here.", box_code(target));
        return FALSE;
    }

    if (loyal_kind(target) != LOY_summon) {
        wout(c->who, "%s is no longer bonded.", box_code(target));
        return FALSE;
    }

    return TRUE;
}


int
v_keep_savage(struct command *c) {
    int target = c->a;

    if (!keep_savage_check(c)) {
        return FALSE;
    }

    return TRUE;
}


int
d_keep_savage(struct command *c) {
    int target = c->a;

    if (!keep_savage_check(c)) {
        return FALSE;
    }

    set_loyal(target, LOY_summon, max(loyal_rate(target) + 2, 4));

    wout(c->who, "%s will remain for %d months.",
         box_code(target),
         loyal_rate(target));
    return TRUE;
}


int
savage_hates(int where) {

    switch (subkind(where)) {
        case sub_inn:
        case sub_inn_notdone:
        case sub_castle:
        case sub_castle_notdone:
        case sub_tower:
        case sub_tower_notdone:
        case sub_galley:
        case sub_galley_notdone:
        case sub_roundship:
        case sub_roundship_notdone:
        case sub_temple:
        case sub_temple_notdone:
            return TRUE;
    }

    return FALSE;
}


int
savage_hate_here(int where) {
    int i;
    int ret = 0;

    loop_here(where, i)
            {
                if (is_loc_or_ship(i) &&
                    loc_depth(i) == LOC_build &&
                    savage_hates(i)) {
                    ret = i;
                    break;
                }
            }
    next_here;

    return ret;
}


void
auto_savage(int who) {
    int where = subloc(who);
    int target;
    int i;

/*
 *  If stacked under someone, do nothing.
 */

    if (stack_parent(who)) {
        return;
    }

/*
 *  If in a structure, issue RAZE
 */

    if (savage_hates(where) && building_owner(where) == who) {
        queue(who, "raze");
        return;
    }

/*
 *  If there is an inn/castle/tower/ship/temple here,
 *	if empty,
 *		enter,
 *		raze.
 *	if human-occupied,
 *		beat drums to attrack other savages,
 *		wait,
 *		attack.
 */

    target = savage_hate_here(where);

    if (target) {
        if (building_owner(target) == 0) {
            queue(who, "enter %s", box_code_less(target));
            queue(who, "raze");
            return;
        }

        if (controlled_humans_here(target)) {
            if (has_item(who, item_drum) < 1) {
                gen_item(who, item_drum, 1);
            }

            queue(who, "use %d 1", item_drum);
            queue(who, "wait time %d", rnd(35, 50));
            queue(who, "attack %s", box_code_less(target));
            return;
        }
    }

/*
 *  Unstack any savages under us
 */

    loop_here(who, i)
            {
                if (kind(i) == T_char) {
                    queue(who, "unstack %d", i);
                }
            }
    next_here;

    if (!controlled_humans_here(where)) {
        queue(who, "die");
        return;
    }

    npc_move(who);
}


/*
 *  1% chance that a savage will be created to attack a structure
 *  each turn.
 *
 *  Mon Jun 26 12:48:05 2000 -- Scott Turner
 *
 *  Probably not fair to create htem in a garrisoned province.
 */

void
init_savage_attacks() {
    int fort;
    struct exit_view **l;
    int where;
    int i;

    num_savages = 0;
    loop_char(i)
            {
                if (noble_item(i) == item_savage) {
                    num_savages++;
                }
            }
    next_char;

    if (num_savages >= MAX_SAVAGES) {
        return;
    }

    loop_loc(fort)
            {
                if (loc_depth(fort) != LOC_build) {
                    continue;
                }

                if (garrison_here(province(fort))) { continue; }

                if (rnd(1, 100) < 95) {
                    continue;
                }

                where = subloc(fort);

                if (loc_depth(where) != LOC_province) {
                    continue;
                }

                l = exits_from_loc_nsew_select(0, where, LAND, RAND);

                if (ev_list_len(l) == 0) {
                    fprintf(stderr, "init_savage_attacks: no exits?\n");
                    continue;        /* probably shouldn't happen */
                }

                for (i = 0; i < ev_list_len(l); i++) {
                    if (call_savage(l[i]->destination, where, fort, 2)) {
                        break;
                    }
                }
            }
    next_loc;
}

