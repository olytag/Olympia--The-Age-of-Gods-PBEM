// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "z.h"
#include "oly.h"
#include "forward.h"
#include "vectors/accept_ent_list.h"
#include "vectors/entity_player_list.h"
#include "vectors/item_ent_list.h"


/*
 *  Mon Oct 11 07:06:02 1999 -- Scott Turner
 *
 *  This produces a "<tag type=tab col=44>" if tags are on,
 *  a null string elsewise.
 *
 */
char *tab_to(int where) {
    static char tab_string[1024];
    if (options.output_tags) {
        sprintf(tab_string, "<tag type=tab col=%d>", where);
        return tab_string;
    };
    return "";
};

static int output_order_comp(const void *q1, const void *q2) {
    int *a = (int *)q1;
    int *b = (int *)q2;

    if (bx[*a]->output_order != bx[*b]->output_order) {
        return bx[*a]->output_order - bx[*b]->output_order;
    }

    return *a - *b;
}


void
sort_for_output(ilist l) {
    qsort(l, ilist_len(l), sizeof(int), output_order_comp);
}


void
determine_output_order() {
    int count = 0;
    int reg;
    int i;

    loop_loc(reg)
            {
                if (loc_depth(reg) != LOC_region) {
                    continue;
                }

                bx[reg]->output_order = count++;

                loop_all_here(reg, i)
                        {
                            bx[i]->output_order = count++;
                        }
                next_all_here;
            }
    next_loc;

/*
 *  Sort all player unit lists
 */

    {
        int pl;
        struct entity_player *p;

        loop_player(pl)
                {
                    p = rp_player(pl);
                    if (p == NULL) {
                        continue;
                    }

                    sort_for_output(p->units);
                }
        next_player;
    }
}


void
show_carry_capacity(int who, int num) {
    struct weights w;
    char *walk_percent = "";
    char buf[LEN];
    int mountains = FALSE;

    out(who, "");

    determine_unit_weights(num, &w, FALSE);

    if (w.land_cap > 0) {
        walk_percent = sout(" (%d%%)",
                            w.land_weight * 100 / w.land_cap);
    }
    sprintf(buf, "%s/%s land%s",
            comma_num(w.land_weight), comma_num(w.land_cap), walk_percent);

    if (w.ride_cap > 0) {
        strcat(buf, sout(", %s/%s ride (%d%%)",
                         comma_num(w.ride_weight), comma_num(w.ride_cap),
                         w.ride_weight * 100 / w.ride_cap));
    }

    if (w.fly_cap > 0) {
        strcat(buf, sout(", %s/%s fly (%d%%)",
                         comma_num(w.fly_weight), comma_num(w.fly_cap),
                         w.fly_weight * 100 / w.fly_cap));
    }

    tagout(who, "<tag type=carry_capacity unit=%d weight=%d "
                "walk_weight=%d walk_cap=%d "
                "ride_weight=%d ride_cap=%d "
                "fly_weight=%d fly_cap=%d>",
           who,
           w.total_weight,
           w.land_weight,
           w.land_cap,
           w.ride_weight,
           w.ride_cap,
           w.fly_weight,
           w.fly_cap);

    wiout(who, strlen("Capacity:  "), "Capacity:  %s", buf);
    tagout(who, "</tag type=capacity>");
}


static void
show_item_skills_sup(int who, int item, struct item_magic *p) {
    int i;
    char *req_s, *days;
    int sk;
    int parent;
    int first;
#if 0
    int see_magic;

    see_magic = is_magician(who);
#endif

    first = TRUE;
    for (i = 0; i < ilist_len(p->may_study); i++) {
        sk = p->may_study[i];
        assert(valid_box(sk));
        parent = skill_school(sk);

#if 0
        if (magic_skill(sk) && !see_magic)
            continue;
#endif

        if (sk == parent) {
            req_s = "";
        } else {
            req_s = sout(" (requires %s)",
                         just_name(parent));
        }

#if 0
        /*
         *  Output the days study remaining in the book.
         *
         */
        days = sout(", %d day%s instruction",
                p->orb_use_count,
                p->orb_use_count == 1 ? "" : "s");
#endif

        tagout(who, "<tag type=may_study id=%d skill=%d parent = %d "
                    "days=%d>",
               item,
               (sk != parent && has_skill(who, parent) < 1) ?
               0 : sk,
               parent,
               p->orb_use_count);

        if (first) {
            out(who, "");
            wout(who, "%s permits %d day%s study of the following skill%s:",
                 box_name(item),
                 p->orb_use_count,
                 p->orb_use_count == 1 ? " of " : "s",
                 ilist_len(p->may_study) == 1 ? "" : "s");
            indent += 3;
            first = FALSE;
        }

        if (sk != parent && has_skill(who, parent) < 1) {
            wiout(who, 3, "???%s", req_s);
        } else {
            wiout(who, 3, "%s%s", box_name(sk), req_s);
        }

        tagout(who, "</tag type=may_study>");
    }

    if (!first) {
        indent -= 3;
    }

    first = TRUE;
    for (i = 0; i < ilist_len(p->may_use); i++) {
        sk = p->may_use[i];
        assert(valid_box(sk));
        parent = skill_school(sk);

#if 0
        if (magic_skill(sk) && !see_magic)
            continue;
#endif

        if (first) {
            out(who, "");
            wout(who, "%s permits use of the following skills:",
                 box_name(item));
            indent += 3;
            first = FALSE;
        }

        if (sk == parent) {
            req_s = "";
        } else {
            req_s = sout(" (requires %s)",
                         just_name(parent));
        }

        tagout(who, "<tag type=may_use id=%d skill=%d parent = %d>",
               item,
               (sk != parent && has_skill(who, parent) < 1) ?
               0 : sk,
               parent);

        if (sk != parent && has_skill(who, parent) < 1) {
            wiout(who, 3, "???%s", req_s);
        } else {
            wiout(who, 3, "%s%s", box_name(sk), req_s);
        }

        tagout(who, "</tag type=may_use>");
    }

    if (!first) {
        indent -= 3;
    }
}


void
show_item_skills(int who, int num) {
    struct item_ent *e;
    struct item_magic *p;
    int first = 1;

    inventory_loop(num, e)
                {
                    p = rp_item_magic(e->item);

                    if (p) {
                        if (first) {
                            tagout(who, "<tag type=item_skill_section id=%d>", who);
                            first = 0;
                        };
                        show_item_skills_sup(who, e->item, p);
                    }
                }
    inventory_next;

    if (!first) {
        tagout(who, "</tag type=item_skill_section>");
    };
}


int inv_item_comp(const void *q1, const void *q2) {
    const struct item_ent **a = (const struct item_ent **)q1;
    const struct item_ent **b = (const struct item_ent **)q2;

    return (*a)->item - (*b)->item;
}


