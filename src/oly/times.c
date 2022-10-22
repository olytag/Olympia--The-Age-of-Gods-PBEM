// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    <time.h>
#include    <stdlib.h>
#include <string.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


/*
 *  Fri Mar  6 09:53:39 1998 -- Scott Turner
 *
 *  Everything to do with the newsletter.
 *
 */
char **parse_text_list(struct command *c);

int line_length_check(char **l);

//
//  COPY_FILE_SLOW
//  Fri Jul  1 15:15:46 1994 -- Scott Turner
//
//  This is the slow version that works on FILEs.
//
//
void copy_file_slow(FILE *inf, FILE *out) {
    char buf[1024];

    if (inf == NULL || out == NULL) { return; }

    while (fgets(buf, 1024, inf) != NULL) {
        fputs(buf, out);
    };

};

void copy_file(char *file1, char *file2) {
    FILE *inf, *outf;
    inf = fopen(file1, "r");
    if (inf == NULL) {
        return;
    };
    outf = fopen(file2, "a");
    if (inf == NULL) {
        return;
    };
    copy_file_slow(inf, outf);
    fclose(inf);
    fclose(outf);
};

#define DASH_LINE "===-===-===-===-===-===-===-===-===-===-===-===-===-===-===-===-===-===-=\n"


static FILE *press_fp = NULL;
static FILE *rumor_fp = NULL;

char *months[] = {
        "January", "Februrary", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December",
};


void
open_times() {
    char *fnam;

    if (press_fp == NULL) {
        fnam = sout("%s/times_press", libdir);
        press_fp = fopen(fnam, "w");
        if (press_fp == NULL) {
            fprintf(stderr, "can't write %s", fnam);
            exit(1);
        }
    }

    fprintf(press_fp, "\n");
    fprintf(press_fp, DASH_LINE);
    fprintf(press_fp, "\n                        Player-contributed press\n\n");
    fprintf(press_fp, DASH_LINE);
    fprintf(press_fp, "\n");
    fflush(press_fp);

    if (rumor_fp == NULL) {
        fnam = sout("%s/times_rumor", libdir);
        rumor_fp = fopen(fnam, "w");
        if (rumor_fp == NULL) {
            fprintf(stderr, "can't write %s", fnam);
            exit(1);
        }
    }

    fprintf(rumor_fp, "                                 Rumors\n\n");
    fprintf(rumor_fp, DASH_LINE);
    fprintf(rumor_fp, "\n");

    fflush(rumor_fp);
}

void
times_masthead() {
    char *fnam;
    time_t l;
    struct tm *tm;
    char date[60];
    char turn_s[60];
    char issue_s[60];
    char date_s[60];
    int nplayers = 0;
    int i;
    FILE *fp;

    fnam = sout("%s/times_0", libdir);
    fp = fopen(fnam, "w");
    if (fp == NULL) {
        fprintf(stderr, "can't write %s", fnam);
        exit(1);
    }

    time(&l);
    tm = localtime(&l);
    sprintf(date, "%s %d, %d",
            months[tm->tm_mon], tm->tm_mday, tm->tm_year + 1900);

    loop_player(i)
            {
                if (subkind(i) == sub_pl_regular) {
                    nplayers++;
                }
            }
    next_player;

    sprintf(turn_s, "Turn %d  %d Players", sysclock.turn, nplayers);
    {
        int month, year;

        month = oly_month(sysclock);
        year = oly_year(sysclock);

        sprintf(issue_s, "Game %d, Season \"%s\", month %d, in the year %d.",
                game_number,
                month_names[month],
                month + 1,
                year + 1);
    }

    fprintf(fp, "From: %s\n", from_host);
    fprintf(fp, "Subject: The Gods Speak (Game %d, Issue %d)\n", game_number, sysclock.turn);
    /* 	fprintf(fp, "To: UPDATE email address here\n\n",game_number); */
    fprintf(fp, "\n   +----------------------------------------------------------------------+\n");
    fprintf(fp, "   | The Gods Speak %53s |\n", date);
    fprintf(fp, "   | %-68s |\n", issue_s);
    fprintf(fp, "   |                                                                      |\n");
    fprintf(fp, "   | %-40s http://olytag.com           |\n", turn_s);
    fprintf(fp, "   |                    Send orders to: tagtest (at) olytag (dot) com     |\n",
            game_number);    /* UPDATE */
    fprintf(fp, "   +----------------------------------------------------------------------+\n\n");
    fprintf(fp, "           Questions, comments, to play:  moderator (at) olytag (dot) com\n\n");    /*UPDATE*/
    fprintf(fp, "                             Olympia PBEM\n\n");
    fprintf(fp, "                                *  *  *\n\n");
    fprintf(fp, "                                *  *  *\n\n");

    fclose(fp);
}

/*
 *  Mon Apr 14 13:10:29 1997 -- Scott Turner
 *
 *  This generates the section of the Times that covers how the
 *  various game goals are doing.
 *
 */
int starting_gold(int nation);

