// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <stdio.h>
#include <string.h>
#include "z.h"
#include "oly.h"
#include "forward.h"
#include "vectors/accept_ent_list.h"
#include "vectors/cs_list.h"
#include "vectors/fe_list.h"
#include "vectors/item_ent_list.h"
#include "vectors/wa_list.h"


int
v_look(struct command *c) {

    if (kind(c->who) != T_char) {
        wout(c->who, "%s is not a character.", box_name(c->who));
        return FALSE;
    }

    show_loc(c->who, subloc(c->who));

    return TRUE;
}


int
v_explore(struct command *c) {
    return TRUE;
}


static int
find_lost_items(int who, int where) {
    struct item_ent *e = 0;
    int item = 0;
    int chance;

    inventory_loop(where, e)
                {
                    if (!item_unique(e->item)) {
                        continue;
                    }

/*
 *  Don't take dead bodies out of graveyards; that's what EXHUME is for
 */
                    if (subkind(where) == sub_graveyard &&
                        subkind(e->item) == sub_dead_body) {
                        continue;
                    }

/*
 *  Don't take magic rings from the market
 *
 *  Wed Apr 22 06:20:30 1998 -- Scott Turner
 *
 *  Don't find anything that is being sold.
 */
                    if (find_trade(where, SELL, e->item)) {
                        continue;
                    }

                    item = e->item;
                    break;
                }
    inventory_next;

    if (loc_depth(where) >= LOC_subloc) {
        chance = 100;
    } else {
        chance = 40;
    }

    if (item == 0 || rnd(1, 100) > chance) {
        return FALSE;
    }

    move_item(where, who, item, 1);
    wout(who, "%s found one %s.", box_name(who), box_name(item));

    log_output(LOG_MISC, "%s found %s in %s.",
               box_name(who), box_name(item),
               char_rep_location(where));

    return TRUE;
}


/*
 *  Probability breakdown for explore
 *
 *	50%	fail
 *	33%	success
 *	17%	fail, but message indicating if there is something to find
 *
 *  Tue Oct 29 11:22:06 1996 -- Scott Turner
 *
 *  Priests of Domingo get an advantage:
 *
 *      30%     fail
 *      50%     success
 *	20%	fail, but message indicating if there is something to find
 */

int
d_explore(struct command *c) {
    int hidden_exits;
    struct exit_view **l;
    int where = subloc(c->who);
    int i, chance;
    int r;

    if (find_lost_items(c->who, where)) {
        return TRUE;
    }

/*
 *  Explore in a ship should explore the surrounding ocean region
 */

    if (is_ship(where) && subkind(loc(where)) == sub_ocean) {
        where = loc(where);
        find_lost_items(c->who, where);
    }

    r = rnd(1, 100);

    if (is_priest(c->who) == sk_domingo) {
        chance = 30;
    } else {
        chance = 50;
    }

#ifdef HERO
    /*
     *  The Improved Explore skill.
     *
     */
    chance += min(50, skill_exp(c->who, sk_improved_explore));
#endif // HERO	

    if (r <= chance) {
        wout(c->who, "Exploration of %s uncovers no new features.",
             box_code(where));
        return FALSE;
    }

    l = exits_from_loc(c->who, where);

    hidden_exits = count_hidden_exits(l);

/*
 *  Nothing to find
 */

    if (hidden_exits <= 0) {
        wout(c->who, "Exploration of %s uncovers no new features.",
             box_code(where));
        return FALSE;
    }

/*
 *  Something to find, but a bad roll
 */

    if (is_priest(c->who) == sk_domingo) {
        chance = 50;
    } else {
        chance = 67;
    }


    if (r <= chance) {
        switch (rnd(1, 4)) {
            case 1:
                wout(c->who, "Rumors speak of hidden features here, "
                             "but none were found.");
                break;

            case 2:
                wout(c->who, "We suspect something is hidden here, "
                             "but did not find anything.");
                break;

            case 3:
                wout(c->who, "Something may be hidden here.  "
                             "Further exploration is needed.");
                break;

            case 4:
                wout(c->who, "Nothing was found, but further "
                             "exploration looks promising.");
                break;

            default:
                assert(FALSE);
        }
        return FALSE;
    }

/*
 *  Choose what we found randomly
 */

    i = rnd(1, hidden_exits);

    find_hidden_exit(c->who, l, hidden_count_to_index(i, l));

    return TRUE;
}


static int
may_name(int who, int target) {

    switch (kind(target)) {
        case T_char:
        case T_player:
            return player(who) == player(target);

        case T_loc:
        case T_ship:
            if (loc_depth(target) == LOC_build) {
                return player(who) == player(building_owner(target));
            }

            return may_rule_here(who, target);

        case T_item:
            if (has_auraculum(who) == target) {
                return TRUE;
            }

            if (item_creator(target) == who) {
                return TRUE;
            }

            if (is_artifact(target)) { return TRUE; }

            switch (item_use_key(target)) {
                case use_death_potion:
                case use_heal_potion:
                case use_slave_potion:
                case use_proj_cast:
                case use_quick_cast:
                    return TRUE;
            }

            return FALSE;

        case T_storm:
            if (npc_summoner(target) == who) {
                return TRUE;
            }
    }

    return FALSE;
}