char *extra_item_info(int who, int item, int qty) {
    char buf[LEN];
    int lc, rc, fc;
    int at, df, mi;
    int n;

    *buf = '\0';

    lc = item_land_cap(item);
    rc = item_ride_cap(item);
    fc = item_fly_cap(item);

    if (fc > 0) {
        sprintf(buf, "fly %s", nice_num(fc * qty));
    } else if (rc > 0) {
        sprintf(buf, "ride %s", nice_num(rc * qty));
    } else if (lc > 0) {
        sprintf(buf, "cap %s", nice_num(lc * qty));
    }

    at = item_attack(item);
    df = item_defense(item);
    mi = item_missile(item);

    if (is_fighter(item)) {
        strcat(buf, sout(" (%d,%d,%d)", at, df, mi));
    }

    if (n = item_attack_bonus(item)) {
        strcat(buf, sout("+%d attack", n));
    }

    if (n = item_defense_bonus(item)) {
        strcat(buf, sout("+%d defense", n));
    }

    if (n = item_missile_bonus(item)) {
        strcat(buf, sout("+%d missile", n));
    }

    if (n = item_aura_bonus(item)) {
        if (who && is_magician(who)) {
            strcat(buf, sout("+%d aura", n));
        }
    }

    return sout("%s", buf);
}


void
show_char_inventory(int who, int num, char *prefix) {
    int first = TRUE;
    struct item_ent *e;
    int weight;
    int count = 0;
    int total_weight = 0;

    if (ie_list_len(bx[num]->items) > 0) {
        qsort(bx[num]->items, ie_list_len(bx[num]->items),
              sizeof(int), inv_item_comp);
    }

    if (!prefix[0]) {
        tagout(who, "<tag type=inventory_section unit=%d>", who);
    }

    inventory_loop(num, e)
                {
                    weight = item_weight(e->item) * e->qty;

                    if (first) {
                        if (!prefix[0]) { out(who, ""); }
                        out(who, "%sInventory:", prefix);
                        out(who, "%s%9s  %-30s %9s", prefix,
                            "qty", "name", "weight");
                        out(who, "%s%9s  %-30s %9s", prefix,
                            "---", "----", "------");
                        first = FALSE;
                    }

                    if (!prefix[0]) {
                        tagout(who, "<tag type=inventory unit=%d item=%d qty=%d weight=%d extra=\"%s\">",
                               who, e->item, e->qty, item_weight(e->item) * e->qty,
                               extra_item_info(who, e->item, e->qty));
                    }

                    out(who, "%s%9s  %-30s %s%9s  %s",
                        prefix,
                        comma_num(e->qty),
                        plural_item_box(e->item, e->qty),
                        tab_to(45),
                        comma_num(weight),
                        extra_item_info(who, e->item, e->qty));

                    if (!prefix[0]) { tagout(who, "</tag type=inventory>"); }

                    count++;
                    total_weight += weight;
                }
    inventory_next;

    if (count > 0) {
        out(who, "%s%9s  %-30s %9s", prefix,
            "", "", "======");
        out(who, "%s%9s  %-30s %9s", prefix,
            "", "", comma_num(total_weight));
    }

    if (first) {
        if (!prefix[0]) { out(who, ""); }
        out(who, "%s has no possessions.", box_name(num));
    }

    if (!prefix[0]) { tagout(who, "</tag type=inventory_section unit=%d>", who); }
}


/*
 *	1.  building		%s, in
 *
 *	2.  land subloc		%s, in province
 *	3.  ocean subloc	%s, in
 *
 *	4.  land province	%s, in region %s
 *	5.  ocean province	%s, in %s
 */

char *char_rep_location(int who) {
    int where = subloc(who);
    char *s = "";
    char *reg_name;

    if (where == 0) {
        return "nowhere";
    }

    while (where && loc_depth(where) > LOC_province) {
        if (*s) {
            s = sout("%s, in %s", s, box_name(where));
        } else {
            s = box_name(where);
        }
        where = loc(where);
    }

    if (!where) {
        s = sout("%s, adrift in the Cosmos", s);
        return s;
    };

    if (subkind(province(where)) == sub_ocean) {
        if (*s) {
            s = sout("%s, in %s", s, box_name(where));
        } else {
            s = box_name(where);
        }

        reg_name = name(region(where));

        if (reg_name && *reg_name) {
            s = sout("%s, in %s", s, reg_name);
        }
    } else {
        if (*s) {
            s = sout("%s, in province %s", s, box_name(where));
        } else {
            s = box_name(where);
        }

        reg_name = name(region(where));

#if 0
        if (reg_name && *reg_name)
            s = sout("%s, in region %s", s, reg_name);
#else
        if (reg_name && *reg_name) {
            s = sout("%s, in %s", s, reg_name);
        }
#endif
    }

    return s;
}


static void
char_rep_stack_info(int who, int num) {
    int n;
    int first = TRUE;

    if (n = stack_parent(num)) {
        tagout(who, "<tag type=stack id=%d under=%d>", num, n);
        wiout(who, 16, "Stacked under:  %s", box_name(n));
    };

    loop_here(num, n)
            {
                if (kind(n) == T_char && !is_prisoner(n)) {
                    if (first) {
                        out(who, "Stacked over:   %s", box_name(n));
                        first = FALSE;
                    } else {
                        out(who, "                %s", box_name(n));
                    }
                    tagout(who, "<tag type=stack id=%d over=%d>", num, n);
                }
            }
    next_here;
}


#if 0
static int pledge_backlinks = FALSE;

static void
collect_pledge_backlinks()
{
    int i;
    int n;
    struct char_magic *p;

    pledge_backlinks = TRUE;

    loop_char(i)
    {
        if (n = char_pledge(i))
        {
            p = p_magic(n);
            ilist_append(&p->pledged_to_us, i);
        }
    }
    next_char;
}


static void
show_pledged(int who, int num)
{
    int i, n;
    int first = TRUE;
    struct char_magic *p;

    if (!pledge_backlinks)
        collect_pledge_backlinks();

    if (n = char_pledge(num))
        wiout(who, 16,  "Pledged to:     %s", box_name(n));

    p = rp_magic(num);

    if (p)
    {
        for (i = 0; i < ilist_len(p->pledged_to_us); i++)
        {
            n = p->pledged_to_us[i];

            if (first)
            {
                out(who, "Pledged to us:  %s", box_name(n));
                first = FALSE;
            }
            else
                out(who, "                %s", box_name(n));
        }
    }
}
#endif

static char *
prisoner_health(int who) {
    int health = char_health(who);

    assert(health != 0);

    if (health < 0) {
        return "";
    }

    return sout(", health %d", health);
}


