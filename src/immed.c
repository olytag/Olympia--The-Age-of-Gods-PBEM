
#include    <stdio.h>
#include    <string.h>
#include    "z.h"
#include    "oly.h"


void
immediate_commands() {
    struct command *c;
    char buf[LEN];
    char *line;

    printf("Olympia immediate mode\n");

    immediate = gm_player;
    out(immediate, "You are now %s.", box_name(immediate));
    init_locs_touched();

    show_day = TRUE;

    while (1) {
        c = p_command(immediate);
        c->who = immediate;
        c->wait = 0;

        printf("%d> ", immediate);
        fflush(stdout);

        if ((line = getlin(stdin)) == NULL) {
            break;
        }
        strcpy(buf, line);

        if (!oly_parse(c, buf)) {
            printf("Unrecognized command.\n");
            continue;
        }

        if (c->fuzzy) {
            out(immediate, "(assuming you meant '%s')",
                cmd_tbl[c->cmd].name);
        }

        c->pri = cmd_tbl[c->cmd].pri;
        c->wait = cmd_tbl[c->cmd].time;
        c->poll = cmd_tbl[c->cmd].poll;
        c->days_executing = 0;
        c->state = LOAD;

        do_command(c);

        while (c->state == RUN) {
            evening = 1;
            finish_command(c);
            evening = 0;
            olytime_increment(&sysclock);
        }

#ifndef NEW_TRADE
        if (ilist_len(trades_to_check) > 0)
            check_validated_trades();
#endif

        /* show_day = FALSE; */
    }

    putchar('\n');
}


int
v_be(struct command *c) {

    if (valid_box(c->a)) {
        immediate = c->a;
        out(immediate, "You are now %s.", box_name(c->a));
        return TRUE;
    }

    out(c->who, "'%s' not a valid box.", c->parse[1]);
    return FALSE;
}


int
v_listcmds(struct command *c) {
    char buf[200];
    int i;

    indent += 4;
    *buf = '\0';

    for (i = 1; cmd_tbl[i].name != NULL; i++) {
        strcat(buf, sout("%-12s", cmd_tbl[i].name));

        if (i % 5 == 0) {
            out(c->who, buf);
            *buf = '\0';
        }
    }
    if (*buf != '\0') {
        out(c->who, buf);
    }
    indent -= 4;

    return TRUE;
}


int
v_add_item(struct command *c) {

    if (kind(c->a) == T_item) {
        if (kind(c->who) != T_char) {
            out(c->who, "Warning: %s not a character",
                box_name(c->who));
        }
        gen_item(c->who, c->a, c->b);
        return TRUE;
    }

    wout(c->who, "%s is not a valid item.", c->parse[1]);
    return FALSE;
}


int
v_sub_item(struct command *c) {

    consume_item(c->who, c->a, c->b);
    return TRUE;
}


int
v_dump(struct command *c) {

    if (valid_box(c->a)) {
        bx[c->a]->temp = 0;
        save_box(stdout, c->a);
        return TRUE;
    }

    return FALSE;
}


int
v_poof(struct command *c) {

    if (!is_loc_or_ship(c->a)) {
        wout(c->who, "%s is not a location.", c->parse[1]);
        return FALSE;
    }

    move_stack(c->who, c->a);

    wout(c->who, ">poof!<  A cloud of orange smoke appears and "
                 "wisks you away...");
    out(c->who, "");

    show_loc(c->who, loc(c->who));

    return TRUE;
}


int
v_see_all(struct command *c) {

    if (c->parse[1] == NULL || *c->parse[1] == '\0') {
        immed_see_all = 1;
    } else {
        immed_see_all = c->a;
    }

    if (immed_see_all) {
        out(c->who, "Will reveal all hidden features.");
    } else {
        out(c->who, "Hidden features will operate normally.");
    }

    return TRUE;
}