void
times_goal_info() {
    char *fnam;
    int i, j, priests = 0, mus = 0, others = 0, n = 0, unfound = 0, total1 = 0;
    FILE *fp;

    fnam = sout("%s/times_info", libdir);
    fp = fopen(fnam, "w");
    if (fp == NULL) {
        fprintf(stderr, "can't write %s", fnam);
        exit(1);
    };

    fprintf(fp, "\nGame Information\n");
    fprintf(fp, "================\n\n");

    fprintf(fp, "  Nations Summary\n");
    fprintf(fp, "  ---------------\n\n");

    calculate_nation_nps();
    loop_nation(i)
            {
                fprintf(fp, "    %s:\n",
                        rp_nation(i)->name);
                fprintf(fp, "        * Next player joining starts with %d NPs.\n",
                        starting_noble_points(i));
                fprintf(fp, "        * Next player joining starts with %d gold.\n",
                        starting_gold(i));
                fprintf(fp, "        * Players: %d.\n", rp_nation(i)->players);
                fprintf(fp, "        * Nobles (and controlled units): %d.\n", rp_nation(i)->nobles);
                fprintf(fp, "        * Noble points: %d.\n", rp_nation(i)->nps);
                total1 = 0;
                loop_province(j)
                            {
                                if (nation(player_controls_loc(j)) == i) {
                                    ++total1;
                                };
                            }next_province;
                fprintf(fp, "        * Provinces controlled: %d.\n", total1);
                total1 = 0;
                loop_city(j)
                        {
                            if (nation(player_controls_loc(j)) == i) {
                                ++total1;
                            };
                        }next_city;
                fprintf(fp, "        * Cities controlled: %d.\n", total1);
                total1 = 0;
                loop_castle(j)
                        {
                            if (nation(player_controls_loc(j)) == i) {
                                ++total1;
                            };
                        }next_castle;
                fprintf(fp, "        * Castles controlled: %d.\n", total1);
            }next_nation;

    if (!options.open_ended) {
        loop_nation(i)
                {
                    if (rp_nation(i)->win == 1) {
                        fprintf(fp, "\n  *********************************************************\n");
                        fprintf(fp, "      The %s will win at the end of next turn if\n", rp_nation(i)->name);
                        fprintf(fp, "      they maintain the win conditions.\n\n");
                        fprintf(fp, "  *********************************************************\n");
                    } else if (rp_nation(i)->win == 2) {
                        fprintf(fp, "  ***********************************************\n");
                        fprintf(fp, "  ***********************************************\n");
                        fprintf(fp, "    Congratulations to the %s!  They have\n",
                                rp_nation(i)->name);
                        fprintf(fp, "    conquered all enemies and have become the most\n");
                        fprintf(fp, "    powerful nation in the history of Olympia!\n");
                        fprintf(fp, "    Final NP Statistics:\n");
                        fprintf(fp, "       %s: %d NPs.\n", rp_nation(i)->name, rp_nation(i)->nps);
                        loop_nation(j)
                                {
                                    if (j != i) {
                                        fprintf(fp, "       %s: %d NPs.\n", rp_nation(j)->name,
                                                rp_nation(j)->nps);
                                    }
                                }next_nation;
                        fprintf(fp, "\n    Winning Players:\n");
                        loop_pl_regular(j)
                                {
                                    if (nation(j) == i) {
                                        fprintf(fp, "      %s\n", rp_player(j)->full_name);
                                    }
                                }next_pl_regular;
                        fprintf(fp, "  ***********************************************\n");
                        fprintf(fp, "  ***********************************************\n");
                    };
                }next_nation;
    };

    if (options.mp_antipathy) {
        fprintf(fp, "\n  Staff of the Sun Summary\n");
        fprintf(fp, "  ------------------------\n\n");

        /*
       *  How many pieces are out there?
       *
       */
        n = 0;
        loop_subkind(sub_special_staff, i)
                {
                    n++;
                    if (kind(item_unique(i)) == T_char) {
                        if (is_priest(item_unique(i))) {
                            priests++;
                        } else if (is_magician(item_unique(i))) {
                            mus++;
                        };
                        others++;
                    } else {
                        unfound++;
                    };
                }next_subkind;

        if (!priests && !mus && !others) {
            fprintf(fp, "    * No pieces of the Staff of the Sun have been found.\n");
        } else {
            if (others) {
                fprintf(fp, "  * %s pieces of the Staff of the Sun are "
                            "at large.\n", cap(nice_num(others)));
                loop_subkind(sub_special_staff, i)
                        {
                            if (kind(item_unique(i)) == T_char &&
                                !is_magician(item_unique(i)) &&
                                !is_priest(item_unique(i))) {
                                fprintf(fp, "    * One piece is held somewhere on %s.\n",
                                        just_name(region(item_unique(i))));
                            };
                        }next_subkind;
            };
            if (priests) {
                fprintf(fp, "  * %s piece%s of the Staff of the Sun %s "
                            "held by priests.\n", nice_num(priests),
                        priests == 1 ? "" : "s",
                        priests == 1 ? "is" : "are");
                loop_subkind(sub_special_staff, i)
                        {
                            if (kind(item_unique(i)) == T_char &&
                                is_priest(item_unique(i))) {
                                fprintf(fp, "    * One piece is held on %s.\n",
                                        just_name(region(item_unique(i))));
                            };
                        }next_subkind;
            };
            if (mus) {
                fprintf(fp, "  * %s piece%s of the Staff of the Sun %s "
                            "held by magicians.\n", nice_num(mus), mus == 1 ? "" : "s",
                        mus == 1 ? "is" : "are");
                loop_subkind(sub_special_staff, i)
                        {
                            if (kind(item_unique(i)) == T_char &&
                                is_magician(item_unique(i))) {
                                fprintf(fp, "    * One piece is held on %s.\n",
                                        just_name(region(item_unique(i))));
                            };
                        }next_subkind;
            };
        };
    };

    fprintf(fp, "\n                                *  *  *\n\n");
    fprintf(fp, "                                *  *  *\n\n");
    fclose(fp);

};

