// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <stdio.h>
#include <stdlib.h>
#include "z.h"
#include "oly.h"
#include "forward.h"
#include "vectors/skill_ent_list.h"


int gold_common_magic = 0;
int gold_lead_to_gold = 0;
int gold_pot_basket = 0;
int gold_trade = 0;
int gold_fish = 0;
int gold_inn = 0;
int gold_taxes = 0;
int gold_tariffs = 0;
int gold_fees = 0;
int gold_combat = 0;
int gold_combat_indep = 0;
int gold_petty_thief = 0;
int gold_temple = 0;
int gold_pillage = 0;
int gold_ferry = 0;
int gold_opium = 0;
int gold_claim = 0;
int gold_times = 0;

static int skill_use_comp(const void *q1, const void *q2) {
    int *a = (int *)q1;
    int *b = (int *)q2;
    struct entity_skill *pa;
    struct entity_skill *pb;

    pa = rp_skill(*a);
    pb = rp_skill(*b);

    return pb->use_count - pa->use_count;
}


static void
gm_show_skill_use_counts(int pl) {
    int sk;
    static ilist l = NULL;
    int i;
    struct entity_skill *p;

    ilist_clear(&l);

    loop_skill(sk)
            {
                if (skill_school(sk) == sk) {    /* skip category skills */
                    continue;
                }

                if (rp_skill(sk) && rp_skill(sk)->use_count) {
                    ilist_append(&l, sk);
                }
            }
    next_skill;

    qsort(l, ilist_len(l), sizeof(int), skill_use_comp);

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "");
    out(pl, "Skill use counts:");
    out(pl, "-----------------");
    out(pl, "");
    out(pl, "%5s  %4s  %5s  %s", "count", "who", "skill", "name");
    out(pl, "%5s  %4s  %5s  %s", "-----", "---", "-----", "----");

    for (i = 0; i < ilist_len(l); i++) {
        char first_use = ' ';

        /*  Note if this is the first use ever. */
        if (!test_known(gm_player, l[i])) {
            set_known(gm_player, l[i]);
            first_use = '*';
        };

        p = rp_skill(l[i]);

        out(pl, "%4d%1c  %4s  %5s  %s",
            p->use_count,
            first_use,
            box_code_less(player(p->last_use_who)),
            box_code_less(l[i]),
            just_name(l[i]));
    }

    out(pl, "");

    out_path = 0;
    out_alt_who = 0;
}


static int skills_known_comp(const void *q1, const void *q2) {
    int *a = (int *)q1;
    int *b = (int *)q2;

    return bx[*b]->temp - bx[*a]->temp;
}

static void
gm_show_skills_known(int pl) {
    int sk;
    static ilist l = NULL;
    int i;
    struct skill_ent *e;
    struct entity_skill *p;

    ilist_clear(&l);
    clear_temps(T_skill);

    loop_char(i)
            {
                loop_char_skill_known(i, e)
                            {
                                bx[e->skill]->temp++;
                            }
                next_char_skill_known;
            }
    next_char;

    loop_skill(sk)
            {
                if (skill_school(sk) == sk) {    /* skip category skills */
                    continue;
                }

#if 0
                if (bx[sk]->temp)
#endif
                ilist_append(&l, sk);
            }
    next_skill;

    qsort(l, ilist_len(l), sizeof(int), skills_known_comp);

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "");
    out(pl, "Skills known by players:");
    out(pl, "------------------------");
    out(pl, "");
    out(pl, "%5s  %5s  %4s  %s", "count", "skill", "use", "name");
    out(pl, "%5s  %5s  %4s  %s", "-----", "-----", "---", "----");

    for (i = 0; i < ilist_len(l); i++) {
        p = rp_skill(l[i]);

        if (bx[l[i]]->temp == 0) { break; }

        out(pl, "%4d   %5s  %4s  %s",
            bx[l[i]]->temp,
            box_code_less(l[i]),
            p->use_count ? comma_num(p->use_count) : "",
            just_name(l[i]));
    }

    out(pl, "");

    out_path = 0;
    out_alt_who = 0;
    ilist_reclaim(&l);
}