int
v_name(struct command *c) {
    int target = c->who;
    char *new_name;
    char old_name[LEN];
    int l;

    if (numargs(c) >= 2 && c->a > 0) {
        target = c->a;
        cmd_shift(c);
    }

    new_name = rest_name(c, 1);

    if (new_name == NULL || *new_name == '\0') {
        wout(c->who, "No new name given.");
        return FALSE;
    }

    if (!may_name(c->who, target)) {
        wout(c->who, "Not allowed to change the name of %s.",
             box_code(target));
        return FALSE;
    }

    switch (kind(target)) {
        case T_char:
            l = 35;
            break;

        default:
            l = 25;
    }

    if (strlen(new_name) > l) {
        wout(c->who, "Name is longer than %d characters.", l);
        return FALSE;
    }

    strcpy(old_name, box_name(target));
    set_name(target, new_name);

    wout(c->who, "%s will now be known as %s.",
         old_name, box_name(target));

    if (target != c->who &&
        (kind(target) == T_char || is_loc_or_ship(target))) {
        wout(target, "%s will now be known as %s.",
             old_name, box_name(target));
    }

    return TRUE;
}


int
v_times(struct command *c) {
    struct entity_player *p;

    if (c->a == 1) {
        c->a = 2;
    } else if (c->a == 2) {
        c->a = 1;
    }

    p = p_player(player(c->who));
    p->compuserve = c->a;

    if (p->compuserve) {
        wout(c->who, "Will not receive the paper.");
    } else {
        wout(c->who, "Will receive the paper.");
    }

    return TRUE;
}


int
v_fullname(struct command *c) {
    char *new_name;
    struct entity_player *p;

    new_name = rest_name(c, 1);

    if (new_name == NULL || *new_name == '\0') {
        wout(c->who, "No new name given.");
        return FALSE;
    }

    if (strlen(new_name) > 60) {
        wout(c->who, "Name is longer than %d characters.", 60);
        return FALSE;
    }

    p = p_player(player(c->who));
    if (p->full_name) {
        my_free(p->full_name);
    }
    p->full_name = str_save(new_name);

    return TRUE;
}


int
v_banner(struct command *c) {
    int target = c->who;
    char *new_name;

    if (numargs(c) >= 2 && c->a > 0) {
        target = c->a;
        cmd_shift(c);

        if (!valid_box(target)) {
            wout(c->who, "%s is not a valid entity.",
                 box_code(target));
            return FALSE;
        }

        if (!may_name(c->who, target)) {
            wout(c->who, "You do not control %s.",
                 box_code(target));
            return FALSE;
        }
    }

    new_name = rest_name(c, 1);

    if (new_name && *new_name == '\0') {
        new_name = NULL;
    }

#if 0
    if (kind(target) != T_char)
    {
        wout(c->who, "Cannot set the banner of %s.", box_name(target));
        return FALSE;
    }
#endif

    if (new_name && strlen(new_name) > 50) {
        wout(c->who, "Banner is longer than 50 characters.");
        return FALSE;
    }

    set_banner(target, new_name);

    if (new_name && *new_name) {
        out(c->who, "Banner set.");
    } else {
        out(c->who, "Banner cleared.");
    }

    return TRUE;
}


int
how_many(int who, int from_who, int item, int qty, int have_left) {
    int num_has;

    num_has = has_item(from_who, item);

    if (num_has <= 0) {
        wout(who, "%s has no %s.",
             just_name(from_who),
             just_name(item));

        return 0;
    }

    if (num_has <= have_left) {
        wout(who, "%s has only %s.",
             just_name(from_who),
             just_name_qty(item, num_has));
        return 0;
    }

    if (qty == 0) {
        qty = num_has;
    }

    qty = min(num_has - have_left, qty);

    assert(qty > 0);

    return qty;
}

