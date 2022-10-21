// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    <unistd.h>
#include    <string.h>
#include    <stdlib.h>
#include    "z.h"
#include    "oly.h"
#include    <sys/stat.h>
#include "forward.h"

/*
 *  pretty_data_files:  include parenthesisted names in the data files,
 *  to make them easier to read.
 */

int pretty_data_files = FALSE;

int immediate = TRUE;
int immed_after = FALSE;
int immed_see_all = FALSE;
int flush_always = FALSE;
int time_self = FALSE;        /* print timing info */
int save_flag = FALSE;
int acct_flag = FALSE;

int main(int argc, char **argv) {
    extern int optind, opterr;
    extern char *optarg;
    int errflag = 0;
    int c;
    int run_flag = FALSE;
    int add_flag = FALSE;
    int eat_flag = FALSE;
    int mail_now = FALSE;
    int lore_flag = FALSE;
    int art_flag = FALSE;
    int inhibit_add_flag = FALSE;
    int map_flag = FALSE;
    int combat_test_flag = FALSE;
    int map_test_flag = FALSE;
    int unspool_first_flag = FALSE;

    printf("\tsizeof(struct box) = %d\n", (int)sizeof(struct box));
    setbuf(stderr, NULL);

    umask(S_IRWXO);

    call_init_routines();

    while ((c = getopt(argc, argv, "axefirmLl:pR?StMTAqXE")) != EOF) {
        switch (c) {
            case 'm':
                map_flag = TRUE;
                break;

            case 'a':
                add_flag = TRUE;
                immediate = FALSE;
                break;

            case 'x':
                inhibit_add_flag = TRUE;
                break;

            case 'X':
                combat_test_flag = TRUE;
                break;

            case 'A':
                acct_flag = TRUE;
                break;

            case 'f':
                flush_always = TRUE;
                setbuf(stdout, NULL);
                break;

            case 'e':
                eat_flag = TRUE;
                immediate = FALSE;
                break;

            case 'E':
                unspool_first_flag = TRUE;
                break;

            case 'i':
                immed_after = TRUE;
                break;

            case 'l':        /* set libdir */
                libdir = str_save(optarg);
                break;

            case 'L':
                lore_flag = TRUE;
                break;

            case 'p':
                pretty_data_files = !pretty_data_files;
                break;

            case 'q':  /* test artifacts */
                art_flag = TRUE;
                break;

            case 'r':        /* run a turn */
                immediate = FALSE;
                run_flag = TRUE;
                break;

            case 'R':        /* test random number generator */
                test_random();
                exit(0);

            case 'S':        /* save database when done */
                save_flag = TRUE;
                break;

            case 't':
                ilist_test();
                exit(0);

            case 'T':
                time_self = TRUE;
                break;

            case 'M':
                mail_now = TRUE;
                break;

            default:
                errflag++;
        }
    }

    if (errflag) {
        fprintf(stderr, "usage: oly [options]\n");
        fprintf(stderr, "  -a        Add new players mode\n");
        fprintf(stderr, "  -x        Inhibit adding players during turn.\n");
        fprintf(stderr, "  -e        Eat orders from libdir/spool\n");
        fprintf(stderr, "  -f        Don't buffer files for debugging\n");
        fprintf(stderr, "  -i        Immediate mode\n");
        fprintf(stderr, "  -l dir    Specify libdir, default ./lib\n");
        fprintf(stderr, "  -L        Generate lore dictionary.\n");
        fprintf(stderr, "  -p        Don't make data files pretty\n");
        fprintf(stderr, "  -r        Run a turn\n");
        fprintf(stderr, "  -R        Test the random number generator\n");
        fprintf(stderr, "  -S        Save the database at completion\n");
        fprintf(stderr, "  -t        Test ilist code\n");
        fprintf(stderr, "  -T        Print timing info\n");
        fprintf(stderr, "  -M        Mail reports\n");
        fprintf(stderr, "  -A        Charge player accounts\n");
        fprintf(stderr, "  -X        Combat test\n");
        exit(1);
    }

    /*
     *  Sat Apr 15 11:55:36 2000 -- Scott Turner
     *
     *  Lock up; prevents multiple TAGs running simultaneously.
     *
     */
    lock_tag();

    load_db();

    /*
     *  Create a couple of stacks and have them battle
     *  it out a few hundred times and report the results.
     *
     */
    if (combat_test_flag) {
        int i, result, sum_a = 0, a, b, c, d;
        struct entity_char *p;
        struct skill_ent *s;
        open_logfile();
        for (i = 0; i < 100; i++) {
            a = new_ent(T_char, 0);
            b = new_ent(T_char, 0);
            c = new_ent(T_char, 0);
            d = new_ent(T_char, 0);
            p = p_char(a);
            p->behind = 9;
            p->health = 100;
            p->attack = 80;
            p->defense = 80;
            p->break_point = 0;
            s = p_skill_ent(a, sk_control_battle);
            s->know = SKILL_know;
            s->experience = 500;
            s = p_skill_ent(a, sk_use_beasts);
            s->know = SKILL_know;
            s->experience = 500;
            set_where(a, 10201);
            set_lord(a, gm_player, LOY_oath, 1);
            set_name(a, "A");
            p = p_char(b);
            p->behind = 0;
            p->health = 100;
            p->attack = 80;
            p->defense = 80;
            p->break_point = 0;
            s = p_skill_ent(b, sk_control_battle);
            s->know = SKILL_know;
            s->experience = 500;
            s = p_skill_ent(b, sk_use_beasts);
            s->know = SKILL_know;
            s->experience = 500;
            set_where(b, 10201);
            set_lord(b, gm_player, LOY_oath, 1);
            set_name(b, "B");
            p = p_char(c);
            p->behind = 0;
            p->health = 100;
            p->attack = 360;
            p->defense = 360;
            p->break_point = 0;
            s = p_skill_ent(c, sk_control_battle);
            s->know = SKILL_know;
            s->experience = 500;
            s = p_skill_ent(c, sk_use_beasts);
            s->know = SKILL_know;
            s->experience = 500;
            set_where(c, 10201);
            set_lord(c, gm_player, LOY_oath, 1);
            set_name(c, "C");
            p = p_char(d);
            p->behind = 9;
            p->health = 100;
            p->attack = 80;
            p->defense = 80;
            p->break_point = 0;
            s = p_skill_ent(d, sk_control_battle);
            s->know = SKILL_know;
            s->experience = 500;
            s = p_skill_ent(d, sk_use_beasts);
            s->know = SKILL_know;
            s->experience = 500;
            set_where(d, 10201);
            set_lord(d, gm_player, LOY_oath, 1);
            set_name(d, "D");
            /*
             *  A & B stacked together and C & D stacked
             *  together.  A & D are behind.
             *
             */
            gen_item(a, item_soldier, 10);
            gen_item(a, item_angel, 15);
            gen_item(c, item_nazgul, 3);
            result = regular_combat(c, a, 0, 0);
            if (result) { sum_a++; }
        };
        close_logfile();
        fprintf(stderr, "Sum = %d.\n", sum_a);
        exit(1);
    };

    if (map_flag) {
        int load_cmap();
        //void print_map();
        if (load_cmap()) {
            print_map(stdout);
        }
        putc('\n', stdout);
        if (load_cmap_players()) {
            print_map(stdout);
        }
        exit(1);
    };

    if (art_flag) {
        int i;
        struct command c;
        c.who = gm_player;
        for (i = 0; i < 10; i++) {
            c.a = create_random_artifact(gm_player);
            v_identify(&c);
            save_box(stderr, c.a);
        };
        /*
         *  Look to see if anyone owns a crown, palantir, etc.
         *
         */
        loop_subkind(sub_npc_token, i)
                {
                    if (item_unique(i) && !is_real_npc(player(item_unique(i)))) {
                        printf("Crown %s held by %s.\n", box_name(i),
                               box_name(player(item_unique(i))));
                    };
                }next_subkind;
        loop_subkind(sub_palantir, i)
                {
                    if (item_unique(i) && !is_real_npc(player(item_unique(i)))) {
                        printf("Palantir %s held by %s.\n", box_name(i),
                               box_name(player(item_unique(i))));
                    };
                }next_subkind;
        loop_subkind(sub_scroll, i)
                {
                    if (item_unique(i) && !is_real_npc(player(item_unique(i)))) {
                        printf("Scroll %s held by %s.\n", box_name(i),
                               box_name(player(item_unique(i))));
                    };
                }next_subkind;
        loop_subkind(sub_suffuse_ring, i)
                {
                    if (item_unique(i) && !is_real_npc(player(item_unique(i)))) {
                        printf("Suffuse ring %s held by %s.\n", box_name(i),
                               box_name(player(item_unique(i))));
                    };
                }next_subkind;
        loop_subkind(sub_artifact, i)
                {
                    if (item_unique(i) && !is_real_npc(player(item_unique(i)))) {
                        printf("Artifact %s held by %s.\n", box_name(i),
                               box_name(player(item_unique(i))));
                    };
                }next_subkind;
        exit(0);
    };

    if (lore_flag) {
        open_logfile();
        gm_show_all_skills(skill_player, TRUE);
        exit(0);
    };

    if (eat_flag) {
        eat_loop(mail_now);
        exit(0);
    }

    if (run_flag) {
        /*
         *  If unspool_first_flag is on, then before running the turn,
         *  eat up any waiting mail.
         *
         */
        if (unspool_first_flag) {
            setbuf(stdout, NULL);
            makedir(sout("%s/orders", libdir), 0755);
            makedir(sout("%s/spool", libdir), 0777);
            chmod(sout("%s/spool", libdir), 0777);
            read_spool(mail_now);
        };

        open_logfile();
        open_times();

        show_day = TRUE;
        pre_month();
        process_orders();
        post_month();
#if 0
        /* This was only to translate to the new system. */
        artifact_fixer();
#endif
        show_day = FALSE;

        determine_output_order();
        turn_end_loc_reports();
        list_order_templates();
        player_ent_info();
        character_report();

        player_banner();
        /*VLNif (acct_flag && !options.free)
            charge_account(); */
        /*VLN report_account();*/
        summary_report();
        player_report();

        scan_char_skill_lore();
        show_lore_sheets();
#if 0
        list_new_players();
#endif
        if (!options.open_ended) { check_win_conditions(); }
        gm_report(gm_player);
        gm_show_all_skills(skill_player, TRUE);
        if (!inhibit_add_flag) { add_new_players(); }
        gen_include_section();        /* must be last */
        close_logfile();

        write_player_list();
        write_nations_lists();
        write_email();
        write_totimes();
        write_forwards();
        write_factions();
    }

    if (add_flag) {
        new_player_top(mail_now);
        mail_now = FALSE;
    }

    if (immediate || immed_after) {
        immediate = TRUE;

        open_logfile();
        immediate_commands();
        close_logfile();
    }

    check_db();            /* check database integrity */

    if (save_flag) {
        save_db();
    }

    if (save_flag && run_flag) {
        save_logdir();
    }

    do_times();

    if (mail_now) {
        mail_reports();
    }

    stage(NULL);

    {
        extern int malloc_size, realloc_size;

        printf("\tmalloc, realloc = %d, %d\n", malloc_size, realloc_size);
    }

    exit(0);
}