static void
gm_count_priests_mages(int pl) {
    int priests = 0, mages = 0, i;
    int max_aura = 0, cur_aura = 0, cur_piety = 0;

    loop_char(i)
            {
                if (is_priest(i)) {
                    priests++;
                    if (rp_char(i)->religion.piety > cur_piety) {
                        cur_piety = rp_char(i)->religion.piety;
                    }
                };
                if (is_wizard(i)) {
                    mages++;
                    if (char_max_aura(i) > max_aura) {
                        max_aura = char_max_aura(i);
                    }
                    if (char_cur_aura(i) > cur_aura) {
                        cur_aura = char_cur_aura(i);
                    }
                };
            }next_char;

    out_path = MASTER;
    out_alt_who = OUT_LORE;
    out(pl, "");
    out(pl, "Mages and Priests");
    out(pl, "-----------------");
    out(pl, "");
    out(pl, " Mages:   %s, Max aura: %s, Cur aura: %s", comma_num(mages),
        comma_num(max_aura), comma_num(cur_aura));
    out(pl, " Priests: %s, Max piety: %s", comma_num(priests),
        comma_num(cur_piety));
    out(pl, "");
    out_path = 0;
}


static void
gm_show_interesting_attributes(int pl) {
    int i;
    int ability_shroud = 0;
    int hinder_meditation = 0;
    int project_cast = 0;
    int quick_cast = 0;
    int loc_shroud = 0;
    int loc_barrier = 0;
    int loc_opium = 0;
    int format_one = 0, nplay = 0;
    struct char_magic *pc;
    struct entity_loc *lc;
    int rams = 0, ngal = 0;

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    loop_char(i)
            {
                pc = rp_magic(i);

                if (pc == NULL) {
                    continue;
                }

                if (pc->ability_shroud) {
                    ability_shroud++;
                }

                if (pc->hinder_meditation) {
                    hinder_meditation++;
                }

                if (pc->project_cast) {
                    project_cast++;
                }

                if (pc->quick_cast) {
                    quick_cast++;
                }
            }
    next_char;

    loop_loc(i)
            {
                lc = rp_loc(i);

                if (lc && lc->shroud) {
                    loc_shroud++;
                }

                if (lc && get_effect(i, ef_magic_barrier, 0, 0)) {
                    loc_barrier++;
                }

                if (loc_opium(i)) {
                    loc_opium++;
                }
            }
    next_loc;

    loop_ship(i)
            {
                if (subkind(i) != sub_galley) {
                    continue;
                }
                ngal++;
                if (ship_has_ram(i)) {
                    rams++;
                }
            }
    next_ship;

    loop_player(i)
            {
                if (subkind(i) != sub_pl_regular) {
                    continue;
                }

                nplay++;
                if (player_format(i)) {
                    format_one++;
                }
            }
    next_player;

    out(pl, "Interesting attribute counts");
    out(pl, "----------------------------");
    out(pl, "");

    out(pl, "char ability shroud:    %d", ability_shroud);
    out(pl, "char hinder meditate:   %d", hinder_meditation);
    out(pl, "char project cast:      %d", project_cast);
    out(pl, "char quick cast:        %d", quick_cast);
    out(pl, "loc shroud:             %d", loc_shroud);
    out(pl, "loc barrier:            %d", loc_barrier);
    out(pl, "loc opium:              %d", loc_opium);
    out(pl, "format one:             %d/%d", format_one, nplay);

    out(pl, "");

    if (ngal) {
        out(pl, "galleys with rams:      %d (%d%%)",
            rams, rams * 100 / ngal);
    }

    out_path = 0;
    out_alt_who = 0;
}


void
gm_list_animate_items(int pl) {
    int i;
    char buf[100];

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "");
    out(pl, "Animate items list");
    out(pl, "------------------");
    out(pl, "");

    out(pl, "%25s %8s %8s %8s  %s",
        "name", "swamp", "man-like", "beast", "fighter");
    out(pl, "%25s %8s %8s %8s  %s",
        "----", "-----", "--------", "-----", "-------");

    loop_item(i)
            {
                if (!is_fighter(i) &&
                    !man_item(i) &&
                    !beast_capturable(i) &&
                    !item_animal(i)) {
                    continue;
                }

                if (is_fighter(i)) {
                    sprintf(buf, "(%d,%d,%d)",
                            item_attack(i),
                            item_defense(i),
                            item_missile(i));
                } else {
                    sprintf(buf, " -");
                }

                out(pl, "%25s %8s %8s %8s  %s",
                    box_name(i),
                    item_animal(i) ? "yes " : "no  ",
                    man_item(i) ? "yes " : "no  ",
                    item_capturable(i) ? "yes " : "no  ",
                    buf);
            }
    next_item;

    out_path = 0;
    out_alt_who = 0;
}