int
v_accept(struct command *c) {
    int from_who = c->a;
    int item = c->b;
    int qty = c->c;
    struct entity_char *p;
    struct accept_ent *new;
    int i;

    p = p_char(c->who);

    if (item && ((kind(item) == T_player || kind(item) == T_char) &&
                 kind(from_who) != T_char)) {  /* fixed VLN */
/* problem was:
 wanted to accept a prisoner [an5o] from noble f1k
so I wrote: accept f1k an5o 1

but got this notice on the report:

1: > ACCEPT f1k an5o 1
1: Accept expects "accept <from-who> <item> <qty>"
1: It looks like you've swapped the first two. I'll swap them back for you.

So I was instead accepting the noble from the prisoner!!!

VLN added second line to check.
*/
        int tmp;
        wout(c->who, "Accept expects \"accept <from-who> <item> <qty>\"");
        wout(c->who, "It looks like you've swapped the first two.  "
                     "I'll swap them back for you.");
        tmp = item;
        item = from_who;
        from_who = tmp;
    };

    /*
     *  Wed Jan  6 06:19:54 1999 -- Scott Turner
     *
     *  Special cases... accept clear && nation names.
     *
     */
    if (from_who == 0 && strncasecmp("clear", c->parse[1], 5) == 0) {
        int i;

        for (i = 0; i < ae_list_len(p->accept); i++) {
            my_free(p->accept[i]);
        }

        ae_list_reclaim( &p->accept);

        wout(c->who, "Accept list cleared.");
        return TRUE;
    };

    /*
     *  Is it a nation name?
     *
     */
    if (from_who == 0) {
        int n = find_nation(c->parse[1]);

        if (n) {
            wout(c->who, "Accepting from nation '%s'.", rp_nation(n)->name);
            from_who = n;
        };
    };

    /*
     *  Maybe you're already doing this?
     *
     */
    for (i = 0; i < ae_list_len(p->accept); i++) {
        if (p->accept[i]->item == item &&
            p->accept[i]->from_who == from_who &&
            p->accept[i]->qty == qty) {
            wout(c->who, "You already have that accept order active.");
            return FALSE;
        };
    };

    new = my_malloc(sizeof(*new));

    new->item = item;
    new->from_who = from_who;
    new->qty = qty;

    ae_list_append(&p->accept, new);

    return TRUE;
}

int
will_accept_sup(int who, int item, int from, int qty) {
    struct entity_char *p;
    int i;

    p = rp_char(who);

    if (p) {
        for (i = 0; i < ae_list_len(p->accept); i++) {
            int item_match = (p->accept[i]->item == item ||
                              p->accept[i]->item == 0);
            int from_match = (p->accept[i]->from_who == from ||
                              nation(from) == p->accept[i]->from_who ||
                              p->accept[i]->from_who == 0);
            int qty_match = (p->accept[i]->qty >= qty ||
                             p->accept[i]->qty == 0);

            if (item_match && from_match && qty_match) {
                if (p->accept[i]->qty) {
                    p->accept[i]->qty -= qty;
                }

                return TRUE;
            }
        }
    }

    return FALSE;
}


int
will_accept(int who, int item, int from, int qty) {

    if (item == item_gold) {
        return TRUE;
    }

    if (player(who) == player(from)) {
        return TRUE;
    }

    if (subkind(who) == sub_garrison) {
        if (may_rule_here(from, who)) {
            return TRUE;
        }

        wout(from, "%s is not under your control.",
             box_name(who));
        return FALSE;
    }

    if (will_accept_sup(who, item, from, qty) ||
        will_accept_sup(player(who), item, from, qty) ||
        will_accept_sup(who, item, player(from), qty) ||
        will_accept_sup(player(who), item, player(from), qty)) {
        return TRUE;
    }

    wout(who, "Refused %s from %s.", just_name_qty(item, qty),
         box_name(from));
    wout(from, "Refused by %s.", just_name(who));

    return FALSE;
}

/*
 *  give <who> <what> [qty] [have-left]
 *
 *  Fri Mar 30 18:15:28 2001 -- Scott Turner
 *
 *  Hacking this so that giving to your faction id puts stuff in your claim.
 *
 */

int
v_give(struct command *c) {
    int target = c->a;
    int item = c->b;
    int qty = c->c;
    int have_left = c->d;
    int ret;
    int fee;

/*
 *  Try to correct the arguments if they got the order wrong
 */

    if (numargs(c) >= 2 &&
        (kind(target) == T_item || has_prisoner(c->who, target)) &&
        (kind(item) == T_char && subloc(c->who) == subloc(item) &&
         !has_prisoner(c->who, item))) {
        int tmp;

        tmp = c->a;
        c->a = c->b;
        c->b = tmp;

        target = c->a;
        item = c->b;

        switch (numargs(c)) {
            case 2:
                wout(c->who, "(assuming you meant 'give %d %d')",
                     target, item);
                break;

            case 3:
                wout(c->who, "(assuming you meant 'give %d %d %d')",
                     target, item, qty);
                break;

            default:
                wout(c->who, "(assuming you meant 'give %d %d %d %d')",
                     target, item, qty, have_left);
        }
    }

    /*
     *  Fri Mar 30 18:17:32 2001 -- Scott Turner
     *
     *  Permit giving to your faction if allowed.
     *
     */
    if (options.claim_give &&
        target == player(c->who)) {
        if (subkind(subloc(c->who)) != sub_city) {
            wout(c->who, "You can only deposit into CLAIM in a city.");
            return FALSE;
        };
        if (item != item_gold) {
            wout(c->who, "You cannot deposit that item into CLAIM.");
            return FALSE;
        };
    } else if (!check_char_here(c->who, target)) {
        return FALSE;
    } else if (!check_char_gone(c->who, target)) {
        return FALSE;
    };

    if (loyal_kind(target) == LOY_summon) {
        wout(c->who, "Summoned entities may not be given anything.");
        return FALSE;
    }

    if (is_prisoner(target)) {
        wout(c->who, "Prisoners cannot accept anything.");
        return FALSE;
    };

    if (has_prisoner(c->who, item)) {
        if (!will_accept(target, item, c->who, 1)) {
            return FALSE;
        }

        return give_prisoner(c->who, target, item);
    }

    if (kind(item) != T_item) {
        wout(c->who, "%s is not an item or a prisoner.",
             box_code(item));
        return FALSE;
    }

    if (has_item(c->who, item) < 1) {
        wout(c->who, "%s does not have any %s.", box_name(c->who),
             box_code(item));
        return FALSE;
    }

    if (rp_item(item)->ungiveable) {
        wout(c->who, "You cannot transfer %s to another noble.",
             plural_item_name(item, 2));
        return FALSE;
    };

    qty = how_many(c->who, c->who, item, qty, have_left);

    if (qty <= 0) {
        return FALSE;
    }

    if (!will_accept(target, item, c->who, qty)) {
        return FALSE;
    }

    if (options.claim_give && target == player(c->who)) {
        fee = qty / 10;
        if (fee < 1) { fee = 1; }
        sub_item(c->who, item, fee);
        qty -= fee;
    };

    ret = move_item(c->who, target, item, qty);
    assert(ret);

    if (target == player(c->who)) {
        wout(c->who, "Deposited %s into CLAIM.", just_name_qty(item, qty));
        wout(c->who, "Paid fee of %s.", just_name_qty(item, fee));
    } else {
        wout(c->who, "Gave %s to %s.", just_name_qty(item, qty),
             box_name(target));
        wout(target, "Received %s from %s.", box_name_qty(item, qty),
             box_name(c->who));
    };

    return TRUE;
}


