
#include    <stdio.h>
#include    <string.h>
#include    "z.h"
#include    "oly.h"


static struct admit *
rp_admit(int pl, int targ) {
    int i;
    struct entity_player *p;

    assert(kind(pl) == T_player);
    p = p_player(pl);

    for (i = 0; i < ilist_len(p->admits); i++) {
        if (p->admits[i]->targ == targ) {
            return p->admits[i];
        }
    }

    return NULL;
}


static struct admit *
p_admit(int pl, int targ) {
    int i;
    struct entity_player *p;
    struct admit *new;

    assert(kind(pl) == T_player);
    p = p_player(pl);

    for (i = 0; i < ilist_len(p->admits); i++) {
        if (p->admits[i]->targ == targ) {
            return p->admits[i];
        }
    }

    new = my_malloc(sizeof(*new));
    new->targ = targ;

    ilist_append((ilist *) &p->admits, (int) new);

    return new;
}


/*
 *  Will pl admit who into targ?
 */

int
will_admit(int pl, int who, int targ) {
    struct admit *p;
    int found;
    int found_pl;
    int found_nation;

    /*
     *  Fri Nov  5 13:02:00 1999 -- Scott Turner
     *
     *  For purposes of admission, a garrison is treated as if it were
     *  the ruler of the castle, e.g., the garrison will admit you if the
     *  ruler of the castle would admit you.  This is a little odd, perhaps,
     *  but that's the way the rules are currently written.
     *
     */
    if (subkind(targ) == sub_garrison) {
        targ = province_admin(targ);
        pl = targ;
        if (!valid_box(targ)) { return FALSE; }
    };

    pl = player(pl);

    if (player(who) == pl) {
        return TRUE;
    }

    p = rp_admit(pl, targ);

    if (p == NULL) {
        return FALSE;
    }

    found = ilist_lookup(p->l, who) >= 0;
    found_pl = ilist_lookup(p->l, player(who)) >= 0;
    found_nation = ilist_lookup(p->l, nation(who)) >= 0;

    /*
     * Wed Jan 20 12:59:51 1999 -- Scott Turner
     *
     * If p->sense is true, then we have unit and player
     * exclusion, e.g., if the unit or player is true, then
     * don't admit them!.
     *
     */
    if (p->sense) {
        if (found || found_pl || found_nation) { return FALSE; }
        return TRUE;
    } else {
        if (found || found_pl || found_nation) {
            return TRUE;
        }
        return FALSE;
    }
}

/*
 *  Wed Jan 20 12:23:16 1999 -- Scott Turner
 *
 *  Add nation admits.
 *
 */
int
v_admit(struct command *c) {
    int targ = c->a;
    int pl = player(c->who);
    struct admit *p;

    if (!valid_box(targ)) {
        wout(c->who, "Must specify an entity for admit.");
        return FALSE;
    }

    cmd_shift(c);

    p = p_admit(pl, targ);

    if (numargs(c) == 0) {
        p->sense = FALSE;
        ilist_clear(&p->l);
    }

    while (numargs(c) > 0) {
        if (i_strcmp(c->parse[1], "all") == 0) {
            p->sense = TRUE;
        } else if (find_nation(c->parse[1])) {
            /*
             *  We can stick the nation # on there because we
             *  can't have a box number that low (hopefully!).
             *
             */
            ilist_add(&p->l, find_nation(c->parse[1]));
            wout(c->who, "Admitting '%s' to %s.",
                 rp_nation(find_nation(c->parse[1]))->name,
                 box_code_less(targ));
        } else if (kind(c->a) == T_char ||
                   kind(c->a) == T_player ||
                   kind(c->a) == T_unform) {
            ilist_add(&p->l, c->a);
        } else {
            wout(c->who, "%s isn't a valid entity to admit.",
                 c->parse[1]);
        }

        cmd_shift(c);
    }

    return TRUE;
}


static int
admit_comp(a, b)
        struct admit **a;
        struct admit **b;
{

    return (*a)->targ - (*b)->targ;
}


static void
print_admit_sup(int pl, struct admit *p) {
    char buf[LEN];
    int i;
    int count = 0;

    sprintf(buf, "admit %4s", box_code_less(p->targ));

    if (p->sense) {
        strcat(buf, "  all");
        count++;
    }

    for (i = 0; i < ilist_len(p->l); i++) {
        if (!valid_box(p->l[i])) {
            continue;
        }

        if (++count >= 12) {
            out(pl, "%s", buf);
#if 0
            sprintf(buf, "admit %4s", p->targ);
#else
            strcpy(buf, "          ");
#endif
            count = 1;
        }

        if (kind(p->l[i]) == T_nation) {
            strcat(buf, sout(" %s", rp_nation(p->l[i])->name));
        } else {
            strcat(buf, sout(" %4s", box_code_less(p->l[i])));
        };
    }

    if (count) {
        out(pl, "%s", buf);
    }
}