static void
gm_show_gold(int pl) {
    int sum;

    sum = gold_common_magic + gold_lead_to_gold + gold_pot_basket +
          gold_trade + gold_opium + gold_inn +
          gold_taxes + gold_tariffs +
          gold_combat_indep + gold_petty_thief + gold_combat +
          gold_temple + gold_pillage + gold_ferry + gold_claim +
          gold_fish + gold_times;

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "");
    out(pl, "Gold report");
    out(pl, "-----------");
    out(pl, "");

    if (sum) {
        out(pl, "Common magic:         %10s %3d%%", comma_num(gold_common_magic), gold_common_magic * 100 / sum);
        out(pl, "Lead to gold:         %10s %3d%%", comma_num(gold_lead_to_gold), gold_lead_to_gold * 100 / sum);
        out(pl, "Pots and baskets:     %10s %3d%%", comma_num(gold_pot_basket), gold_pot_basket * 100 / sum);
        out(pl, "Opium:                %10s %3d%%", comma_num(gold_opium), gold_opium * 100 / sum);
        out(pl, "Fish:                 %10s %3d%%", comma_num(gold_fish), gold_fish * 100 / sum);
        out(pl, "Trade to cities:      %10s %3d%%", comma_num(gold_trade), gold_trade * 100 / sum);
        out(pl, "Inn income:           %10s %3d%%", comma_num(gold_inn), gold_inn * 100 / sum);
        out(pl, "Taxes:                %10s %3d%%", comma_num(gold_taxes), gold_taxes * 100 / sum);
        out(pl, "Tariffs:              %10s %3d%%", comma_num(gold_tariffs), gold_tariffs * 100 / sum);
        out(pl, "Fees:                 %10s %3d%%", comma_num(gold_fees), gold_fees * 100 / sum);
        out(pl, "Combat with indeps:   %10s %3d%%", comma_num(gold_combat_indep), gold_combat_indep * 100 / sum);
        out(pl, "Combat with players:  %10s %3d%%", comma_num(gold_combat), gold_combat * 100 / sum);
        out(pl, "Petty thievery:       %10s %3d%%", comma_num(gold_petty_thief), gold_petty_thief * 100 / sum);
        out(pl, "Temple income:        %10s %3d%%", comma_num(gold_temple), gold_temple * 100 / sum);
        out(pl, "Pillaging:            %10s %3d%%", comma_num(gold_pillage), gold_pillage * 100 / sum);
        out(pl, "Ferry boarding:       %10s %3d%%", comma_num(gold_ferry), gold_ferry * 100 / sum);
        out(pl, "Claim:                %10s %3d%%", comma_num(gold_claim), gold_claim * 100 / sum);
        out(pl, "Times:                %10s %3d%%", comma_num(gold_times), gold_times * 100 / sum);
        out(pl, "                      %10s %4s", "", "----");
    };
    out(pl, "Total:                %10s", comma_num(sum));
    out_path = 0;
    out_alt_who = 0;
}


static void
gm_show_control_arts(int pl) {
    int item;
    int n;

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "");
    out(pl, "Captured control artifacts");
    out(pl, "--------------------------");
    out(pl, "");

    loop_subkind(sub_magic_artifact, item)
            {
                if (!is_real_npc(player(item_unique(item)))) {
                    struct command c;
                    c.who = pl;
                    c.a = item;
                    out(pl, "%-33s  %s", box_name(item),
                        box_name(player(item_unique(item))));
                    artifact_identify("    ", &c);
                };
            }
    next_subkind;

    out(pl, "");

    out_path = 0;
    out_alt_who = 0;
}