int
v_pay(struct command *c) {
    int target = c->a;
    int qty = c->b;
    int have_left = c->c;
    int ret;

    ret = oly_parse(c, sout("give %d 1 %d %d", target, qty, have_left));
    assert(ret);

    return v_give(c);
}


static int
may_take(int who, int target) {

    if (!check_char_here(who, target)) {
        return FALSE;
    }
    if (!check_char_gone(who, target)) {
        return FALSE;
    }

    if (subkind(target) == sub_garrison) {
        if (may_rule_here(who, target)) {
            return TRUE;
        }

        wout(who, "%s is not under your control.",
             box_name(target));
        return FALSE;
    }

    if (!my_prisoner(who, target) &&
        player(target) != player(who)) {
        wout(who, "May only take items from other units in "
                  "your faction.");
        return FALSE;
    }

    return TRUE;
}


/*
 *  get <who> <what> <qty> <have-left>
 */

int
v_get(struct command *c) {
    int target = c->a;
    int item = c->b;
    int qty = c->c;
    int have_left = c->d;
    int ret;

    if (!may_take(c->who, target)) {
        return FALSE;
    }

    if (has_prisoner(target, item)) {
        return give_prisoner(target, c->who, item);
    }

    if (kind(item) != T_item) {
        wout(c->who, "%s is not an item or a prisoner.",
             box_code(item));
        return FALSE;
    }

    if (has_item(target, item) < 1) {
        wout(c->who, "%s does not have any %s.", box_name(target),
             box_code(item));
        return FALSE;
    }

    qty = how_many(c->who, target, item, qty, have_left);

    if (qty <= 0) {
        return FALSE;
    }

    if (subkind(target) == sub_garrison && man_item(item)) {
        int garr_men = count_man_items(target);

        garr_men -= qty;

        if (garr_men < 10) {
            wout(c->who, "Garrisons must be left with a "
                         "minimum of ten men.");
            return FALSE;
        }
    }

    if (rp_item(item)->ungiveable) {
        wout(c->who, "You cannot transfer %s between nobles.",
             plural_item_name(item, 2));
        return FALSE;
    };

    ret = move_item(target, c->who, item, qty);
    assert(ret);

    wout(c->who, "Took %s from %s.", just_name_qty(item, qty),
         box_name(target));

    wout(target, "%s took %s from us.", box_name(c->who),
         box_name_qty(item, qty));

    if (item == item_sailor || item == item_pirate) {
        check_captain_loses_sailors(qty, target, c->who);
    }

    return TRUE;
}


#define    noble_cost(pl)            (1)


int
next_np_turn(int pl) {
    struct entity_player *p;
    int ft, ct;
    int n;

    p = p_player(pl);

    ct = (7 - (sysclock.turn + 1) % NUM_MONTHS);
    /* ct = (7 - (sysclock.turn + 1)) % NUM_MONTHS; */
    ft = p->first_turn % NUM_MONTHS;
    n = (ft + ct) % NUM_MONTHS;

    return n;
}


void
print_hiring_status(int pl) {
    int n;

    assert(kind(pl) == T_player);

    if (subkind(pl) != sub_pl_regular) {
        return;
    }

    n = next_np_turn(pl);

    if (n == 0) {
        n += NUM_MONTHS;
    }

    wout(pl, "The next NP will be received at the end of turn %d.",
         sysclock.turn + n);
}


