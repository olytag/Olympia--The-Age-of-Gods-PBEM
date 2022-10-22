// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include <stdlib.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


#define    SHOW_TOP    FALSE


/*
 *  Comparison is reversed so maximum will be first in list
 */

static int rank_comp(const void *q1, const void *q2) {
    const int *a = (const int *)q1;
    const int *b = (const int *)q2;

    return bx[*b]->temp - bx[*a]->temp;
}


static ilist ranks = NULL;


/*
 *  Return the ranking of a player within ranks[], allowing
 *  for ties.
 */

static int
ranking(int n) {

    while (n > 0 && bx[ranks[n]]->temp == bx[ranks[n - 1]]->temp) {
        n--;
    }

    return n + 1;
}


static char *
top_rank() {
    int i;
    int n = 0;
    char *s = "";

    i = 0;
    while (1) {
        s = comma_append(s, box_code(ranks[i]));
        n++;
        i++;

        if (n > 3) {
            return "-";
        }

        if (i >= ilist_len(ranks)) {
            break;
        }

        if (bx[ranks[i]]->temp != bx[ranks[i - 1]]->temp) {
            break;
        }
    }

    return s;
}


static void
out_rank_mine_top(int who, int num, char *title, int total) {
    char *s;
    int j = 20 + 1 + 11 + 2 + 5 + 2;
    int val = bx[ranks[num]]->temp;

    s = comma_num(val);

    if (total != 0) {
        s = sout("%s/%s", s, comma_num(total));
    }

    tagout(who, "<tag type=faction_entry title=\"%s\" value=\"%s\" "
                "rank=\"%s\" top=\"%s\">",
           title,
           s,
           ordinal(ranking(num)),
           SHOW_TOP ? top_rank() : "");

    wiout(who, j, "%-20s %16s  %-5s  %s",
          title,
          s,
          ordinal(ranking(num)),
          SHOW_TOP ? top_rank() : "");
}


static void
out_ranking(char *title, int total) {
    int i;

    ilist_clear(&ranks);

    loop_player(i)
            {
                if (subkind(i) != sub_pl_regular) {
                    continue;
                }

                ilist_append(&ranks, i);
            }
    next_player;

    qsort(ranks, (unsigned) ilist_len(ranks), sizeof(int), rank_comp);

    for (i = 0; i < ilist_len(ranks); i++) {
        out_rank_mine_top(ranks[i], i, title, total);
    }

#if 0
    if (top_rank > 0)		/* for the GM */
    {
        out_rank_mine_top(gm_player, 0, title, total);
    }
#endif
}


static int nplayers;
static int ncontrolled;
static int nother;


static void
collect_game_totals() {
    int i, j;

    nplayers = 0;
    ncontrolled = 0;
    nother = 0;

    clear_temps(T_char);

    loop_player(i)
            {
                if (subkind(i) != sub_pl_regular) {
                    continue;
                }

                nplayers++;
                loop_units(i, j)
                        {
                            ncontrolled++;
                            bx[j]->temp = 1;
                        }
                next_unit;
            }
    next_player;

    loop_char(i)
            {
                if (bx[i]->temp == 0) {
                    nother++;
                }
            }
    next_char;
}


static void
summary_units() {
    int i;

    clear_temps(T_player);

    loop_char(i)
            {
                bx[player(i)]->temp++;
            }
    next_char;

    out_ranking("Characters:", 0);
}


static void
summary_skills() {
    int pl, who;
    int i;
    int nskills = 0;
    struct skill_ent *e;

    loop_skill(i)
            {
                if (magic_skill(i)) {
                    continue;
                }

                nskills++;
            }
    next_skill;

    clear_temps(T_player);

    loop_player(pl)
            {
                if (subkind(pl) != sub_pl_regular) {
                    continue;
                }

                clear_temps(T_skill);

                loop_units(pl, who)
                        {
                            loop_char_skill_known(who, e)
                                        {
                                            bx[e->skill]->temp++;
                                        }
                            next_char_skill_known;
                        }
                next_unit;

                loop_skill(i)
                        {
                            if (magic_skill(i)) {
                                continue;
                            }

                            if (bx[i]->temp) {
                                bx[pl]->temp++;
                            }
                        }
                next_skill;
            }
    next_player;

    out_ranking("Skills known:", nskills);
}