void call_init_routines(void) {

    init_lower();
    dir_assert();
    glob_init();        /* initialize global tables */
    initialize_buffer();    /* used by sout() */
    init_spaces();
    init_random();        /* seed random number generator */
}

void write_totimes() {
    FILE *fp;
    char *fnam;
    int pl;

    fnam = sout("%s/totimes", libdir);

    fp = fopen(fnam, "w");

    if (fp == NULL) {
        fprintf(stderr, "can't write %s", fnam);
        perror("");
        return;
    }

    loop_player(pl)
            {
                if (rp_player(pl) &&
                    rp_player(pl)->email &&
                    !player_compuserve(pl)) {
                    fprintf(fp, "%s\n", rp_player(pl)->email);
                }
            }
    next_player;

    fclose(fp);
}


void write_email() {
    FILE *fp;
    char *fnam;
    int pl;

    fnam = sout("%s/email", libdir);

    fp = fopen(fnam, "w");

    if (fp == NULL) {
        fprintf(stderr, "can't write %s", fnam);
        perror("");
        return;
    }

    loop_player(pl)
            {
                if (rp_player(pl) && rp_player(pl)->email) {
                    fprintf(fp, "%s\n", rp_player(pl)->email);
                }
            }
    next_player;

    fclose(fp);
}

