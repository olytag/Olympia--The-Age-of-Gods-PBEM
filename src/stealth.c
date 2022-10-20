
#include    <stdio.h>
#include    "z.h"
#include    "oly.h"

int
v_spy_inv(struct command *c) {
    int target = c->a;

    if (!check_char_here(c->who, target)) {
        return FALSE;
    }

    return TRUE;
}


int
d_spy_inv(struct command *c) {
    int target = c->a;

    if (!check_still_here(c->who, target)) {
        return FALSE;
    }

    wout(c->who, "Discovered the inventory of %s:", box_name(target));
    show_char_inventory(c->who, target, "");

    return TRUE;
}


int
v_spy_skills(struct command *c) {
    int target = c->a;

    if (!check_char_here(c->who, target)) {
        return FALSE;
    }

    return TRUE;
}


int
d_spy_skills(struct command *c) {
    int target = c->a;

    if (!check_still_here(c->who, target)) {
        return FALSE;
    }

    wout(c->who, "Learned the skills of %s:", box_name(target));
    list_skills(c->who, target, "");

    return TRUE;
}


int
v_spy_lord(struct command *c) {
    int target = c->a;

    if (!check_char_here(c->who, target)) {
        return FALSE;
    }

    return TRUE;
}


int
d_spy_lord(struct command *c) {
    int target = c->a;
    int parent;

    if (!check_still_here(c->who, target)) {
        return FALSE;
    }

    if (cloak_lord(target)) {
        wout(c->who, "Failed to learn the lord of %s.",
             box_code(target));
        return FALSE;
    }

    parent = player(target);

    assert(valid_box(parent));

    wout(c->who, "%s is sworn to %s.", box_name(target),
         box_name(parent));

    return TRUE;
}


int
v_hide(struct command *c) {
    int flag = c->a;

    if (!check_skill(c->who, sk_hide_self)) {
        return FALSE;
    }

    if (flag && !char_alone_stealth(c->who)) {
        wout(c->who, "Must be alone to hide.");
        return FALSE;
    }

    if (!flag) {
        p_magic(c->who)->hide_self = FALSE;
        wout(c->who, "No longer hidden.");

        c->wait = 0;
        c->inhibit_finish = TRUE;
        return TRUE;
    }

    return TRUE;
}


int
d_hide(struct command *c) {

    if (!char_alone_stealth(c->who)) {
        wout(c->who, "Must be alone to hide.");
        return FALSE;
    }

    p_magic(c->who)->hide_self = TRUE;

    wout(c->who, "Now hidden.");
    return TRUE;
}


int
v_sneak(struct command *c) {
    struct exit_view *v;
    int where = subloc(c->who);
    int outside = subloc(where);
    int dest;

    if (!char_alone_stealth(c->who)) {
        wout(c->who, "Must be alone in order to sneak.");
        return FALSE;
    }

    if (numargs(c) > 0) {
        v = parse_exit_dir(c, where, "sneak");

        if (v == NULL) {
            return FALSE;
        }

        dest = v->destination;
    } else {
        dest = outside;
    }

    if (dest == outside) {
        if (loc_depth(where) != LOC_build) {
            wout(c->who, "Not in a structure.");
            return FALSE;
        }

        if (subkind(outside) == sub_ocean) {
            wout(c->who, "May not leave while on the ocean.");
            return FALSE;
        }

        return TRUE;
    }

    if (loc_depth(v->destination) != LOC_build) {
        wout(c->who, "May only sneak into buildings and ships.");
        return FALSE;
    }

    if (v->impassable) {
        wout(c->who, "That route is impassable.");
        return FALSE;
    }

    if (v->in_transit) {
        wout(c->who, "%s is underway.  Boarding is not "
                     "possible.", box_name(v->destination));
        return FALSE;
    }

    return TRUE;
}


