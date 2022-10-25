
// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    <string.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


static char *liner_desc_char(int n);


#if 0
char *
loc_inside_string(int where)
{
    char *reg_name;

    if (loc_depth(where) == LOC_build)
    {
        if (subkind(loc(where)) == sub_ocean)
            return sout(", in %s", box_name(province(where)));

        if (loc_depth(loc(where)) == LOC_province)
            return sout(", in province %s",
                    box_name(province(where)));

        return sout(", in %s", box_name(province(where)));
    }
    else if (loc_depth(where) == LOC_subloc)
    {
        if (subkind(province(where)) == sub_ocean)
            return sout(", in %s", box_name(province(where)));
        else if (!valid_box(province(where)))
          return sout(", adrift in the Cosmos");
        else
          return sout(", in province %s",
                    box_name(province(where)));
    }
    else if (subkind(where) == sub_ocean)
    {
        reg_name = name(region(where));

        if (reg_name && *reg_name)
            return sout(", in %s", reg_name);
        return "";
    }
    else
    {
        reg_name = name(region(where));

        if (reg_name && *reg_name)
            return sout(", in %s", reg_name);

        return "";
    }
}
#endif

char *
loc_inside_string(int where) {
    char *name = NULL;
    char *trail = NULL;

    if (loc_depth(where) > LOC_region) {
        trail = loc_inside_string(loc(where));
    };

    switch (loc_depth(where)) {
        case LOC_region:
            name = just_name(region(where));
            break;
        case LOC_province:
            if (!valid_box(where)) {
                name = sout("adrift in the Cosmos");
            } else {
                name = box_name(where);
            }
            break;
        default:
            name = box_name(where);
    };

    if (trail != NULL) {
        return sout(", in %s%s", name, trail);
    }

    return sout(", in %s", name);
};

static int
show_loc_barrier(int who, int where) {

    if (loc_barrier(where)) {
        tagout(who, "<tag type=loc_barrier id=%d>", where);
        wout(who, "A magical barrier surrounds %s.", box_name(where));
        tagout(who, "</tag type=loc_barrier>");
        return TRUE;
    }

    return FALSE;
}


static char *
safe_haven_s(int n) {

    if (safe_haven(n)) {
        return ", safe haven";
    }

    return "";
}


static char *
ship_cap_s(int n) {
    int sc, sw;

    if (sc = ship_cap(n)) {
        sw = ship_weight(n);
        return sout(", %d%% loaded", sw * 100 / sc);
    }

    return "";
}


static void
show_loc_stats(int who, int where) {
    int sc, sw, n;
    int first = TRUE;

    tagout(who, "<tag type=loc_stats loc=%d>", where);

    if (n = loc_damage(where)) {
        if (first) {
            out(who, "");
            first = FALSE;
        }
        tagout(who, "<tag type=loc_dam loc=%d damage=%d tot=%d>",
               where, n, loc_hp(where));
        out(who, "Damage: %d%%", (int) (n * 100) / loc_hp(where));
    }

    if (sc = ship_cap(where)) {
        sw = ship_weight(where);

        if (first) {
            out(who, "");
            first = FALSE;
        }

        tagout(who, "<tag type=loc_cap loc=%d cap=%d weight=%d>",
               where, sc, sw);
        out(who, "Ship capacity: %s/%s (%d%%)",
            comma_num(sw), comma_num(sc), sw * 100 / sc);
    }

    tagout(who, "</tag type=loc_stats>");
}


char *
loc_civ_s(int where) {
    int n;

    if (loc_depth(where) != LOC_province ||
        subkind(where) == sub_ocean ||
        in_faery(where) || in_hades(where)) {
        return "";
    }

    n = has_item(where, item_peasant);

    if (n < 100) {
        return ", wilderness";
    }

    return sout(", peasants: %d", n);
}


/*
 *  Thu Nov 12 11:20:28 1998 -- Scott Turner
 *
 *  Add display of shoring.
 *
 */