static int
gm_count_stuff(int pl) {
    int castle = 0;
    int castle_notdone = 0;
    int tower = 0;
    int tower_notdone = 0;
    int mine = 0;
    int mine_notdone = 0;
    int temple = 0;
    int temple_notdone = 0;
    int galley = 0;
    int galley_notdone = 0;
    int round = 0;
    int round_notdone = 0;
    int inn = 0;
    int inn_notdone = 0;
    int i;

    loop_loc_or_ship(i)
            {
                switch (subkind(i)) {
                    case sub_castle:
                        castle++;
                        break;
                    case sub_castle_notdone:
                        castle_notdone++;
                        break;
                    case sub_tower:
                        tower++;
                        break;
                    case sub_tower_notdone:
                        tower_notdone++;
                        break;
                    case sub_galley:
                        galley++;
                        break;
                    case sub_galley_notdone:
                        galley_notdone++;
                        break;
                    case sub_roundship:
                        round++;
                        break;
                    case sub_roundship_notdone:
                        round_notdone++;
                        break;
                    case sub_temple:
                        temple++;
                        break;
                    case sub_temple_notdone:
                        temple_notdone++;
                        break;
                    case sub_inn:
                        inn++;
                        break;
                    case sub_inn_notdone:
                        inn_notdone++;
                        break;
                    case sub_mine_shaft:
                    case sub_mine:
                        mine++;
                        break;
                    case sub_mine_shaft_notdone:
                    case sub_mine_notdone:
                        mine_notdone++;
                        break;
                }
            }
    next_loc_or_ship;

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "%10s  %9s  %s", "", "finished", "unfinished");
    out(pl, "%10s +----------------------", "");
    out(pl, "%10s |%9s  %8s",
        "galley", comma_num(galley), comma_num(galley_notdone));
    out(pl, "%10s |%9s  %8s",
        "roundship", comma_num(round), comma_num(round_notdone));
    out(pl, "%10s |%9s  %8s",
        "tower", comma_num(tower), comma_num(tower_notdone));
    out(pl, "%10s |%9s  %8s",
        "castle", comma_num(castle), comma_num(castle_notdone));
    out(pl, "%10s |%9s  %8s",
        "mine", comma_num(mine), comma_num(mine_notdone));
    out(pl, "%10s |%9s  %8s",
        "inn", comma_num(inn), comma_num(inn_notdone));
    out(pl, "%10s |%9s  %8s",
        "temple", comma_num(temple), comma_num(temple_notdone));
    out(pl, "");

    out_path = 0;
    out_alt_who = 0;
    return 0;
}


static void
gm_show_gate_stats(int pl) {
    int n_gates = 0;
    int n_found = 0;
    int ngate_seal = 0, ngate_jump = 0, ngate_unseal = 0;
    int i, j;
    struct entity_gate *g;

    clear_temps(T_gate);

    loop_player(i)
            {
                known_sparse_loop(p_player(i)->known, j)
                        {
                            if (kind(j) != T_gate) {
                                continue;
                            }

                            bx[j]->temp++;
                        }
                known_sparse_next;
            }
    next_player;

    loop_gate(i)
            {
                n_gates++;
                if (bx[i]->temp) {
                    n_found++;
                }

                if (g = rp_gate(i)) {
                    if (g->seal_key) {
                        ngate_seal++;
                    }
                    if (g->notify_jumps) {
                        ngate_jump++;
                    }
                    if (g->notify_unseal) {
                        ngate_unseal++;
                    }
                }
            }
    next_gate;

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "%d/%d gates found (%d%%)", n_found, n_gates,
        n_found * 100 / n_gates);
    out(pl, "    %d sealed (%d%%), %d notify jump (%d%%), %d notify "
            "unseal (%d%%)",
        ngate_seal, ngate_seal * 100 / n_gates,
        ngate_jump, ngate_jump * 100 / n_gates,
        ngate_unseal, ngate_unseal * 100 / n_gates);
    out(pl, "");

    out_path = 0;
    out_alt_who = 0;
}