int
d_sneak(struct command *c) {
    struct exit_view *v;
    int where = subloc(c->who);
    int outside = subloc(where);
    int dest;

    if (!char_alone_stealth(c->who)) {
        wout(c->who, "Must be alone in order to sneak.");
        return FALSE;
    }

    if (numargs(c) > 0) {
        v = parse_exit_dir(c, where, "sneak");

        if (v == NULL) {
            return FALSE;
        }

        dest = v->destination;
    } else {
        dest = outside;
    }

    if (dest == outside) {
        if (loc_depth(where) != LOC_build) {
            wout(c->who, "Not in a structure.");
            return FALSE;
        }

        if (subkind(outside) == sub_ocean) {
            wout(c->who, "May not leave while on the ocean.");
            return FALSE;
        }

        move_stack(c->who, outside);
        wout(c->who, "Now outside of %s.", box_name(where));

        return TRUE;
    }

    if (loc_depth(v->destination) != LOC_build) {
        wout(c->who, "May only sneak into buildings and ships.");
        return FALSE;
    }

    if (v->impassable) {
        wout(c->who, "That route is impassable.");
        return FALSE;
    }

    if (v->in_transit) {
        wout(c->who, "%s is underway.  Boarding is not "
                     "possible.", box_name(v->destination));
        return FALSE;
    }

    move_stack(c->who, v->destination);
    wout(c->who, "Now inside %s.", box_name(v->destination));

    return TRUE;
}


void
clear_contacts(int stack) {
    int i;

    if (kind(stack) == T_char) {
        loop_stack(stack, i)
                {
                    ilist_clear(&(p_char(i)->contact));
                }
        next_stack;
    }
}


static void
add_contact(int a, int b) {

    assert(kind(a) == T_char);

    ilist_append(&(p_char(a)->contact), b);
}


int
v_contact(struct command *c) {

    while (numargs(c) > 0) {
        if (kind(c->a) != T_char && kind(c->a) != T_player) {
            wout(c->who, "%s is not a character or player entity.",
                 c->parse[1]);
        } else {
            ilist_append(&p_char(c->who)->contact, c->a);
        }

        cmd_shift(c);
    }

    return TRUE;
}


int
v_seek(struct command *c) {
    int target = c->a;

    if (target)    /* target specified */
    {
        if (kind(target) != T_char) {
            wout(c->who, "%s is not a character.", box_code(target));
            return FALSE;
        }

        if (char_here(c->who, target)) {
            wout(c->who, "%s is here.", box_name(target));
            add_contact(target, c->who);

            c->wait = 0;
            c->inhibit_finish = TRUE;
            return TRUE;
        }
    }

    return TRUE;
}


int
d_seek(struct command *c) {
    int target = c->a;
    int i;
    int chance = 50, tmp;

    /*
     *  Thu Jul  3 16:14:59 1997 -- Scott Turner
     *
     *  Bonus for "spot hidden"
     *
     */
    if (has_skill(c->who, sk_spot_hidden)) {
        chance += 5 * skill_exp(c->who, sk_spot_hidden);
    };

    if (target)        /* target specified */
    {
        if (kind(target) != T_char) {
            wout(c->who, "%s is not a character.", box_code(target));
            return FALSE;
        }

        if (char_here(c->who, target)) {
            wout(c->who, "%s is here.", box_name(target));
            add_contact(target, c->who);

            c->wait = 0;
            c->inhibit_finish = TRUE;    /* don't call d_wait */
            return TRUE;
        }

        /*
         *  Thu Jul  3 16:15:47 1997 -- Scott Turner
         *
         *  Chance is reduced if target is hiding.
         *
         */
        if (has_skill(target, sk_hide_self) &&
            char_really_hidden(target)) {
            chance -= 5 * skill_exp(target, sk_hide_self);
        };

        if ((subloc(c->who) == subloc(target)) &&
            rnd(1, 100) < chance) {
            add_contact(target, c->who);
            wout(c->who, "Found %s.", box_name(target));

            c->wait = 0;
            c->inhibit_finish = TRUE;    /* don't call d_wait */
            return TRUE;
        }

        return TRUE;
    }

/*
 *  5% chance of finding any hidden noble present
 */
    loop_here(subloc(c->who), i)
            {
                if (kind(i) != T_char || char_here(c->who, i)) {
                    continue;
                }

                tmp = chance;
                if (has_skill(i, sk_hide_self) &&
                    char_really_hidden(i)) {
                    tmp -= 5 * skill_exp(i, sk_hide_self);
                };

                if (rnd(1, 200) > tmp) {
                    continue;
                }

                add_contact(i, c->who);
                wout(c->who, "Found %s.", box_name(i));

                break;
            }
    next_here;

    return TRUE;
}


