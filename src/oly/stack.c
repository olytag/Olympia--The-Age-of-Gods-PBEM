// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


int
here_pos(int who) {
    struct loc_info *p;
    int ret;

    p = rp_loc_info(loc(who));

    assert(p != NULL);

    ret = ilist_lookup(p->here_list, who);

    assert(ret >= 0);

    return ret;
}


/*
 *  Does a come before b in the here list?
 */

int
here_precedes(int a, int b) {
    struct loc_info *p;
    int i;

    if (loc(a) != loc(b)) {
        return FALSE;
    }    /* they're in different here lists */

    p = rp_loc_info(loc(a));
    assert(p != NULL);

    for (i = 0; i < ilist_len(p->here_list); i++)
        if (p->here_list[i] == a)
            return TRUE;
        else if (p->here_list[i] == b)
            return FALSE;

    assert(FALSE);
}


static int
first_prisoner_pos(int where) {
    struct loc_info *p;
    int i;

    p = rp_loc_info(where);

    if (p == NULL) {
        return -1;
    }

    for (i = 0; i < ilist_len(p->here_list); i++) {
        if (kind(p->here_list[i]) == T_char &&
            is_prisoner(p->here_list[i])) {
            return i;
        }
    }

    return -1;
}


int
stack_parent(int who) {
    int n;

    n = loc(who);

    if (kind(n) == T_char) {
        return n;
    }

    return 0;
}


int
stack_leader(int who) {
    int n;
    int count = 0;

    assert(kind(who) == T_char);

    n = who;
    while (kind(n) == T_char) {
        who = n;
        n = stack_parent(n);

        // todo: count may not increment in release builds
        assert(count++ < 1000);        /* infinite loop check */
    }

    return who;
}


/*
 *  is b stacked somewhere beneath a?
 */

int
stacked_beneath(int a, int b) {

    assert(kind(a) == T_char);
    assert(kind(b) == T_char);

    if (a == b) {
        return FALSE;
    }

    while (b > 0) {
        b = stack_parent(b);
        if (a == b) {
            return TRUE;
        }
    }

    return FALSE;
}


void
promote(int who, int new_pos) {
    struct loc_info *p;
    int i;
    int who_pos;

    p = rp_loc_info(loc(who));
    assert(p != NULL);

    who_pos = ilist_lookup(p->here_list, who);

    assert(who_pos >= new_pos);

    for (i = who_pos; i > new_pos; i--) {
        p->here_list[i] = p->here_list[i - 1];
    }
    p->here_list[new_pos] = who;
}


static void
unstack(int who) {
    int leader = stack_leader(who);

    assert(valid_box(leader));

/*
 *  This assert is a late add-on, to convince myself that the
 *  promote is correct for an unstack from multiple levels deep.
 */

    assert(subloc(leader) == loc(leader));

    if (release_swear(who))
        p_magic(who)->swear_on_release = FALSE;

    set_where(who, subloc(leader));
    promote(who, here_pos(leader) + 1);

/*
 *  If unstacking while moving, we have to call restore_stack_actions
 *  on who after who has been unstacked.
 */

    restore_stack_actions(who);

    if (loyal_kind(who) == LOY_summon) {
        set_loyal(who, LOY_npc, 0);
    }
}


void
leave_stack(int who) {
    int leader;

    leader = stack_parent(who);
    if (leader <= 0) {
        return;
    }

    wout(leader, "%s unstacks from us.", box_name(who));
    wout(who, "%s unstacks from %s.", box_name(who), box_name(leader));

    vector_char_here(who);
    wout(VECT, "%s unstacks from %s.",
         box_name(who), box_name(leader), just_name(who));

    unstack(who);
}


static void
stack(int who, int target) {
    int pos;

    assert(stack_parent(who) == 0);
    set_where(who, target);
    p_char(who)->moving = char_moving(target);

/*
 *  Keep prisoners at the end of the stacking list
 *  by promoting non-prisoners who join
 */

    if (!is_prisoner(who)) {
        pos = first_prisoner_pos(target);

        if (pos >= 0) {
            promote(who, pos);
        }
    }
}


void
join_stack(int who, int target) {

    assert(!stacked_beneath(who, target));
    assert(!is_prisoner(target));

    leave_stack(who);

    assert(subloc(target) == subloc(who));

    wout(who, "%s stacks beneath %s.", box_name(who), box_name(target));
    wout(target, "%s stacks beneath us.", box_name(who));

    vector_char_here(who);
    wout(VECT, "%s stacks beneath %s.", box_name(who), box_name(target));

    stack(who, target);
}


int
check_prisoner_escape(int who, int chance) {

    if (rnd(1, 100) > chance) {
        return FALSE;
    }

    prisoner_escapes(who);
    return TRUE;
}