static void
char_rep_prisoners(int who, int num) {
    int n;
    int first = TRUE;

    loop_here(num, n)
            {
                if (kind(n) == T_char && is_prisoner(n)) {
                    if (first) {
                        out(who, "Prisoners:      %s%s%s%s",
                            box_name(n),
                            nation_s(n),
                            deserted_s(n),
                            prisoner_health(n));
                        first = FALSE;
                    } else {
                        out(who, "                %s%s%s%s",
                            box_name(n),
                            nation_s(n),
                            deserted_s(n),
                            prisoner_health(n));
                    }
                    tagout(who, "<tag type=prisoner id=%d prisoner=%d "
                                "nation=\"%s\" deserted=\"%s\" health=%d> ",
                           num,
                           n,
                           nation_s(n),
                           deserted_s(n),
                           char_health(n));

                }
            }
    next_here;
}


void
char_rep_health(int who, int num, char *prefix) {
    int n;
    char *s = "";

    n = char_health(num);

    if (n == -1) {
        out(who, "%sHealth:         n/a", prefix);
        return;
    }

    if (n > 0 && n < 100) {
        if (char_sick(num)) {
            s = " (getting worse)";
        } else {
            s = " (getting better)";
        }
    }

    if (!prefix[0]) {
        tagout(who, "<tag type=health id=%d health=%d sick=%d>",
               num, n, char_sick(num));
    }
    out(who, "%sHealth:         %d%%%s", prefix, n, s);
}

static void
char_rep_combat(int who, int num) {
    int n;
    char *s;
    int mk, att, def, mis;

    mk = noble_item(num);
    if (mk == 0) {
        att = char_attack(num);
        def = char_defense(num);
        mis = char_missile(num);
    } else {
        att = item_attack(mk);
        def = item_defense(mk);
        mis = item_missile(mk);
    }

    n = char_behind(num);

    if (n == 0) {
        s = " (front line in combat)";
    } else {
        s = " (stay behind in combat)";
    }


    if (char_break(num) == 0) {
        s = " (fight to the death)";
    } else if (char_break(num) == 100) {
        s = " (break almost immediately)";
    } else {
        s = "";
    }

    tagout(who, "<tag type=combat id=%d attack=%d defense=%d "
                "missile=%d behind=%d break=%d personal_break=%d>",
           num, att, def, mis, n, char_break(num), personal_break(num));
    out(who, "Combat:         attack %d, defense %d, missile %d",
        att, def, mis);
    out(who, "                behind %d %s", n, s);
    out(who, "Break point:    %d%%%s", char_break(num), s);

    if (has_skill(num, sk_personal_fttd)) {
        out(who, "Personal break point:	When health reaches %d.",
            personal_break(num));
    };


}


static void
char_rep_misc(int who, int num) {
    char *s;
    struct char_magic *p;

    p = rp_magic(num);
    if (p && p->ability_shroud) {
        out(who, "Ability shroud: %d aura", p->ability_shroud);
    }

    if (has_skill(num, sk_hide_self)) {
        if (char_hidden(num)) {
            if (char_alone_stealth(num)) {
                s = "concealing self";
            } else {
                s = "concealing self, but not alone";
            }
        } else {
            s = "not concealing self";
        }

        out(who, "use %4d %d      (%s)",
            sk_hide_self, char_hidden(num), s);
    }
    /*
     *  Smuggling.
     *
     */
    if (has_skill(num, sk_smuggle_goods) ||
        has_skill(num, sk_smuggle_men)) {
        s = " nothing";
        if (get_effect(num, ef_smuggle_goods, 0, 0)) {
            if (get_effect(num, ef_smuggle_men, 0, 0)) {
                s = " men and goods";
            } else {
                s = " goods";
            };
        } else if (get_effect(num, ef_smuggle_men, 0, 0)) {
            s = " men";
        };
        out(who, "Smuggling:      %s.", s);
    };
}


void
char_rep_magic(int who, int num, char *prefix) {
    int ca, ma, mea;

    ca = char_cur_aura(num);
    ma = char_max_aura(num);
    mea = max_eff_aura(num);

    if (!prefix[0]) { out(who, ""); }
    out(who, "%sCurrent aura:   %d", prefix, ca);

    if (ma < mea) {
        out(who, "%sMaximum aura:   %d (%d+%d)",
            prefix, mea, ma, mea - ma);
    } else {
        out(who, "%sMaximum aura:   %d", prefix, ma);
    }

    if (char_abil_shroud(num) > 0) {
        out(who, "%sAbility shroud: %d aura",
            prefix, char_abil_shroud(num));
    }

    if (is_loc_or_ship(char_proj_cast(num))) {
        out(who, "%sProject cast:   %s",
            prefix,
            box_name(char_proj_cast(num)));
    }

    if (char_quick_cast(num)) {
        out(who, "%sQuicken cast:   %d",
            prefix,
            char_quick_cast(num));
    }

    if (!prefix[0]) {
        tagout(who, "<tag type=magic id=%d cur_aura=%d max_aura=%d "
                    "max_eff_aura=%d abil_shroud=%d project=%d quick=%d>",
               num, ca, ma, mea, char_abil_shroud(num),
               char_proj_cast(num), char_quick_cast(num));
    }

}

static void
char_rep_religion(int who, int num) {
    int i;

    if (is_priest(num)) {
        out(who, "Current piety:  %d", rp_char(num)->religion.piety);
        if (ilist_len(rp_char(num)->religion.followers) > 0) {
            out(who, "Followers:  %s", box_name(rp_char(num)->religion.followers[0]));
            for (i = 1; i < ilist_len(rp_char(num)->religion.followers); i++) {
                out(who, "            %s", box_name(rp_char(num)->religion.followers[i]));
            }
        };
    } else if (is_follower(num)) {
        out(who, "Dedicated to:   %s (%s)",
            box_name(is_priest(is_follower(num))),
            box_name(is_follower(num)));
    };
    tagout(who, "<tag type=religion id=%d followers=%d priest=%d>",
           num, is_priest(num) ? ilist_len(rp_char(num)->religion.followers) : 0,
           is_follower(num));
}


static void
list_accepts(int who, int num);

