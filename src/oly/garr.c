// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "z.h"
#include "oly.h"
#include "forward.h"
#include "vectors/cs_list.h"
#include "vectors/item_ent_list.h"


/*
 *	garrison -> castle -> owner [ -> char ]*
 *			     "admin"          "top ruler"
 */

/*

1-	touch loc on pledge of terroritory
2-	initial touch loc for both castle and ruler
3-	hook pledge into glob.c
4-	determine everyone's status at turn end
5-	display status in display.c
6.	who may get/give to a garrison
7-	maintenance of men in garrison
8-	forwarding of gold to castle
9-	circular pledge detector
10-	replace loc_owner with building_owner, province_ruler
11.	credit for castles as well as garrisoned provinces
12-	"garrison" keyword that matches the garrison in the current loc
13-	take -- leave behind 10
14-	there can't be an ni - 0, can there?  what if attacked?
15-	status = min(own status, pledge lord's status - 1)
16-	allow owner or admin to name province, sublocs
17-	garrison log for output

*/


/*
 *  Garrison should always be first; we should just have to look
 *  at the first character
 */

int
garrison_here(int where) {
    int n;

    n = first_character(where);

    if (n && subkind(n) == sub_garrison) {
        return n;
    }

    return 0;
}


int
province_admin(int n) {
    int garr;
    int castle;

    if (kind(n) == T_loc) {
        assert(loc_depth(n) == LOC_province);
        garr = garrison_here(n);
        if (garr == 0) {
            return 0;
        }
    } else {
        assert(subkind(n) == sub_garrison);
        garr = n;
    }

    castle = garrison_castle(garr);

    if (!valid_box(castle)) {
        return 0;
    }

    return building_owner(castle);
}


int
top_ruler(int n) {
    return province_admin(n);
}


#if 0
/*
 *  is b pledged somewhere beneath a?
 */

static int
pledged_beneath(int a, int b)
{

    assert(kind(a) == T_char);
    assert(kind(b) == T_char);

    if (a == b)
        return FALSE;

    while (b > 0)
    {
        b = char_pledge(b);
        if (a == b)
            return TRUE;
    }

    return FALSE;
}
#endif

#if 0
int
may_rule_here(int who, int where)
{
  int pl = player(who);
  int i;
  int ret = FALSE;

  if (is_loc_or_ship(where))
    where = province(where);
  else
    assert(subkind(where) == sub_garrison);

  loop_loc_owner(where, i)
    {
      if (player(i) == pl)
    {
      ret = TRUE;
      break;
    }
    }
  next_loc_owner;

  return ret;
}
#endif

/*
 *  Fri May  7 18:34:54 1999 -- Scott Turner
 *
 *  The new rule is that you rule here if you have permission to
 *  stack with the castle ruler.
 *
 */
int
may_rule_here(int who, int where) {
    return will_admit(province_admin(province(where)),
                      province_admin(province(where)), who);
}

#if 0
ilist
players_who_rule_here(int where)
{
    static ilist l = NULL;
    int i;
    int pl;
    int loop_check = 5000;

    ilist_clear(&l);

    loop_loc_owner(where, i)
    {
        pl = player(i);

        if (pl && ilist_lookup(l, pl) < 0)
            ilist_append(&l, pl);

        if (loop_check <= 0)
        {
            int j;
            for (j = 0; j < ilist_len(l); j++)
                fprintf(stderr, "l[%d] = %d\n", j, l[j]);
            fprintf(stderr, "where = %d, i = %d\n", where, i);
        }
        assert(loop_check-- > 0);
    }
    next_loc_owner;

    return l;
}
#endif

#if 0
void
touch_garrison_locs()
{
    int garr;
    int where;
    int owner;

    loop_garrison(garr)
    {
        where = subloc(garr);

        loop_loc_owner(garr, owner)
        {
            touch_loc_pl(player(owner), where);
        }
        next_loc_owner;
    }
    next_garrison;
}
#endif