static char *
fix_email(char *email) {
    static char buf[256];
    int i, j;
    for (i = 0, j = 0; i < 256 && i < strlen(email); i++, j++) {
        if (email[i] == '@') {
            buf[j] = '(';
            buf[j + 1] = 'a';
            buf[j + 2] = 't';
            buf[j + 3] = ')';
            j += 3;
        } else {
            buf[j] = email[i];
        }
    };
    buf[j] = 0;
    return buf;
};

static void
list_a_player(FILE *fp, int pl, int *flag) {
    struct entity_player *p;
    char *s;
    int n;
    char c;
    char *email;

    p = p_player(pl);

    if (p->email || p->vis_email) {
        if (p->vis_email) {
            email = p->vis_email;
        } else {
            email = p->email;
        }

        if (p->full_name) {
            s = sout("%s &lt;%s&gt;", p->full_name, fix_email(email));
        } else {
            s = sout("&lt;%s&gt", fix_email(email));
        }
    } else if (p->full_name) {
        s = p->full_name;
    } else {
        s = "";
    }

    if (ilist_lookup(new_players, pl) >= 0) {
        c = '*';
        *flag = TRUE;
    } else {
        c = ' ';
    }

    fprintf(fp, "<TR><TD>%4s %c</TD>", box_code_less(pl), c);
    fprintf(fp, "<TD>%s</TD>", just_name(pl));
    fprintf(fp, "<TD>%s</TD>", rp_nation(nation(pl))->name);

    if (*s) {
        fprintf(fp, "<TD>%s</TD>", s);
    }
    fprintf(fp, "</TR>\n");
}