static void
gm_show_locs_visited(int pl) {
    int n_prov = 0;
    int n_prov_visit = 0;
    int n_sub = 0;
    int n_sub_visit = 0;
    int hid = 0;
    int vis = 0;
    int i, j;
    int nf = 0, nt = 0, nf_vis = 0, nf_hid = 0;

    clear_temps(T_loc);

    loop_player(i)
            {
                known_sparse_loop(p_player(i)->known, j)
                        {
                            if (kind(j) != T_loc) {
                                continue;
                            }

                            bx[j]->temp++;
                        }
                known_sparse_next;
            }
    next_player;

    loop_loc(i)
            {
                if (loc_depth(i) == LOC_province) {
                    n_prov++;
                    if (bx[i]->temp) {
                        n_prov_visit++;
                    }
                } else if (loc_depth(i) == LOC_subloc) {
                    n_sub++;
                    if (bx[i]->temp) {
                        n_sub_visit++;
                        if (loc_hidden(i)) {
                            hid++;
                        } else {
                            vis++;
                        }
                    }
                }
            }
    next_loc;

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "%d/%d provinces visited (%d%%)", n_prov_visit, n_prov,
        n_prov_visit * 100 / n_prov);
    out(pl, "%d/%d sublocs visited (%d%%)", n_sub_visit, n_sub,
        n_sub_visit * 100 / n_sub);
    out(pl, "    %d%% visible, %d%% hidden",
        vis * 100 / n_sub_visit,
        hid * 100 / n_sub_visit);

    loop_loc(i)
            {
                if (loc_depth(i) != LOC_province || bx[i]->temp == 0) {
                    continue;
                }

                loop_here(i, j)
                        {
                            if (kind(j) != T_loc || loc_depth(j) != LOC_subloc) {
                                continue;
                            }

                            nt++;
                            if (bx[j]->temp) {
                                nf++;
                                if (loc_hidden(j)) {
                                    nf_hid++;
                                } else {
                                    nf_vis++;
                                }
                            }
                        }
                next_here;
            }
    next_loc;

    out(pl, "    %d%% of visisted province's sublocs found",
        nf * 100 / nt);
    out(pl, "    %d%% visible, %d%% hidden",
        nf_vis * 100 / nf,
        nf_hid * 100 / nf);

    out(pl, "");

    out_path = 0;
    out_alt_who = 0;
}


static void
gm_loyalty_stats(int pl) {
    int i;
    int tot = 0, oath = 0, fear = 0, cont = 0, unsw = 0, npc = 0;

    loop_char(i)
            {
                if (subkind(i) != 0) {
                    continue;
                }

                tot++;
                switch (loyal_kind(i)) {
                    case LOY_oath:
                        oath++;
                        break;
                    case LOY_fear:
                        fear++;
                        break;
                    case LOY_contract:
                        cont++;
                        break;
                    case LOY_unsworn:
                        unsw++;
                        break;
                    case LOY_npc:
                        npc++;
                        break;

                    default:
                        fprintf(stderr, "loy_kind = %d\n", loyal_kind(i));
                        assert(FALSE);
                }
            }
    next_char;

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "%d chars: %d oath (%d%%), %d fear (%d%%), %d contract "
            "(%d%%), %d unsworn (%d%%)",
        tot, oath, oath * 100 / tot,
        fear, fear * 100 / tot,
        cont, cont * 100 / tot,
        unsw, unsw * 100 / tot);

    out(pl, "");

    out_path = 0;
    out_alt_who = 0;
}


static int region_occupy_comp(const void *q1, const void *q2) {
    int *a = (int *)q1;
    int *b = (int *)q2;
    return bx[*b]->temp - bx[*a]->temp;
}


static void
gm_land_stats(int pl) {
    int i;
    static ilist l = NULL;
    int n_chars = 0, n_beasts = 0;

    ilist_clear(&l);

    clear_temps(T_loc);

    loop_char(i)
            {
                if (region(i)) {
                    if (player(i) < 1000) {
                        n_beasts++;
                    } else {
                        bx[region(i)]->temp++;
                        n_chars++;
                    };
                };
            }
    next_char;

    loop_loc(i)
            {
                if (bx[i]->temp) {
                    ilist_append(&l, i);
                }
            }
    next_loc;

    qsort(l, ilist_len(l), sizeof(int), region_occupy_comp);

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "%10s  %6s  %s", "nobles", "beasts", "region");
    out(pl, "%10s  %6s  %s", "------", "------", "------");

    for (i = 0; i < ilist_len(l); i++) {
        out(pl, "%10s  %6s  %s",
            comma_num(bx[l[i]]->temp),
            0,
            just_name(l[i]));
    }
    out(pl, "%10s  %6s  %s", "======", "======", "");
    out(pl, "%10s  %6s  %s", comma_num(n_chars), comma_num(n_beasts), "");

    out(pl, "");

    out_path = 0;
    out_alt_who = 0;
}