void
char_rep_sup(int who, int num) {
    tagout(who, "<tag type=char_report id=%d "
                "location=%d loc_string=\"%s\" loyalty=%s loy_type=%d "
                "loy_num=%d guild=%d>", who, subloc(num),
           char_rep_location(num), cap(loyal_s(num)),
           loyal_kind(num), loyal_rate(num), guild_member(who));

    wiout(who, 16, "Location:       %s", char_rep_location(num));
    out(who, "Loyalty:        %s", cap(loyal_s(num)));
    if (guild_member(who)) {
        out(who, "Guild:          %s", box_name(guild_member(who)));
    }

    char_rep_stack_info(who, num);
    char_rep_health(who, num, "");
    char_rep_combat(who, num);
    char_rep_misc(who, num);

    if (banner(num)) {
        tagout(who, "<tag type=banner id=%d value=\"%s\">", num, banner(num));
        out(who, "Banner:         %s", banner(num));
    };
#if 0
    show_pledged(who, num);
#endif

    char_rep_religion(who, num);

    if (is_magician(num)) {
        char_rep_magic(who, num, "");
    }
    char_rep_prisoners(who, num);

    print_att(who, num);
    list_skills(who, num, "");
    list_partial_skills(who, num, "");
    list_accepts(who, num);
    show_char_inventory(who, num, "");
    show_carry_capacity(who, num);
    show_item_skills(who, num);
    list_pending_trades(who, num);

    out(who, "");
    tagout(who, "</tag type=char_report id=%d>", who);
}


void
character_report() {
    int who;

    stage("character_report()");

    indent += 3;

    loop_char(who)
            {
                if (subkind(player(who)) == sub_pl_silent) {
                    continue;
                }

                tagout(who, "</tag type=unit_report>");

                if (is_prisoner(who)) {
                    p_char(who)->prisoner = FALSE;    /* turn output on */
                    tagout(who, "<tag type=char_report id=%d>", who);
                    out(who, "%s is being held prisoner.", box_name(who));
                    tagout(who, "<tag type=header>");
                    out(who, "");
                    tagout(who, "</tag type=header>");
                    tagout(who, "</tag type=char_report id=%d>", who);
                    p_char(who)->prisoner = TRUE;    /* output off again */
                } else {
                    out(who, "");
                    out(who, "%s", box_name(who));
                    out(who, "");
                    char_rep_sup(who, who);
                }
            }
    next_char;

    indent -= 3;
}


void
show_unclaimed(int who, int num) {
    int first = TRUE;
    struct item_ent *e;
    int weight;

    if (ie_list_len(bx[num]->items) > 0) {
        qsort(bx[num]->items, ie_list_len(bx[num]->items),
              sizeof(int), inv_item_comp);
    }

    inventory_loop(num, e)
                {
                    weight = item_weight(e->item) * e->qty;

                    if (first) {
                        out(who, "");
                        out(who, "Unclaimed items:");
                        out(who, "");
                        out(who, "%9s  %-30s %9s", "qty", "name", "weight");
                        out(who, "%9s  %-30s %9s", "---", "----", "------");
                        first = FALSE;
                    }

                    out(who, "%9s  %-30s %s%9s  %s",
                        comma_num(e->qty),
                        plural_item_box(e->item, e->qty),
                        tab_to(42),
                        comma_num(weight),
                        extra_item_info(0, e->item, e->qty));
                }
    inventory_next;

    if (rp_player(who)->first_tower == FALSE) {
        if (first) {
            out(who, "");
            out(who, "Unclaimed items:");
            out(who, "");
        };
        out(who, "");
        out(who, "   You have not yet built your 'free' tower.");
    };
}


void
player_ent_info() {
    int pl;

    loop_player(pl)
            {
                if (subkind(pl) == sub_pl_silent) {
                    continue;
                }

                tagout(pl, "</tag type=unit_report>");
                print_admit(pl);
                print_att(pl, pl);
                list_accepts(pl, pl);
                show_unclaimed(pl, pl);
            }
    next_player
}


static int
sum_fighters(int who) {
    int sum = 0;
    struct item_ent *t;

    inventory_loop(who, t)
                {
                    if (man_item(t->item) && is_fighter(t->item)) {
                        int val;

                        val = max(item_attack(t->item), max(item_defense(t->item), item_missile(t->item)));

                        if (val > 1) {
                            sum += t->qty;
                        }
                    }
                }
    inventory_next;

    return sum;
}


#define    TRUNC_NAME    15


static char *
        stupid_words[] = {
                "a",
                "the",
                "of",
                "de",
                "des",
                "la",
                "and",
                "du",
                "aux",
                "et",
                "ses",
                "avec",
                "un",
                "van",
                "von",
                "-",
                "--",
                NULL
        };


static char *
strip_leading_stupid_word(char *s) {
    char *t;
    int i;
    int len;

    for (i = 0; stupid_words[i]; i++) {
        len = strlen(stupid_words[i]);

        if (i_strncmp(s, stupid_words[i], len) == 0 &&
            s[len] == ' ') {
            t = &s[len];
            while (*t == ' ') {
                t++;
            }
            if (*t) {
                return t;
            } else {
                return s;
            }
        }
    }

    return s;
}


int
stupid_word(char *s) {

    return lookup(stupid_words, s) >= 0;
}


static char *
prev_word(char *s, char *t) {

    while (t > s && *t != ' ') {
        t--;
    }

    if (t > s) {
        return t;
    }
    return NULL;
}


static char *
summary_trunc_name(int who) {
    char *s, *t;

    s = sout("%s", just_name(who));

    if (strlen(s) <= TRUNC_NAME) {
        return s;
    }

    s = strip_leading_stupid_word(s);

    if (strlen(s) <= TRUNC_NAME) {
        return s;
    }

    t = prev_word(s, &s[TRUNC_NAME]);
    if (t) {
        *t = '\0';
    }

    while ((t = prev_word(s, t)) && *t == ' ' && stupid_word(t + 1)) {
        *t = '\0';
    }

    s[TRUNC_NAME] = '\0';        /* catches a case */

    return s;
}


static int sum_gold;
static int sum_peas;
static int sum_work;
static int sum_sail;
static int sum_fight;

static char *loyal_chars = "ucofns";