/*
 *  Mon Nov  9 18:09:30 1998 -- Scott Turner
 *
 *  Drop a person (or resubscribe him) to the nation's mailing list.
 *
 */
int
v_nationlist(struct command *c) {
    struct entity_player *p;

    p = p_player(player(c->who));

    if (p->nationlist) {
        p->nationlist = 0;
        wout(c->who, "Will receive the nation mailing list.");
    } else {
        p->nationlist = 1;
        wout(c->who, "Will not receive the nation mailing list.");
    };

    return TRUE;

};

/*
 *  Fri Nov  6 16:01:01 1998 -- Scott Turner
 *
 *  Write out the files that serve as nation mailing lists.
 *
 *  Tue Nov 10 05:55:45 1998 -- Scott Turner
 *
 *  Add DM to all the lists.
 *
 */
void write_nations_lists(void) {
    FILE *fp;
    char *fnam;
    int i;
    int pl;

    stage("write_nations_lists()");

    loop_nation(i)
            {
                fnam = sout("%s/%s", libdir, rp_nation(i)->citizen);
                fp = fopen(fnam, "w");

                if (fp == NULL) {
                    fprintf(stderr, "can't write %s", fnam);
                    perror("");
                    continue;
                }

                /*
                 *  Add regular players who belong to this nation, have
                 *  an email address, and haven't "opted-out".
                 *
                 */
                loop_player(pl)
                        {
                            if (rp_player(pl) &&
                                subkind(pl) == sub_pl_regular &&
                                nation(pl) == i &&
                                rp_player(pl)->email &&
                                !rp_player(pl)->nationlist) {
                                fprintf(fp, "%s\n", rp_player(pl)->email);
                            }
                        }next_player;

                /*
                 *  Add the DM...
                 *
                 */
                if (rp_player(gm_player) &&
                    !rp_player(gm_player)->nationlist) {
                    fprintf(fp, "%s\n", rp_player(gm_player)->email);
                }
                /*
                 *  Close the file.
                 *
                 */
                fclose(fp);
            }next_nation;
}