void
print_admit(int pl) {
    struct entity_player *p;
    int i;
    int first = TRUE;

    assert(kind(pl) == T_player);

    p = p_player(pl);

    if (ilist_len(p->admits) > 0) {
        qsort(p->admits, ilist_len(p->admits), sizeof(int), admit_comp);
    }

    for (i = 0; i < ilist_len(p->admits); i++) {
        if (valid_box(p->admits[i]->targ)) {
            if (first) {
                tagout(pl, "<tag type=header>");
                out(pl, "");
                tagout(pl, "</tag type=header>");
                out(pl, "Admit permissions:");
                out(pl, "");
                indent += 3;
                first = FALSE;
            }

            print_admit_sup(pl, p->admits[i]);
        }
    }

    if (!first) {
        indent -= 3;
    }
}


void
clear_all_att(int who) {
    struct att_ent *p;

    p = rp_disp(who);
    if (p == NULL) {
        return;
    }

    ilist_clear(&p->neutral);
    ilist_clear(&p->hostile);
    ilist_clear(&p->defend);
}

void
clear_att(int who, int disp) {
    struct att_ent *p;

    p = rp_disp(who);
    if (p == NULL) {
        return;
    }

    switch (disp) {
        case NEUTRAL:
            ilist_clear(&p->neutral);
            break;

        case HOSTILE:
            ilist_clear(&p->hostile);
            break;

        case DEFEND:
            ilist_clear(&p->defend);
            break;

        case ATT_NONE:
            break;

        default:
            assert(FALSE);
    }
}

void
set_att(int who, int targ, int disp) {
    struct att_ent *p;
    extern int int_comp();

    p = p_disp(who);

    ilist_rem_value(&p->neutral, targ);
    ilist_rem_value(&p->hostile, targ);
    ilist_rem_value(&p->defend, targ);

    switch (disp) {
        case NEUTRAL:
            ilist_append(&p->neutral, targ);
            qsort(p->neutral, ilist_len(p->neutral), sizeof(int), int_comp);
            break;

        case HOSTILE:
            ilist_append(&p->hostile, targ);
            qsort(p->hostile, ilist_len(p->hostile), sizeof(int), int_comp);
            break;

        case DEFEND:
            ilist_append(&p->defend, targ);
            qsort(p->defend, ilist_len(p->defend), sizeof(int), int_comp);
            break;

        case ATT_NONE:
            break;

        default:
            assert(FALSE);
    }
}

/*
 *  Mon May 18 19:07:03 1998 -- Scott Turner
 *
 *  Macro doesn't work because of conceal_nation_ef...
 *
 #define nation(n)	(n && player(n) && rp_player(player(n)) ?
			 rp_player(player(n))->nation : 0)
 *
 */
int
nation(int who) {
    int n, pl;
    /*
     *  Sanity checks.
     *
     */
    if (!valid_box(who)) { return 0; }
    /*
     *  Return the phony nation, if any!
     *
     */
    n = get_effect(who, ef_conceal_nation, 0, 0);
    if (n) {
        assert(kind(n) == T_nation);
        return n;
    };
    /*
     *  A garrison ought to be considered to be of the nation
     *  of its lord.
     *
     */
    if (subkind(who) == sub_garrison) {
        int ruler = province_admin(who);
        if (ruler && rp_player(player(ruler))) {
            return rp_player(player(ruler))->nation;
        };
    };
    /*
     *  A deserted noble ought to be considered still of the nation
     *  of his old lord, if he has one.
     *
     */
    pl = player(who);
    if (is_real_npc(pl) && body_old_lord(who) &&
        rp_player(player(body_old_lord(who)))) {
        return rp_player(player(body_old_lord(who)))->nation;
    };
    /*
     *  Otherwise...
     *
     */
    if (pl && rp_player(pl)) {
        return rp_player(pl)->nation;
    }

    return 0;
}

/*
 *  Try to find a nation.
 *
 */
int
find_nation(char *name) {
    int i;
    loop_nation(i)
            {
                if (fuzzy_strcmp(rp_nation(i)->name, name) ||
                    strncasecmp(rp_nation(i)->name, name, strlen(name)) == 0) {
                    return i;
                }
            }next_nation;
    return 0;
};