void
print_unformed(int pl) {
    struct entity_player *p = rp_player(pl);
    int n;
    char buf[LEN];
    int i;

    if (p == NULL || ilist_len(p->unformed) < 1) {
        return;
    }

    n = ilist_len(p->unformed);
    *buf = '\0';

    for (i = 0; i < n && i < 5; i++) {
        strcat(buf, sout(" %s", box_code_less(p->unformed[i])));
    }

    out(pl, "");
    wout(pl, "The next %s nobles formed will be: %s", nice_num(n), buf);
}


#if 0
/*
 *  Some micromodeling nonsense to randomly equip a new noble
 *  with a few items or skills
 */

static void
equip_new_noble(int who, int new)
{
    int where = subloc(who);
    int n;
    int qty;

    if (rnd(1,4) == 1)	/* appropriate region skill */
    {
        if (is_port_city(where) && rnd(1,5) < 5)
        {
            n = sk_shipcraft;
        }
        else if (has_ocean_access(where) && rnd(1,2) == 1)
        {
            n = sk_shipcraft;
        }
        else
        {
            switch (rnd(1,4))
            {
            case 1:
            case 2:
                n = sk_combat;
                break;

            case 3:
                n = sk_construction;
                break;

            case 4:
                n = sk_stealth;
                break;

            default:
                assert(FALSE);
            }
        }

        set_skill(new, n, SKILL_know);
        wout(who, "%s knows %s.", just_name(new), box_name(n));
    }

    if (rnd(1,5) == 1)	/* a possession */
    {
        qty = 1;

        switch (rnd(1,5))
        {
        case 1:		/* gold */
            qty = rnd(50, 550);
            n = item_gold;
            break;

        case 2:
            n = item_riding_horse;
            break;

        case 3:
            n = item_longsword;
            break;

        case 4:
            n = item_longbow;
            break;

        case 5:
            n = item_warmount;
            break;

        default:
            assert(FALSE);
        }

        gen_item(new, n, qty);
        wout(who, "%s has %s.", just_name(new), just_name_qty(n, qty));
    }
}
#endif


static void
form_new_noble(int who, char *name, int new) {
    struct entity_char *p;
    struct entity_char *op;

    assert(kind(new) == T_unform);

    change_box_kind(new, T_char);

    p = p_char(new);
    op = p_char(who);

    p->behind = op->behind;
    p->fresh_hire = TRUE;
    p->health = 100;

    p_char(new)->attack = 80;
    p_char(new)->defense = 80;
    p_char(new)->break_point = 50;

    set_name(new, name);

    set_where(new, subloc(who));
    set_lord(new, player(who), LOY_contract, 500);

    join_stack(new, who);

#if 0
    equip_new_noble(who, new);
#endif
}


int
v_form(struct command *c) {
    int pl;
    int cost;

    if (subkind(subloc(c->who)) != sub_city) {
        wout(c->who, "Nobles may only be formed in cities.");
        return FALSE;
    }

    pl = player(c->who);
    cost = noble_cost(pl);

    if (player_np(pl) < cost) {
        wout(c->who, "To form another noble requires %d "
                     "Noble Point%s.", cost, add_s(cost));
        return FALSE;
    }

    return TRUE;
}


int
d_form(struct command *c) {
    char *new_name;
    int pl;
    int cost;
    int new = c->a;
    struct entity_player *p;

    pl = player(c->who);
    cost = noble_cost(pl);

    if (player_np(pl) < cost) {
        wout(c->who, "To form another noble requires %d Noble Point%s.",
             cost, add_s(cost));
        return FALSE;
    }

    p = p_player(player(c->who));

    if (new) {
        if (kind(new) == T_char && player(new) == pl) {
            wout(c->who, "You've already created that noble.");
            return FALSE;
        }
        if (kind(new) != T_unform ||
            ilist_lookup(p->unformed, new) < 0) {
            wout(c->who, "%s is not a valid unformed "
                         "noble entity.", box_code(new));
            wout(c->who, "I will use one of your unformed noble "
                         "codes at random.");
            new = 0;
        }
    }

    if (new == 0 && ilist_len(p->unformed) > 0) {
        new = p->unformed[0];
    }

#if 1
    if (new == 0) {
        wout(c->who, "No further nobles may be formed this turn.");
        return FALSE;
    }
#else
    if (new == 0)
        new = new_ent(T_unform);

    if (new < 0)
    {
        wout(c->who, "No nobles were interested in joining.");
        return FALSE;
    }
#endif

    if (numargs(c) < 2 || c->parse[2] == NULL || *(c->parse[2]) == '\0') {
        new_name = "New noble";
    } else {
        new_name = c->parse[2];
    }

    form_new_noble(c->who, new_name, new);

    ilist_rem_value(&p->unformed, new);
    deduct_np(pl, cost);

    return TRUE;
}

struct flag_ent {
    int who;
    char *flag;
};

static struct flag_ent **flags = NULL;


static int
flag_raised(int who, char *flag) {
    int i;

    for (i = 0; i < fe_list_len(flags); i++) {
        if (who != 0 &&
            player(flags[i]->who) != who &&
            flags[i]->who != who) {
            continue;
        }

        if (i_strcmp(flags[i]->flag, flag) == 0) {
            return i;
        }
    }

    return -1;
}