void write_player_list() {
    FILE *fp;
    char *fnam;
    int pl;
    int flag = FALSE;

    stage("write_player_list()");

    fnam = sout("%s/players.html", libdir);

    fp = fopen(fnam, "w");

    if (fp == NULL) {
        fprintf(stderr, "can't write %s", fnam);
        perror("");
        return;
    }

    fprintf(fp, "<HTML>\n");
    fprintf(fp, "<HEAD>\n");
    fprintf(fp, "<TITLE>Olympia Game %d Player List</TITLE>\n", game_number);
    fprintf(fp, "</HEAD>\n");
    fprintf(fp, "<BODY>\n");
    fprintf(fp, "<TABLE ALIGN=ABSCENTER CELLSPACING=0 CELLPADDING=5 WIDTH=\"100%%\" BGCOLOR=\"#48D1CC\" >\n");
    fprintf(fp, "<TR><TD><B><FONT SIZE=+1>Num</FONT></B></TD>"
                "<TD><B><FONT SIZE=+1>Faction</FONT></B></TD>"
                "<TD><B><FONT SIZE=+1>Nation</FONT></B></TD>"
                "<TD><B><FONT SIZE=+1>Email Address</FONT></B></TD>"
                "</TR>\n");

    loop_player(pl)
            {
                if (rp_player(pl) &&
                    rp_player(pl)->email &&
                    subkind(pl) == sub_pl_regular) {
                    list_a_player(fp, pl, &flag);
                }
            }
    next_player;

    fprintf(fp, "</TABLE>\n");

    if (flag) {
        fprintf(fp, "* -- New player this turn");
    }

    fprintf(fp, "</BODY></HTML>\n");

    fclose(fp);
}


void write_forward_sup(int who_for, int target, FILE *fp) {
    int pl;
    char *s, *u;

    pl = player(who_for);
    s = player_email(pl);

    if (s && *s) {
        fprintf(fp, "%s|%s\n", box_code_less(target), s);
    }
}


void write_forwards() {
    FILE *fp;
    char *fnam;
    int i, j;
    ilist l;

    fnam = sout("%s/forward", libdir);

    fp = fopen(fnam, "w");

    if (fp == NULL) {
        fprintf(stderr, "can't write %s", fnam);
        perror("");
        return;
    }

    loop_player(i)
            {
                write_forward_sup(i, i, fp);
            }
    next_player;

    loop_char(i)
            {
                write_forward_sup(i, i, fp);
            }
    next_char;

    loop_garrison(i)
            {
                if (player(province_admin(i))) {
                    write_forward_sup(player(province_admin(i)), i, fp);
                }
            }
    next_garrison;

    fclose(fp);
}


void write_faction_sup(int who_for, int target, FILE *fp) {
    int pl;
    char *s, *u;

    pl = player(who_for);
    s = player_email(pl);

    if (s && *s) {
        fprintf(fp, "%s|%s\n", box_code_less(target), s);
    }
}


void write_factions() {
    FILE *fp;
    char *fnam;
    int i, j;
    ilist l;

    fnam = sout("%s/factions", libdir);

    fp = fopen(fnam, "w");

    if (fp == NULL) {
        fprintf(stderr, "can't write %s", fnam);
        perror("");
        return;
    }

    loop_player(i)
            {
                write_faction_sup(i, i, fp);
            }
    next_player;

    fclose(fp);
}

/*
 *  Make a report from a raw file into the specified file.
 *
 */
int make_report(int format, char *fnam, char *report, int pl) {
    char form[16], entab[16], tags[16], *tmp;

    tags[0] = 0;
    form[0] = 0;
    entab[0] = 0;
    switch (format) {
        case HTML:
            strcpy(form, "-html");
            break;
        case RAW:
            strcpy(form, "-raw");
            break;
        case TAGS:
            strcpy(form, "-text");
            strcpy(tags, "-tags");
            break;
        default:
            strcpy(form, "-text");
            break;
    };

    if (player_notab(pl)) {
        strcpy(entab, "-notab");
    }

    tmp = sout("rep %s %s %s %s >> %s", form, tags, entab, fnam, report);
    return system(tmp);
};

/*
 *  Wed Nov 10 05:50:57 1999 -- Scott Turner
 *
 *  A little function to return a format string.
 *
 */
static char *
format_string(int i) {
    switch (i) {
        case HTML:
            return "HTML";
        case TEXT:
            return "TEXT";
        case TAGS:
            return "TAGS";
        case RAW:
            return "RAW";
        default:
            break;
    };
    return "UNKNOWN";
};


/*
 *  Sat Oct 30 13:13:45 1999 -- Scott Turner
 *
 *  Added a loop over all the formats.
 *
 */