static int wealth_list_comp(const void *q1, const void *q2) {
    int *a = (int *)q1;
    int *b = (int *)q2;
    return bx[*b]->temp - bx[*a]->temp;
}


static void
gm_faction_wealth(int pl) {
    int i;
    static ilist l = NULL;

    ilist_clear(&l);
    clear_temps(T_player);

    loop_char(i)
            {
                bx[player(i)]->temp += has_item(i, item_gold);
            }
    next_char;

    loop_player(i)
            {
                if (subkind(i) == sub_pl_regular) {
                    ilist_append(&l, i);
                }
            }
    next_player;

    qsort(l, ilist_len(l), sizeof(int), wealth_list_comp);

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "%4s %11s  %s", "rank", "gold", "faction");
    out(pl, "%4s %11s  %s", "----", "----", "-------");

    for (i = 0; i < ilist_len(l); i++) {
        out(pl, "%4d %11s  %s", i + 1, comma_num(bx[l[i]]->temp),
            box_name(l[i]));
    }

    out(pl, "");

    out_path = 0;
    out_alt_who = 0;
}


static int nobles_list_comp(const void *q1, const void *q2) {
    int *a = (int *)q1;
    int *b = (int *)q2;
    return bx[*b]->temp - bx[*a]->temp;
}


static void
gm_nobles_list(int pl) {
    int i;
    static ilist l = NULL;

    ilist_clear(&l);
    clear_temps(T_player);

    loop_char(i)
            {
                bx[player(i)]->temp++;
            }
    next_char;

    loop_player(i)
            {
                if (subkind(i) == sub_pl_regular) {
                    ilist_append(&l, i);
                }
            }
    next_player;

    qsort(l, ilist_len(l), sizeof(int), nobles_list_comp);

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "%4s %11s  %s", "rank", "nobles", "faction");
    out(pl, "%4s %11s  %s", "----", "------", "-------");

    for (i = 0; i < ilist_len(l); i++) {
        out(pl, "%4d %11s  %s", i + 1, comma_num(bx[l[i]]->temp),
            box_name(l[i]));
    }

    out(pl, "");

    out_path = 0;
    out_alt_who = 0;
}


static void
gm_player_details(int pl) {
    int i, j;
    int sum_gold, sum_units, sum_bld, sum_subloc, sum_ship, sum_skills;
    char *age;

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "%4s %3s %5s %5s %4s %6s %5s %6s",
        "who", "age", "gold", "units", "bld", "subloc", "ships", "skills");
    out(pl, "%4s %3s %5s %5s %4s %6s %5s %6s",
        "---", "---", "----", "-----", "---", "------", "-----", "------");

    loop_player(i)
            {
                if (subkind(i) == sub_pl_system) {
                    continue;
                }

                sum_gold = 0;
                loop_units(i, j)
                        {
                            sum_gold += has_item(j, item_gold);
                        }
                next_unit;

                sum_units = 0;
                loop_units(i, j)
                        {
                            sum_units++;
                        }
                next_unit;

                sum_bld = 0;
                loop_units(i, j)
                        {
                            int where = subloc(j);

                            if (loc_depth(where) == LOC_build &&
                                building_owner(where) == j &&
                                !is_ship(where) &&
                                !is_ship_notdone(where)) {
                                sum_bld++;
                            }
                        }
                next_unit;

                sum_subloc = 0;
                loop_units(i, j)
                        {
                            int where = subloc(j);

                            if (loc_depth(where) == LOC_subloc &&
                                first_character(where) == j) {
                                sum_subloc++;
                            }
                        }
                next_unit;

                sum_ship = 0;
                loop_units(i, j)
                        {
                            int where = subloc(j);

                            if ((is_ship(where) || is_ship_notdone(where)) &&
                                building_owner(where) == j) {
                                sum_ship++;
                            }
                        }
                next_unit;

                sum_skills = 0;
                clear_temps(T_skill);
                loop_units(i, j)
                        {
                            struct skill_ent *e;

                            loop_char_skill_known(j, e)
                                        {
                                            bx[e->skill]->temp++;
                                        }
                            next_char_skill_known;
                        }
                next_unit;

                loop_skill(j)
                        {
                            if (bx[j]->temp) {
                                sum_skills++;
                            }
                        }
                next_skill;

                if (rp_player(i)) {
                    age = sout("%d", sysclock.turn - rp_player(i)->first_turn);
                } else {
                    age = "???";
                }

                out(pl, "%4s %3s %5s %5s %4s %6s %5s %6s  %s",
                    box_code_less(i),
                    age,
                    knum(sum_gold, FALSE),
                    knum(sum_units, FALSE),
                    knum(sum_bld, TRUE),
                    knum(sum_subloc, TRUE),
                    knum(sum_ship, TRUE),
                    knum(sum_skills, TRUE),
                    just_name(i));
            }
    next_player;

    out(pl, "");

    out_path = 0;
    out_alt_who = 0;
}


