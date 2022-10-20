#include    <stdio.h>
#include    <sys/types.h>
#include    <dirent.h>
#include    <string.h>
#include    "z.h"
#include    "oly.h"


/*
 *  add.c  --  add new players to Olympia
 *
 *  oly -a will read data on new characters from stdin:
 *
 *	player number (provided by accounting system)
 *	faction name
 *	primary character name
 *	player's full name
 *	player's email address
 */


ilist new_players = NULL;
static ilist new_chars = NULL;
#define RANDOM_START -1

static char *
fetch_inp(FILE *fp) {
    char *s;

    while ((s = getlin_ew(fp)) && *s == '\0') {}

    if (s == NULL || *s == '\0') {
        return NULL;
    }

    return str_save(s);
}


#if 1

/*
 *  Tue Apr  8 12:29:28 1997 -- Scott Turner
 *
 *  You can start in any city controlled by your nation; if not,
 *  you start in the default start city for your nation.
 *
 */
int
pick_starting_city(int nat, int start_city) {
    int i;
    int n = 0;
    int which;
    int ret = 0;
    char city_name[256], nation_name[256];
    int sum = 0, choice = 0;

    if (start_city && valid_box(start_city) &&
        subkind(start_city) == sub_city &&
        nation(player_controls_loc(start_city)) == nat) {
        return start_city;
    }

    /* RANDOM_START indicates we want to start in a random city. */
    if (start_city == RANDOM_START) {
        loop_city(i)
                {
                    if (nation(player_controls_loc(i)) == nat) {
                        sum++;
                        if (rnd(1, sum) == 1) { choice = i; }
                    };
                }next_city;

        if (choice) { return choice; }
    };

    /*  return nations[nat].capital; */
    return rp_nation(nat)->capital;
}

#else

static int
pick_starting_city()
{
    int i;
    int n = 0;
    int which;
    int ret = 0;
    loop_loc(i)
    {
        if (subkind(i) == sub_city && safe_haven(i))
            n++;
    }
    next_loc;

/*
 *  If this assert fails, the above loop could find no
 *  safe haven cities to start a player in.
 */

    assert(n > 0);

    which = rnd(1, n);

    loop_loc(i)
    {
        if (subkind(i) == sub_city && safe_haven(i) && --which <= 0)
        {
            ret = i;
            break;
        }
    }
    next_loc;

    assert(ret > 0);

    return ret;
}

#endif

/*
 *  Tue Apr  8 15:51:14 1997 -- Scott Turner
 *
 *  Total a unit's nps.
 *
 */
int
nps_invested(int who) {
    int i, total = 1, categories = 0;
    struct skill_ent *e;

    if (kind(who) != T_char) { return 0; }

    loop_char_skill(who, e)
            {
                if (e->skill && e->know == SKILL_know) {
                    total += skill_np_req(e->skill);
                }
                if (skill_school(e->skill) == e->skill) { ++categories; }
            }next_char_skill;

    /*
     *  Loyalty
     *
     */
    if (p_char(who)->loy_kind == LOY_oath) {
        total += p_char(who)->loy_rate;
    }

    if (categories > 3) { total += categories - 3; }

    return total;
};

/*
 *  Tue Apr  8 15:58:35 1997 -- Scott Turner
 *
 *  Do all the nations.
 *
 */
void
calculate_nation_nps() {
    int i, pl;

    loop_nation(i)
            {
                rp_nation(i)->nps = 0;
                rp_nation(i)->gold = 0;
                rp_nation(i)->players = 0;
                rp_nation(i)->nobles = 0;
            }next_nation;
    loop_player(pl)
            {
                if (!nation(pl)) { continue; }
                rp_nation(nation(pl))->players++;
                rp_nation(nation(pl))->gold += has_item(pl, item_gold); /* CLAIM */
                loop_units(pl, i)
                        {
                            rp_nation(nation(pl))->nobles++;
                            rp_nation(nation(pl))->nps += nps_invested(i);
                            rp_nation(nation(pl))->gold += has_item(i, item_gold);
                        }next_unit;
            }next_player;

};