static void
unit_summary_sup(int pl, int who) {
    char *nam;
    char *health_s;
    char *under_s;
    char *loy_s;
    char *cur_aura_s;
    int pr = is_prisoner(who);
    int gold, peas, work, sail, fight;
    char buf[LEN];
    int n;

    nam = summary_trunc_name(who);

    n = char_health(who);

    if (n == 100) {
        health_s = "100 ";
    } else if (n == -1) {
        health_s = "n/a ";
    } else if (char_sick(who)) {
        health_s = sout("%d-", char_health(who));
    } else {
        health_s = sout("%d+", char_health(who));
    }

    if (pr) {
        under_s = " ?? ";
    } else if (stack_parent(who)) {
        under_s = box_code_less(stack_parent(who));
    } else {
        under_s = "";
    }

    if (is_magician(who)) {
        cur_aura_s = sout("%d", char_cur_aura(who));
    } else if (is_priest(who)) {
        cur_aura_s = sout("%d", rp_char(who)->religion.piety);
    } else {
        cur_aura_s = "";
    }

    gold = has_item(who, item_gold);
    peas = has_item(who, item_peasant);
    work = has_item(who, item_worker);
    sail = has_item(who, item_sailor);
    fight = sum_fighters(who);

    loy_s = sout("%c%s", loyal_chars[loyal_kind(who)],
                 knum(loyal_rate(who), FALSE));

    if (options.output_tags > 0) {
        sprintf(buf,
                "<tag type=unit_summary unit=%d loc=%d loyal=%s health=%s "
                "behind=%d aura=%d gold=%d peas=%d work=%d sail=%d fight=%d "
                "under=%d name=\"%s\">",
                who,
                pr ? -1 : subloc(who),
                loy_s, health_s,
                char_behind(who), char_cur_aura(who),
                has_item(who, item_gold),
                has_item(who, item_peasant),
                has_item(who, item_worker),
                has_item(who, item_sailor),
                fight,
                pr ? -1 : stack_parent(who),
                nam);
        tagout(pl, buf);
    };

    sprintf(buf, "%-*s %-*s %-5s %4s%2d%5s %4s %4s %4s %4s %4s %-*s %s",
            CHAR_FIELD, box_code_less(who),
            CHAR_FIELD, pr ? " ?? " : box_code_less(subloc(who)),
            loy_s,
            health_s,
            char_behind(who),
            cur_aura_s,
            knum(gold, TRUE),
            knum(peas, TRUE),
            knum(work, TRUE),
            knum(sail, TRUE),
            knum(fight, TRUE),
            CHAR_FIELD, under_s,
            nam);

    out(pl, "%s", buf);

    tagout(pl, "</tag type=unit_summary>");

    sum_gold += gold;
    sum_peas += peas;
    sum_work += work;
    sum_sail += sail;
    sum_fight += fight;
}


void
unit_summary(int pl) {
    int i;
    int count = 0;

    clear_temps(T_char);

    sum_gold = 0;
    sum_peas = 0;
    sum_work = 0;
    sum_sail = 0;
    sum_fight = 0;


    count = ilist_len(p_player(pl)->units);

    if (count <= 0) {
        return;
    }

    tagout(pl, "<tag type=unit_section pl=%d>", pl);
    out(pl, "");
    out(pl, "Unit Summary:");
    out(pl, "");
    out(pl, "%-*s %-*s loyal heal B  CA  gold peas work sail figh %-*s name",
        CHAR_FIELD, "unit",
        CHAR_FIELD, "where",
        CHAR_FIELD, "under");
    out(pl, "%-*s %-*s ----- ---- - ---- ---- ---- ---- ---- ---- %-*s ----",
        CHAR_FIELD, "----",
        CHAR_FIELD, "-----",
        CHAR_FIELD, "-----");

    loop_units(pl, i)
            {
                unit_summary_sup(pl, i);
            }
    next_unit;

    if (count > 1) {
        out(pl, "%*s %-*s                   ==== ==== ==== ==== ====",
            CHAR_FIELD, "",
            CHAR_FIELD, "");
        out(pl, "%*s %-*s                   %4s %4s %4s %4s %4s",
            CHAR_FIELD, "",
            CHAR_FIELD, "",
            knum(sum_gold, TRUE),
            knum(sum_peas, TRUE),
            knum(sum_work, TRUE),
            knum(sum_sail, TRUE),
            knum(sum_fight, TRUE));
    }
    tagout(pl, "</tag type=unit_section>");
}


static char *
loc_ind_s(int where) {
    int ld;

    ld = loc_depth(where) - 1;

    if (ld <= 0) {
        return just_name(where);
    }

    return sout("%s%s", &spaces[spaces_len - (ld * 2)], box_name(where));
}


static void
loc_stack_catchup(int pl, int where) {

    if (where == 0 || bx[where]->temp) {
        return;
    }

    loc_stack_catchup(pl, loc(where));
    out(pl, "%s", loc_ind_s(where));
    bx[where]->temp = -1;
}


static int loc_stack_explain;


static void
loc_stack_rep_sup(int pl, int where, int who) {
    char *where_s = "";
    char *star = "";
    char *ind = "";

    if (where) {
        loc_stack_catchup(pl, loc(where));
        where_s = loc_ind_s(where);
    }

    if (player(who) != pl) {
        star = " *";
        loc_stack_explain = TRUE;
    }

    if (kind(loc(who)) == T_char) {
        ind = "  ";
    }

    out(pl, "%-34s %s%s%s%s", where_s, tab_to(35),
        ind, box_name(who), star);
    if (where) { bx[loc(where)]->temp = 0; }
}


void
loc_stack_report(int pl) {
    int i, j;
    static ilist l = NULL;
    static ilist locs = NULL;

    ilist_clear(&l);
    ilist_clear(&locs);

    clear_temps(T_loc);
    clear_temps(T_ship);
    clear_temps(T_char);

    loc_stack_explain = FALSE;

    tagout(pl, "<tag type=loc_summary pl=%d>", pl);

    loop_units(pl, i)
            {
                if (is_prisoner(i)) {
                    continue;
                }

                ilist_append(&l, i);
            }
    next_unit;

    if (ilist_len(l) < 1) {
        return;
    }

    tagout(pl, "<tag type=header>");
    out(pl, "");
    out(pl, "Stack Locations:");
    out(pl, "");
    out(pl, "%-34s %s", "Location", "Stack");
    out(pl, "%-34s %s", "--------", "-----");
    tagout(pl, "</tag type=header>");

    sort_for_output(l);

    for (i = ilist_len(l) - 1; i >= 0; i--) {
        int where = subloc(l[i]);

        if (bx[where]->temp == 0) {
            ilist_append(&locs, where);
        }

        bx[l[i]]->temp = bx[where]->temp;
        bx[where]->temp = l[i];
    }

    sort_for_output(locs);

    for (i = 0; i < ilist_len(locs); i++) {
        j = bx[locs[i]]->temp;
        assert(valid_box(j));

        loc_stack_rep_sup(pl, locs[i], j);

        while (j = bx[j]->temp) {
            assert(valid_box(j));
            loc_stack_rep_sup(pl, 0, j);
        }
    }

    if (loc_stack_explain) {
        out(pl, "");
        out(pl, "%-34s    %s%s", "",
            tab_to(38),
            "* -- unit belongs to another faction");
    }

    tagout(pl, "</tag type=loc_summary>", pl);
    ilist_reclaim(&l);
    ilist_reclaim(&locs);
}


void
player_report_sup(int pl) {
    struct entity_player *p;

    if (subkind(pl) == sub_pl_system) {
        return;
    }

    p = p_player(pl);

    tagout(pl, "<tag type=player_summary pl=%d np=%d sp=%d>",
           pl, p->noble_points, p->jump_start);
    out(pl, "Noble points:  %d     (%d gained, %d spent)",
        p->noble_points,
        p->np_gained,
        p->np_spent);
    out(pl, "Study points:  %d",
        p->jump_start);

    print_hiring_status(pl);
    print_unformed(pl);

    tagout(pl, "</tag type=player_summary>");
}