int v_flag(struct command *c) {
    struct flag_ent *new;
    char *flag;

    if (numargs(c) < 1) {
        wout(c->who, "Must specify what message to signal.");
        return FALSE;
    }

    flag = c->parse[1];

    if (flag_raised(c->who, flag) >= 0) {
        wout(c->who, "%s has already given that signal this month.",
             box_name(c->who));
        return FALSE;
    }

    new = my_malloc(sizeof(*new));
    new->who = c->who;
    new->flag = str_save(flag);

    fe_list_append(&flags, new);

    return TRUE;
}


static char *wait_tags[] = {
        "time",        /* 0 */
        "day",        /* 1 */
        "unit",        /* 2 */
        "gold",        /* 3 */
        "item",        /* 4 */
        "flag",        /* 5 */
        "loc",        /* 6 */
        "stack",    /* 7 */
        "top",        /* 8 */
        "ferry",    /* 9 */
        "ship",        /* 10 */
        "rain",        /* 11 */
        "fog",        /* 12 */
        "wind",        /* 13 */
        "not",        /* 14 */
        "owner",    /* 15 */

        "raining",    /* 16 -> 11 */
        "foggy",    /* 17 -> 12 */
        "windy",    /* 18 -> 13 */

        "clear",    /* 19 */
        "shiploc",    /* 20 */
        "month",    /* 21 */
        "turn",        /* 22 */

        "mist",         /* 23 */
        "misty",        /* 24 */
        "alone",    /* 25 */
        "teacher",    /* 26 */
        "teach",    /* 27 */

        NULL
};


void clear_wait_parse(struct command *c) {
    int i;

    for (i = 0; i < wa_list_len(c->wait_parse); i++) {
        my_free(c->wait_parse[i]);
        c->wait_parse[i] = NULL;
    }

    wa_list_clear(&c->wait_parse);
}


char *parse_wait_args(struct command *c) {
    int tag;
    int i;
    char *tag_s;
    struct wait_arg *new;

    assert(wa_list_len(c->wait_parse) == 0);

    i = 1;
    while (i <= numargs(c)) {
        tag_s = c->parse[i];
        tag = lookup(wait_tags, tag_s);

        switch (tag) {
            case 16:
                tag = 11;
                break;
            case 17:
                tag = 12;
                break;
            case 18:
                tag = 13;
                break;
            case 24:
                tag = 23;
                break;
            case 27:
                tag = 26;
                break;
        }

        if (tag >= 0) {
            i++;
        } else {
            return sout("Unknown condition '%s'.", tag_s);
        }

        new = my_malloc(sizeof(*new));
        wa_list_append( &c->wait_parse, new);

        new->tag = tag;
        new->a1 = 0;
        new->a2 = 0;
        new->flag = NULL;

        switch (tag) {
            case 0:        /* time n */
            case 1:        /* day n */
            case 21:    /* month */
            case 22:    /* turn */
            case 2:        /* unit n */
            case 3:        /* gold n */
            case 6:        /* loc n */
            case 20:    /* shiploc */
            case 7:        /* stack n */
            case 9:        /* ferry n */
            case 10:    /* ship n */
            case 26:    /* teacher n */
                if (i <= numargs(c)) {
                    new->a1 = parse_arg(c->who, c->parse[i++]);
                } else {
                    return sout("Argument missing for '%s'.", tag_s);
                }
                break;

            case 4:        /* item n q */
                if (i <= numargs(c)) {
                    new->a1 = parse_arg(c->who, c->parse[i++]);
                } else {
                    return sout("Argument missing for '%s'.", tag_s);
                }

                if (i <= numargs(c)) {
                    new->a2 = parse_arg(c->who, c->parse[i++]);
                } else {
                    new->a2 = 1;
                }    /* missing arg, really */
                break;

            case 5:        /* flag f [n] */
                if (i <= numargs(c))
                    new->flag = c->parse[i++];
                else
                    return sout("Flag missing.");

                new->a1 = player(c->who);    /* special default */

                if (i <= numargs(c) &&
                    (isdigit(*(c->parse[i])) || parse_arg(c->who, c->parse[i])))
                    new->a1 = parse_arg(c->who, c->parse[i++]);
                break;

            case 8:        /* top */
            case 11:    /* rain */
            case 12:    /* fog */
            case 13:    /* wind */
            case 14:    /* not */
            case 15:    /* owner */
            case 19:    /* clear */
            case 23:        /* mist */
            case 25:    /* alone */
                break;

            default:
                fprintf(stderr, "tag is %d\n", tag);
                assert(FALSE);
        }
    }

    return NULL;
}