void
prisoner_escapes(int who) {
    int leader;
    int where, out_one;

    leader = stack_parent(who);

    wout(leader, "Prisoner %s escaped!", box_name(who));
    p_char(who)->prisoner = FALSE;
    p_magic(who)->swear_on_release = FALSE;
    unstack(who);
    touch_loc(who);

    wout(who, "We escaped!");

    where = subloc(who);

    if (loc_depth(where) <= LOC_province) {
        return;
    }

    out_one = loc(where);

    if (is_ship(where) && subkind(out_one) == sub_ocean) {
        out_one = find_nearest_land(out_one);

        wout(who, "After jumping over the side of the boat and "
                  "enduring a long, grueling, swim, we finally "
                  "washed ashore at %s.", box_name(out_one));

        wout(leader, "%s jumped overboard and presumably "
                     "drowned.", just_name(who));

        log_output(LOG_SPECIAL, "!! Someone swam ashore, who=%s",
                   box_code_less(who));
    }

    move_stack(who, out_one);
}


void
prisoner_movement_escape_check(int who) {
    int i;

    loop_char_here(who, i)
            {
                if (is_prisoner(i)) {
                    (void) check_prisoner_escape(i, 2);
                }
            }
    next_char_here;
}


void
weekly_prisoner_escape_check() {
    int i;
    int who;
    int chance;

    loop_char(who)
            {
                if (is_prisoner(who)) {
                    continue;
                }

                if (subkind(subloc(who)) == sub_ocean) {
                    continue;
                }        /* they're flying */

                loop_here(who, i)
                        {
                            if (kind(i) == T_char && is_prisoner(i) && !release_swear(i)) {
                                chance = loc_depth(subloc(who)) >= LOC_build ? 1 : 2;
                                (void) check_prisoner_escape(i, chance);
                            }
                        }
                next_here;
            }
    next_char;
}


void
drop_stack(int who, int to_drop) {
    int release_swear_flag = FALSE;

    assert(stack_parent(to_drop) == who);

    if (is_prisoner(to_drop)) {
        p_char(to_drop)->prisoner = FALSE;
        touch_loc(to_drop);
        wout(who, "Freed prisoner %s.", box_name(to_drop));
        wout(to_drop, "%s set us free.", box_name(who));

        if (release_swear(to_drop)) {
            release_swear_flag = TRUE;
        }
    } else {
        wout(who, "Dropped %s from stack.", box_name(to_drop));
        wout(to_drop, "%s dropped us from the stack.", box_name(who));

        vector_char_here(to_drop);
        wout(VECT, "%s dropped %s from the stack.",
             box_name(who),
             box_name(to_drop),
             just_name(to_drop));
    }

    unstack(to_drop);

    if (release_swear_flag) {
        log_output(LOG_SPECIAL, "%s frees a swear_on_release prisoner",
                   box_name(who));

        if (rnd(1, 5) < 5) {
            wout(who, "%s is grateful for your gallantry.",
                 box_name(to_drop));
            wout(who, "%s pledges fealty to us.",
                 box_name(to_drop));

            set_lord(to_drop, player(who), LOY_oath, 1);
        } else {
            switch (rnd(1, 3)) {
                case 1:
                    wout(who, "%s spits on you, and vanishes in a cloud of orange smoke.", box_name(to_drop));
                    break;
                case 2:
                    wout(who, "%s cackles wildly and vanishes.", box_name(to_drop));
                    break;
                case 3:
                    wout(who, "%s smiles briefly at you, then vanishes.", box_name(to_drop));
                    break;
            }

            (void) unit_deserts(to_drop, 0, TRUE, LOY_unsworn, 0);
            put_back_cookie(to_drop);
            set_where(to_drop, 0);
            change_box_kind(to_drop, T_deadchar);
        }
    }
}


void
free_all_prisoners(int who) {
    int i;

    loop_here(who, i)
            {
                if (kind(i) == T_char && is_prisoner(i)) {
                    drop_stack(who, i);
                }
            }
    next_here;
}


/*
 *  Remove who from a stack, leaving those above and below him behind.
 *
 *  This routine used to let who's prisoners go free, but now it is
 *  possible to use this routine manually, from the UNSTACK command.
 */