void
stack_capacity_report(int pl) {
    struct weights w;
    int who;
    char *walk_s = "";
    char *ride_s = "";
    char *fly_s = "";
    char *s;
    int first = TRUE;
    int n;

    loop_units(pl, who)
            {
                if (first) {
                    tagout(pl, "<tag type=capacity_summary pl=%d>", pl);
                    tagout(pl, "<tag type=header >", pl);
                    out(pl, "");
                    out(pl, "Stack Capacities:");
                    out(pl, "  - First number is additional weight "
                            "you can walk, ride or fly with.");
                    out(pl, "  - If you're overloaded, it shows the "
                            "excess in parentheses.");
                    out(pl, "  - Second number is the percentage of "
                            "your total capacity used.");
                    out(pl, "");
                    out(pl, "%*s  %10s %15s %15s %15s",
                        CHAR_FIELD, "stack",
                        "total wt",
                        "   walk   ",
                        "   ride   ",
                        "   fly    ");
                    out(pl, "%*s  %10s %15s %15s %15s",
                        CHAR_FIELD, "-----",
                        "--------",
                        "-----------",
                        "-----------",
                        "-----------");
                    tagout(pl, "</tag type=header >", pl);
                    first = FALSE;
                }

                determine_stack_weights(who, &w, FALSE);

                if (w.land_cap > 0) {
                    n = w.land_weight * 100 / w.land_cap;

                    if (n > 999) {
                        s = " -- ";
                    } else {
                        s = sout("%3d%%", n);
                    }

                    if (w.land_weight > w.land_cap) {
                        walk_s = sout("(%s) %s",
                                      comma_num(w.land_weight - w.land_cap),
                                      s);
                    } else {
                        walk_s = sout("%s %s",
                                      comma_num(w.land_cap - w.land_weight),
                                      s);
                    }
                } else {
                    walk_s = "";
                }

                if (w.ride_cap > 0) {
                    n = w.ride_weight * 100 / w.ride_cap;

                    if (n > 999) {
                        s = " -- ";
                    } else {
                        s = sout("%3d%%", n);
                    }

                    if (w.ride_weight > w.ride_cap) {
                        ride_s = sout("(%s) %s",
                                      comma_num(w.ride_weight - w.ride_cap),
                                      s);
                    } else {
                        ride_s = sout("%s %s",
                                      comma_num(w.ride_cap - w.ride_weight),
                                      s);
                    }
                } else {
                    ride_s = "";
                }

                if (w.fly_cap > 0) {
                    n = w.fly_weight * 100 / w.fly_cap;

                    if (n > 999) {
                        s = " -- ";
                    } else {
                        s = sout("%3d%%", n);
                    }

                    if (w.fly_weight > w.fly_cap) {
                        fly_s = sout("(%s) %s",
                                     comma_num(w.fly_weight - w.fly_cap),
                                     s);
                    } else {
                        fly_s = sout("%s %s",
                                     comma_num(w.fly_cap - w.fly_weight),
                                     s);
                    }
                } else {
                    fly_s = "";
                }

                tagout(pl, "<tag type=capacity_unit unit=%d weight=%d "
                           "walk_weight=%d walk_cap=%d "
                           "ride_weight=%d ride_cap=%d "
                           "fly_weight=%d fly_cap=%d>",
                       who,
                       w.total_weight,
                       w.land_weight,
                       w.land_cap,
                       w.ride_weight,
                       w.ride_cap,
                       w.fly_weight,
                       w.fly_cap);

                out(pl, "%*s  %10s %15s %15s %15s",
                    CHAR_FIELD, box_code_less(who),
                    comma_num(w.total_weight),
                    walk_s,
                    ride_s,
                    fly_s);

            }
    next_unit;

    if (!first) {
        tagout(pl, "</tag type=capacity_summary>", pl);
    }
}


void
player_report() {
    int pl;

    stage("player_report()");

    out_path = MASTER;
    out_alt_who = OUT_BANNER;

    loop_player(pl)
            {
                if (subkind(pl) == sub_pl_silent) {
                    continue;
                }

                player_report_sup(pl);
                unit_summary(pl);
                loc_stack_report(pl);
                stack_capacity_report(pl);
                storm_report(pl);
                ship_summary(pl);
                garrison_summary(pl);
                out(pl, "");
            }
    next_player;

    out_path = 0;
    out_alt_who = 0;
}


static void
rep_player(int pl) {
    char *s;

    tags_off();
    s = box_name(pl);
    tags_on();

    tagout(pl, "<tag type=unit_report id=%d name=\"%s\">", pl,
           box_name(pl));
    tagout(pl, "<tag type=header>");
    lines(pl, s);
    tagout(pl, "</tag type=header>");

    out(pl, "#include %d", pl);
    /*
     *  Don't do this; it has been done in player_ent_report.
     *
    tagout(pl,"</tag type=unit_report>");
    */
    tagout(pl, "<tag type=header>");
    out(pl, "");
    tagout(pl, "</tag type=header>");
}


static void
rep_char(int pl, ilist l) {
    int i;
    char *s, *t;

    sort_for_output(l);

    for (i = 0; i < ilist_len(l); i++) {
        if (subkind(l[i]) == sub_dead_body ||
            subkind(l[i]) == sub_lost_soul) {
            tags_off();
            s = sout("%s~%s",
                     p_misc(l[i])->save_name, box_code(l[i]));
            tags_on();
            t = sout("%s~%s",
                     p_misc(l[i])->save_name, box_code(l[i]));
        } else {
            tags_off();
            s = box_name(l[i]);
            tags_on();
            t = box_name(l[i]);
        };

        tagout(pl, "<tag type=unit_report id=%d name=\"%s\">", l[i], t);
        tagout(pl, "<tag type=header>");
        out(pl, "");
        lines(pl, s);
        tagout(pl, "</tag type=header>");
        out(pl, "#include %d", l[i]);
    }
}


static void
rep_loc(int pl, ilist l) {
    int i;
    char *s;

    sort_for_output(l);

    for (i = 0; i < ilist_len(l); i++) {
        tagout(pl, "<tag type=loc_report id=%d name=\"%s\">", l[i],
               show_loc_header(l[i]));
        tagout(pl, "<tag type=header>");
        tags_off();
        lines(pl, show_loc_header(l[i]));
        tags_on();
        tagout(pl, "</tag type=header>");
        out(pl, "#include %d", l[i]);
        tagout(pl, "<tag type=header>");
        out(pl, "");
        tagout(pl, "</tag type=header>");
    }
}