char *
show_loc_header(int where) {
    char buf[LEN];

    strcpy(buf, box_name_kind(where));
    strcat(buf, loc_inside_string(loc(where)));

    if (subkind(where) == sub_mine_shaft) {
        struct entity_mine *mi = get_mine_info(where);
        if (mine_depth(where)) {
            strcat(buf, sout(", depth~%d feet",
                             (mine_depth(where) * 100) + 100));
        }
        if (mi) {
            switch (mi->shoring[mine_depth(where)]) {
                case WOODEN_SHORING:
                    strcat(buf, sout(", wooden shoring"));
                    break;
                case IRON_SHORING:
                    strcat(buf, sout(", iron shoring"));
                    break;
                case NO_SHORING:
                default:
                    strcat(buf, sout(", no shoring"));
                    break;
            }
        };
    };

    strcat(buf, safe_haven_s(where));

    if (loc_hidden(where)) {
        strcat(buf, ", hidden");
    }

    strcat(buf, loc_civ_s(where));

    return sout("%s", buf);
}


static char *
with_inventory_string(int who) {
    char with[LEN];
    struct item_ent *e;
    int mk;

    mk = noble_item(who);
    with[0] = '\0';

    loop_inv(who, e)
                {
                    if (mk == e->item || !item_prominent(e->item)) {
                        continue;
                    }

                    if (with[0]) {
                        strcat(with, ", ");
                    } else {
                        strcpy(with, ", with ");
                    }

                    strcat(with, just_name_qty(e->item, e->qty));
                }
    next_inv;

    return sout("%s", with);
}


char *
display_with(int who) {

    if (rp_loc_info(who) && ilist_len(rp_loc_info(who)->here_list) > 0) {
        return ", accompanied~by:";
    }

    return "";
}


char *
display_owner(int who) {

    if (first_character(who) <= 0) {
        return "";
    }

    return ", owner:";
}


static char *
incomplete_string(int n) {
    struct entity_subloc *p;
    struct entity_build *b;

    p = rp_subloc(n);
    if (p == NULL) { return ""; }

    b = get_build(n, BT_BUILD);

    if (b == NULL || b->effort_required == 0) {
        return "";
    }

    return sout(", %d%% completed",
                b->effort_given * 100 / b->effort_required);
}


static char *
liner_desc_ship(int n) {
    char buf[LEN];
    struct entity_ship *ship = rp_ship(n);

    sprintf(buf, "%s%s",
            box_name_kind(n),
            incomplete_string(n));

    if (ship) {
        if (ship->hulls) {
            strcat(buf, sout(", %d hull%s", ship->hulls,
                             ship->hulls == 1 ? "" : "s"));
        }
        if (ship->ports) {
            strcat(buf, sout(", %d rowing port%s", ship->ports,
                             ship->ports == 1 ? "" : "s"));
        }
        if (ship->sails) {
            strcat(buf, sout(", %d sail%s", ship->sails,
                             ship->sails == 1 ? "" : "s"));
        }
    };

#if 0
    strcat(buf, ship_cap_s(n));

    if (loc_defense(n))
      strcat(buf, sout(", defense~%d", loc_defense(n)));
#endif

    if (loc_damage(n)) {
        strcat(buf, sout(", %d%%~damaged", (loc_damage(n) * 100) / loc_hp(n)));
    }

    if (show_display_string) {
        char *s = banner(n);
        if (s && *s) {
            strcat(buf, sout(", \"%s\"", s));
        }
    }

    if (loc_hidden(n)) {
        strcat(buf, ", hidden");
    }

    if (ship_has_ram(n)) {
        strcat(buf, ", with ram");
    }

    if (rp_subloc(n)) {
        if (rp_subloc(n)->control.nobles) {
            strcat(buf, sout(", boarding fee %s per noble",
                             gold_s(rp_subloc(n)->control.nobles)));
        };
        if (rp_subloc(n)->control.weight) {
            strcat(buf, sout(", boarding fee %s per 1000 weight",
                             gold_s(rp_subloc(n)->control.weight)));
        };
        if (rp_subloc(n)->control.men) {
            strcat(buf, sout(", boarding fee %s per 100 men",
                             gold_s(rp_subloc(n)->control.men)));
        };
    };

#if 0
    if (fee = board_fee(n))
      strcat(buf, ", %s per 100 wt. to board", gold_s(fee));
#endif

    return sout("%s", buf);
}


/*
 *	Name, mountain province, in region foo
 *	Name, port city, in province Name [, in region foo]
 *	Name, ocean, in Sea
 *	Name, island, in Ocean [, in Sea]
 *
 *	Mountain [aa01], mountain province, in region Tollus
 *	Island [aa01], island, in Ocean [bb01]
 *	City [aa01], port city, in province Mountain [aa01]
 *	Ocean [bb02], ocean, in South Sea
 */