void
extract_stacked_unit(int who) {
    int i;
    int first = 0;

/*
 *  locate first stacked non-pris char
 */

    loop_here(who, i)
            {
                if (kind(i) == T_char && !is_prisoner(i)) {
                    first = i;
                    break;
                }
            }
    next_here;

/*
 *  move all other chars beneath
 *  move up & out one level, position just after us
 */

    if (first) {
        loop_here(who, i)
                {
                    if (i != first && kind(i) == T_char) {
                        set_where(i, first);
                    }
                }
        next_here;

        set_where(first, loc(who));
        promote(first, here_pos(who) + 1);
    }

/*
 *  Free any prisoners left
 */

    loop_here(who, i)
            {
                if (kind(i) == T_char && is_prisoner(i)) {
                    prisoner_escapes(i);
                }
            }
    next_here;

    vector_char_here(who);
    wout(VECT, "%s unstacks.", box_name(who));

    leave_stack(who);
}


/*
 *  Promote lower to be before higher
 */

void
promote_stack(int lower, int higher) {
    struct loc_info *p;
    int pos;

    assert(!stacked_beneath(lower, higher));

    set_where(lower, loc(higher));

/*
 *  Now the here list has higher at some point, at or after the
 *  beginning, and lower as the last element.  We want to move
 *  lower to be before higher.
 */

    p = rp_loc_info(loc(higher));
    assert(p != NULL);

    assert(p->here_list[ilist_len(p->here_list) - 1] == lower);
    pos = ilist_lookup(p->here_list, higher);

    promote(lower, pos);

    wout(higher, "Promoted %s.", box_name(lower));
    wout(lower, "%s promoted us.", box_name(higher));
#if 0
    vector_char_here(lower);
    wout(VECT, "%s promoted %s.",
            box_name(higher), box_name(lower));
#endif
}


void
take_prisoner(int who, int target) {
    int ni = 0;

    assert(who != target);
    assert(kind(who) == T_char);
    assert(kind(target) == T_char);

    if (subkind(target) == sub_ni && beast_capturable(target)) {
        ni = TRUE;
    }

    vector_stack(stack_leader(target), TRUE);
    vector_add(who);

    if (ni) {
        wout(VECT, "%s disbands.", box_name(target));
    } else {
        wout(VECT, "%s is %s by %s.",
             box_name(target),
             "taken prisoner",
             box_name(who));
    }

#if 0
    {
        show_day = FALSE;
        out(target, "");
        show_day = TRUE;
    }
#endif

/*
 *  Suppresses further output to target
 */
    p_char(target)->prisoner = TRUE;

    take_unit_items(target, who, ni ? TAKE_NI : TAKE_ALL);
    extract_stacked_unit(target);
    interrupt_order(target);

    if (ni) {
        (void) unit_deserts(target, 0, TRUE, LOY_unsworn, 0);
        put_back_cookie(target);
        set_where(target, 0);
        change_box_kind(target, T_deadchar);
    } else {
        stack(target, who);
    }
}


int
has_prisoner(int who, int pris) {
    int i;
    int ret = FALSE;

    loop_here(who, i)
            {
                if (i == pris && is_prisoner(i)) {
                    ret = TRUE;
                    break;
                }
            }
    next_here;

    return ret;
}


int
move_prisoner(int who, int target, int pris) {
    int rs = release_swear(pris);

    unstack(pris);
    stack(pris, target);

    if (rs)
        p_magic(pris)->swear_on_release = TRUE;

    assert(rp_char(pris)->prisoner);

    return TRUE;
}


int
give_prisoner(int who, int target, int pris) {

    if (check_prisoner_escape(pris, 2)) {
        return FALSE;
    }

    move_prisoner(who, target, pris);

    wout(who, "Transferred prisoner %s to %s.",
         box_name(pris), box_name(target));

    wout(target, "%s transferred the prisoner %s to us.",
         box_name(who), box_name(pris));

    return TRUE;
}

/*
 *  Test for priests, mu/undead for stacking restraints
 *
 */
int
stack_contains_priest(int who) {
    int i;
    loop_stack(who, i)
            {
                if (is_priest(i) && !is_prisoner(i)) { return 1; }
            }next_stack;
    return 0;
};

int
contains_mu_undead(int i) {
    struct item_ent *j;
    if (is_magician(i) && !char_hide_mage(i)) { return 1; }
    /*
     *  These checks are for "independent" undead.
     *
     */
    if (noble_item(i) && subkind(noble_item(i)) == sub_undead) { return 1; }
    if (noble_item(i) && subkind(noble_item(i)) == sub_demon_lord) { return 1; }
    /*
     *  Check inventory for undead.
     *
     */
    loop_inv(i, j)
                {
                    if (subkind(j->item) == sub_undead || subkind(j->item) == sub_demon_lord) {
                        return 1;
                    }
                }next_inv;

    return 0;
};

int
stack_contains_mu_undead(int who) {
    int i;

    loop_stack(who, i)
            {
                if (contains_mu_undead(i) && !is_prisoner(i)) { return 1; }
            }next_stack;
    return 0;
};