static char *
check_wait_conditions(struct command *c) {
    int i;
    struct wait_arg *p;
    char *ret;
    int not = FALSE;
    int setnot = FALSE;
    int cond;
    int where_ship;

    where_ship = subloc(c->who);
    if (is_ship_either(where_ship)) {
        where_ship = subloc(where_ship);
    }

    if (wa_list_len(c->wait_parse) < 1) {
        if (ret = parse_wait_args(c)) {
            return ret;
        }

        assert(wa_list_len(c->wait_parse) > 0);
    }

    for (i = 0; i < wa_list_len(c->wait_parse); i++) {
        p = c->wait_parse[i];

        if (setnot) {
            setnot = FALSE;
        } else if (not) {
            not = FALSE;
        }

        switch (p->tag) {
            case 0:        /* time n */
                cond = (command_days(c) >= p->a1);
                cond = not - cond;
                if (cond) {
                    return sout("%s day%s%s passed.",
                                nice_num(p->a1),
                                p->a1 == 1 ? " has" : "s have",
                                not ? " not" : "");
                }
                break;

            case 1:        /* day n */
                cond = (sysclock.day >= p->a1);
                cond = not - cond;
                if (cond) {
                    if (not) {
                        return sout("today is%s day %d.",
                                    not ? " not" : "", p->a1);
                    } else {
                        return sout("today is day %d.",
                                    sysclock.day);
                    }
                }
                break;

            case 21:    /* month n */
            case 22:    /* turn n */
                cond = (sysclock.turn >= p->a1);
                cond = not - cond;
                if (cond) {
                    if (not) {
                        return sout("it is%s turn %d.",
                                    not ? " not" : "", p->a1);
                    } else {
                        return sout("it is turn %d.",
                                    sysclock.turn);
                    }
                }
                break;

            case 2:        /* unit n */
                if (!valid_box(p->a1)) {
                    return sout("%s does not exist.", box_code(p->a1));
                }

#if 0
                cond = (subloc(c->who) == subloc(p->a1));
#else
                cond = (char_here(c->who, p->a1));
#endif
                cond = not - cond;
                if (cond) {
                    return sout("%s is%s here.", box_code(p->a1),
                                not ? " not" : "");
                }
                break;

            case 3:        /* gold n */
                cond = (has_item(c->who, item_gold) >= p->a1);
                cond = not - cond;
                if (cond) {
                    if (not) {
                        return sout("%s doesn't have %s.",
                                    just_name(c->who),
                                    gold_s(p->a1));
                    }

                    return sout("%s has %s.",
                                just_name(c->who),
                                gold_s(has_item(c->who, item_gold)));
                }
                break;

            case 4:        /* item n q */
#if 0
                /* fails if we wait for a corpse */

                if (kind(p->a1) != T_item)
                  return sout("%s is not an item.", box_code(p->a1));
#endif

                cond = (kind(p->a1) == T_item && has_item(c->who, p->a1) >= p->a2);
                cond = not - cond;
                if (cond) {
                    if (not) {
                        return sout("%s doesn't have %s.",
                                    just_name(c->who),
                                    just_name_qty(p->a1, p->a2));
                    }

                    return sout("%s has %s.",
                                just_name(c->who),
                                just_name_qty(p->a1,
                                              has_item(c->who, p->a1)));
                }
                break;

            case 5:        /* flag */
            {
                int j;

                if (p->a1 && !valid_box(p->a1)) {
                    return sout("%s does not exist.", box_code(p->a1));
                }

                j = flag_raised(p->a1, p->flag);

                if (not) {
                    if (j < 0) {
                        return "received no signal";
                    }
                } else {
                    if (j >= 0) {
                        return sout("%s signaled '%s'",
                                    box_name(flags[j]->who),
                                    flags[j]->flag);
                    }
                }
                break;
            }

            case 6:        /* loc */
                if (!is_loc_or_ship(p->a1) && !c->f) {
                    wout(c->who, "Warning: %s is not a location or ship.",
                         box_code(p->a1));
                    c->f = 1;
                };

                cond = (subloc(c->who) == p->a1);
                cond = not - cond;
                if (cond) {
                    return sout("%sat %s.",
                                not ? "not " : "",
                                box_name(p->a1));
                }
                break;

            case 20:    /* shiploc */
            {
                int ship = subloc(c->who);
                int where;

                if (!is_ship(ship) && !is_ship_notdone(ship)) {
                    return sout("%s is not on a ship.", box_name(c->who));
                }

                if (!is_loc_or_ship(p->a1)) {
                    return sout("%s is not a location or ship.",
                                box_code(p->a1));
                }

                where = subloc(ship);

                cond = (where == p->a1);
                cond = not - cond;
                if (cond) {
                    return sout("%sat %s.",
                                not ? "not " : "",
                                box_name(p->a1));
                }
                break;
            }

            case 7:        /* stack */
#if 0
                if (kind(p->a1) != T_char)
                  return sout("%s is not a live character.", box_code(p->a1));
#else
                if (kind(p->a1) != T_char) {
                    break;
                }            /* just hang */
#endif

                cond = (stack_leader(c->who) == stack_leader(p->a1));
                cond = not - cond;
                if (cond)
                    return sout("%s is%s stacked with us.",
                                box_name(p->a1),
                                not ? " not" : "");
                break;

            case 8:        /* top */
                cond = (stack_leader(c->who) == c->who);
                cond = not - cond;
                if (cond)
                    return sout("we are%s the stack leader",
                                not ? " not" : "");
                break;

            case 9:        /* ferry n */
                if (!is_ship(p->a1))
                    return sout("%s is not a ship", box_code(p->a1));

                cond = (subloc(p->a1) == subloc(c->who) && ferry_horn(p->a1));
                cond = not - cond;
                if (cond)
                    return sout("the ferry has%s signaled.",
                                not ? " not" : "");

                break;

            case 10:    /* ship n */
                if (kind(p->a1) != T_ship)
                    return sout("%s is not a ship.", box_code(p->a1));

                cond = (where_ship == subloc(p->a1));
                cond = not - cond;
                if (cond)
                    return sout("%s is%s here.", box_code(p->a1),
                                not ? " not" : "");
                break;

            case 11:    /* rain */
                cond = (weather_here(province(c->who), sub_rain));
                cond = not - cond;
                if (cond)
                    return sout("it is%s raining.",
                                not ? " not" : "");
                break;

            case 12:    /* fog */
                cond = (weather_here(province(c->who), sub_fog));
                cond = not - cond;
                if (cond)
                    return sout("it is%s foggy.",
                                not ? " not" : "");
                break;

            case 13:    /* wind */
                cond = (weather_here(province(c->who), sub_wind));
                cond = not - cond;
                if (cond)
                    return sout("it is%s windy.",
                                not ? " not" : "");
                break;

            case 23:    /* mist */
                cond = (weather_here(province(c->who), sub_mist));
                cond = not - cond;
                if (cond)
                    return sout("it is%s misty.",
                                not ? " not" : "");
                break;
            case 14:    /* not */
                not = TRUE;
                setnot = TRUE;
                break;

            case 15:        /* owner */
                cond = (first_character(subloc(c->who)) == c->who);
                cond = not - cond;
                if (cond)
                    return sout("we are%s the first character here",
                                not ? " not" : "");
                break;

            case 19:    /* clear */
                cond = (!weather_here(subloc(c->who), sub_fog) &&
                        !weather_here(subloc(c->who), sub_rain) &&
                        !weather_here(subloc(c->who), sub_wind));
                cond = not - cond;
                if (cond)
                    return sout("it is%s clear.",
                                not ? " not" : "");
                break;

            case 25:    /* alone */
                cond = ilist_len(rp_loc_info(subloc(c->who))->here_list) == 1;
                cond = not - cond;
                if (cond)
                    return sout("you are%s alone.",
                                not ? " not" : "");
                break;

            case 26:    /* teach <n> */
            {
                int item, bonus;
                cond = being_taught(c->who, p->a1, &item, &bonus);
                cond = not - (cond == TAUGHT_SPECIFIC);
                if (cond)
                    return sout("instruction for %s is%s available.",
                                box_code(p->a1),
                                not ? " not" : "");
                break;
            };

            default:
                fprintf(stderr, "tag is %d\n", p->tag);
                assert(FALSE);
        }
    }

    return NULL;
}