static void
add_fill(int where, ilist *l, int max_depth, int depth) {
    int i;
    struct entity_loc *p;

    assert(loc_depth(where) == LOC_province);

    if (ilist_lookup(*l, where) >= 0) {
        return;
    }

    ilist_append(l, where);

    p = rp_loc(where);
    if (p == NULL) {
        return;
    }

    if (depth >= max_depth) {
        return;
    }

    for (i = 0; i < ilist_len(p->prov_dest); i++) {
        if (p->prov_dest[i]) {
            add_fill(p->prov_dest[i], l, max_depth, depth + 1);
        }
    }
}


int
v_find_rich(struct command *c) {
    int where = subloc(c->who);

    if (subkind(where) != sub_inn) {
        wout(c->who, "May only be used in an inn.");
        return FALSE;
    }

    return TRUE;
}


int
d_find_rich(struct command *c) {
    static ilist l = NULL;
    int pl = player(c->who);
    int max_gold = 500;
    int who_gold = 0;
    int i, j, n;
    int where = subloc(c->who);
    char *s;

    if (subkind(where) != sub_inn) {
        wout(c->who, "May only be used in an inn.");
        return FALSE;
    }

    ilist_clear(&l);

    add_fill(province(where), &l, 3, 1);

    for (i = 0; i < ilist_len(l); i++) {
        loop_all_here(l[i], j)
                {
                    if (kind(j) != T_char || player(j) == pl) {
                        continue;
                    }

                    n = has_item(j, item_gold);
                    if (n >= max_gold) {
                        max_gold = n;
                        who_gold = j;
                    }
                }
        next_all_here;
    }

    if (who_gold == 0) {
        wout(c->who, "No weathy nobles are rumored to be nearby.");
    } else {
        if (max_gold <= 1000) {
            s = "large sum";
        } else if (max_gold <= 2000) {
            s = "considerable amount";
        } else {
            s = "vast quantity";
        }

        wout(c->who, "Rumors claim that one %s is nearby, and "
                     "possesses a %s of gold.", box_name(who_gold), s);
    }

    return TRUE;
}


int
v_torture(struct command *c) {
    int target = c->a;

    if (!has_skill(c->who, sk_torture)) {
        wout(c->who, "Requires %s.", box_name(sk_torture));
        return FALSE;
    }

    if (!is_prisoner(target) ||
        stack_leader(target) != stack_leader(c->who)) {
        wout(c->who, "%s is not a prisoner of %s.",
             box_code(target), box_name(c->who));
        return FALSE;
    }

    if (is_npc(target) || loyal_kind(target) == LOY_npc ||
        loyal_kind(target) == LOY_summon) {
        wout(c->who, "NPC's cannot be tortured.");
        return FALSE;
    }

    return TRUE;
}


int
d_torture(struct command *c) {
    int target = c->a;
    int chance;

    if (!is_prisoner(target) ||
        stack_leader(target) != stack_leader(c->who)) {
        wout(c->who, "%s is not a prisoner of %s.",
             box_code(target), box_name(c->who));
        return FALSE;
    }

    if (is_npc(target) || loyal_kind(target) == LOY_npc ||
        loyal_kind(target) == LOY_summon) {
        wout(c->who, "NPC's cannot be tortured.");
        return FALSE;
    }

    add_char_damage(target, 50, c->who);

    if (!alive(target)) {
        wout(c->who, "%s died under torture.", box_name(target));
        return FALSE;
    }

    if (get_effect(target, ef_guard_loyalty, 0, 0)) {
        wout(c->who, "The prisoner laughs at your torture!");
        return FALSE;
    };

    switch (loyal_kind(target)) {
        case LOY_oath:
            if (loyal_rate(target) == 1) {
                chance = 10;
            } else {
                chance = 0;
            }
            break;

        case LOY_contract:
            chance = 50;
            break;

        case LOY_fear:
            chance = 100;
            break;

        default:
            chance = 0;
    }

    if (rnd(1, 100) > chance) {
        wout(c->who, "The prisoner refused to talk.");
        return FALSE;
    }

    add_skill_experience(c->who, sk_torture);

    wout(c->who, "%s belongs to faction %s.",
         box_name(target),
         box_name(player(target)));

    return TRUE;
}