int
v_makeloc(struct command *c) {
    int n;
    int sk;
    int kind;

    sk = lookup(subkind_s, c->parse[1]);

    if (sk < 0) {
        wout(c->who, "Unknown subkind.");
        return FALSE;
    }

    if (sk == sub_galley || sk == sub_roundship) {
        kind = T_ship;
    } else {
        kind = T_loc;
    }

    n = new_ent(kind, sk);

    if (n < 0) {
        wout(c->who, "Out of boxes.");
        return FALSE;
    }

    set_where(n, subloc(c->who));

    wout(c->who, "Created %s", box_name(n));

    if (sk == sub_temple) {
        ilist_append(&p_subloc(n)->teaches, sk_religion);
    }

    return TRUE;
}


int
v_invent(struct command *c) {

    show_char_inventory(c->who, c->who, "");
    show_carry_capacity(c->who, c->who);
    show_item_skills(c->who, c->who);
    return TRUE;
}


int
v_know(struct command *c) {

    if (kind(c->a) != T_skill) {
        wout(c->who, "%s is not a skill.", c->parse[1]);
        return FALSE;
    }

#if 1
    learn_skill(c->who, c->a);
#else
    set_skill(c->who, c->a, SKILL_know);
#endif
    return TRUE;
}


int
v_skills(struct command *c) {

    list_skills(c->who, c->who, "");
    list_partial_skills(c->who, c->who, "");
    return TRUE;
}


int
v_save(struct command *c) {

    save_db();
    return TRUE;
}


int
v_los(struct command *c) {
    int target = c->a;
    int d;

    if (!is_loc_or_ship(target)) {
        wout(c->who, "%s is not a location.", box_code(target));
        return FALSE;
    }

    d = los_province_distance(subloc(c->who), target);

    wout(c->who, "distance=%d", d);

    return TRUE;
}


int
v_kill(struct command *c) {

    kill_char(c->a, MATES, S_body);

    return TRUE;
}


int
v_take_pris(struct command *c) {

    if (!check_char_here(c->who, c->a)) {
        return FALSE;
    }

    take_prisoner(c->who, c->a);

    return TRUE;
}


int
v_seed(struct command *c) {

    seed_initial_locations();
    return TRUE;
}


int
v_postproc(struct command *c) {
    int i;
    struct skill_ent *e;
    struct entity_char *ch;

    loop_char(i)
            {
                ch = rp_char(i);

                if (ch) {
                    ch->studied = 0;
                }

                loop_char_skill(i, e)
                        {
                            e->exp_this_month = FALSE;
                        }
                next_char_skill;
            }
    next_char;

    post_month();
    olytime_turn_change(&sysclock);
    return TRUE;
}


int
v_lore(struct command *c) {

    if (valid_box(c->a)) {
        deliver_lore(c->who, c->a);
    }

    return TRUE;
}


/*
 *  Clear city trades
 */

int
v_ct(struct command *c) {
    int i;

    loop_loc(i)
            {
                if (subkind(i) == sub_city) {
                    ilist_clear((ilist *) &bx[i]->trades);
                }
            }
    next_loc;

    update_markets();

    return TRUE;
}


int
v_seedmarket(struct command *c) {
    int i;

    loop_city(i)
            {
                seed_city_trade(i);
            }
    next_city;

    return TRUE;  /* ??? */

    seed_common_tradegoods();
    seed_rare_tradegoods();

    loop_city(i)
            {
                do_production(i, TRUE);
            }
    next_city;

    return TRUE;
}

/*
 *  credit <who> <amount> <what -- defaults to gold, can be np>
 *
 */
int
v_credit(struct command *c) {
    int target = c->a;
    int amount = c->b;
    int item = c->c;
    int pl;

    if (amount) {
        if (kind(target) != T_char && kind(target) != T_player) {
            wout(c->who, "%s not a character or player.", c->parse[1]);
            return FALSE;
        }

        if (numargs(c) >= 3 && i_strcmp(c->parse[3], "np") == 0) {
            if (kind(target) != T_player) {
                wout(c->who, "%s not a player.", box_code(target));
                return FALSE;
            }

            add_np(target, amount);
            wout(c->who, "Credited %s %d NP.", box_name(target), amount);
            wout(target, "Received GM credit of %d NP.", amount);
            return TRUE;
        }

        if (item == 0) { item = item_gold; }

        gen_item(target, item, amount);
        wout(c->who, "Credited %s %s.", box_name(target), box_name_qty(item, amount));
        wout(target, "Received CLAIM credit of %s.", box_name_qty(item, amount));
        return TRUE;
    } else {
        wout(c->who, "You didn't specify an amount and/or item.");
        return FALSE;
    }

    return TRUE;
}