int send_rep(int pl, int turn) {
    struct entity_player *p;
    char report[LEN];
    FILE *fp;
    int ret;
    char *zfnam = NULL;
    char *fnam;
    char *cmd;
    int split_lines = player_split_lines(pl);
    int split_bytes = player_split_bytes(pl);
    int formats = (int)(p_player(pl)->format);
    int i;
    char *email;

    p = rp_player(pl);

    if (p == NULL || p->email == NULL || *p->email == '\0') {
        return FALSE;
    }

    /*
     *  Default for text format if nothing is set.
     *
     */
    if (formats == 0) { formats = TEXT; }

    /*
     *  Prepare the report file and the input file before the
     *  format loop.
     *
     */
    sprintf(report, "/tmp/sendrep%d.%s", getpid(), box_code_less(pl));

    fnam = sout("%s/save/%d/%d", libdir, turn, pl);

    if (access(fnam, R_OK) < 0) {
        zfnam = sout("%s/save/%d/%d.gz", libdir, turn, pl);

        if (access(zfnam, R_OK) < 0) {
            unlink(report);
            return FALSE;
        }

        fnam = sout("/tmp/zrep.%d", pl);

        ret = system(sout("gzcat %s > %s", zfnam, fnam));

        if (ret) {
            fprintf(stderr, "couldn't unpack %s\n", zfnam);
            unlink(fnam);
            unlink(report);
            return FALSE;
        }
    }

    /*
     *  Here's the format loop.
     *
     */
    for (i = 1; i < ALT; i = i << 1) {
        if (formats & i) {

            fp = fopen(report, "w");
            if (fp == NULL) {
                fprintf(stderr, "send_rep: can't write %s:", report);
                perror("");
                return FALSE;
            }

            fprintf(fp, "From: %s\n", from_host);
            if (reply_host && *reply_host) {
                fprintf(fp, "Reply-To: %s\n", reply_host);
            }
            fprintf(fp, "To: %s (%s)\n", p->email,
                    p->full_name ? p->full_name : "???");
            fprintf(fp, "Subject: Olympia:TAG game %d turn %d report [%s]\n",
                    game_number, turn, format_string(i));
            fprintf(fp, "\n");
            fclose(fp);

            ret = make_report(i, fnam, report, pl);
            if (ret) {
                fprintf(stderr, "send_rep: command failed: %s\n", cmd);
                unlink(report);
                if (zfnam) { unlink(fnam); }
                return FALSE;
            }

            if (split_lines == 0 && split_bytes == 0) {
                /* VLN cmd = sout("sendmail -t -odq < %s", report); */
                cmd = sout("msmtp -t < %s", report);
            } else {
                /* VLN cmd = sout("mailsplit -s %d -l %d -c 'sendmail -t -odq' < %s",
                       split_bytes, split_lines, report); */
                cmd = sout("mailsplit -s %d -l %d -c 'msmtp -t' < %s",
                           split_bytes, split_lines, report);
            }

            fprintf(stderr, "   %s\n", cmd);
            ret = system(cmd);

            if (ret) {
                fprintf(stderr, "send_rep: mail to %s failed: %s\n",
                        p->email, cmd);
            }

            unlink(report);
        }
    };

    if (zfnam) { unlink(fnam); }

    /*
     *  Sun Dec 31 17:45:28 2000 -- Scott Turner
     *
     *  Send the Times also.
     *
     */
    /* VLN cmd = sout("sendmail -odq %s < %s/Times", p->email, libdir); */

    /* VLN remove "," from email adddress string */
    email = p->email;
    for (i = 0; i < strlen(email); i++) {
        if (email[i] == ',') {
            email[i] = ' ';
        }
    };

    /*VLN cmd = sout("msmtp %s < %s/Times", p->email, libdir); */
    cmd = sout("msmtp %s < %s/Times", email, libdir);
    ret = system(cmd);
    if (ret) {
        fprintf(stderr, "send_rep: mail to %s failed: %s\n",
                p->email, cmd);
    }

    return TRUE;
}

void mail_reports(void) {
    int pl;
    int i;

    stage("mail_reports()");

    /*
     *  Refactor -- move all the formats down into send_rep
     *
     */
    loop_player(pl)
            {
                send_rep(pl, sysclock.turn);
            }next_player;

    setup_html_all();
}