void
close_times() {

    if (press_fp != NULL) {
        fclose(press_fp);
        press_fp = NULL;
    }

    if (rumor_fp != NULL) {
        fclose(rumor_fp);
        rumor_fp = NULL;
    }
}

static int
times_credit(struct command *c) {
    int pl;
    extern int gold_times;

    if (!options.times_pay) { return TRUE; }

    pl = player(c->who);

    if (times_paid(pl)) {
        wout(c->who, "The Times has already paid faction %s this month.", box_name(pl));
        return FALSE;
    }

    p_player(pl)->times_paid = TRUE;

    wout(pl, "The Times adds %s to your CLAIM.", gold_s(options.times_pay));
    gen_item(pl, item_gold, options.times_pay);
    gold_times += options.times_pay;

    return TRUE;
}

int
v_rumor(struct command *c) {
    char **l = NULL;
    int i;

    l = parse_text_list(c);

    if (l == NULL) {
        return FALSE;
    }

    if (line_length_check(l) > 78) {
        wout(c->who, "Line length of message text exceeds %d "
                     "characters.", 78);
        wout(c->who, "Post rejected.");
        text_list_free(l);
        return FALSE;
    }

    for (i = 0; i < ilist_len(l); i++) {
        if (strncmp(l[i], "===-", 4) == 0) {
            fprintf(rumor_fp, "> %s\n", l[i]);
        } else {
            fprintf(rumor_fp, "%s\n", l[i]);
        }
    }

    fprintf(rumor_fp, "\n");
    fprintf(rumor_fp, DASH_LINE);
    fprintf(rumor_fp, "\n");
    fflush(rumor_fp);

    text_list_free(l);

    wout(c->who, "Rumor posted.");
    times_credit(c);
    return TRUE;
}


int
v_press(struct command *c) {
    char **l = NULL;
    int i;
    char attrib[100];
    char *t;

    l = parse_text_list(c);

    if (l == NULL) {
        return FALSE;
    }

    if (line_length_check(l) > 78) {
        wout(c->who, "Line length of message text exceeds %d "
                     "characters.", 78);
        wout(c->who, "Post rejected.");
        text_list_free(l);
        return FALSE;
    }

    for (i = 0; i < ilist_len(l); i++) {
        if (strncmp(l[i], "===-", 4) == 0) {
            fprintf(press_fp, "> %s\n", l[i]);
        } else {
            fprintf(press_fp, "%s\n", l[i]);
        }
    }

    /*
     *  Turn off tags in the Times?
     *
     */
    tags_off();
    sprintf(attrib, "-- %s", box_name(player(c->who)));
    tags_on();
    for (t = attrib; *t; t++) {
        if (*t == '~') {
            *t = ' ';
        }
    }
    fprintf(press_fp, "\n%55s\n\n", attrib);

    fprintf(press_fp, DASH_LINE);
    fprintf(press_fp, "\n");
    fflush(press_fp);

    text_list_free(l);

    wout(c->who, "Press posted.");
    times_credit(c);
    return TRUE;
}

void times_index() {
    char *fnam;
    FILE *fp;
    int i;

    fnam = sout("%s/index_times_middle", libdir);
    unlink(fnam); /* remove old copy */
    /*
     *  Open the output file.
     *
     */
    fp = fopen(fnam, "a");
    if (fp == NULL) {
        fprintf(stderr, "can't write %s", fnam);
        exit(1);
    };
    /*
     *  Put in the list of back issues.
     *
     */
    for (i = sysclock.turn - 1; i > 0; i--) {
        fprintf(fp, "<A HREF=\"%02d.html\">%02d</A>\n", i);
    };
    fclose(fp);
};

void do_times() {
    char *cmd;
    int ret;

    times_goal_info();
    times_masthead();
    close_times();
    times_index();
    /*
     *  Sun Dec 31 17:50:33 2000 -- Scott Turner
     *
     *  Put it all together.
     *
     */
    cmd = sout("/bin/rm %s/*~", libdir);
    ret = system(cmd);

    if (ret) {
        fprintf(stderr, "Cmd failed: %s\n", cmd);
    }

    cmd = sout("cat %s/times* > %s/Times", libdir, libdir);
    ret = system(cmd);

    if (ret) {
        fprintf(stderr, "Cmd failed: %s\n", cmd);
    }

};