static void
inc(int pl, int code, char *s) {

    tagout(pl, "<tag type=unit_report id=%d name=\"%s\">", code, s);
    tagout(pl, "<tag type=header>");
    lines(pl, s);
    tagout(pl, "</tag type=header>");
    out(pl, "#include %d", code);
    tagout(pl, "</tag type=unit_report>");
    /*VLN this might be the spot to "#include 2" in the GM report */
    tagout(pl, "<tag type=header>");
    out(pl, "");
    tagout(pl, "</tag type=header>");
}


void
gen_include_sup(int pl) {
    static ilist char_l = NULL;
    static ilist loc_l = NULL;
    int n;

    int player_output = FALSE;
    int lore_flag = FALSE;
    int new_flag = FALSE;
    int loc_flag = FALSE;
    int code_flag = FALSE;
    int special_flag = FALSE;
    int death_flag = FALSE;
    int misc_flag = FALSE;
    int eat_queue = FALSE;
    int eat_warn = FALSE;
    int eat_error = FALSE;
    int eat_headers = FALSE;
    int eat_okay = FALSE;
    int eat_players = FALSE;
    int template_flag = FALSE;
    int garr_flag = FALSE;
    int drop_flag = FALSE;
    int show_post = FALSE;

    ilist_clear(&char_l);
    ilist_clear(&loc_l);

    known_sparse_loop(p_player(pl)->output, n)
            {
                switch (n) {
                    case OUT_BANNER:
                    case OUT_INCLUDE:
                        continue;

                    case OUT_LORE:
                        lore_flag = TRUE;
                        continue;
                    case OUT_NEW:
                        new_flag = TRUE;
                        continue;
                    case OUT_LOC:
                        loc_flag = TRUE;
                        continue;
                    case OUT_TEMPLATE:
                        template_flag = TRUE;
                        continue;
                    case OUT_GARR:
                        garr_flag = TRUE;
                        continue;
                    case OUT_SHOW_POSTS:
                        show_post = TRUE;
                        continue;
                    case LOG_CODE:
                        code_flag = TRUE;
                        continue;
                    case LOG_SPECIAL:
                        special_flag = TRUE;
                        continue;
                    case LOG_DROP:
                        drop_flag = TRUE;
                        continue;
                    case LOG_DEATH:
                        death_flag = TRUE;
                        continue;
                    case LOG_MISC:
                        misc_flag = TRUE;
                        continue;
                    case EAT_ERR:
                        eat_error = TRUE;
                        continue;
                    case EAT_WARN:
                        eat_warn = TRUE;
                        continue;
                    case EAT_QUEUE:
                        eat_queue = TRUE;
                        continue;
                    case EAT_HEADERS:
                        eat_headers = TRUE;
                        continue;
                    case EAT_OKAY:
                        eat_okay = TRUE;
                        continue;
                    case EAT_PLAYERS:
                        eat_players = TRUE;
                        continue;
                }

                if (!valid_box(n)) {    /* doesn't exist anymore */
                    continue;
                }

                switch (kind(n)) {
                    case T_char:
                    case T_deadchar:
                        ilist_append(&char_l, n);
                        break;

                    case T_loc:
                    case T_ship:
                        ilist_append(&loc_l, n);
                        break;

                    case T_player:
                        assert(n == pl);
                        player_output = TRUE;
                        break;

                    case T_item:
                        if (subkind(n) == sub_dead_body) {
                            ilist_append(&char_l, n);
                        } else assert(FALSE);
                        break;

                    default:
                        assert(FALSE);
                }
            }
    known_sparse_next;

    out(pl, "#include %d", OUT_BANNER);
    out(pl, "");

    if (eat_okay) {
        out(pl, "#include %d", EAT_OKAY);
        out(pl, "");
    }

    if (drop_flag) { inc(pl, LOG_DROP, "Player drops"); }
    if (code_flag) { inc(pl, LOG_CODE, "Code alerts"); }
    if (special_flag) { inc(pl, LOG_SPECIAL, "Special events"); }
    if (misc_flag) { inc(pl, LOG_MISC, "Miscellaneous"); }
    if (death_flag) { inc(pl, LOG_DEATH, "Character deaths"); }
    if (eat_error) { inc(pl, EAT_ERR, "Errors"); }
    if (eat_warn) { inc(pl, EAT_WARN, "Warnings"); }
    if (show_post) { inc(pl, OUT_SHOW_POSTS, "Press and rumors"); }
    if (eat_queue) { inc(pl, EAT_QUEUE, "Current order queues"); }

    if (pl != eat_pl && player_output) {
        rep_player(pl);
    }

    if (garr_flag) {
        inc(pl, OUT_GARR, "Garrison log");
    }

    if (loc_flag) {
        out(pl, "#include %d", OUT_LOC);
    }

    rep_char(pl, char_l);
    rep_loc(pl, loc_l);

    /* if (lore_flag)	inc(pl, OUT_LORE, ""); */

    if (new_flag) {
        inc(pl, OUT_NEW, "New players");
    }

    if (template_flag) {
        inc(pl, OUT_TEMPLATE, "Order template");
    }

    if (eat_players) {
        inc(pl, EAT_PLAYERS, "Current player list");
    }

    if (eat_headers) {
        inc(pl, EAT_HEADERS, "Original message");
    }

    ilist_reclaim(&char_l);
    ilist_reclaim(&loc_l);

}


void
gen_include_section() {
    int pl;

    out_path = MASTER;
    out_alt_who = OUT_INCLUDE;

    loop_player(pl)
            {
                if (subkind(pl) != sub_pl_silent) {
                    gen_include_sup(pl);
                }
                /*
                 *  Any tags needed to finish everything up.
                 *
                 */
                tagout(pl, "</tag type=turn>");
            }
    next_player;

    out_path = 0;
    out_alt_who = 0;
}


void
turn_end_loc_reports() {
    int pl;
    int i;
    struct entity_player *p;
    extern int show_loc_no_header;    /* argument to show_loc() */
    int separate;

    stage("turn_end_loc_reports()");

    out_path = MASTER;
    show_loc_no_header = TRUE;

    loop_player(pl)
            {
                if (subkind(pl) == sub_pl_silent) {
                    continue;
                }

                if (player_format(pl) & ALT) {
                    separate = TRUE;
                } else {
                    separate = FALSE;
                }

                p = p_player(pl);

                known_sparse_loop(p->locs, i)
                        {
                            if (!valid_box(i)) {    /* loc doesn't exit anymore */
                                continue;
                            }    /* ex: mine has collapsed */

                            if (separate) {
                                out_alt_who = OUT_LOC;

                                tags_off();
                                lines(pl, show_loc_header(i));
                                tags_on();
                                show_loc(pl, i);
                                out(pl, "");
                            } else {
                                out_alt_who = i;
                                out(pl, "");
                                show_loc(pl, i);
                            }
                        }
                known_sparse_next;
            }
    next_player;

    out_path = 0;
    out_alt_who = 0;
    show_loc_no_header = FALSE;
}