/*
 *  Tue Apr  8 15:39:07 1997 -- Scott Turner
 *
 *  Calculate NP bonus for weak/strong nations.
 *
 */
int
starting_noble_points(int nation) {
    int i, total_np = 0, total_nations = 0;
    float ratio = 0.0;

    if (rp_nation(nation)->player_limit) { return 12; }

    loop_nation(i)
            {
                total_nations++;
            }next_nation;
    if (total_nations == 1) { return 12; }

    total_nations = 0;
    loop_nation(i)
            {
                if (i != nation && !rp_nation(i)->player_limit) {
                    total_np += rp_nation(i)->nps;
                    total_nations++;
                };
            }next_nation;

    if (total_np) {
        ratio = ((float) total_np / (float) total_nations) /
                (float) rp_nation(nation)->nps;
    } else if (!rp_nation(nation)->nps) {
        ratio = 1.0;
    };

    if (ratio >= 2.0) { return 20; }
    else if (ratio >= 1.75) { return 18; }
    else if (ratio >= 1.50) { return 16; }
    else if (ratio >= 1.25) { return 14; }
    else if (ratio >= 0.75) { return 12; }
    else if (ratio >= 0.50) { return 10; }
    else if (ratio >= 0.25) { return 8; }
    else { return 6; }

};

/*
 *  Thu Jul  3 13:55:17 1997 -- Scott Turner
 *
 *  Calculate gold bonus for weak/strong nations.
 *
 */
int
starting_gold(int nation) {
    int i, total_gold = 0, total_nations = 0;
    float ratio = 0.0;

    if (rp_nation(nation)->player_limit) { return 5000; }

    loop_nation(i)
            {
                total_nations++;
            }next_nation;
    if (total_nations == 1) { return 5000; }

    total_nations = 0;
    loop_nation(i)
            {
                if (i != nation && !rp_nation(i)->player_limit) {
                    total_gold += rp_nation(i)->gold;
                    total_nations++;
                };
            }next_nation;

    if (total_gold && rp_nation(nation)->gold) {
        ratio = ((float) total_gold / (float) total_nations) /
                (float) rp_nation(nation)->gold;
    } else if (!rp_nation(nation)->nps) {
        ratio = 1.0;
    };

    if (ratio >= 2.0) { ratio = 2.0; }
    else if (ratio < 0.25) { ratio = 0.25; }
    return ratio * 5000;
};

static int
add_new_player(int pl, char *faction, char *character, char *full_name,
               char *email, int nation, int start_city) {
    int who;
    struct entity_char *cp;
    struct entity_player *pp;
    extern int new_ent_prime;        /* allocate short numbers */

    new_ent_prime = TRUE;
    who = new_ent(T_char, 0);
    new_ent_prime = FALSE;

    if (who < 0) {
        return 0;
    }

    set_name(pl, faction);
    set_name(who, character);

    pp = p_player(pl);
    cp = p_char(who);

    pp->full_name = full_name;
    pp->email = email;
    /*
     *  Tue Apr  8 12:40:56 1997 -- Scott Turner
     *
     *  Noble points need to depend upon how the other nations
     *  are doing.
     *
     */
    pp->noble_points = starting_noble_points(nation);
    pp->first_turn = sysclock.turn + 1;
    pp->last_order_turn = sysclock.turn;
    pp->nation = nation;
    /*
     *  Thu Apr  9 08:41:42 1998 -- Scott Turner
     *
     *  Jump start points come from the nation... plus some
     *  per turn?
     *
     */
    pp->jump_start = rp_nation(nation)->jump_start + (sysclock.turn / 5);
    if (pp->jump_start > 56) { pp->jump_start = 56; }

    if (i_strcmp(email + (strlen(email) - 15), "@compuserve.com") == 0) {
        pp->compuserve = TRUE;
    }

    cp->health = 100;
    cp->break_point = 50;
    cp->attack = 80;
    cp->defense = 80;

    set_where(who, pick_starting_city(nation, start_city));
    set_lord(who, pl, LOY_oath, 2);

    gen_item(who, item_peasant, 25);
    gen_item(who, item_gold, 200);
    gen_item(pl, item_gold, starting_gold(nation));    /* CLAIM item */
    gen_item(pl, item_lumber, 50);        /* CLAIM item */

    ilist_append(&new_players, pl);
    ilist_append(&new_chars, who);

    add_unformed_sup(pl);

    return pl;
}