ilist wait_list = NULL;


int
v_wait(struct command *c) {
    char *s;

    if (numargs(c) < 1) {
        wout(c->who, "Must say what condition to wait for.");
        return FALSE;
    }

    clear_wait_parse(c);

    if (s = check_wait_conditions(c)) {
        wout(c->who, "Wait finished: %s", s);

        c->wait = 0;
        c->inhibit_finish = TRUE;    /* don't call d_wait */
        return TRUE;
    }

    ilist_append(&wait_list, c->who);
    return TRUE;
}


int
d_wait(struct command *c) {
    char *s;

    if (s = check_wait_conditions(c)) {
        wout(c->who, "Wait finished: %s", s);
        ilist_rem_value(&wait_list, c->who);

        c->wait = 0;
        c->inhibit_finish = TRUE;
        clear_wait_parse(c);
        return TRUE;
    }

    return TRUE;
}


int
i_wait(struct command *c) {

    ilist_rem_value(&wait_list, c->who);
    return TRUE;
}


int
v_split(struct command *c) {
    int lines = c->a;
    int bytes = c->b;
    struct entity_player *p;
    int pl;

    pl = player(c->who);
    p = p_player(pl);

    if (lines > 0 && lines < 1000) {
        lines = 1000;
        out(c->who, "Minimum lines to split at is 1,000");
    }

    if (bytes > 0 && bytes < 10000) {
        bytes = 10000;
        out(c->who, "Minimum bytes to split at is 10,000");
    }

    p->split_lines = lines;
    p->split_bytes = bytes;

    if (lines == 0 && bytes == 0) {
        out(c->who, "Reports will not be split when mailed.");
    } else if (lines && bytes) {
        out(c->who, "Reports will be split at %d lines or "
                    "%d bytes, whichever limit is hit first.",
            lines, bytes);
    } else if (lines) {
        out(c->who, "Reports will be split at %d lines.", lines);
    } else {
        out(c->who, "Reports will be split at %d bytes.", bytes);
    }

    return 0;
}


int
v_emote(struct command *c) {
    int target = c->a;

    if (numargs(c) < 2) {
        wout(c->who, "Usage: EMOTE <target> <message>");
        return FALSE;
    }

    wout(target, c->parse[2], box_name(target));

    return TRUE;
}