int
cloak_lord(int n) {

    return has_skill(n, sk_hide_lord);
}


int
v_petty_thief(struct command *c) {
    int where = subloc(c->who);
    int garr = garrison_here(where);
    int chance = 0;

    if (!char_alone_stealth(c->who)) {
        wout(c->who, "Must be alone to thieve.");
        return FALSE;
    }

    if (has_item(province(where), item_peasant) < 100) {
        wout(c->who, "This area is so deserted there's no opportunity for"
                     "thievery.");
        return FALSE;
    };

    switch (c->use_exp) {
        case exp_novice:
            chance = 5;
            break;
        case exp_journeyman:
            chance = 3;
            break;
        case exp_teacher:
            chance = 1;
            break;
        case exp_master:
            chance = 0;
            break;
        case exp_grand:
            chance = 0;
            break;
        default:
            chance = 0;
    }

    if (garr && rnd(1, 100) <= chance) {
        show_to_garrison = TRUE;
        vector_clear();
        vector_add(where);
        vector_add(c->who);
        wout(VECT, "%s was caught thieving the local peasants!",
             box_name(c->who));
        take_prisoner(garr, c->who);
        show_to_garrison = FALSE;
        return FALSE;
    }

    c->g = 0;  /* c->h used by "basis" in use.c */

    return TRUE;
}

int
d_petty_thief(struct command *c) {
    int where = subloc(c->who), j;
    int amount;
    extern int gold_petty_thief;

    if (subkind(where) == sub_city) {
        where = province(where);
    }

    if (!char_alone_stealth(c->who)) {
        wout(c->who, "Must be alone to thieve.");
        if (c->g) { wout(c->who, "Earned a total of %s gold.", nice_num(c->g)); }
        return FALSE;
    }

    if (has_item(where, item_peasant) < 100) {
        wout(c->who, "Not enough peasantry here to use this skill.");
        if (c->g) { wout(c->who, "Earned a total of %s gold.", nice_num(c->g)); }
        return FALSE;
    }

    /*
     *  Maybe no money?
     *
     */
    if (has_item(where, item_gold) < 1) {
        wout(c->who, "The peasants here have no more gold.");
        return FALSE;
    };

    /*
     *  Earn gold for today, if possible!
     *
     */
    switch (c->use_exp) {
        case exp_novice:
            amount = 1;
            break;
        case exp_journeyman:
            amount = 2;
            break;
        case exp_teacher:
            amount = 4;
            break;
        case exp_master:
            amount = 7;
            break;
        case exp_grand:
            amount = 10;
            break;
        default:
            amount = 0;
            break;
    }
    if (amount > has_item(where, item_gold)) {
        amount = has_item(where, item_gold);
    }
    sub_item(where, item_gold, amount);

    /*
     *  "Draw Crowds" improves the take.
     *
     */
    if (get_effect(where, ef_improve_production, 0, item_petty_thief)) {
        amount += amount * 0.50 + 0.50;
    };
    gen_item(c->who, item_gold, amount);
    gold_petty_thief += amount;
    c->g += amount;

    /* Drive away some peasants */
    assert(consume_item(province(where), item_peasant, rnd(1, 3)));

    if (!c->wait) {
        wout(c->who, "Earned %s gold thieving.", nice_num(c->g));
        /*
         *  Possibly some complaints!
         *
         */
        if (rnd(1, 100) < 10) {
            char *third;
            switch (rnd(1, 3)) {
                case 1:
                    third = sout("%s peasants complain that they were robbed "
                                 "by a thief.",
                                 rnd(0, 1) ? "Several" :
                                 cap(nice_num(rnd(2, 3))));
                    break;

                case 2:
                    third = sout("%s peasants complain that their pockets were "
                                 "picked.",
                                 rnd(0, 1) ? "Several" :
                                 cap(nice_num(rnd(2, 3))));
                    break;

                case 3:
                    switch (rnd(1, 3)) {
                        case 1:
                            third = "There are rumors that a thief is loose in "
                                    "the area.";
                            break;

                        case 2:
                            third = "There are rumors that a thief has been "
                                    "working the area.";
                            break;

                        case 3:
                            third = "Reports of con games are widespread.";
                            break;

                        default:
                            assert(FALSE);
                    }
                    break;

                default:
                    assert(FALSE);
            }

            if (third) {
                show_to_garrison = TRUE;
                wout(where, "%s", third);
                show_to_garrison = FALSE;
            }
        };
    };

    return TRUE;
}