static void
failed_join(char *email, char *reason) {
    char tmpfile[1024];
    char *cmd;
    FILE *tmp;
    extern int save_flag;

    if (save_flag == FALSE) { return; }

    sprintf(tmpfile, "failedjoin%d", getpid());
    tmp = fopen(tmpfile, "w");
    fprintf(tmp, "From: %s\n", from_host);
    fprintf(tmp, "Subject: Failed attempt to join "
                 "Olympia: The Age of Gods, Game %d\n", game_number);
    fprintf(tmp, "To: %s, %s\n\n", email, rp_player(gm_player)->email);
    fprintf(tmp, "%s", reason);
    fprintf(tmp, "\n\n -- The Game Master\n\n");
    fclose(tmp);
    /*VLN cmd = sout("sendmail -t -odq < %s", tmpfile);*/
    cmd = sout("msmtp -t < %s", tmpfile);
    if (system(cmd)) {
        fprintf(stderr, "Failed to send 'failed join' mail?\n");
    } else {
        unlink(tmpfile);
    }
};


static int
make_new_players_sup(char *acct, FILE *fp) {
    char *faction;
    char *character;
    char *full_name;
    char *email;
    char *nat;
    char *start_city;
    int pl, n = 0, i, sc = 0;
    char fail_buf[1024];

    faction = fetch_inp(fp);
    if (faction == NULL) {
        fprintf(stderr, "%s: Unable to add %s <%s>n", acct, full_name, email);
        fprintf(stderr, "    partial read of faction.");
        return FALSE;
    }
    character = fetch_inp(fp);
    if (character == NULL) {
        fprintf(stderr, "%s: Unable to add %s <%s>n", acct, full_name, email);
        fprintf(stderr, "    partial read of character.");
        return FALSE;
    }
    full_name = fetch_inp(fp);
    if (full_name == NULL) {
        fprintf(stderr, "%s: Unable to add %s <%s>n", acct, full_name, email);
        fprintf(stderr, "    partial read of full_name.");
        return FALSE;
    }
    email = fetch_inp(fp);
    if (email == NULL) {
        fprintf(stderr, "%s: Unable to add %s <%s>n", acct, full_name, email);
        fprintf(stderr, "    partial read of email.");
        return FALSE;
    }
    nat = fetch_inp(fp);
    if (nat == NULL) {
        fprintf(stderr, "%s: Unable to add %s <%s>n", acct, full_name, email);
        fprintf(stderr, "    partial read of nation.");
        return FALSE;
    }

    /*
     *  Start_City can be null (blank)
     *
     */
    start_city = fetch_inp(fp);

    pl = scode(acct);
    assert(pl > 0 && pl < MAX_BOXES);

    /*
     * Maybe he's already in the game.
     *
     */
    if (bx[pl] != NULL) {
        sprintf(fail_buf, "Olympia was unable to add you to this game because\n"
                          "there is already a faction assigned to your account.\n");
        failed_join(email, fail_buf);
        return TRUE;
    };

    /*
     *  Figure out the nation
     *
     */
    n = find_nation(nat);
    if (!n) {
        wout(gm_player, "Couldn't add player %s: bad nation.", acct);
        sprintf(fail_buf, "Olympia was unable to add you to this game because\n"
                          "we could not decipher the nation name (%s) that\n"
                          "you provided.  Please try to join again using a valid nation.\n",
                nat);
        failed_join(email, fail_buf);
        return TRUE;
    };
    /*
     *  Wed Jul  2 11:32:01 1997 -- Scott Turner
     *
     *  Check player limits on the nation they've chosen.
     *
     */
    if (rp_nation(n)->player_limit) {
        int total = 0;
        loop_player(i)
                {
                    if (nation(i) == n) { total++; }
                }next_player;
        if (total >= rp_nation(n)->player_limit) {
            sprintf(fail_buf, "Olympia was unable to add you to this game because\n"
                              "the %s nation has already reached its limit of "
                              "%d players.\n  Please try again using a different nation.\n",
                    rp_nation(n)->name,
                    rp_nation(n)->player_limit);
            failed_join(email, fail_buf);
            return TRUE;
        };
    };
    /*
     *  Check to make sure the city someone is trying to
     *  start in is controlled by the appropriate nation.
     *  If not, give him the default starting city.
     *
     */
    if (start_city && strncasecmp("rand", start_city, 4) == 0) {
        sc = RANDOM_START;
    } else {
        sc = code_to_int(start_city);
    };

    alloc_box(pl, T_player, sub_pl_regular);

    add_new_player(pl, faction, character, full_name,
                   email, n, sc);
    my_free(faction);
    my_free(character);
    if (start_city) { my_free(start_city); }
    fprintf(stderr, "\tadded player %s\n", box_name(pl));

    return TRUE;
}