void
list_all_notices(int pl) {
    int i;

    clear_temps(T_loc);

    loop_post(i)
            {
                bx[subloc(i)]->temp++;
            }
    next_post;

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    loop_loc(i)
            {
                if (bx[i]->temp == 0) {
                    continue;
                }


                show_loc_posts(pl, i, TRUE);
            }
    next_loc;

    out_path = 0;
    out_alt_who = 0;
}


void
list_all_items(int pl) {
    int i;
    char buf[LEN];

    out_path = MASTER;
    out_alt_who = OUT_LORE;

    out(pl, "%4s %-24s %3s %6s %4s %4s %4s %4s %3s", "item", "name", "mnt", "weight", "land", "ride", "fly", "cost",
        "cap");
    out(pl, "%4s %-24s %3s %6s %4s %4s %4s %4s %3s", "----", "----", "---", "------", "----", "----", "---", "----",
        "---");

    loop_item(i)
            {
                if (item_attack(i)) {
                    sprintf(buf, "%s (%d,%d,%d)", just_name(i),
                            item_attack(i), item_defense(i), item_missile(i));
                } else {
                    sprintf(buf, "%s", just_name(i));
                };
/* VLN */
/*
printf("player = %d\n",pl);
printf("box_code_less = %s\n",box_code_less(i));
printf("buf = %s\n",buf);
printf("maint = %d\n",rp_item(i)->maintenance);
printf("wt = %d\n",item_weight(i));
printf("land = %d\n",item_land_cap(i));
printf("ride = %d\n",item_ride_cap(i));
printf("fly = %d\n",item_fly_cap(i));
printf("price = %d\n",rp_item(i)->base_price);
printf("animal? = %s\n",item_animal(i) ? "yes" : "no");
*/
                out(pl, "%4s %-24s %3d %4d %4d %4d %4d %4d %3s",
                    box_code_less(i),
                    buf,
                    rp_item(i)->maintenance,
                    item_weight(i),
                    item_land_cap(i),
                    item_ride_cap(i),
                    item_fly_cap(i),
                    rp_item(i)->base_price,
                    item_animal(i) ? "yes" : "no"
                );
            }
    next_item;

    out(pl, "");

    out_path = 0;
    out_alt_who = 0;
}


void
gm_report(int pl) {

    stage("gm_report()");
    gm_show_gold(pl);
    gm_show_control_arts(pl);
    gm_count_priests_mages(pl);
    gm_show_skill_use_counts(pl);
    gm_count_stuff(pl);
    gm_land_stats(pl);
    gm_show_gate_stats(pl);
    gm_show_locs_visited(pl);
    gm_loyalty_stats(pl);
    gm_show_skills_known(pl);
    gm_faction_wealth(pl);
    gm_nobles_list(pl);
    gm_player_details(pl);
    list_all_notices(pl);
    gm_show_interesting_attributes(pl);
    gm_list_animate_items(pl);

    list_all_items(skill_player);
}