static int
new_province_garrison(int where, int castle) {
    int new;

    new = new_char(sub_garrison,
                   0, where, -1, garr_pl, LOY_npc,
                   0, "Garrison");

    if (new < 0) {
        return -1;
    }

    gen_item(new, item_soldier, 10);
    p_misc(new)->cmd_allow = 'g';
    p_misc(new)->garr_castle = castle;
    p_char(new)->guard = TRUE;
    p_char(new)->break_point = 0;

    promote(new, 0);

    out(where, "%s now guards %s.", liner_desc(new), box_name(where));

    return new;
}


int
v_garrison(struct command *c) {
    int castle = c->a;
    int where = subloc(c->who);
    int new;

    if (loc_depth(where) != LOC_province) {
        out(c->who, "Garrisons may only be installed at "
                    "province level.");
        return FALSE;
    }

    if (garrison_here(where)) {
        out(c->who, "There is already a garrison here.");
        return FALSE;
    }

#if 0
    if (first_character(where) != c->who)
    {`<
        out(c->who, "Must be the first unit in the location to "
                "install a garrison.");
        return FALSE;
    }
#endif

    if (numargs(c) < 1) {
        out(c->who, "Must specify a castle to claim the province "
                    "in the name of.");
        return FALSE;
    }

    if (subkind(castle) == sub_castle_notdone) {
        out(c->who, "%s is not finished.  Garrisons may only"
                    " be bound to completed castles.",
            box_name(castle));
        return FALSE;
    }

    if (subkind(castle) != sub_castle) {
        out(c->who, "%s is not a castle.", c->parse[1]);
        return FALSE;
    }

    if (region(castle) != region(where)) {
        out(c->who, "%s is not in this region.", box_name(castle));
        return FALSE;
    }

#if 0
    {
        out(c->who, "A garrison here must be bound to %s.",
                    box_name(castle));
        return FALSE;
    }
#endif

    if (has_item(c->who, item_soldier) < 10) {
        out(c->who, "Must have %s to establish a new garrison.",
            box_name_qty(item_soldier, 10));
        return FALSE;
    }

    new = new_province_garrison(where, castle);

    if (new < 0) {
        out(c->who, "Failed to install garrison.");
        return FALSE;
    }

    /*
     *  Reset the tax rate for this province.
     *
     */
    rp_loc(where)->tax_rate = 0;

    consume_item(c->who, item_soldier, 10);

    out(c->who, "Installed %s", liner_desc(new));
    out(c->who, "Local tax rate set to zero percent.");

    return TRUE;
}


#if 0
int
v_pledge(struct command *c)
{
    int target = c->a;

    if (target == c->who)
    {
        wout(c->who, "Can't pledge to yourself.");
        return FALSE;
    }

    if (target == 0)
    {
        p_magic(c->who)->pledge = 0;
        out(c->who, "Pledge cleared.  "
                "Lands will be claimed for ourselves.");
        return TRUE;
    }

    if (kind(target) != T_char)
    {
        out(c->who, "%s is not a character.", c->parse[1]);
        return FALSE;
    }

    if (is_npc(target))
    {
        out(c->who, "May not pledge land to %s.", c->parse[1]);
        return FALSE;
    }

    if (pledged_beneath(c->who, target))
    {
        wout(c->who, "Cannot pledge to %s since %s is pledged to you.",
                box_name(target), just_name(target));
        return FALSE;
    }

    out(c->who, "Lands are now pledged to %s.", box_name(target));

    out(target, "%s pledges to us.", box_name(c->who));

    p_magic(c->who)->pledge = target;

    return TRUE;
}
#endif

static int
nprovs_to_rank(int n) {

    if (n == 0) { return 0; }
    if (n <= 3) { return RANK_lord; }
    if (n <= 7) { return RANK_knight; }
    if (n <= 11) { return RANK_baron; }
    if (n <= 15) { return RANK_count; }
    if (n <= 19) { return RANK_earl; }
    if (n <= 22) { return RANK_marquess; }

    return RANK_duke;
}


char *
rank_s(int who) {
    int n = char_rank(who);

    switch (n) {
        case 0:
            return "";
        case RANK_lord:
            return ", lord";
        case RANK_knight:
            return ", knight";
        case RANK_baron:
            return ", baron";
        case RANK_count:
            return ", count";
        case RANK_earl:
            return ", earl";
        case RANK_marquess:
            return ", marquess";
        case RANK_duke:
            return ", duke";
        case RANK_king:
            return ", king";

        default:
            /* Temp fix for old ranks. */
            return "";
            assert(FALSE);
    }
}


static void
find_kings() {
    int reg;
    int where;
    int ruler;
    int nprovs;

    loop_loc(reg)
            {
                if (loc_depth(reg) != LOC_region) {
                    continue;
                }

                ruler = -1;
                nprovs = 0;

                loop_here(reg, where)
                        {
                            if (kind(where) != T_loc) {
                                continue;
                            }

                            nprovs++;

                            if (ruler == -1) {
                                ruler = top_ruler(where);
                                if (ruler == 0) {
                                    break;
                                }        /* fail */
                            } else {
                                if (ruler != top_ruler(where)) {
                                    ruler = 0;
                                    break;        /* fail */
                                }
                            }
                        }
                next_here;

                if (ruler && nprovs >= 25) {
                    p_char(ruler)->rank = RANK_king;
                };
            }
    next_loc;
}


/*
 *  A noble's status is:
 *
 *	min(status by own provinces, lord's status - 1)
 *
 *  Fri May  7 18:11:34 1999 -- Scott Turner
 *
 *  Only by your own provinces.
 *
 */

void
determine_noble_ranks() {
    int garr;
    int owner;
    int who, i, j;

    stage("determine_noble_ranks()");

    clear_temps(T_player);
    clear_temps(T_char);

    /*
     *  Cal # of controlled provinces for every castle-sitter.
     *
     */
    loop_garrison(garr)
            {
                if (valid_box(province_admin(garr))) {
                    bx[province_admin(garr)]->temp++;
                }
            }
    next_garrison;

    /*
     *  Now find the best castle-sitter per each player.
     *
     */
    loop_player(i)
            {
                loop_units(i, j)
                        {
                            if (bx[j]->temp > bx[i]->temp) { bx[i]->temp = bx[j]->temp; }
                        }next_unit;
            }next_player;

    /*
     *  Now give each char the rank of the best castle-sitter in his faction.
     *
     */
    loop_char(who)
            {
                if (char_rank(who))
                    rp_char(who)->rank = 0;

                if (bx[player(who)]->temp == 0) {
                    continue;
                }

                p_char(who)->rank = nprovs_to_rank(bx[player(who)]->temp);
            }
    next_char;

    find_kings();
}


int
garrison_notices(int garr, int target) {
    struct entity_misc *p;

    if (is_npc(target) ||
        count_stack_units(target) >= 5 ||
        count_stack_figures(target) >= 20) {
        return TRUE;
    }

    p = rp_misc(garr);

    if (p && ilist_lookup(p->garr_watch, target) >= 0) {
        return TRUE;
    }

    return FALSE;
}


int
garrison_spot_check(int garr, int target) {
    int i;
    int found = FALSE;
    struct entity_misc *p;

    assert(valid_box(garr));

    p = rp_misc(garr);
    if (p == NULL) {
        return FALSE;
    }

    loop_stack(target, i)
            {
                if (ilist_lookup(p->garr_watch, i) >= 0) {
                    found = TRUE;
                    break;
                }
            }
    next_stack;

    if (found) {
        wout(garr, "Spotted in %s:", box_name(province(garr)));
    }

    return found;
}


static char *
garr_own_s(int garr) {
    static char buf[LEN];

    *buf = '\0';
    strcat(buf, box_code_less(province_admin(garr)));

    return buf;
}


void
garrison_summary(int pl) {
    int garr;
    static ilist l = NULL;
    int i;
    int taxr;
    int first = TRUE;
    struct item_ent *e;

    ilist_clear(&l);

    /*
     *  Fri May  7 18:41:08 1999 -- Scott Turner
     *
     *  Only the castle owner gets the garrison report.
     *
     */
    loop_garrison(garr)
            {
                if (player(province_admin(garr)) != pl) {
                    continue;
                }

                ilist_append(&l, garr);
            }
    next_garrison;

    if (ilist_len(l) == 0) {
        return;
    }

    tagout(pl, "<tag type=garr_report pl=%d>", pl);
    tagout(pl, "<tag type=header>");

    out(pl, "");
    out(pl, "Garrison Report:");
    out(pl, "");
    out(pl, "%4s %5s %6s %4s %4s %4s %4s %6s %s",
        "garr", "where", "border", "men", "cost", "tax", "rate", "castle", "rulers");
    out(pl, "%4s %5s %6s %4s %4s %4s %4s %6s %s",
        "----", "-----", "------", "---", "----", "---", "----", "------", "------");
    tagout(pl, "</tag type=header>");

    sort_for_output(l);

    for (i = 0; i < ilist_len(l); i++) {
        garr = l[i];

        taxr = rp_loc(subloc(garr))->tax_rate;

        tagout(pl, "<tag type=garrison unit=%d loc=%d men=%d "
                   "cost=%d tax=%d rate=%d castle=%d ruler=%d closed=%d>",
               garr,
               subloc(garr),
               count_stack_figures(garr),
               unit_maint_cost(garr, 0),
               rp_misc(garr)->garr_tax,
               taxr,
               garrison_castle(garr),
               province_admin(garr),
               p_loc(subloc(garr))->control.closed);

        out(pl, "%4s %5s %6s %4d %4d %4d %4d %5s  %s",
            box_code_less(garr),
            box_code_less(subloc(garr)),
            rp_loc(subloc(garr))->control.closed ? "closed" : "open",
            count_stack_figures(garr),
            unit_maint_cost(garr, 0),
            rp_misc(garr)->garr_tax,
            taxr,
            box_code_less(garrison_castle(garr)),
            garr_own_s(garr));

        tagout(pl, "</tag type=garrison>");
    }
    tagout(pl, "</tag type=garr_report pl=%d>", pl);

    out(pl, "");
    for (i = 0; i < ilist_len(l); i++) {
        /* Output inventories for any garrisons w/ inventory. */
        first = TRUE;
        garr = l[i];
        if (ie_list_len(bx[garr]->items) > 0) {
            qsort(bx[garr]->items, ie_list_len(bx[garr]->items),
                  sizeof(int), inv_item_comp);
            inventory_loop(garr, e)
                        {
                            if (first) {
                                tagout(pl, "<tag type=garr_inv id=%d>", garr);
                                out(pl, "%s in %s<tag type=tab col=48>weight",
                                    box_name(garr),
                                    box_name(province(garr)));
                                out(pl, "-----------------------------------<tag type=tab col=48>------");
                                first = FALSE;
                            };
                            tagout(pl, "<tag type=inventory unit=%d item=%d qty=%d weight=%d "
                                       "extra=\"%s\">",
                                   garr, e->item, e->qty, item_weight(e->item) * e->qty,
                                   extra_item_info(garr, e->item, e->qty));
                            tagout(pl, "%9s  %-30s <tag type=tab col=45>%9s  %s",
                                   comma_num(e->qty),
                                   plural_item_box(e->item, e->qty),
                                   comma_num(item_weight(e->item) * e->qty),
                                   extra_item_info(i, e->item, e->qty));
                            tagout(pl, "</tag type=inventory>");
                        }
            inventory_next;
        };
        if (!first) {
            out(pl, "");
            tagout(pl, "</tag type=garr_inv>");
        };
    };
}


int
v_decree_watch(struct command *c) {
    int garr;
    int target = c->a;
    struct entity_misc *p;
    int ncontrol = 0;
    int nordered = 0;

    if (kind(target) != T_char) {
        wout(c->who, "%s is not a character.", c->parse[1]);
        return FALSE;
    }

    loop_garrison(garr)
            {
                if (c->who != province_admin(garr)) {
                    continue;
                }

                ncontrol++;
                p = p_misc(garr);

                if (ilist_len(p->garr_watch) < 3) {
                    ilist_append(&p->garr_watch, target);
                    wout(garr, "%s orders us to watch for %s.",
                         box_name(c->who), box_code(target));

                    nordered++;
                }
            }
    next_garrison;

    if (ncontrol == 0) {
        wout(c->who, "We rule over no garrisons.");
        return FALSE;
    }

    if (nordered == 0) {
        wout(c->who, "Garrisons may only watch for up to three "
                     "units per month.");
        return FALSE;
    }

    wout(c->who, "Watch decree given to %s garrison%s.",
         nice_num(nordered), add_s(nordered));

    return TRUE;
}


int
v_decree_hostile(struct command *c) {
    int garr;
    int target = c->a;
    struct entity_misc *p;
    int ncontrol = 0;
    int nordered = 0;

    if (kind(target) != T_char) {
        wout(c->who, "%s is not a character.", c->parse[1]);
        return FALSE;
    }

    loop_garrison(garr)
            {
                if (c->who != province_admin(garr)) {
                    continue;
                }

                ncontrol++;
                p = p_misc(garr);

                if (ilist_len(p->garr_host) < 3) {
                    ilist_append(&p->garr_host, target);
                    wout(garr, "%s orders us to attack %s on sight.",
                         box_name(c->who), box_code(target));

                    nordered++;
                }
            }
    next_garrison;

    if (ncontrol == 0) {
        wout(c->who, "We rule over no garrisons.");
        return FALSE;
    }

    if (nordered == 0) {
        wout(c->who, "Garrisons may be hostile to at most "
                     "three units.");
        return FALSE;
    }

    wout(c->who, "Hostile decree given to %s garrison%s.",
         nice_num(nordered), add_s(nordered));

    return TRUE;
}


static char *decree_tags[] = {
        "watch",    /* 0 */
        "hostile",    /* 1 */
        NULL
};


int
v_decree(struct command *c) {
    int tag;

    if (numargs(c) < 1) {
        wout(c->who, "Must specify what to decree.");
        return FALSE;
    }

    tag = lookup(decree_tags, c->parse[1]);

    if (tag < 0) {
        wout(c->who, "Unknown decree '%s'.", c->parse[1]);
        return FALSE;
    }

    cmd_shift(c);

    switch (tag) {
        case 0:
            return v_decree_watch(c);
        case 1:
            return v_decree_hostile(c);

        default:
            assert(FALSE);
    }

    /* NOT REACHED */
    exit(2);
}

void
ping_garrisons() {
    int garr;
    int where;

    show_to_garrison = TRUE;

    loop_garrison(garr)
            {
                where = subloc(garr);

                assert(rp_char(garr) != NULL);

                rp_char(garr)->guard = FALSE;

                wout(where, "%s guards %s.",
                     liner_desc(garr), box_name(where));

                rp_char(garr)->guard = TRUE;
            }
    next_garrison;

    show_to_garrison = FALSE;
}


int
v_ungarrison(struct command *c) {
    int garr = c->a;
    int where = subloc(c->who);
    struct item_ent *e;
    int first = TRUE;

    if (garr == 0) {
        garr = garrison_here(where);

        if (garr == 0) {
            wout(c->who, "There is no garrison here.");
            return FALSE;
        }
    } else if (garrison_here(where) != garr) {
        wout(c->who, "No garrison %s is here.", c->parse[1]);
        return FALSE;
    }

    if (player(c->who) != player(province_admin(garr))) {
        wout(c->who, "%s does not rule over %s.",
             box_name(c->who), box_name(garr));
        return FALSE;
    }


    wout(c->who, "%s disbands.", box_name(garr));

    vector_clear();
    vector_add(garr);
    vector_add(where);
    wout(VECT, "%s is disbanded by %s.", box_name(garr), box_name(c->who));

    inventory_loop(garr, e)
                {
                    if (first) {
                        first = FALSE;
                        wout(c->who, "Received from %s:", box_name(garr));
                        indent += 3;
                    }

                    wout(c->who, "%s", box_name_qty(e->item, e->qty));

                    move_item(garr, c->who, e->item, e->qty);
                }
    inventory_next;

    if (!first) {
        indent -= 3;
    }

    p_misc(garr)->garr_castle = 0;        /* become silent */
    kill_char(garr, 0, S_nothing);

    return TRUE;
}