static void
make_new_players() {
    DIR *d;
    struct dirent *e;
    char *fnam;
    char *acct;
    FILE *fp;

    d = opendir(options.accounting_dir);

    if (d == NULL) {
        fprintf(stderr, "make_new_players: can't open %s: ",
                options.accounting_dir);
        perror("");
        return;
    }

    while ((e = readdir(d)) != NULL) {
        if (*(e->d_name) == '.') {
            continue;
        }

        acct = e->d_name;

        printf("VLN account = %s\n", acct);
        fnam = sout("%s/%s/Join-tag-%d",
                    options.accounting_dir,
                    acct, game_number);

        fp = fopen(fnam, "r");
        if (fp == NULL) {
            continue;
        }

        if (!make_new_players_sup(acct, fp)) {
            printf("Failed to add new player\n");
            /*
             *  Fri Mar  6 09:16:51 1998 -- Scott Turner
             *
             *  This should generate some notice of a failed
             *  add and alert the GM/user.
             */
            fclose(fp);
            continue;
        }

        fclose(fp);
    }

    closedir(d);
}


void
rename_act_join_files() {
    int i;
    int pl;
    char acct[LEN];
    char *old_name;
    char *new_name;


    for (i = 0; i < ilist_len(new_players); i++) {
        pl = new_players[i];
        sprintf(acct, box_code_less(pl));

        old_name = sout("%s/%s/Join-tag-%d",
                        options.accounting_dir, acct, game_number);
        new_name = sout("%s/%s/Join-tag-%d-",
                        options.accounting_dir, acct, game_number);

        if (rename(old_name, new_name) < 0) {
            fprintf(stderr, "rename(%s, %s) failed:",
                    old_name, new_name);
            perror("");
        }
    }
}


static void
new_player_banners() {
    int pl;
    int i;
    struct entity_player *p;

    out_path = MASTER;
    out_alt_who = OUT_BANNER;

    for (i = 0; i < ilist_len(new_players); i++) {
        pl = new_players[i];
        p = p_player(pl);

#if 0
        out(pl, "From: %s", from_host);
        out(pl, "Reply-To: %s", reply_host);
        if (p->email)
            out(pl, "To: %s (%s)", p->email,
                p->full_name ? p->full_name : "???");
        out(pl, "Subject: Welcome to Olympia");
        out(pl, "");
#endif

        wout(pl, "Welcome to Olympia!");
        wout(pl, "");
        wout(pl, "This is an initial position report for your new "
                 "faction.");

        wout(pl, "You are player %s, \"%s\".", box_code_less(pl),
             just_name(pl));
        wout(pl, "");

        wout(pl, "The next turn will be turn %d.", sysclock.turn + 1);

        {
            int month, year;

            month = (sysclock.turn) % NUM_MONTHS;
            year = (sysclock.turn + 1) / NUM_MONTHS;

            wout(pl, "It is season \"%s\", month %d, in the "
                     "year %d.",
                 month_names[month],
                 month + 1,
                 year + 1);
        }

        out(pl, "");

        /*VLN report_account_sup(pl); */
    }

    out_path = 0;
    out_alt_who = 0;
}