static char *
liner_desc_loc(int n) {
    char buf[LEN];

    sprintf(buf, "%s%s%s",
            box_name_kind(n),
            safe_haven_s(n),
            incomplete_string(n));

    if (loc_depth(n) == LOC_province &&
        rp_loc(n) && province_admin(n)) {
        if (rp_loc(n)->control.nobles) {
            strcat(buf, sout(", entrance fee %s per noble",
                             gold_s(rp_loc(n)->control.nobles)));
        };
        if (rp_loc(n)->control.weight) {
            strcat(buf, sout(", entrance fee %s per 1000 weight",
                             gold_s(rp_loc(n)->control.weight)));
        };
        if (rp_loc(n)->control.men) {
            strcat(buf, sout(", entrance fee %s per 100 men",
                             gold_s(rp_loc(n)->control.men)));
        };
    };

    if (rp_subloc(n) && first_character(n)) {
        if (rp_subloc(n)->control.nobles) {
            strcat(buf, sout(", entrance fee %s per noble",
                             gold_s(rp_subloc(n)->control.nobles)));
        };
        if (rp_subloc(n)->control.weight) {
            strcat(buf, sout(", entrance fee %s per 1000 weight",
                             gold_s(rp_subloc(n)->control.weight)));
        };
        if (rp_subloc(n)->control.men) {
            strcat(buf, sout(", entrance fee %s per 100 men",
                             gold_s(rp_subloc(n)->control.men)));
        };
    };

    if (loc_depth(n) == LOC_province &&
        rp_loc(n) && province_admin(n)) {
        if (rp_loc(n)->control.closed) {
            strcat(buf, sout(", border closed"));
        };
    };

    if (rp_subloc(n) && first_character(n)) {
        if (rp_subloc(n)->control.closed) {
            strcat(buf, sout(", closed"));
        };
    };

    if (entrance_size(n)) {
        strcat(buf, sout(", entrance size: %d", entrance_size(n)));
    };

    if (loc_depth(n) == LOC_build) {
        if (loc_defense(n)) {
            strcat(buf, sout(", defense~%d", loc_defense(n)));
        }

        if (get_effect(n, ef_improve_fort, 0, 0)) {
            strcat(buf, sout(", magical defense~%d",
                             get_effect(n, ef_improve_fort, 0, 0)));
        }

        if (loc_moat(n)) {
            strcat(buf, ", with moat");
        }

        if (loc_damage(n)) {
            strcat(buf, sout(", %d%%~damaged", (100 * loc_damage(n)) / loc_hp(n)));
        }
    }

    if (show_display_string) {
        char *s = banner(n);

        if (s && *s) {
            strcat(buf, sout(", \"%s\"", s));
        }
    }

    if (loc_hidden(n)) {
        strcat(buf, ", hidden");
    }

    if (subkind(n) == sub_mine_shaft) {
        struct entity_mine *mi = get_mine_info(n);
        if (mine_depth(n)) {
            strcat(buf, sout(", depth~%d feet",
                             (mine_depth(n) * 100) + 100));
        }
        if (mi) {
            switch (mi->shoring[mine_depth(n)]) {
                case WOODEN_SHORING:
                    strcat(buf, sout(", wooden shoring"));
                    break;
                case IRON_SHORING:
                    strcat(buf, sout(", iron shoring"));
                    break;
                case NO_SHORING:
                default:
                    strcat(buf, sout(", no shoring"));
                    break;
            }
        };
    };

    if (loc_depth(n) == LOC_subloc) {
        if (subkind(n) == sub_hades_pit) {
            strcat(buf, ", 28 days");
        } else if (subkind(n) != sub_mine_shaft &&
                   subkind(n) != sub_mine_shaft_notdone) {
            strcat(buf, ", 1 day");
        }
    }

    return sout("%s", buf);
}


static char *
mage_s(int n) {
    int a;

    if (!is_magician(n) || char_hide_mage(n)) {
        return "";
    }

    a = max_eff_aura(n);

    if (a <= 5) { return ""; }
    if (a <= 10) { return ", conjurer"; }
    if (a <= 15) { return ", mage"; }
    if (a <= 20) { return ", wizard"; }
    if (a <= 30) { return ", sorcerer"; }
    if (a <= 40) { return ", 6th black circle"; }
    if (a <= 50) { return ", 5th black circle"; }
    if (a <= 60) { return ", 4th black circle"; }
    if (a <= 70) { return ", 3rd black circle"; }
    if (a <= 80) { return ", 2nd black circle"; }

    return ", master of the black arts";
}