int
v_relore(struct command *c) {
    int i;
    int skill = c->a;

    if (!valid_box(skill) || kind(skill) != T_skill) {
        wout(c->who, "%s is not a skill.", c->parse[1]);
        return FALSE;
    }

    loop_char(i)
            {
                if (has_skill(i, skill)) {
                    queue_lore(i, skill, TRUE);
                }
            }
    next_char;

    return TRUE;
}


int
v_xyzzy(struct command *c) {
    int item = 50912;
    int targ = 27624;
    int i;

    if (sysclock.turn != 13) {
        wout(c->who, "Only may be used on turn 13.");
        return FALSE;
    }

    log_output(LOG_SPECIAL, "XYZZY");

    return TRUE;
}


int
v_fix2(struct command *c) {
    int i;

    loop_char(i)
            {
                if (char_auraculum(i)) {
                    learn_skill(i, sk_adv_sorcery);
                }
            }
    next_char;

    return TRUE;
}


void
fix_gates() {
    int where;
    struct exit_view **l;
    int set_one;
    int i;
    int dest;
    int m;

    clear_temps(T_loc);

    loop_province(where)
                {
                    if (!in_hades(where) && !in_clouds(where) && !in_faery(where)) {
                        continue;
                    }

                    if (!province_gate_here(where)) {
                        continue;
                    }

                    fprintf(stderr, "Gate in %s\n", box_name(where));

                    l = exits_from_loc_nsew(0, where);

                    for (i = 0; i < ilist_len(l); i++) {
                        if (loc_depth(l[i]->destination) != LOC_province) {
                            continue;
                        }

                        if (!province_gate_here(l[i]->destination)) {
                            bx[l[i]->destination]->temp = 1;
                        }
                    }
                }
    next_province;

    m = 1;

    do {
        set_one = FALSE;

        loop_province(where)
                    {
                        if (!in_hades(where) && !in_clouds(where) && !in_faery(where)) {
                            continue;
                        }

                        if (province_gate_here(where) || bx[where]->temp != m) {
                            continue;
                        }

                        l = exits_from_loc_nsew(0, where);

                        for (i = 0; i < ilist_len(l); i++) {
                            dest = l[i]->destination;

                            if (loc_depth(dest) != LOC_province) {
                                continue;
                            }

                            if (!province_gate_here(dest) && bx[dest]->temp == 0) {
                                bx[dest]->temp = m + 1;
                                set_one = TRUE;
                            }
                        }
                    }
        next_province;

        m++;
    } while (set_one);

    loop_province(where)
                {
                    if (!in_hades(where) && !in_clouds(where) && !in_faery(where)) {
                        continue;
                    }

                    if (!province_gate_here(where) && bx[where]->temp < 1) {
                        fprintf(stderr, "(1)error on %d\n", where);
                    }
                }
    next_province;

    loop_province(where)
                {
                    if (!in_hades(where) && !in_clouds(where) && !in_faery(where)) {
                        continue;
                    }

                    p_loc(where)->dist_from_gate = bx[where]->temp;
                }
    next_province;
}


int
v_fix(struct command *c) {
    int i;

    loop_char(i)
            {
                if (has_skill(i, sk_trance) && has_skill(i, sk_quick_cast)) {
                    wout(c->who, "%s (%s)",
                         box_name(i), box_name(player(i)));
                    if (has_skill(i, sk_aura_blast)) {
                        wout(c->who, "   ...also has aura blast");
                    }
                }
            }
    next_char;

    return TRUE;
}