int
v_stack(struct command *c) {
    int target = c->a;

    if (!check_char_gone(c->who, target)) {
        return FALSE;
    }

    if (target == c->who) {
        wout(c->who, "Can't stack beneath oneself.");
        return FALSE;
    }

    if (stacked_beneath(c->who, target)) {
        wout(c->who, "Cannot stack beneath %s since %s is "
                     "stacked under you.",
             box_name(target), just_name(target));
        return FALSE;
    }

    if (is_prisoner(target)) {
        wout(c->who, "Can't stack beneath prisoners.");
        return FALSE;
    }

    if (!will_admit(target, c->who, target)) {
        wout(c->who, "%s refuses to let us stack.", box_name(target));
        wout(target, "Refused to let %s stack with us.",
             box_name(c->who));
        return FALSE;
    }

    /*
     *  Clerics can't stack w/ magicians/undead and vice versa
     *
     */
    if (options.mp_antipathy &&
        ((stack_contains_priest(c->who) &&
          stack_contains_mu_undead(target)) ||
         (stack_contains_priest(target) &&
          stack_contains_mu_undead(c->who)))) {
        wout(c->who, "Priests cannot stack with magicians or undead, and vice versa.");
        return FALSE;
    };
    join_stack(c->who, target);
    return TRUE;
}


int
v_unstack(struct command *c) {
    int target = c->a;
    int i;

    if (numargs(c) < 1) {
        if (stack_parent(c->who) <= 0) {
            wout(c->who, "Not stacked under anyone.");
            return FALSE;
        }

        leave_stack(c->who);
        return TRUE;
    }

    if (c->who == target) {
#if 1
        extract_stacked_unit(c->who);
#else
        leave_stack(c->who);

        loop_here(c->who, i)
        {
            drop_stack(c->who, i);
        }
        next_here;
#endif

        return TRUE;
    }

    if (!valid_box(target) || stack_parent(target) != c->who) {
        wout(c->who, "%s is not stacked beneath us.", c->parse[1]);
        return FALSE;
    }

    drop_stack(c->who, target);
    return TRUE;
}


int
v_surrender(struct command *c) {
    int target = c->a;

    if (!check_char_gone(c->who, target)) {
        return FALSE;
    }

    if (player(target) == player(c->who)) {
        wout(c->who, "Can't surrender to oneself.");
        return FALSE;
    }

    if (is_prisoner(target)) {
        wout(c->who, "Can't surrender to a prisoner.");
        return FALSE;
    }

    log_output(LOG_SPECIAL, "Player %s surrenders %s",
               box_code_less(player(c->who)),
               box_name(c->who));

    vector_stack(stack_leader(c->who), TRUE);
    vector_stack(stack_leader(target), FALSE);

    wout(VECT, "%s surrenders to %s.", box_name(c->who), box_name(target));

    take_prisoner(target, c->who);
    return TRUE;
}


/*
 *  Does b appear later in location order than a
 */

static int
promote_after(int a, int b) {
    int where = subloc(a);
    int ret = 0;
    int i;

    assert(subloc(b) == where);

    loop_char_here(where, i)
            {
                if (i == a) {
                    ret = a;
                    break;
                } else if (i == b) {
                    ret = b;
                    break;
                }

            }
    next_char_here;

    if (ret == a) {
        return TRUE;
    }
    if (ret == b) {
        return FALSE;
    }

    assert(FALSE);
}


int
v_promote(struct command *c) {
    int target = c->a;
    int targ_par;

    if (numargs(c) < 1) {
        wout(c->who, "Must specify which character to promote.");
        return FALSE;
    }

    if (kind(target) != T_char) {
        wout(c->who, "%s is not a character.", c->parse[1]);
        return FALSE;
    }

    if (is_prisoner(target)) {
        wout(c->who, "Can't promote prisoners.");
        return FALSE;
    }

    targ_par = stack_parent(target);

    if (!check_char_here(c->who, target)) {
        return FALSE;
    }

    if (target == c->who) {
        wout(c->who, "Can't promote oneself.");
        return FALSE;
    }

/*
 *  Only do the strict check if the unit belongs to another character.
 *  If its one of ours, just promote it, since the units are in the
 *  same location.
 */

    if (player(c->who) == player(target)) {
        if (!promote_after(c->who, target)) {
            wout(c->who, "%s already comes before us in "
                         "location order.", box_name(target));
            return FALSE;
        }
    } else if (targ_par != c->who && !here_precedes(c->who, target)) {
        wout(c->who, "Characters to be promoted must be stacked "
                     "immediately beneath the promoter, or be listed after the "
                     "promoter at the same level.");

        return FALSE;
    }

    promote_stack(target, c->who);
    return TRUE;
}