char *
nation_s(int n) {
    static char nation_title[80];
    static char neutral_title[80];

    nation_title[0] = 0;
    neutral_title[0] = 0;
    /*
     *  Wed Apr 16 11:29:27 1997 -- Scott Turner
     *
     *  Check for a concealment.
     *
     */
    if (get_effect(n, ef_conceal_nation, 0, 0)) {
        int new_nation = get_effect(n, ef_conceal_nation, 0, 0);
        assert(new_nation >= 1 && kind(new_nation) == T_nation);
        if (rp_nation(new_nation)->neutral) {
            strcpy(neutral_title, ", neutral");
        }
        sprintf(nation_title, ", %s%s", rp_nation(new_nation)->name, neutral_title);
    } else if (nation(n)) {
        if (rp_nation(nation(n))->neutral) {
            strcpy(neutral_title, ", neutral");
        }
        sprintf(nation_title, ", %s%s", rp_nation(nation(n))->name, neutral_title);
    } else if (subkind(n) == sub_garrison) {
        /*
         *  Special case for an uncommanded garrison?
         *
         */
        strcpy(nation_title, ", uncontrolled");
    } else if (refugee(n)) {
        strcpy(nation_title, ", refugee");
    };

    return nation_title;

};

char *
deserted_s(int n) {
    static char deserted[80];
    int pl = player(n);

    deserted[0] = 0;
    /*
     *  Wed Apr 14 10:50:34 1999 -- Scott Turner
     *
     *  Print "deserted" if a noble belongs to an NPC faction
     *  but has a body_old_lord.
     *
     */
    if (is_real_npc(pl) && body_old_lord(n)) {
        sprintf(deserted, ", deserted");
    };

    return deserted;
};

static char *
priest_s(int n) {
    static char priest_title[200];
    struct entity_religion_skill *e;

    if (!is_priest(n)) { return ""; }

    e = rp_relig_skill(is_priest(n));

    if (e && e->high_priest == n) {
        sprintf(priest_title, ", the High Priest of the %s", box_name(is_priest(n)));
    } else if (e && (e->bishops[0] == n || e->bishops[1] == n)) {
        sprintf(priest_title, ", Bishop of the %s", box_name(is_priest(n)));
    } else {
        sprintf(priest_title, ", priest of the %s", box_name(is_priest(n)));
    };
    return priest_title;

}


int show_display_string = FALSE;
char *combat_ally = "";


static char *
liner_desc_char(int n) {
    char buf[LEN];
    extern int show_combat_flag;
    char *s;
    int sk;

    strcpy(buf, box_name(n));

    sk = subkind(n);

    if (sk == sub_ni) {
        int mk = noble_item(n);
        int num = has_item(n, mk) + 1;

        if (num == 1) {
            strcat(buf, sout(", %s", plural_item_name(mk, num)));
        } else {
            strcat(buf, sout(", %s, number:~%s",
                             plural_item_name(mk, num),
                             comma_num(num)));
        }
    } else if (sk) {
        if (sk == sub_temple) {
            if (is_temple(n)) {
                strcat(buf, sout(", Temple of %s", god_name(is_temple(n))));
            } else {
                strcat(buf, sout(", undedicated temple"));
            };
        } else if (sk == sub_guild) {
            strcat(buf, sout(", %s Guild", box_name(is_guild(n))));
        } else {
            strcat(buf, sout(", %s", subkind_s[sk]));
        };
    }

    strcat(buf, nation_s(n));
    strcat(buf, deserted_s(n));
    strcat(buf, rank_s(n));
    strcat(buf, mage_s(n));
    strcat(buf, priest_s(n));
#if 0
    strcat(buf, wield_s(n));
#endif

    if (show_combat_flag) {
        if (char_behind(n)) {
            strcat(buf, sout(", behind~%d%s",
                             char_behind(n), combat_ally));
        } else {
            strcat(buf, combat_ally);
        }
    } else if (char_guard(n) && stack_leader(n) == n &&
               subkind(n) != sub_garrison) {
        strcat(buf, ", on guard");
    }

#if 0
    if (subkind(n) == 0)	/* only show lord for regular players */
    {
        int sp = lord(n);

        if (sp != indep_player && !cloak_lord(n))
            strcat(buf, sout(", of~%s", box_code_less(sp)));
    }
#endif

    if (show_display_string) {
        s = banner(n);

        if (s && *s) {
            strcat(buf, sout(", \"%s\"", s));
        }
    }

    strcat(buf, with_inventory_string(n));

    if (is_prisoner(n)) {
        strcat(buf, ", prisoner");
    }

    return sout("%s", buf);
}