/*
 *  Tue Jan 12 12:11:32 1999 -- Scott Turner
 *
 *  Added support for hostile to monsters.
 *
 */
int
is_hostile(who, targ)
        int who;
        int targ;
{
    struct att_ent *p;

    if (player(who) == player(targ)) {
        return FALSE;
    }

    if (subkind(who) == sub_garrison) {
        struct entity_misc *p;

        p = rp_misc(who);
        if (p && ilist_lookup(p->garr_host, targ) >= 0) {
            return TRUE;
        }
    }

    if (p = rp_disp(who)) {
        if (ilist_lookup(p->hostile, targ) >= 0) {
            return TRUE;
        }
        /*
         *  Mon May 18 19:04:22 1998 -- Scott Turner
         *
         *  Might be a nation...
         *
         */
        if (nation(targ) && ilist_lookup(p->hostile, nation(targ)) >= 0) {
            return TRUE;
        };
        /*
         *  Tue Jan 12 12:09:53 1999 -- Scott Turner
         *
         *  Might be a "monster"
         *
         */
        if (!is_real_npc(who) &&
            is_real_npc(targ) &&
            kind(targ) == T_char &&
            subkind(targ) == sub_ni &&
            ilist_lookup(p->hostile, MONSTER_ATT) >= 0) {
            return TRUE;
        }
    }

    if (p = rp_disp(player(who))) {
        if (ilist_lookup(p->hostile, targ) >= 0) {
            return TRUE;
        }
        /*
         *  Mon May 18 19:04:22 1998 -- Scott Turner
         *
         *  Might be a nation...
         *
         */
        if (nation(targ) && ilist_lookup(p->hostile, nation(targ)) >= 0) {
            return TRUE;
        };
        /*
         *  Tue Jan 12 12:09:53 1999 -- Scott Turner
         *
         *  Might be a "monster"
         *
         */
        if (!is_real_npc(who) &&
            is_real_npc(targ) &&
            kind(targ) == T_char &&
            subkind(targ) == sub_ni &&
            ilist_lookup(p->hostile, MONSTER_ATT) >= 0) {
            return TRUE;
        }
    }
    return FALSE;
}


int
is_defend(who, targ)
        int who;
        int targ;
{
    struct att_ent *p;
    int pl;

    /*
     *  Mon Mar  3 13:24:58 1997 -- Scott Turner
     *
     *  All npcs defend each other!
     *
     *  Sun Mar  9 20:57:06 1997 -- Scott Turner
     *
     *  A little simplistic.  But we should have all intelligent
     *  NPCs defend each other, and all animals of the same type.
     *
     */
    if (is_real_npc(who) && is_real_npc(targ) &&
        npc_program(who) &&
        npc_program(who) != PROG_dumb_monster &&
        npc_program(targ) == npc_program(who)) {
        wout(who, "Smart enough to help %s in battle.",
             box_name(targ));
        return TRUE;
    };

    if (is_real_npc(who) && is_real_npc(targ) &&
        subkind(who) == sub_ni &&
        subkind(targ) == sub_ni &&
        noble_item(who) == noble_item(targ)) {
        wout(who, "Rushing to the defense of similar beast %s.",
             box_name(targ));
        return TRUE;
    };

    if (is_hostile(who, targ)) {
        return FALSE;
    }

    if (p = rp_disp(who)) {
        if (ilist_lookup(p->defend, targ) >= 0) {
            return TRUE;
        }
        if (ilist_lookup(p->neutral, targ) >= 0) {
            return FALSE;
        }

        if (ilist_lookup(p->defend, player(targ)) >= 0) {
            return TRUE;
        }
        if (ilist_lookup(p->neutral, player(targ)) >= 0) {
            return FALSE;
        }
        /*
         *  Mon May 18 19:04:22 1998 -- Scott Turner
         *
         *  Might be a nation...
         *
         */
        if (nation(targ) &&
            ilist_lookup(p->defend, nation(targ)) >= 0) {
            return TRUE;
        };
        if (nation(targ) &&
            ilist_lookup(p->neutral, nation(targ)) >= 0) {
            return FALSE;
        };
    }

    pl = player(who);

    if (p = rp_disp(pl)) {
        if (ilist_lookup(p->defend, targ) >= 0) {
            return TRUE;
        }
        if (ilist_lookup(p->neutral, targ) >= 0) {
            return FALSE;
        }

        if (ilist_lookup(p->defend, player(targ)) >= 0) {
            return TRUE;
        }
        if (ilist_lookup(p->neutral, player(targ)) >= 0) {
            return FALSE;
        }

        /*
         *  Mon May 18 19:04:22 1998 -- Scott Turner
         *
         *  Might be a nation...
         *
         */
        if (nation(targ) &&
            ilist_lookup(p->defend, nation(targ)) >= 0) {
            return TRUE;
        };
        if (nation(targ) &&
            ilist_lookup(p->neutral, nation(targ)) >= 0) {
            return FALSE;
        };
    }

    if (pl == player(targ) && pl != indep_player) {
        if (cloak_lord(who)) {
            return FALSE;
        }
        return TRUE;
    }

    return FALSE;
}