int
v_conceal_nation(struct command *c) {
    int i;

    if (!check_skill(c->who, sk_conceal_nation)) {
        return FALSE;
    }

    /*
     *  Wed Apr 16 11:18:32 1997 -- Scott Turner
     *
     *  Do we match a nation?
     *
     */
    i = find_nation(c->parse[1]);
    if (!i) {
        wout(c->who, "The prefix \"%s\" doesn't match any nation.",
             c->parse[1]);
        return FALSE;
    };

    if (get_effect(c->who, ef_conceal_nation, 0, 0)) {
        delete_effect(c->who, ef_conceal_nation, 0);
        wout(c->who, "You remove your previous disguise to reveal your true nation.");
    };

    return TRUE;
}


int
d_conceal_nation(struct command *c) {
    int i;

    /*
     *  Wed Apr 16 11:18:32 1997 -- Scott Turner
     *
     *  Do we match a nation?
     *
     */
    i = find_nation(c->parse[1]);
    if (!i) {
        wout(c->who, "The prefix \"%s\" doesn't match any nation.",
             c->parse[1]);
        return FALSE;
    };

    if (!add_effect(c->who, ef_conceal_nation, 0, 14, i) || kind(i) != T_nation) {
        wout(c->who, "For some reason, you cannot conceal your nation.");
        return FALSE;
    };

    wout(c->who, "You now have the appearance of a citizen of the %s.",
         rp_nation(i)->name);
    return TRUE;
}

/*
 *  Assassination.
 *
 *  Mon Jul  7 16:07:16 1997 -- Scott Turner
 *
 */
int
v_assassinate(struct command *c) {
    if (subkind(subloc(c->who)) != sub_city &&
        subkind(subloc(c->who)) != sub_castle) {
        wout(c->who, "You may only assassinate in cities and castles.");
        return FALSE;
    };

    return TRUE;
}


int
d_assassinate(struct command *c) {
    int target = c->a;
    int chance = rnd(1, 50) - 25;
    int i;
    int where = province(subloc(c->who));
    int bodyguards = 0;

    if (subkind(subloc(c->who)) != sub_city &&
        subkind(subloc(c->who)) != sub_castle) {
        wout(c->who, "You may only assassinate in cities and castles.");
        return FALSE;
    };

    if (!check_still_here(c->who, target)) {
        wout(c->who, "No one here to assassinate.");
        return FALSE;
    };

    /*
     *  Can't attack in the fog...
     *
     */
    if (loc_depth(subloc(c->who)) == LOC_province &&
        weather_here(subloc(c->who), sub_fog) &&
        !contacted(target, c->who)) {
        wout(c->who, "That target is not visible in the fog.");
        return FALSE;
    };

    chance += 5 * skill_exp(c->who, sk_assassinate);

    /*
     *  Bodyguards.
     *
     */
    loop_stack(stack_leader(target), i)
            {
                if (i != target &&
                    skill_exp(i, sk_protect_noble)) {
                    chance -= 5 * skill_exp(i, sk_protect_noble);
                    bodyguards = TRUE;
                };
            }next_stack;

    if (chance > 75) { chance = 75; }

    if (rnd(1, 100) < chance) {
        wout(c->who, "You assassinate %s!", box_name(target));
        wout(where, "An assassin kills %s!", box_name(target));
        kill_char(target, MATES, S_body);
    } else {
        wout(where, "An assassin attempts to kill %s!", box_name(target));
        wout(c->who,
             "Your assassination attempt fails!");
        wout(target, "An assassin nearly kills you!");
    };

    if (bodyguards && rnd(1, 100) < 25) {
        wout(target, "You capture the assassin!");
        take_prisoner(target, c->who);
        return FALSE;
    };

    return TRUE;
}