int
preprocess(char *in, char *out, char *args) {
    char buf[LEN];
    int err = 0;

    sprintf(buf, "%s %s < %s > %s",
            options.cpp, args, in, out);
    err = system(buf);
    if (err) {
        fprintf(stderr, "Error preprocessing %s.\n", in);
    };
    return err;
};


/*
 *  Rich's on-line web stuff
 *
 *  Fri Oct 15 10:22:40 1999 -- Scott Turner
 *
 *  Modify our umask and permissions so that any file created is
 *  readable by all.  This is necessary for the reports-on-line stuff.
 *
 *  Thu Nov 11 06:56:58 1999 -- Scott Turner
 *
 *  Add stuff to write out the %d.pre file.
 *
 */
void setup_html_all(void) {
    int pl;
    char fnam[LEN];
    char fnam2[LEN];
    FILE *fp;

    stage("setup_html()");

    sprintf(fnam, "%s/%d/%d.pre", options.html_path, game_number, game_number);
    fp = fopen(fnam, "w");
    if (fp == NULL) {
        fprintf(stderr, "Can't open %s for writing?", fnam);
        fp = stderr;
    };
    fprintf(fp, "<TABLE CELLSPACING=0 CELLPADDING=5 WIDTH=\"100%%\" "
                "BGCOLOR=\"#48D1CC\" >\n");

    /* write and execute only for self. */
    umask(S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH);

    loop_player(pl)
            {
                if (!is_real_npc(pl)) {
                    setup_html_dir(pl);
                    set_html_pass(pl);
                    /* VLN output_html_rep(pl);  because these are broken */
                    /*VLN output_html_map(pl); */
                    fprintf(fp, "<TR><TD><A HREF=\"http://UPDATE.com/oly/tag/reports"
                                "/%d/%s/tag%d-%d.html.gz\">[%s] %s</A></TD></TR>\n",
                            game_number,
                            box_code_less(pl),
                            game_number,
                            sysclock.turn,
                            box_code_less(pl),
                            just_name(pl));
                };
            }next_player;

    /* Now switch back. */
    umask(S_IRWXO);

    fprintf(fp, "</TABLE>");
    fclose(fp);

    /*
     *  Now we need to call the C pre-processor on the reports.pre file
     *  to incorporate this latest set of changes.
     *
     */
    sprintf(fnam, "%s/%d/reports.pre", options.html_path, game_number);
    sprintf(fnam2, "%s/%d/reports.html", options.html_path, game_number);
    preprocess(fnam, fnam2, "-P");

    /* copy_public_turns(); */
}

void setup_html_dir(int pl) {
    char fnam[LEN];
    char fnam2[LEN];
    FILE *fp;

    /* read and execute for all. */
    umask(S_IWGRP | S_IWOTH);
    sprintf(fnam, "%s/%d/%s", options.html_path, game_number, box_code_less(pl));
    makedir(fnam, 0777);
    umask(S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH);

    sprintf(fnam2, "%s/.htaccess", fnam);

    fp = fopen(fnam2, "w");
    if (fp == NULL) {
        fprintf(stderr, "can't write %s: ", fnam2);
        perror("");
        return;
    }

    fprintf(fp, "AuthUserFile %s.%d\n", options.html_passwords, game_number);
    fprintf(fp, "AuthGroupFile /dev/null\n");
    fprintf(fp, "AuthName ByPassword\n");
    fprintf(fp, "AuthType Basic\n");
    fprintf(fp, "\n");
    fprintf(fp, "<Limit GET>\n");
    fprintf(fp, "require user %s admin\n", box_code_less(pl));
    fprintf(fp, "</Limit>\n");

    fclose(fp);
}

void set_html_pass(int pl) {
    char buf[LEN];
    struct entity_player *p;
    char *pw;

    p = rp_player(pl);
    if (p == NULL) { return; }

    pw = p->password;
    if (pw == NULL) {
        pw = DEFAULT_PASSWORD;
    }
/*
Usage:
	htpasswd [-cmdpsD] passwordfile username
	htpasswd -b[cmdpsD] passwordfile username password

	htpasswd -n[mdps] username
	htpasswd -nb[mdps] username password
*/
    sprintf(buf, "htpasswd -b %s.%d %s \"%s\"",
            options.html_passwords, game_number,
            box_code_less(pl), pw);

    system(buf);
}