static char *
liner_desc_road(int n) {
    int dest;
    char *hid = "";
    int dist;

    dest = road_dest(n);

    if (road_hidden(n)) {
        hid = ", hidden";
    }

    dist = exit_distance(loc(n), dest);

    return sout("%s, to %s%s, %d~day%s",
                box_name(n), box_name(dest), hid,
                add_ds(dist));
}


static char *
liner_desc_storm(int n) {
    char buf[LEN];
    int owner;
    struct entity_misc *p;

    sprintf(buf, "%s", box_name_kind(n));

    p = rp_misc(n);

    owner = npc_summoner(n);
    if (owner) {
        strcat(buf, sout(", owner~%s", box_code_less(owner)));
    }

    strcat(buf, sout(", strength~%s", comma_num(storm_strength(n))));

    if (p && p->npc_dir) {
        strcat(buf, sout(", heading %s", full_dir_s[p->npc_dir]));
    }

    return sout("%s", buf);
}


/*
 *  Viewed from outside
 */

char *
liner_desc(int n) {

    switch (kind(n)) {
        case T_ship:
            return liner_desc_ship(n);
        case T_loc:
            return liner_desc_loc(n);
        case T_char:
            return liner_desc_char(n);
        case T_road:
            return liner_desc_road(n);
        case T_storm:
            return liner_desc_storm(n);

        default:
            assert(FALSE);
    }
}


char *
highlight_units(int who, int n, int depth) {

    assert(depth >= 3);
    assert(indent == 0);

    if (kind(who) == T_player && player(n) == who) {
        return sout(" *%s", &spaces[spaces_len - (depth - 2)]);
    }

    return &spaces[spaces_len - depth];
}


void
show_chars_below(int who, int n) {
    int i;

    assert(valid_box(who));

    indent += 3;
    loop_char_here(n, i)
            {
                assert(valid_box(who));
                wiout(who, 3, "%s", liner_desc(i));
            }
    next_char_here;
    indent -= 3;
}


static void
show_chars_below_highlight(int who, int n, int depth, int where) {
    int i;

    depth += 3;

    loop_char_here(n, i)
            {
                tagout(who, "<tag type=char_here id=%d where=%d under=%d>",
                       i, where, n);
                wiout(who, depth, "%s%s",
                      highlight_units(who, i, depth),
                      liner_desc(i));
                tagout(who, "</tag type=char_here>");
            }
    next_char_here;
}


void
show_owner_stack(int who, int n) {
    int i;
    int depth;
    int first = TRUE;

    depth = indent + 3;
    indent = 0;

    loop_here(n, i)
            {
                if (kind(i) == T_char) {
                    if (!first && char_really_hidden(i)) {
                        continue;
                    }

                    wiout(who, depth, "%s%s%s",
                          highlight_units(who, i, depth),
                          liner_desc(i),
                          display_with(i));

                    show_chars_below_highlight(who, i, depth, n);

                    first = FALSE;
                }
            }
    next_here;

    indent = depth - 3;
}