void
player_banner() {
    int pl;
    struct entity_player *p;

    stage("player_banner()");

    out_path = MASTER;
    out_alt_who = OUT_BANNER;

    loop_player(pl)
            {
                if (subkind(pl) == sub_pl_silent) {
                    continue;
                }

                p = p_player(pl);

#if 0
                out(pl, "From: %s", from_host);
                out(pl, "Reply-To: %s", reply_host);
                if (p->email)
                    out(pl, "To: %s (%s)", p->email,
                        p->full_name ? p->full_name : "???");
                out(pl, "Subject: Olympia:TAG game %d, turn %d report", game_number, sysclock.turn);
                out(pl, "");
#endif

#if 0
                switch (player_compuserve(pl))
                {
                case 1:
                    indent += 3;
                    wout(pl, "Note:  Please download the Olympia paper "
                        "for this turn from Library 3 "
                        "in the PBMGAMES forum on Compuserve.  "
                        "It should be available in file "
                        "OT%d.OLY as soon as it is released by "
                        "the sysops.", sysclock.turn);
                    indent -= 3;
                    out(pl, "");
                    break;

                case 2:
                    indent += 3;
                    wout(pl, "Note:  Olympia paper mailing is turned off for you."
                        "To begin receiving the paper again, issue the order TIMES 0");
                    indent -= 3;
                    out(pl, "");
                    break;
                }
#else
                if (player_compuserve(pl)) {
                    indent += 3;
                    wout(pl, "Note:  Olympia paper mailing is turned off for you."
                             "To begin receiving the paper via email again, issue "
                             "the order TIMES 0");
                    indent -= 3;
                    out(pl, "");
                    break;
                }
#endif

                tagout(pl, "<tag type=turn pl=%d game=%d turn=%d>",
                       pl, game_number, sysclock.turn);
                wout(pl, "Olympia:TAG game %d turn %d report for %s.",
                     game_number,
                     sysclock.turn, box_name(pl));

                {
                    int month, year;

                    month = oly_month(sysclock);
                    year = oly_year(sysclock);

                    wout(pl, "Season \"%s\", month %d, in the year %d.",
                         month_names[month],
                         month + 1,
                         year + 1);
                }

                out(pl, "");
            }
    next_player;

    out_path = 0;
    out_alt_who = 0;
}

int report_account_out(int pl, int who) {
    char fnam[LEN];
    char cmd[LEN];
    FILE *fp;
    char *line;

    sprintf(fnam, "/tmp/oly-acct.tmp");

    sprintf(cmd, "%s -a %s -A %s-old -g tag%d -p %s -s 4 > %s",
            options.accounting_prog,
            options.accounting_dir,
            options.accounting_dir,
            game_number,
            box_code_less(pl), fnam);
    system(cmd);

    fp = fopen(fnam, "r");

    if (fp == NULL) {
        out(who, "<account summary not available>");
        out(who, "");
        fprintf(stderr, "can't open %s: ", fnam);
        perror("");
        unlink(fnam);
        return 0;
    };

    while (line = getlin(fp)) {
        out(who, "%s", line);
    }

    fclose(fp);
    unlink(fnam);
    return 1;

};


void
report_account_sup(int pl) {
    tagout(pl, "<tag type=account_summary>");
    tagout(pl, "<tag type=header>");
    out(pl, "Account summary");
    out(pl, "---------------");
    tagout(pl, "</tag type=header>");

    indent += 3;
    (void) report_account_out(pl, pl);
    indent -= 3;
    tagout(pl, "<tag type=header>");
    out(pl, "---------------");
    out(pl, "");
    tagout(pl, "</tag type=header>");
    tagout(pl, "</tag type=account_summary>");
}


void
report_account() {
    int pl;

    stage("report_account()");
    close_logfile();

    out_path = MASTER;
    out_alt_who = OUT_BANNER;

    loop_player(pl)
            {
                if (subkind(pl) != sub_pl_regular) {
                    continue;
                }

                report_account_sup(pl);
            }
    next_player;

    out_path = 0;
    out_alt_who = 0;
}


void
charge_account() {
    int pl;
    struct entity_player *p;
    char cmd[LEN];
    char *val_s;

    stage("charge_account()");

    out_path = MASTER;
    out_alt_who = OUT_BANNER;

    loop_player(pl)
            {
                if (subkind(pl) != sub_pl_regular) {
                    continue;
                }

                p = rp_player(pl);
                if (p == NULL) {
                    continue;
                }

                sprintf(cmd, "%s -g tag%d -a %s -A %s-old -p %s -t%s -y \"Olympia TAG turn %d\"",
                        options.accounting_prog,
                        game_number,
                        options.accounting_dir,
                        options.accounting_dir,
                        box_code_less(pl),
                        turn_charge(pl),
                        sysclock.turn);

                if (system(cmd)) {
                    out(gm_player, "Could not charge %s.",
                        box_code(pl));
                };
            }
    next_player;

    out_path = 0;
    out_alt_who = 0;
}

static void
print_accept_sup(int who, struct accept_ent *a, int parens) {
    char buf[LEN];

    sprintf(buf, "Accepting ");

    if (!a->item) {
        strcat(buf, "any amount of anything ");
    } else {
        if (!a->qty) {
            strcat(buf, "any amount of ");
        } else {
            strcat(buf, "up to ");
            strcat(buf, nice_num(a->qty));
        };
        strcat(buf, box_name(a->item));
        strcat(buf, " ");
    };

    strcat(buf, "from ");

    if (!a->from_who) {
        strcat(buf, "anyone");
    } else if (kind(a->from_who) == T_nation) {
        strcat(buf, "any member of ");
        strcat(buf, rp_nation(a->from_who)->name);
    } else {
        strcat(buf, box_name(a->from_who));
    };

    if (parens) {
        out(who, "(%s.)", buf);
    } else {
        out(who, "%s.", buf);
    }

};

static void
list_accepts(int who, int num) {
    int i;
    struct accept_ent **accept;
    int flag = TRUE;

    assert(valid_box(who) && p_char(num));
    accept = p_char(who)->accept;

    out(who, "");
    out(who, "Accepting:");
    out(who, "");
    indent += 3;

    for (i = 0; i < ae_list_len(accept); i++) {
        print_accept_sup(num, accept[i], FALSE);
        flag = FALSE;
    };

    /*
     *  Wed Jan 13 12:57:35 1999 -- Scott Turner
     *
     *  And print the "player" accepts too.
     *
     */
    if (player(num) != num) {
        accept = p_char(player(num))->accept;
        for (i = 0; i < ae_list_len(accept); i++) {
            print_accept_sup(num, accept[i], TRUE);
            flag = FALSE;
        };
    };

    if (flag) {
        out(num, "Nothing");
    }

    indent -= 3;
}