void output_html_rep(int pl) {
    char *report, *fnam;
    int ret;

    fnam = sout("%s/save/%d/%d", libdir, sysclock.turn, pl);
    report = sout("%s/%d/%s/tag%d-%d.html",
                  options.html_path,
                  game_number,
                  box_code_less(pl),
                  game_number,
                  sysclock.turn);
    ret = make_report(HTML, fnam, report, pl);

    if (ret) {
        fprintf(stderr, "Cannot make HTML report for %s?", box_code_less(pl));
        return;
    };

    /*
     *  Compress the output.
     *
     */
    system(sout("gzip -f %s", report));
/* VLN

  system(sout("rm %s/%d/%s/index.html; ln -s %s.gz %s/%d/%s/index.html",
	      options.html_path, game_number, box_code_less(pl),
	      report, options.html_path, game_number, box_code_less(pl)));
*/
}

/*
 *  Mon May 14 13:53:57 2001 -- Scott Turner
 *
 *  Add in what we need to create the info subdirectory and the
 *  map.
 *
 */
void output_html_map(int pl) {
    char *info, *fnam;
    int ret;
    int minx = 1000, miny = 1000, maxx = 0, maxy = 0, i;

    fnam = sout("%s/save/%d/%d", libdir, sysclock.turn, pl);
    info = sout("%s/%d/%s/info",
                options.html_path,
                game_number,
                box_code_less(pl));
    /*
     *  Create info subdir if necessary.
     *
     */
    umask(S_IWGRP | S_IWOTH);
    (void) makedir(info, 0777);
    umask(S_IWOTH | S_IXOTH);

    /*
     *  Run the analyze script on the current turn report, outputting
     *  to the info directory.
     *
     */
    if (system(sout("anlz.pl %s %s", fnam, info)) != 0) {
        fprintf(stderr, "Cannot run anlyz for %s?", box_code_less(pl));
        umask(S_IRWXO);
        return;
    };

    /*
     *  Now run map3.pl.
     *
     *  Mon May 14 17:47:14 2001 -- Scott Turner
     *
     *  Have to figure out min/max values for x and y.
     */
    loop_known(p_player(pl)->known, i)
            {
                if (kind(i) == T_loc &&
                    loc_depth(i) == LOC_province &&
                    region(i) != hades_region &&
                    region(i) != faery_region &&
                    region(i) != cloud_region) {
                    if (region_col(i) < minx) { minx = region_col(i); }
                    if (region_col(i) > maxx) { maxx = region_col(i); }
                    if (region_row(i) < miny) { miny = region_row(i); }
                    if (region_row(i) > maxy) { maxy = region_row(i); }
                };
            }next_known;

    loop_known(p_player(pl)->locs, i)
            {
                if (kind(i) == T_loc &&
                    loc_depth(i) == LOC_province &&
                    region(i) != hades_region &&
                    region(i) != faery_region &&
                    region(i) != cloud_region) {
                    if (region_col(i) < minx) { minx = region_col(i); }
                    if (region_col(i) > maxx) { maxx = region_col(i); }
                    if (region_row(i) < miny) { miny = region_row(i); }
                    if (region_row(i) > maxy) { maxy = region_row(i); }
                };
            }next_known;

    if (system(sout("cd %s;map3.pl map.html %d %d %d %d full",
                    info, minx, maxx, miny, maxy)) != 0) {
        fprintf(stderr, "Cannot run map3 for %s?", box_code_less(pl));
        umask(S_IRWXO);
        return;
    };
    umask(S_IRWXO);
}

#if 0
copy_public_turns()
{
    char fnam[LEN];
    char cmd[LEN];
    int pl;

    loop_player(pl)
    {
        if (!player_public_turn(pl))
            continue;

        sprintf(fnam, "%s/html/%s", libdir, box_code_less(pl));

        sprintf(cmd, "sed -e '/Account summary/,/Balance/d' -e 's/^begin %s.*$/begin %s/' %s/index.html > %s.html",
                box_code_less(pl), box_code_less(pl),
                fnam, fnam);
        system(cmd);
    }
    next_player;
}
#endif