static void
summary_spells() {
    int pl, who;
    int i;
    int nskills = 0;
    struct skill_ent *e;

    loop_skill(i)
            {
                if (!magic_skill(i)) {
                    continue;
                }

                nskills++;
            }
    next_skill;

    clear_temps(T_player);

    loop_player(pl)
            {
                if (subkind(pl) != sub_pl_regular) {
                    continue;
                }

                clear_temps(T_skill);

                loop_units(pl, who)
                        {
                            loop_char_skill_known(who, e)
                                        {
                                            bx[e->skill]->temp++;
                                        }
                            next_char_skill_known;
                        }
                next_unit;

                loop_skill(i)
                        {
                            if (!magic_skill(i)) {
                                continue;
                            }

                            if (bx[i]->temp) {
                                bx[pl]->temp++;
                            }
                        }
                next_skill;
            }
    next_player;

    out_ranking("Spells known:", nskills);
}


static void
summary_men() {
    int i;
    struct item_ent *e;

    clear_temps(T_player);

    loop_char(i)
            {
                loop_inv(i, e)
                            {
                                if (man_item(e->item)) {
                                    bx[player(i)]->temp += e->qty;
                                }
                            }
                next_inv;
            }
    next_char;

    out_ranking("Men:", 0);
}


static void
summary_gold() {
    int i;

    clear_temps(T_player);

    loop_char(i)
            {
                bx[player(i)]->temp += has_item(i, item_gold);
            }
    next_char;

    out_ranking("Gold:", 0);
}


static void
summary_land_owned() {
    int i, j;
    ilist l;

    clear_temps(T_player);

    loop_garrison(i)
            {
                if (player(province_admin(i))) {
                    bx[player(province_admin(i))]->temp++;
                }
            }
    next_garrison;

    out_ranking("Land controlled:", 0);
}


static void
summary_provinces() {
    int i;
    int pl;
    int nlocs = 0;

    loop_loc(i)
            {
                if (loc_depth(i) != LOC_province) {
                    continue;
                }

                nlocs++;
            }
    next_loc;

    clear_temps(T_player);

    loop_player(pl)
            {
                clear_temps(T_loc);

                loop_known(p_player(pl)->known, i)
                        {
                            bx[i]->temp++;
                        }
                next_known;

                loop_loc(i)
                        {
                            if (loc_depth(i) != LOC_province) {
                                continue;
                            }

                            if (bx[i]->temp) {
                                bx[pl]->temp++;
                            }
                        }
                next_loc;
            }
    next_player;

    out_ranking("Provinces visited:", nlocs);
}


#if 0
static void
summary_sublocs()
{
    int i;
    int pl;
    int nlocs = 0;

    loop_loc(i)
    {
        if (loc_depth(i) != LOC_subloc)
            continue;

        nlocs++;
    }
    next_loc;

    clear_temps(T_player);

    loop_player(pl)
    {
        clear_temps(T_loc);

        loop_known(p_player(pl)->known, i)
        {
            bx[i]->temp++;
        }
        next_known;

        loop_loc(i)
        {
            if (loc_depth(i) != LOC_subloc)
                continue;

            if (bx[i]->temp)
                bx[pl]->temp++;
        }
        next_loc;
    }
    next_player;

    out_ranking("Sublocs found:", nlocs);
}
#endif


void
summary_report() {
    int pl;
    char *s, *p;

    stage("summary_report()");

    collect_game_totals();

    out_path = MASTER;
    out_alt_who = OUT_SUMMARY;

    loop_player(pl)
            {
                if (subkind(pl) != sub_pl_regular) {
                    continue;
                }

                tagout(pl, "<tag type=game_totals players=%s controlled=%s "
                           "other=%s>",
                       comma_num(nplayers), comma_num(ncontrolled),
                       comma_num(nother));
                out(pl, "");
                out(pl, "Game totals:");
                indent += 3;
                out(pl, "%-20s  %5s", "Players:", comma_num(nplayers));
                out(pl, "%-20s  %5s", "Controlled units:",
                    comma_num(ncontrolled));
                out(pl, "%-20s  %5s", "Other units:", comma_num(nother));
                indent -= 3;
                out(pl, "");
                tagout(pl, "</tag type=game_totals>");

                tagout(pl, "<tag type=faction_summary pl=%d>", pl);
                s = sout("Faction %s", box_code(pl));
                out(pl, "%-20s %16s  %-5s  %s", s, "", "rank",
                    SHOW_TOP ? "top faction" : "");

                for (p = s; *p; p++) {
                    *p = '-';
                }

                out(pl, "%-20s %16s  %-5s  %s", s, "", "----",
                    SHOW_TOP ? "-----------" : "");
            }
    next_player;

    summary_units();
    summary_men();
    summary_gold();
    summary_land_owned();
    summary_skills();
    summary_spells();
    summary_provinces();

    loop_player(pl)
            {
                if (subkind(pl) != sub_pl_regular) {
                    continue;
                }

                out(pl, "");
                out(pl, "");
                tagout(pl, "</tag type=faction_summary pl=%d>", pl);
            }
    next_player;

    out_path = 0;
    out_alt_who = 0;
}