static void
show_chars_here(int who, int where) {
    int first = TRUE;
    int i;
    int depth;
    char *flying = "";

    tagout(who, "<tag type=chars_here where=%d>", where);
    if (loc_depth(where) == LOC_province &&
        weather_here(where, sub_fog) &&
        is_priest(who) != sk_domingo) {
        out(who, "");
        out(who, "No one can be seen through the fog.");
        return;
    }

    if (loc_depth(where) == LOC_province &&
        weather_here(where, sub_mist) &&
        is_priest(who) != sk_domingo) {
        out(who, "");
        out(who, "A dank, unhealthy mist conceals everything.");
        return;
    }

    depth = indent;
    indent = 0;

    if (subkind(where) == sub_ocean) {
        flying = ", flying";
    }

    loop_here(where, i)
            {
                if (kind(i) == T_char) {
                    if (char_really_hidden(i)) {
                        continue;
                    }

                    if (first) {
                        first = FALSE;
                        out(who, "");
                        out(who, "Seen here:");
                        depth += 3;
                    }

                    tagout(who, "<tag type=char_here id=%d where=%d under=0>",
                           i, where);
                    wiout(who, depth, "%s%s%s%s",
                          highlight_units(who, i, depth),
                          liner_desc(i),
                          flying,
                          display_with(i));
                    tagout(who, "</tag type=char_here>");

                    show_chars_below_highlight(who, i, depth, where);
                }
            }
    next_here;

    if (!first) {
        depth -= 3;
    }

    indent = depth;
    tagout(who, "</tag type=chars_here>");
}


static void
show_inner_locs(int who, int where) {
    int first = TRUE;
    int i;

    loop_here(where, i)
            {
                if (is_loc_or_ship(i)) {
                    if (loc_hidden(i) &&
                        !test_known(who, i) &&
                        !see_all(who)) {
                        continue;
                    }

                    if (first) {
                        first = FALSE;
                        if (subkind(who) != sub_mine_shaft) { indent += 3; }
                    }

                    wout(who, "%s%s", liner_desc(i), display_owner(i));
                    show_owner_stack(who, i);
                    show_loc_barrier(who, i);
                    show_inner_locs(who, i);
                }
            }
    next_here;

    if (!first) {
        indent -= 3;
    }
}


static void
show_sublocs_here(int who, int where) {
    int first = TRUE;
    int i;
    struct entity_subloc *p;

    tagout(who, "<tag type=sublocs_here loc=%d>", where);
    loop_here(where, i)
            {
                if (kind(i) == T_loc) {
                    if (loc_hidden(i) &&
                        !test_known(who, i) &&
                        !see_all(who)) {
                        continue;
                    }

                    if (first) {
                        first = FALSE;
                        out(who, "");
                        out(who, "Inner locations:");
                        indent += 3;
                    }

                    tagout(who, "<tag type=subloc id=%d where=%d>",
                           i, where);
                    if (subkind(i) == sub_city) {
                        wout(who, "%s", liner_desc(i));
                        show_loc_barrier(who, i);
                    } else {
                        wout(who, "%s%s", liner_desc(i),
                             display_owner(i));
                        show_owner_stack(who, i);
                        show_loc_barrier(who, i);
                        show_inner_locs(who, i);
                    }
                    tagout(who, "</tag type=subloc>");
                }
            }
    next_here;

    p = rp_subloc(where);

    if (p) {
        for (i = 0; i < ilist_len(p->link_from); i++) {
            if (loc_hidden(p->link_from[i]) &&
                !test_known(who, p->link_from[i]) &&
                !see_all(who)) {
                continue;
            }

            /*
             *  Mon Dec  9 15:19:58 1996 -- Scott Turner
             *
             *  All links are "open" now...
             *
             */
            /*		if (loc_link_open(p->link_from[i])) */
            if (p->link_from[i]) {
                if (first) {
                    first = FALSE;
                    out(who, "");
                    out(who, "Inner locations:");
                    indent += 3;
                }

                tagout(who, "<tag type=subloc id=%d where=%d>",
                       p->link_from[i], where);
                wout(who, "%s", liner_desc(p->link_from[i]));
                tagout(who, "</tag type=subloc>");
            }

        }
    }

    if (!first) {
        indent -= 3;
    }
    tagout(who, "<tag type=sublocs_here>");
}


static void
show_ships_here(int who, int where) {
    int first = TRUE;
    int i;

    tagout(who, "<tag type=ships_here where=%d>", where);

    loop_here(where, i)
            {
                if (kind(i) == T_ship) {
                    if (loc_hidden(i) &&
                        !test_known(who, i) &&
                        !see_all(who)) {
                        continue;
                    }

                    if (first) {
                        first = FALSE;

                        out(who, "");
                        if (subkind(where) == sub_ocean) {
                            out(who, "Ships sighted:");
                        } else {
                            out(who, "Ships docked at port:");
                        }

                        indent += 3;
                    }

                    tagout(who, "<tag type=ship_here id=%d where=%d "
                                "owner=%d>",
                           i, where,
                           first_character(i));
                    wiout(who, 3, "%s%s", liner_desc(i), display_owner(i));
                    show_owner_stack(who, i);
                    show_loc_barrier(who, i);
                    tagout(who, "</tag type=ship_here>");
                }
            }
    next_here;

    if (!first) {
        indent -= 3;
    }

    tagout(who, "</tag type=ships_here>");
}