static void
show_new_char_locs() {
    int i;
    int where;
    int who;
    extern int show_loc_no_header;    /* argument to show_loc() */

    out_path = MASTER;
    show_loc_no_header = TRUE;

    for (i = 0; i < ilist_len(new_chars); i++) {
        who = new_chars[i];
        where = subloc(who);

        out_alt_who = where;
        show_loc(player(who), where);

        where = loc(where);
        if (loc_depth(where) == LOC_province) {
            out_alt_who = where;
            show_loc(player(who), where);
        }
        mark_loc_stack_known(who, where);
    }

    show_loc_no_header = FALSE;
    out_path = 0;
    out_alt_who = 0;
}


static void
new_player_report() {
    int i;

    out_path = MASTER;
    out_alt_who = OUT_BANNER;

    for (i = 0; i < ilist_len(new_players); i++) {
        player_report_sup(new_players[i]);
    }

    out_path = 0;
    out_alt_who = 0;

    for (i = 0; i < ilist_len(new_players); i++) {
        show_unclaimed(new_players[i], new_players[i]);
    }
}


static void
new_char_report() {
    int i;

    indent += 3;

    for (i = 0; i < ilist_len(new_chars); i++) {
        char_rep_sup(new_chars[i], new_chars[i]);
    }

    indent -= 3;
}


static void
mail_initial_reports() {
    int i;
    char *s, *t;
    int pl;
    int ret;

    for (i = 0; i < ilist_len(new_players); i++) {
        pl = new_players[i];

        s = sout("%s/log/%d", libdir, pl);
        t = sout("%s/save/%d/%d", libdir, sysclock.turn, pl);

        ret = rename(s, t);

        if (ret < 0) {
            fprintf(stderr, "couldn't rename %s to %s:", s, t);
            perror("");
        }

        send_rep(pl, sysclock.turn);
    }
}


static void
new_order_templates() {
    int pl, i;

    out_path = MASTER;
    out_alt_who = OUT_TEMPLATE;

    for (i = 0; i < ilist_len(new_players); i++) {
        pl = new_players[i];
        orders_template(pl, pl);
    }

    out_path = 0;
    out_alt_who = 0;
}


static void
new_player_list_sup(int who, int pl) {
    struct entity_player *p;
    char *s;

    p = p_player(pl);

    if (p->email) {
        if (p->full_name) {
            s = sout("%s <%s>", p->full_name, p->email);
        } else {
            s = sout("<%s>", p->email);
        }
    } else if (p->full_name) {
        s = p->full_name;
    } else {
        s = "";
    }

    out(who, "%4s   %s  (%s)", box_code_less(pl), just_name(pl),
        rp_nation(nation(pl))->name);
    if (*s) {
        out(who, "       %s", s);
    }
    out(who, "");
}


void
new_player_list() {
    int pl;
    int i;

    stage("new_player_list()");

    out_path = MASTER;
    out_alt_who = OUT_NEW;

    vector_players();

#if 0
    for (i = 0; i < ilist_len(new_players); i++)
    {
        pl = new_players[i];
        ilist_rem_value(&out_vector, pl);
    }
#endif

    for (i = 0; i < ilist_len(new_players); i++) {
        pl = new_players[i];
        new_player_list_sup(VECT, pl);
    }

    out_path = 0;
    out_alt_who = 0;
}


void
new_player_top(int mail) {

    stage("new_player_top()");

    open_logfile();
    /* Need to do this before making a new player! */
    calculate_nation_nps();
    make_new_players();
    show_new_char_locs();
    new_char_report();
    new_player_banners();
    new_player_report();
    new_order_templates();
    gen_include_section();        /* must be last */
    close_logfile();

    if (mail) {
        mail_initial_reports();
    }
}


void
add_new_players() {

    stage("add_new_players()");

    calculate_nation_nps();
    make_new_players();
    show_new_char_locs();
    new_char_report();
    new_player_banners();
    new_player_report();
    new_order_templates();
    new_player_list();    /* show new players to the old players */
}