/*
 *  Mon May 18 18:47:41 1998 -- Scott Turner
 *
 *  Accept nation names as well.
 *
 *  Tue Jan 12 11:58:09 1999 -- Scott Turner
 *
 *  Accept "monster" as well?
 *
 */
static char *verbs[] = {
        "no attitude",
        "neutral",
        "hostile",
        "defend"
};

static int
v_set_att(struct command *c, int k) {
    int n;

    if (numargs(c) == 0) {
        /*
         *  Clear a list.
         *
         */
        wout(c->who, "Cleared %s list.", verbs[k]);
        clear_att(c->who, k);
        return TRUE;
    };

    while (numargs(c) > 0) {
        if (!valid_box(c->a)) {
            /*
             *  Look for a nation name.
             *
             */
            n = find_nation(c->parse[1]);
            if (n) {
                set_att(c->who, n, k);
                wout(c->who, "Declared %s toward nation %s.",
                     verbs[k], rp_nation(n)->name);
            } else {
                /*
                 *  Might be "monster"
                 *
                 */
                if (fuzzy_strcmp(c->parse[1], "monster") ||
                    fuzzy_strcmp(c->parse[1], "monsters")) {
                    set_att(c->who, MONSTER_ATT, k);
                } else {
                    wout(c->who, "%s is not a valid entity.", c->parse[1]);
                };
            };
        } else if (k == HOSTILE && player(c->who) == player(c->a) &&
                   player(c->who) != indep_player) {
            wout(c->who, "Can't be hostile to a unit in the "
                         "same faction.");
        } else {
            set_att(c->who, c->a, k);
            wout(c->who, "Declared %s towards %s.",
                 verbs[k], box_code(c->a));
        }
        cmd_shift(c);
    }
    return TRUE;
}

int
v_hostile(struct command *c) {
    return v_set_att(c, HOSTILE);
}


int
v_defend(struct command *c) {
    return v_set_att(c, DEFEND);
}


int
v_neutral(struct command *c) {
    return v_set_att(c, NEUTRAL);
}


int
v_att_clear(struct command *c) {
    return v_set_att(c, ATT_NONE);
}


static void
print_att_sup(int who, ilist l, char *header, int *first) {
    int i;
    int count = 0;
    char buf[LEN];
    extern int int_comp();

    if (ilist_len(l) == 0) {
        return;
    }

    strcpy(buf, header);

    qsort(l, ilist_len(l), sizeof(int), int_comp);

    for (i = 0; i < ilist_len(l); i++) {
        if (l[i] != MONSTER_ATT && !valid_box(l[i])) {
            continue;
        }

        if (*first) {
            out(who, "");
            out(who, "Declared attitudes:");
            out(who, "");
            indent += 3;
            *first = FALSE;
        }

        if (++count >= 12) {
            out(who, "%s", buf);
            sprintf(buf, "%s",
                    &spaces[spaces_len - strlen(header)]);
            count = 1;
        }

        if (l[i] == MONSTER_ATT) {
            strcat(buf, " Monsters ");
        } else if (kind(l[i]) == T_nation) {
            strcat(buf, sout(" %s", rp_nation(l[i])->name));
        } else {
            strcat(buf, sout(" %4s", box_code_less(l[i])));
        };
    }

    if (count) {
        out(who, "%s", buf);
    }
}


void
print_att(int who, int n) {
    int first = TRUE;
    struct att_ent *p;

    p = rp_disp(n);

    if (p == NULL) {
        return;
    }

    print_att_sup(who, p->hostile, "hostile", &first);
    print_att_sup(who, p->neutral, "neutral", &first);
    print_att_sup(who, p->defend, "defend ", &first);

    if (!first) {
        indent -= 3;
    }
}