static void
show_nearby_cities(int who, int where) {
    struct entity_subloc *p;
    int i;
    char *s;

    p = rp_subloc(where);

    if (p == NULL || ilist_len(p->near_cities) < 1) {
        return;
    }

    tagout(who, "<tag type=nearby_cities loc=%d>", where);
    out(who, "");
    out(who, "Cities rumored to be nearby:");
    indent += 3;
    for (i = 0; i < ilist_len(p->near_cities); i++) {
        if (safe_haven(p->near_cities[i])) {
            s = ", safe haven";
        } else {
            s = "";
        }

        tagout(who, "<tag type=nearby_city loc=%d city=%d name=\"%s\" "
                    "province=%d province_name=\"%s\">",
               where,
               p->near_cities[i],
               box_name(p->near_cities[i]),
               province(p->near_cities[i]),
               box_name(province(p->near_cities[i])));
        out(who, "%s, in %s%s",
            box_name(p->near_cities[i]),
            box_name(province(p->near_cities[i])), s);
    }
    indent -= 3;
    tagout(who, "</tag type=nearby_cities>");
}


static void
show_loc_skills(int who, int where) {
    int i;
    char *s = "";

    tagout(who, "<tag type=taught_here loc=%d>", where);

    loop_loc_teach(where, i)
            {
                tagout(who, "<tag type=taught_skill id=%d name=\"%s\">",
                       i, box_name(i));
                s = comma_append(s, box_name(i));
            }
    next_loc_teach;

    if (s && *s) {
        out(who, "");
        out(who, "Skills taught here:");
        indent += 3;
        wout(who, "%s", s);
        indent -= 3;
    }

    tagout(who, "</tag type=taught_here>");
}


void
show_loc_posts(int who, int where, int show_full_loc) {
    int post;
    int i;
    int flag = TRUE;
    int first;
    char **l;

    tagout(who, "<tag type=loc_posts>");
    loop_here(where, post)
            {
                if (kind(post) != T_post) {
                    continue;
                }

                if (rp_misc(post) == NULL ||
                    ilist_len(rp_misc(post)->post_txt) < 1) {
                    assert(FALSE);    /* what happened to the post? */
                    continue;
                }

                l = rp_misc(post)->post_txt;

                if (flag) {
                    out(who, "");
                    wout(who, "Posted in %s:",
                         show_full_loc ? box_name(where) :
                         just_name(where));
                    flag = FALSE;
                    indent += 3;
                } else {
                    out(who, "");
                }

                if (item_creator(post)) {
                    wout(who, "Posted by %s:",
                         box_name(item_creator(post)));
                } else {
                    wout(who, "Posted:");
                }

                indent += 3;

                first = TRUE;

                for (i = 0; i < ilist_len(l); i++) {
                    wout(who, "%s%s%s",
                         first ? "\"" : "",
                         l[i],
                         i + 1 == ilist_len(l) ? "\"" : "");

                    if (first) {
                        first = FALSE;
                        indent += 1;
                    }
                }

                if (!first) {
                    indent -= 1;
                }

                indent -= 3;
            }
    next_here;

    if (!flag) {
        indent -= 3;
    }
    tagout(who, "</tag type=loc_posts>");
}


static void show_weather(int who, int where) {
    int rain, wind, fog, mist;

    rain = weather_here(where, sub_rain);
    wind = weather_here(where, sub_wind);
    fog = weather_here(where, sub_fog);
    mist = weather_here(where, sub_mist);

    if (!rain && !wind && !fog && !mist) {
        return;
    }

    tagout(who, "<tag type=weather loc=%d rain=%d wind=%d fog=%d mist=%d>",
           where, rain, wind, fog, mist);

    out(who, "");

    if (rain) {
        out(who, "It is raining.");
    }

    if (wind) {
        out(who, "It is windy.");
    }

    if (fog) {
        out(who, "The province is blanketed in fog.");
    }

    if (mist) {
        out(who, "The province is covered with a dank mist.");
    }

    if (can_see_weather_here(who, where)) {
        int i;
        int first = TRUE;

        loop_here(where, i)
                {
                    if (kind(i) != T_storm) {
                        continue;
                    }

                    if (first) {
                        indent += 3;
                        first = FALSE;
                    }

                    tagout(who, "<tag type=storm id=%d loc=%d>",
                           i, where);
                    wout(who, "%s", liner_desc(i));
                    tagout(who, "<tag type=storm>");
                }
        next_here;

        if (!first) {
            indent -= 3;
        }
    }
    tagout(who, "</tag type=weather>");
}


static void
show_loc_ruler(int who, int where) {
    int garr;
    int castle;
    int ruler;
    char buf[LEN];
    int fee;

    if (loc_depth(where) != LOC_province || subkind(where) == sub_ocean) {
        return;
    }

    garr = garrison_here(where);

    if (garr == 0) {
        return;
    }

    castle = garrison_castle(garr);
    ruler = top_ruler(garr);

    if (castle) {
        int prov = province(castle);

        tagout(who, "<tag type=ruler id=%d castle=%d province=%d ruler=%d>",
               castle, prov, ruler);

        out(who, "Province controlled by %s, in %s",
            box_name_kind(castle), box_name(prov));

        if (ruler) {
            out(who, "Ruled by %s%s",
                box_name(ruler), rank_s(ruler));
        }

        tagout(who, "</tag type=ruler>");
    }

    if (ruler == 0) { return; }

    if (rp_loc(where)->control.closed) {
        tagout(who, "<tag type=border_closed id=%d>", where);
        out(who, "Border closed.");
        tagout(who, "</tag type=border_closed>");
    };

    if (rp_loc(where)->control.nobles) {
        strcat(buf, sout("%s per noble", gold_s(rp_loc(where)->control.nobles)));
        fee = 1;
    };

    if (rp_loc(where)->control.weight) {
        if (fee) { strcat(buf, ", "); }
        strcat(buf, sout("entrance fee %s per 1000 weight",
                         gold_s(rp_loc(where)->control.weight)));
        fee = 1;
    };
    if (rp_loc(where)->control.men) {
        if (fee) { strcat(buf, ", "); }
        strcat(buf, sout(", entrance fee %s per 100 men",
                         gold_s(rp_loc(where)->control.men)));
        fee = 1;
    };

    if (fee) {
        tagout(who, "<tag type=fees id=%d nobles=%d weight=%d men=%d>",
               where, rp_loc(where)->control.nobles,
               rp_loc(where)->control.weight,
               rp_loc(where)->control.men);
        strcpy(buf, "Entrance fees: ");
        out(who, buf);
        tagout(who, "</tag type=fees>");
    };

    out(who, "");
}

/*
 *  Don't include the leading location name, kind, etc. on the location 
 *  report.  Handled with a global since we don't have default parameters.
 */

int show_loc_no_header = FALSE;

void
show_loc(int who, int where) {
    int pil;

    assert(valid_box(where));
    if (loc_depth(where) < LOC_province) { return; }

    show_display_string = TRUE;  /* previously only for show_chars_here */

    tagout(who, "</tag type=loc_report>");
    tagout(who, "<tag type=location id=%d>", where);

    if (!show_loc_no_header) {
        wout(who, "%s", show_loc_header(where));
        out(who, "");
    }

    if (show_loc_barrier(who, where)) {
        out(who, "");
    }

    if (pil = loc_pillage(where)) {
        tagout(who, "<tag type=pillaged id=%d>", where);
        wout(who, "Recent pillaging has frightened the peasants.");
        out(who, "");
        tagout(who, "</tag type=pillaged>");
    }

    show_loc_ruler(who, where);

    list_sailable_routes(who, where);
    list_exits(who, where);
    show_loc_stats(who, where);
    show_nearby_cities(who, where);
    show_loc_skills(who, where);

    if (subkind(where) == sub_city ||
        is_guild(where) == sk_trading) {
        market_report(who, where);
    }

    show_sublocs_here(who, where);
    show_loc_posts(who, where, FALSE);
    show_weather(who, where);
    show_chars_here(who, where);
    show_ships_here(who, where);

    show_display_string = FALSE;

    tagout(who, "</tag type=location>");

}

