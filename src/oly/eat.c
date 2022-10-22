// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include <string.h>
#include    <sys/types.h>
#include    <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"
#include "os/generic.h"

#define        MAX_ERR        50


static int cmd_begin = -1;
static int cmd_end = -1;
static int cmd_unit = -1;
static int cmd_email = -1;
static int cmd_vis_email = -1;
static int cmd_lore = -1;
static int cmd_post = -1;
static int cmd_message = -1;
static int cmd_rumor = -1;
static int cmd_press = -1;
static int cmd_format = -1;
static int cmd_notab = -1;
static int cmd_resend = -1;
static int cmd_passwd = -1;
static int cmd_password = -1;
static int cmd_stop = -1;
static int cmd_players = -1;
static int cmd_split = -1;
static int cmd_wait = -1;
static int cmd_build = -1;
static int cmd_set = -1;

static char *cc_addr = NULL;
static char *reply_addr = NULL;
static int pl = 0;
static int unit = 0;
static int n_queued = 0;
static int n_fail = 0;
static int already_seen = FALSE;

static char who_to[LEN];
static char save_line[LEN];
static int line_count = 0;


static void
find_meta_commands() {
    extern int fuzzy_find;

    cmd_begin = find_command("begin");
    assert(cmd_begin > 0);
    assert(!fuzzy_find);

    cmd_end = find_command("end");
    assert(cmd_end > 0);
    assert(!fuzzy_find);

    cmd_unit = find_command("unit");
    assert(cmd_unit > 0);
    assert(!fuzzy_find);

    cmd_email = find_command("email");
    assert(cmd_email > 0);
    assert(!fuzzy_find);

    cmd_vis_email = find_command("vis_email");
    assert(cmd_vis_email > 0);
    assert(!fuzzy_find);

    cmd_lore = find_command("lore");
    assert(cmd_lore > 0);
    assert(!fuzzy_find);

    cmd_post = find_command("post");
    assert(cmd_post > 0);
    assert(!fuzzy_find);

    cmd_rumor = find_command("rumor");
    assert(cmd_rumor > 0);
    assert(!fuzzy_find);

    cmd_press = find_command("press");
    assert(cmd_press > 0);
    assert(!fuzzy_find);

    cmd_format = find_command("format");
    assert(cmd_format > 0);
    assert(!fuzzy_find);

    cmd_notab = find_command("notab");
    assert(cmd_notab > 0);
    assert(!fuzzy_find);

    cmd_message = find_command("message");
    assert(cmd_message > 0);
    assert(!fuzzy_find);

    cmd_resend = find_command("resend");
    assert(cmd_resend > 0);
    assert(!fuzzy_find);

    cmd_passwd = find_command("passwd");
    assert(cmd_passwd > 0);
    assert(!fuzzy_find);

    cmd_password = find_command("password");
    assert(cmd_password > 0);
    assert(!fuzzy_find);

    cmd_stop = find_command("stop");
    assert(cmd_stop > 0);
    assert(!fuzzy_find);

    cmd_players = find_command("players");
    assert(cmd_players > 0);
    assert(!fuzzy_find);

    cmd_split = find_command("split");
    assert(cmd_split > 0);
    assert(!fuzzy_find);

    cmd_wait = find_command("wait");
    assert(cmd_wait > 0);
    assert(!fuzzy_find);

    cmd_build = find_command("build");
    assert(cmd_build > 0);
    assert(!fuzzy_find);

    cmd_set = find_command("option");
    assert(cmd_set > 0);
    assert(!fuzzy_find);
}


static void
init_eat_vars() {

    if (cmd_begin < 0) {
        find_meta_commands();
    }

    if (cc_addr) {
        my_free(cc_addr);
        cc_addr = NULL;
    }

    already_seen = FALSE;
    pl = 0;
    unit = 0;
    n_queued = 0;
    n_fail = 0;
    line_count = 0;
    *save_line = '\0';
}


/*
 *	rmatch("?*<(?*)>", s, &pat))
 *	rmatch("(?*)[ \t]+\\(?**\\)", s, &pat))
 */

static char *
crack_address_sup(char *s) {
    char *t;
    extern char *strchr();

    if (t = strchr(s, '<')) {
        char *u;

        t++;
        for (u = t; *u && *u != '>'; u++) {}
        *u = '\0';
        return t;
    }

    while (*s && iswhite(*s)) {
        s++;
    }

    for (t = s; *t && !iswhite(*t); t++) {}
    *t = '\0';

    return s;
}


#if 0
/*
 *  01234567890123456
 *  a%b@princeton.edu	==>  a@b
 */

static char *
local_kludge(char *s)
{
  int l = strlen(s);
  char *t;
  extern char *strchr();

  if (l < 17)
    return s;

  if (i_strcmp("@Princeton.EDU", s + (l - 14)) == 0 &&
      (t = strchr(s, '%')))
    {
      s[l-14] = '\0';
      *t = '@';
    }

  return s;
}
#endif


static char *
crack_address(char *s) {

    s = crack_address_sup(s);

    if (s) {
#if 0
        s = local_kludge(s);
#endif
        return str_save(s);
    }

    return NULL;
}


static char *
parse_reply(FILE *fp, int *cpp) {
    static char *from_space = NULL;
    static char *from_colon = NULL;
    static char *reply_to = NULL;
    char *s;
    char *t;

    *cpp = 0;

    if (from_space) {
        my_free(from_space);
        from_space = NULL;
    }

    if (from_colon) {
        my_free(from_colon);
        from_colon = NULL;
    }

    if (reply_to) {
        my_free(reply_to);
        reply_to = NULL;
    }

    s = getlin(fp);

    if (strncmp(s, "From ", 5) != 0) {
        return NULL;
    }

    s += 5;
    for (t = s; *t && !iswhite(*t); t++) {}
    *t = '\0';

    if (!*s) {        /* did we get the From_ address? */
        return NULL;
    }

    from_space = str_save(s);

    while (s = getlin(fp)) {
        if (!*s) {
            break;
        }

        if (strncmp(s, "Subject:", 8) == 0 &&
            (strstr(s, "cpp") != 0 || strstr(s, "preprocess") != 0)) {
            /* Subject line indicator asking for preprocessing? */
            *cpp = 1;
        } else if (i_strncmp(s, "From:", 5) == 0) {
            from_colon = crack_address(&s[5]);
        } else if (i_strncmp(s, "Reply-To:", 9) == 0) {
            reply_to = crack_address(&s[9]);
        } else if (i_strncmp(s, "X-Loop", 6) == 0) {
            already_seen = TRUE;
        }
    }

    if (reply_to) {
        return reply_to;
    }
    if (from_colon) {
        return from_colon;
    }
    return from_space;
}


static char *
eat_line_2(FILE *fp, int eat_white) {
    char *line;

    if (eat_white) {
        line = getlin_ew(fp);
    } else {
        line = getlin(fp);
    }

    if (line == NULL) {
        return NULL;
    }

    remove_ctrl_chars(line);

    if (eat_white) {
        while (iswhite(*line)) {
            line++;
        }
    }

    {
        strncpy(save_line, line, LEN - 1);
        save_line[LEN - 1] = '\0';

        line_count++;
    }

    return line;
}


static char *
eat_next_line_sup(FILE *fp) {
    char *line;

    line = getlin_ew(fp);

    if (line == NULL) {
        return NULL;
    }

    remove_comment(line);
    remove_ctrl_chars(line);

    while (iswhite(*line)) {
        line++;
    }

    {
        strncpy(save_line, line, LEN - 1);
        save_line[LEN - 1] = '\0';

        line_count++;
    }

    return line;
}


static char *
eat_next_line(FILE *fp) {
    char *line;

    do {
        line = eat_next_line_sup(fp);
    } while (line && *line == '\0');

    return line;
}

static int last_line = -1;

static void
err(int k, char *s) {

    out_alt_who = k;

    if (k == EAT_ERR) {
        n_fail++;
    }

    if (line_count < last_line) { last_line = 0; }

    if (line_count > last_line) {
        out(eat_pl, "line %d: %s", line_count, save_line, s);
        last_line = line_count;
    };

    indent += 3;
    wiout(eat_pl, 2, "* %s", s);
    indent -= 3;
}


static void
next_cmd(FILE *fp, struct command *c) {
    char *line;

    c->cmd = 0;

    while (1) {
        line = eat_next_line(fp);

        if (!line) {
            c->cmd = cmd_end;
            return;
        }

        if (!oly_parse(c, line)) {
            err(EAT_ERR, "unrecognized command");

            if (n_fail > MAX_ERR) {
                err(EAT_ERR, "too many errors, aborting");
                c->cmd = cmd_end;
                return;
            }
            continue;
        }

        if (c->fuzzy) {
            err(EAT_WARN, sout("assuming you meant '%s'",
                               cmd_tbl[c->cmd].name));
        }

        return;
    }
}

/*
 *  Tue Apr 17 12:23:03 2001 -- Scott Turner
 *
 *  How much should we charge someone?
 *
 */
char *
turn_charge(int pl) {
    int i, nps = 0;
    loop_units(pl, i)
            {
                nps += nps_invested(i);
            }next_unit;

    if (nps > options.free_np_limit) {
        return options.turn_charge;
    } else {
        return "0.00";
    };

};


static int
do_begin(struct command *c) {
    char *pl_pass;

    if (numargs(c) < 1) {
        err(EAT_ERR, "No player specified on BEGIN line");
        return FALSE;
    }

    if (kind(c->a) != T_player) {
        err(EAT_ERR, "No such player");
        return FALSE;
    }

    /*
     *  Tue Apr 17 12:09:14 2001 -- Scott Turner
     *
     *  Check for a low balance, and reject this order set if
     *  they can't afford to pay for the next turn.
     *
     */
    if (options.check_balance) {
        char *cmd = sout("%s -p %s -g tag%d -T %s > /dev/null",
                         options.accounting_prog,
                         box_code_less(pl),
                         game_number,
                         turn_charge(pl));
        int result = system(cmd);

        if (result != -1 && result != 0) {
            err(EAT_ERR, "*********************************************************");
            err(EAT_ERR, "**                                                     **");
            err(EAT_ERR, "** Warning: Low account balance                        **");
            err(EAT_ERR, "**                                                     **");
            err(EAT_ERR, "** Your account has a low balance and you cannot       **");
            err(EAT_ERR, "** afford to pay for your next turn.                   **");
            err(EAT_ERR, "**                                                     **");
            err(EAT_ERR, "** See FIXME                                           **");
            err(EAT_ERR, "**                                                     **");
            err(EAT_ERR, "*********************************************************");
            return FALSE;
        };
    };

    pl_pass = p_player(c->a)->password;

    if (numargs(c) > 1) {
        if (pl_pass == NULL || *pl_pass == '\0') {
            err(EAT_WARN, "No password is currently set");
        } else if (i_strcmp(pl_pass, c->parse[2]) != 0) {
            err(EAT_ERR, "Incorrect password");
            return FALSE;
        }
    } else if (pl_pass && *pl_pass) {
        err(EAT_ERR, "Incorrect password");
        err(EAT_ERR, "Must give password on BEGIN line.");
        return FALSE;
    }

    pl = c->a;

    p_player(pl)->sent_orders = 1;    /* okay, they sent something in */

#if 0
    /*
     *  We set unit here in case they forget the UNIT command for the
     *  player entity.  If they do, they lose the auto-flush ability,
     *  but at least their command will get queued, and echoed back
     *  in the confirmation.
     */

    unit = pl;
#endif

    return TRUE;
}


static int
valid_char_or_player(int who) {

    if (kind(who) == T_char || kind(who) == T_player) {
        return TRUE;
    }

    if (kind(who) == T_item && subkind(who) == sub_dead_body) {
        return TRUE;
    }

    return FALSE;
}


static int
do_unit(struct command *c) {

    unit = -1;    /* ignore following unit commands */

    if (pl == 0) {
        err(EAT_ERR, "BEGIN must appear before UNIT");
        out(eat_pl, "      rest of commands for unit ignored");
        return TRUE;
    }

    if (kind(c->a) == T_unform) {
        if (ilist_lookup(p_player(pl)->unformed, c->a) < 0) {
            err(EAT_WARN, "Not an unformed unit of yours.");
        }
    } else if (!valid_char_or_player(c->a)) {
        err(EAT_ERR, "Not a character or unformed unit.");
        return TRUE;
    } else if (player(c->a) != pl) {
        err(EAT_WARN, "Not one of your controlled characters.");
    }

    unit = c->a;
    flush_unit_orders(pl, unit);
    return TRUE;
}


static int
do_email(struct command *c) {

    if (cc_addr) {
        err(EAT_ERR, "no more than one EMAIL order per message");
        out(eat_pl, "      new email address not set");
        return TRUE;
    }

    if (pl == 0) {
        err(EAT_ERR, "BEGIN must come before EMAIL");
        out(eat_pl, "      new email address not set");
        return TRUE;
    }

    if (numargs(c) < 1 || c->parse[1] == NULL || !*(c->parse[1])) {
        err(EAT_ERR, "no new email address given");
        out(eat_pl, "      new email address not set");
        return TRUE;
    }

    cc_addr = rp_player(pl)->email;
    p_player(pl)->email = str_save(c->parse[1]);

    return TRUE;
}


static int
do_vis_email(struct command *c) {

    if (pl == 0) {
        err(EAT_ERR, "BEGIN must come before VIS_EMAIL");
        out(eat_pl, "      new address not set");
        return TRUE;
    }

    if (numargs(c) < 1 || c->parse[1] == NULL || !*(c->parse[1])) {
        p_player(pl)->vis_email = NULL;
        return TRUE;
    }

    p_player(pl)->vis_email = str_save(c->parse[1]);

    return TRUE;
}


static int
do_lore(struct command *c) {
    int sheet = c->a;

    if (pl == 0) {
        err(EAT_ERR, "BEGIN must appear before LORE");
        return TRUE;
    }

    if (kind(sheet) == T_item) {
        sheet = item_lore(sheet);
    }

    if (!valid_box(sheet)) {
        err(EAT_ERR, "no such lore sheet");
        return TRUE;
    }

    if (!test_known(pl, sheet)) {
        err(EAT_ERR, "you haven't seen that lore sheet before");
        return TRUE;
    }

    out_alt_who = OUT_LORE;
    deliver_lore(eat_pl, c->a);

    return TRUE;
}


static int
do_players(struct command *c) {
    FILE *fp;
    char *fnam;
    char *s;

    fnam = sout("%s/save/%d/players.html", libdir, sysclock.turn);
    fp = fopen(fnam, "r");

    if (fp == NULL) {
        err(EAT_ERR, sout("Sorry, couldn't find the player list."));
        return TRUE;
    }

    out_alt_who = EAT_PLAYERS;

    while (s = getlin(fp)) {
        out(eat_pl, "%s", s);
    }

    fclose(fp);

    return TRUE;
}


static int
do_resend(struct command *c) {
    int turn;

    if (pl == 0) {
        err(EAT_ERR, "BEGIN must appear before RESEND");
        return TRUE;
    }

    turn = c->a;
    if (turn == 0) {
        turn = sysclock.turn;
    }

    if (send_rep(pl, turn)) {
        out_alt_who = EAT_OKAY;
        wout(eat_pl, "Turn %d report has been mailed to you "
                     "in a separate message.", turn);
    } else {
        err(EAT_ERR, sout("Sorry, couldn't find your turn %d "
                          "report", turn));
    }

    return TRUE;
}


static int
do_format(struct command *c) {
    if (pl == 0) {
        err(EAT_ERR, "BEGIN must appear before FORMAT");
        return TRUE;
    }

    c->who = pl;
    v_format(c);
    err(EAT_WARN, "Formatting set.");

    return TRUE;
}


static int
do_split(struct command *c) {
    int lines = c->a;
    int bytes = c->b;

    if (pl == 0) {
        err(EAT_ERR, "BEGIN must appear before SPLIT");
        return TRUE;
    }

    out_alt_who = EAT_OKAY;

    if (lines > 0 && lines < 500) {
        lines = 500;
        out(eat_pl, "Minimum lines to split at is 500");
    }

    if (bytes > 0 && bytes < 10000) {
        bytes = 10000;
        out(eat_pl, "Minimum bytes to split at is 10,000");
    }

    p_player(pl)->split_lines = lines;
    p_player(pl)->split_bytes = bytes;

    if (lines == 0 && bytes == 0) {
        out(eat_pl, "Reports will not be split when mailed.");
    } else if (lines && bytes) {
        out(eat_pl, "Reports will be split at %d lines or "
                    "%d bytes, whichever limit is hit first.",
            lines, bytes);
    } else if (lines) {
        out(eat_pl, "Reports will be split at %d lines.", lines);
    } else {
        out(eat_pl, "Reports will be split at %d bytes.", bytes);
    }

    return TRUE;
}


static int
do_notab(struct command *c) {

    if (pl == 0) {
        err(EAT_ERR, "BEGIN must appear before NOTAB");
        return TRUE;
    }

    p_player(pl)->notab = c->a;

    out_alt_who = EAT_OKAY;
    if (c->a) {
        wout(eat_pl, "No TAB characters will appear in turn "
                     "reports.");
    } else {
        wout(eat_pl, "TAB characters may appear in turn "
                     "reports.");
    }

    return TRUE;
}


static int
do_password(struct command *c) {

    if (pl == 0) {
        err(EAT_ERR, "BEGIN must appear before PASSWORD");
        return TRUE;
    }

    if (numargs(c) < 1 || *(c->parse[1]) == '\0') {
        p_player(pl)->password = NULL;

        out_alt_who = EAT_OKAY;
        wout(eat_pl, "Password cleared.");
        return TRUE;
    }

    p_player(pl)->password = str_save(c->parse[1]);

    out_alt_who = EAT_OKAY;
    wout(eat_pl, "Password set to \"%s\".", c->parse[1]);

    return TRUE;
}


#define DASH_LINE "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="

static void
show_post(char **l, int cmd) {
    int i;
    char attrib[100];
    char *t;
    int sav = out_alt_who;

    out_alt_who = OUT_SHOW_POSTS;

    for (i = 0; i < ilist_len(l); i++) {
        if (strncmp(l[i], "=-=-", 4) == 0) {
            out(eat_pl, "> %s", l[i]);
        } else {
            out(eat_pl, "%s", l[i]);
        }
    }

    sprintf(attrib, "-- %s", box_name(player(unit)));
    for (t = attrib; *t; t++) {
        if (*t == '~') {
            *t = ' ';
        }
    }

    out(eat_pl, "");

    if (cmd == cmd_press) {
        out(eat_pl, "%55s", attrib);
        out(eat_pl, "");
    }

    out(eat_pl, DASH_LINE);

    out_alt_who = sav;
}


/*
 *  Mon Dec 20 12:06:30 1999 -- Scott Turner
 *
 *  The "SET" command, which will incorporate a number of exiting
 *  commands.  It is intended to capture "meta-level" stuff, not
 *  actual noble actions.  All these should be "immediate" actions.
 *
 *      email
 *      format
 *  	notab
 *  	password
 *      vis_email
 * 	rules_url
 *	db_url
 *
 */
int do_set(struct command *c) {
    char *cmd = c->parse[1];

    /*
     *  Shift the "set" out of the way.
     *
     */
    cmd_shift(c);

    if (strcasecmp(cmd, "email") == 0) {
        return do_email(c);
    } else if (strcasecmp(cmd, "format") == 0) {
        return v_format(c);
    } else if (strcasecmp(cmd, "notab") == 0) {
        return v_notab(c);
    } else if (strcasecmp(cmd, "password") == 0) {
        return do_password(c); /* ? */
    } else if (strcasecmp(cmd, "split") == 0) {
        return do_split(c); /* ? */
    } else if (strcasecmp(cmd, "vis_email") == 0) {
        return do_vis_email(c);
    } else if (strcasecmp(cmd, "rules_url") == 0) {
        return do_rules_url(c);
    } else if (strcasecmp(cmd, "db_url") == 0) {
        return do_db_url(c);
    } else {
        out_alt_who = EAT_OKAY;
        wout(c->who ? c->who : eat_pl,
             "I don't know how to set option \"%s\".", cmd);
        return FALSE;
    };
};

/*
 *  Wed Dec 27 12:50:15 2000 -- Scott Turner
 *
 *  Command commenters.
 *
 */

/*
 *  It's safe to give a full name for something if it's
 *  either known to the player or a common item or skill.
 *
 */
int
is_safe(int who, int n) {
    /*
     *  Might not be something.
     *
     */
    if (!valid_box(n)) { return 0; }
    /*
     *  Common item.
     *
     */
    if (kind(n) == T_item && !item_unique(n)) { return 1; }
    /*
     *  Skill
     *
     */
    if (kind(n) == T_skill) { return 1; }
    /*
     *  Might be who
     *
     */
    if (who == n) { return 1; }
    /*
     *  Might be a unit of this player.
     *
     */
    if (kind(n) == T_char && player(who) && is_unit(player(who), n)) { return 1; }
    /*
     *  Otherwise, if they "know" about it.
     *
     */
    if (valid_box(who) && valid_box(n) && player(who)) {
        return test_known(who, n);
    }

    return 0;
};


char *
safe_name_qty(int who, int n, int qty, char *str) {
    if (n == 0) {
        return str;
    }

    if (!is_safe(who, n)) {
        if (qty == 1) {
            return box_code(n);
        } else {
            return sout("%s %s", nice_num(qty), box_code(n));
        };
    } else {
        return box_name_qty(n, qty);
    };
};

char *
safe_name(int who, int n, char *str) {
    if (n == 0) { return str; }

    if (!is_safe(who, n)) {
        return box_code(n);
    } else {
        return box_name(n);
    };
};

char *
buy_comment(struct command *c) {
    if (numargs(c) < 1) {
        return sout("probably incorrect");
    } else if (numargs(c) == 1 ||
               (numargs(c) == 2 && c->b == 0) ||
               (numargs(c) == 3 && c->b == 0 && c->c == 0)) {
        return sout("clear %s for %s",
                    c->parse[0],
                    safe_name(c->who, c->a, c->parse[1]));
    } else if (numargs(c) == 2) {
        return sout("probably incorrect: specify an item, quantity and price");
    } else if (numargs(c) == 3) {
        return sout("%s %s for %s gold each",
                    c->parse[0],
                    safe_name_qty(c->who, c->a, c->b, c->parse[1]),
                    nice_num(c->c));
    } else {
        return sout("%s %s for %s gold each, keeping %s",
                    c->parse[0],
                    safe_name_qty(c->who, c->a, c->b, c->parse[1]),
                    nice_num(c->c),
                    nice_num(c->d));
    };
};

char *
drop_comment(struct command *c) {
    if (numargs(c) < 2) {
        return sout("probably incorrect");
    } else if (numargs(c) < 3) {
        if (c->b == 0) {
            return sout("drop all %s",
                        safe_name(c->who, c->a, c->parse[1]));
        } else {
            return sout("drop %s %s",
                        nice_num(c->b),
                        safe_name(c->who, c->a, c->parse[1]));
        }
    } else {
        if (c->b == 0) {
            return sout("drop all %s (keeping %s)",
                        safe_name(c->who, c->a, c->parse[1]),
                        nice_num(c->c));
        } else {
            return sout("drop %s %s (keeping %s)",
                        nice_num(c->b),
                        safe_name(c->who, c->a, c->parse[1]),
                        nice_num(c->c));
        }
    };
};

char *
collect_comment(struct command *c) {
    if (numargs(c) < 1) {
        return sout("probably incorrect");
    } else if (numargs(c) == 1 ||
               (numargs(c) == 2 && c->b == 0) ||
               (numargs(c) == 3 && c->b == 0 && c->c == 0)) {
        return sout("collect all %s", safe_name(c->who, c->a, c->parse[1]));
    } else if (numargs(c) == 2 ||
               (numargs(c) == 3 && c->c == 0)) {
        return sout("collect %s %s",
                    nice_num(c->b),
                    safe_name(c->who, c->a, c->parse[1]));
    } else if (numargs(c) == 3) {
        return sout("collect %s %s for %s day%s",
                    nice_num(c->b),
                    safe_name(c->who, c->a, c->parse[1]),
                    nice_num(c->c), (c->c == 1) ? "" : "s");
    };
    return "";
};

static struct command d_cmd;

char *
catch_comment(struct command *c) {
    struct command *cp = &d_cmd;
    char *s;
    int ret;

    ret = oly_parse(cp,
                    sout("collect %d %d %d", item_wild_horse, c->a, c->b));
    assert(ret);
    return collect_comment(cp);
};

char *
give_comment(struct command *c) {
    if (numargs(c) < 2) {
        return sout("probably incorrect");
    } else if (numargs(c) < 3) {
        return sout("give all %s to %s",
                    safe_name(c->who, c->b, c->parse[2]),
                    safe_name(c->who, c->a, c->parse[1]));
    } else if (numargs(c) < 4) {
        if (c->c == 0) {
            return sout("give all %s to %s",
                        safe_name(c->who, c->b, c->parse[2]),
                        safe_name(c->who, c->a, c->parse[1]));
        } else {
            return sout("give %s to %s",
                        safe_name_qty(c->who, c->b, c->c, c->parse[2]),
                        safe_name(c->who, c->a, c->parse[1]));
        };
    } else {
        if (c->c == 0) {
            return sout("give all %s to %s, keeping %s",
                        safe_name(c->who, c->b, c->parse[2]),
                        safe_name(c->who, c->a, c->parse[1]),
                        nice_num(c->d));
        } else {
            return sout("give %s to %s, keeping %s",
                        safe_name_qty(c->who, c->b, c->c, c->parse[2]),
                        safe_name(c->who, c->a, c->parse[1]),
                        nice_num(c->d));

        };
    };
};

char *
get_comment(struct command *c) {
    if (numargs(c) < 2) {
        return sout("probably incorrect");
    } else if (numargs(c) < 3) {
        return sout("get all %s from %s",
                    safe_name(c->who, c->b, c->parse[2]),
                    safe_name(c->who, c->a, c->parse[1]));
    } else if (numargs(c) < 4) {
        if (c->c == 0) {
            return sout("get all %s from %s",
                        safe_name(c->who, c->b, c->parse[2]),
                        safe_name(c->who, c->a, c->parse[1]));
        } else {
            return sout("get %s from %s",
                        safe_name_qty(c->who, c->b, c->c, c->parse[2]),
                        safe_name(c->who, c->a, c->parse[1]));
        };
    } else {
        if (c->c == 0) {
            return sout("get all %s from %s, leaving %s",
                        safe_name(c->who, c->b, c->parse[2]),
                        safe_name(c->who, c->a, c->parse[1]),
                        nice_num(c->d));
        } else {
            return sout("get %s from %s, leaving %s",
                        safe_name_qty(c->who, c->b, c->c, c->parse[2]),
                        safe_name(c->who, c->a, c->parse[1]),
                        nice_num(c->d));
        };
    };
};

char *
study_comment(struct command *c) {
    if (numargs(c) < 1) {
        return sout("probably incorrect");
    } else {
        return sout("%s %s",
                    c->parse[0],
                    safe_name(c->who, c->a, c->parse[1]));
    };
};

char *
admit_comment(struct command *c) {
    if (numargs(c) < 1) {
        return sout("probably incorrect");
    } else if (numargs(c) == 1) {
        return sout("clear admits for %s",
                    safe_name(c->who, c->a, c->parse[1]));
    } else {
        int args[6], i;
        char *arg = sout("admit to %s: %s",
                         safe_name(c->who, c->a, c->parse[1]),
                         safe_name(c->who, c->b, c->parse[2]));
        args[3] = c->c;
        args[4] = c->d;
        args[5] = c->e;

        if (strncasecmp(c->parse[1], "all", strlen(c->parse[1])) == 0) {
            arg = sout("%s EXCEPT", arg);
        }

        for (i = 3; i <= min(numargs(c), 5); i++) {
            arg = sout("%s, %s", arg, safe_name(c->who, args[i], c->parse[i]));
        };
        return arg;
    };
    return "";
};

static int first_admit_check = 1;

void
admit_check(struct command *c) {
    if (numargs(c) < 1) {
        return;
    } else {
        /*
         *  Look for a nation or monster name.
         *
         */
        if (!find_nation(c->parse[1]) &&
            !fuzzy_strcmp(c->parse[1], "monster") &&
            !fuzzy_strcmp(c->parse[1], "monsters") &&
            !fuzzy_strcmp(c->parse[1], "garrison") &&
            (!valid_box(c->a) || kind(c->a) != T_char)) {
            /*
             *  Possibly an "unformed" unit.
             *
             */
            if (kind(c->a) == T_unform) {
                err(EAT_WARN, sout("Note: %s is currently an unformed unit.",
                                   safe_name(c->who, c->a, c->parse[1])));
            } else {
                if (first_admit_check) {
                    err(EAT_WARN, "The first argument to admit should be a noble.  "
                                  "If you're trying to admit someone to a location "
                                  "(such as a province or castle) then you should "
                                  "admit them to the noble that controls that "
                                  "location, not the location itself.");
                    first_admit_check = 0;
                } else {
                    err(EAT_WARN, "The first argument to admit should be a noble.");
                }
            };
        };
    };
};

void
quit_check(struct command *c) {
    err(EAT_WARN, "This command will drop you from the game.");
    err(EAT_WARN, "If you are quitting, please send moderator@olytag.com a quick "    /*UPDATE */
                  "email to indicate why you are dropping.  Your feedback is important "
                  "and helps to keep improving The Age of Gods.  Thanks.");
};

char *
accept_comment(struct command *c) {
    char *s1, *s2;

    if (numargs(c) < 1) {
        return sout("probably incorrect");
    } else if (numargs(c) == 1) {
        if (strcasecmp("clear", c->parse[1]) == 0) {
            return sout("clearing all accepts");
        } else {
            return sout("accept all from %s",
                        safe_name(c->who, c->a, c->parse[1]));
        };
    } else {
        if (c->a) {
            s1 = safe_name(c->who, c->a, c->parse[1]);
        } else {
            s1 = "anyone";
        };
        if (c->b) {
            s2 = safe_name(c->who, c->b, c->parse[2]);
        } else {
            s2 = "anything";
        };
        if (numargs(c) == 2) {
            return sout("accept %s from %s", s2, s1);
        } else {
            return sout("accept up to %s %s from %s",
                        nice_num(c->c),
                        s2, s1);
        };
    }
    return "";
};

static char *
destination_name(struct command *c, int i, int num) {
    int dir;

    dir = lookup(full_dir_s, c->parse[i]);

    if (dir < 0) {
        dir = lookup(short_dir_s, c->parse[i]);
    }

    if (dir < 0) {
        return sout("to %s",
                    safe_name(c->who, num, c->parse[i]));
    } else {
        return sout("%s", full_dir_s[dir]);
    };
    return "";
};

char *
move_comment(struct command *c) {
    int dir, i;
    int args[6];
    char *arg = "";

    args[1] = c->a;
    args[2] = c->b;
    args[3] = c->c;
    args[4] = c->d;

    if (numargs(c) < 1) {
        return "probably incorrect";
    } else {
        arg = sout("%s %s", c->parse[0], destination_name(c, 1, args[1]));
        if (numargs(c) > 1) {
            arg = sout("%s (or %s", arg, destination_name(c, 2, args[2]));
            for (i = 3; i <= min(numargs(c), 4); i++) {
                arg = sout("%s, %s", arg, destination_name(c, i, args[i]));
            };
            if (numargs(c) > 4) {
                arg = sout("%s...)", arg);
            } else {
                arg = sout("%s)", arg);
            };
        };
        return arg;
    };
    return "";
};

char *
attack_comment(struct command *c) {
    int dir, i;
    int args[6];
    char *arg = "";
    int num = numargs(c);
    int nomove = 0;

    args[1] = c->a;
    args[2] = c->b;
    args[3] = c->c;
    args[4] = c->d;

    /*
     *  Possibly a flag at the end.
     *
     */
    if (num > 1 && strcmp("1", c->parse[num]) == 0) {
        nomove = 1;
        num--;
    };

    if (num < 1) {
        return "probably incorrect";
    } else {
        arg = sout("%s %s", c->parse[0], destination_name(c, 1, args[1]));
        if (num > 1) {
            arg = sout("%s (or %s", arg, destination_name(c, 2, args[2]));
            for (i = 3; i <= min(num, 4); i++) {
                arg = sout("%s, %s", arg, destination_name(c, i, args[i]));
            };
            if (num > 4) {
                arg = sout("%s...)", arg);
            } else {
                arg = sout("%s)", arg);
            };
        };
        if (nomove) {
            arg = sout("%s (without entering)", arg);
        };
        return arg;
    };
    return "";
};

char *
default_comment(struct command *c) {
    if (numargs(c) != 1) {
        return "";
    } else {
        return sout("%s %s",
                    c->parse[0],
                    safe_name(c->who, c->a, c->parse[1]));
    };
    return "";
};

char *
attitude_comment(struct command *c) {
    char *arg;
    int i;

    if (numargs(c) == 0) {
        arg = sout("Clear %s list.", c->parse[0]);
    } else {
        arg = sout("%s to", c->parse[0]);
        for (i = 1; i <= numargs(c); i++) {
            arg = sout("%s %s", arg,
                       safe_name(c->who, c->a, c->parse[i]));
        };
    };
    return arg;
};

/*
 *  Wed Dec 27 08:20:27 2000 -- Scott Turner
 *
 *  Check an argument against a "type".
 *
 */
static void
check_arg(struct command *c, int i, int t) {
    int args[6];

    if (i < 1 || i > 5) { return; }

    args[1] = c->a;
    args[2] = c->b;
    args[3] = c->c;
    args[4] = c->d;
    args[5] = c->e;

    switch (t) {
        case CMD_undef:
            return;
        case CMD_unit:
            /*
             *  Look for a nation or monster name.
             *
             */
            if (find_nation(c->parse[i])) { return; }
            if (fuzzy_strcmp(c->parse[i], "monster")) { return; }
            if (fuzzy_strcmp(c->parse[i], "monsters")) { return; }
            if (fuzzy_strcmp(c->parse[i], "garrison")) { return; }
            if (fuzzy_strcmp(c->parse[i], "garison")) { return; }
            /*
             *  Otherwise, it should be a unit, player, etc.
             *
             */
            if (valid_box(args[i]) &&
                (kind(args[i]) == T_player ||
                 kind(args[i]) == T_char)) {
                return;
            }
            /*
             *  Possibly an "unformed" unit.
             *
             */
            if (valid_box(args[i]) &&
                kind(args[i]) == T_unform) {
                err(EAT_WARN, sout("Note: %s is currently an unformed unit.",
                                   safe_name(c->who, args[i], c->parse[i])));
                return;
            };

            /*
             *  Hmmm.
             *
             */
            err(EAT_WARN, sout("The %s argument of this command should be a unit "
                               "(player, noble, or nation)."
                               "  %s does not appear to be a unit.",
                               ordinal(i), safe_name(c->who, args[i], c->parse[i])));

            return;
        case CMD_item:
            if (!valid_box(args[i]) || kind(args[i]) != T_item) {
                err(EAT_WARN,
                    sout("The %s argument of this command should be an item. %s does not appear to be an item.",
                         ordinal(i), safe_name(c->who, args[i], c->parse[i])));
            };
            return;
        case CMD_skill:
            if (!valid_box(args[i]) || kind(args[i]) != T_skill) {
                err(EAT_WARN,
                    sout("The %s argument of this command should be a skill. %s does not appear to be an skill.",
                         ordinal(i), safe_name(c->who, args[i], c->parse[i])));
            };
            return;
        case CMD_days:
            if (args[i] > 30) {
                err(EAT_WARN, "Note: This command may last more than a month.");
            };
            return;
        case CMD_qty:
            return;
        case CMD_gold:
            if (unit && has_item(unit, item_gold) < args[i]) {
                err(EAT_WARN, sout("This command uses %s gold, and this unit "
                                   "currently has only %s gold.",
                                   nice_num(args[i]),
                                   nice_num(has_item(unit, item_gold))));
            };
            return;
            /*
             *  An item or skill to use.
             *
             */
        case CMD_use:
            if (!valid_box(args[i]) ||
                (kind(args[i]) != T_skill && kind(args[i]) != T_item)) {
                err(EAT_WARN,
                    sout("The %s argument of this command should be a skill or an item. %s does not appear to be either.",
                         ordinal(i), safe_name(c->who, args[i], c->parse[i])));
            } else if (kind(args[i]) == T_skill && find_use_entry(args[i]) == -1) {
                err(EAT_WARN, sout("%s doesn't appear to be a skill you can 'use'.",
                                   safe_name(c->who, args[i], c->parse[i])));
            } else if (kind(args[i]) == T_item && !item_unique(args[i])) {
                err(EAT_WARN, sout("%s doesn't appear to be an item you can 'use'.",
                                   safe_name(c->who, args[i], c->parse[i])));
            };
            return;
            /*
             *  A skill to practice.
             *
             */
        case CMD_practice:
            if (!valid_box(args[i]) || kind(args[i]) != T_skill) {
                err(EAT_WARN,
                    sout("The %s argument of this command should be a skill. %s does not appear to be an skill.",
                         ordinal(i), safe_name(c->who, args[i], c->parse[i])));
            } else if (!has_skill(c->who, args[i])) {
                err(EAT_WARN, sout("Note: %s doesn't currently know %s.",
                                   box_name(c->who),
                                   safe_name(c->who, args[i], c->parse[i])));
            } else if (!practice_time(args[i])) {
                err(EAT_WARN, sout("You cannot practice %s.",
                                   safe_name(c->who, args[i], c->parse[i])));
            } else if (has_item(c->who, item_gold) < practice_cost(args[i])) {
                err(EAT_WARN, sout("Note: %s doesn't currently have the %s gold required to practice %s.",
                                   box_name(c->who),
                                   nice_num(practice_cost(args[i])),
                                   safe_name(c->who, args[i], c->parse[i])));
            };
            return;
        default:
            return;
    };
};

/*
 *  Wed Dec 27 07:41:27 2000 -- Scott Turner
 *
 *  Command checker.
 *
 */
static void
check_cmd(struct command *c) {
    int i;
    char t;
    /*
     *  If we didn't get an entry into the command table,
     *  we'd better bail.
     *
     */
    if (!c->cmd) { return; }
    /*
     *  Check to make sure this entity is allowed to use this
     *  command, e.g., some commands cannot be issued by the player.
     *
     */
    if (cmd_tbl[c->cmd].allow &&
        is_safe(pl, c->who)) {
        switch (bx[c->who]->kind) {
            case T_player:
                t = 'p';
                break;
            case T_char:
                t = restricted_control(c->who);
                if (t == 0) { t = 'c'; }
                break;
            default:
                t = 'c';
        };

        if (strchr(cmd_tbl[c->cmd].allow, t) == NULL &&
            !(strchr(cmd_tbl[c->cmd].allow, 'm') != NULL &&
              player(c->who) == gm_player)) {
            err(EAT_WARN, sout("%s may not issue that order.",
                               safe_name(c->who, c->who, "This unit")));
        };
    };

    /*
     *  Required args?
     *
     */
    if (cmd_tbl[c->cmd].num_args_required &&
        numargs(c) < cmd_tbl[c->cmd].num_args_required) {
        err(EAT_WARN, sout("This command requires %s argument%s, and "
                           "you've only provided %s.",
                           nice_num(cmd_tbl[c->cmd].num_args_required),
                           cmd_tbl[c->cmd].num_args_required == 1 ? "" : "s",
                           nice_num(numargs(c))));
    };
    /*
     *  Extra args?
     *
     */
    if (cmd_tbl[c->cmd].max_args &&
        numargs(c) > cmd_tbl[c->cmd].max_args) {
        err(EAT_WARN, sout("This command takes only %s argument%s, and you"
                           " have %s.",
                           nice_num(cmd_tbl[c->cmd].max_args),
                           ((cmd_tbl[c->cmd].max_args == 1) ? "" : "s"),
                           nice_num(numargs(c))));
    };

    /*
     *  Step through the arguments and do some checking on them.  We are
     *  currently limited to the first five arguments.
     *
     */
    for (i = 1; i <= min(numargs(c), 5); i++) {
        check_arg(c, i, cmd_tbl[c->cmd].arg_types[i - 1]);
    }

    /*
     *  Possibly a specific error checker.
     *
     */
    if (cmd_tbl[c->cmd].cmd_check) {
        (*cmd_tbl[c->cmd].cmd_check)(c);
    };
};

static int
do_eat_command(struct command *c, FILE *fp) {

    assert(c->cmd != cmd_end);

    if (c->cmd == cmd_begin) { return do_begin(c); }
    if (c->cmd == cmd_unit) { return do_unit(c); }
    if (c->cmd == cmd_email) { return do_email(c); }
    if (c->cmd == cmd_vis_email) { return do_vis_email(c); }
    if (c->cmd == cmd_lore) { return do_lore(c); }
    if (c->cmd == cmd_resend) { return do_resend(c); }
    if (c->cmd == cmd_format) { return do_format(c); }
    if (c->cmd == cmd_notab) { return do_notab(c); }
    if (c->cmd == cmd_split) { return do_split(c); }
    if (c->cmd == cmd_set) { return do_set(c); }
    if (c->cmd == cmd_players) { return do_players(c); }
    if (c->cmd == cmd_passwd ||
        c->cmd == cmd_password) {
        return do_password(c);
    }

    if (unit == 0) {
        err(EAT_ERR, "can't queue orders, missing UNIT command");
        unit = -1;
        return TRUE;
    }

    if (unit == -1) {
        n_fail++;
        return TRUE;
    }

    if (c->cmd == cmd_stop) {
        queue_stop(pl, unit);
    } else {
        queue_order(pl, unit, c->line);
    }
    n_queued++;

    if (c->cmd == cmd_wait) {
        extern char *parse_wait_args();
        extern char *clear_wait_parse();
        char *s;

        s = parse_wait_args(c);

        if (s) {
            err(EAT_ERR, sout("Bad WAIT: %s", s));
        }

        clear_wait_parse(c);
    }

    if (c->cmd == cmd_post || c->cmd == cmd_message ||
        c->cmd == cmd_rumor || c->cmd == cmd_press) {
        int count = c->a;
        char *s;
        int len = 0;
        int reject_flag = FALSE;
        int max_len = MAX_POST;
        char **l = NULL;

        if (c->cmd == cmd_rumor || c->cmd == cmd_press) {
            max_len = 78;
        }

        while (1) {
            if (c->cmd == cmd_post || c->cmd == cmd_message) {
                s = eat_line_2(fp, TRUE);
            } else {
                s = eat_line_2(fp, FALSE);
            }
            if (!s) {
                err(EAT_ERR, "End of input reached before end of post!");
                break;
            }

            len = strlen(s);
            if (len > max_len) {
                err(EAT_ERR, sout("Line length exceeds %d characters", max_len));
                reject_flag = TRUE;
            }

            queue_order(pl, unit, s);

            if (count == 0) {
                char *t = eat_leading_trailing_whitespace(s);

                if (i_strcmp(t, "end") == 0) {
                    break;
                }

                ilist_append((ilist *) &l, (int) str_save(s));
            } else {
                ilist_append((ilist *) &l, (int) str_save(s));

                if (--count <= 0) {
                    break;
                }
            }
        }

        if (reject_flag) {
            err(EAT_ERR, "Post will be rejected.");
        } else if (c->cmd == cmd_press || c->cmd == cmd_rumor) {
            show_post(l, c->cmd);
        }

        text_list_free(l);
    }

    /*
     *  Wed Dec 27 07:40:41 2000 -- Scott Turner
     *
     *  Do some checking on the command, if possible.
     *
     */
    if (unit && pl && player(unit) && player(unit) == pl) {
        c->who = unit;
        check_cmd(c);
    };

    return TRUE;
}


static struct command dummy_cmd;


static void
parse_and_munch(FILE *fp) {
    struct command *c;

    c = &dummy_cmd;

    first_admit_check = 1;
    next_cmd(fp, c);
    while (c->cmd != cmd_end) {
        if (!do_eat_command(c, fp)) {
            return;
        }
        next_cmd(fp, c);
    }
}

static char *
no_spaces(char *str) {
    int i;
    char *buf = sout("");

    for (i = 0; i < (LEN - 1) && str[i]; i++) {
        if (str[i] == ' ') {
            buf[i] = '~';
        } else {
            buf[i] = str[i];
        }
    }

    buf[i] = 0;
    return buf;

};

/*
 *  Thu Dec  2 12:18:28 1999 -- Scott Turner
 *
 *  Make sure there are no spaces in the mail header so that
 *  those lines don't get wrapped!
 *
 */
static void
eat_banner() {
    char *to;
    char *full_name = "";

    out_alt_who = OUT_BANNER;

    out(eat_pl, "From: %s", no_spaces(from_host));
    out(eat_pl, "Reply-To: %s", no_spaces(reply_host));

    if (pl && rp_player(pl)->email && *(rp_player(pl)->email)) {
        to = rp_player(pl)->email;
    } else {
        to = reply_addr;
    }

    if (valid_box(pl)) {
        struct entity_player *p = rp_player(pl);

        if (p && p->full_name && *(p->full_name)) {
            full_name = sout(" (%s)", p->full_name);
        }
    }

    if (already_seen) {
        to = "moderator@olytag.com";    /*UPDATE*/
        full_name = " (Error Watcher)";

        if (cc_addr) {
            my_free(cc_addr);
            cc_addr = NULL;
        }
    }

    strcpy(who_to, to);

    out(eat_pl, "To:~%s%s", no_spaces(to), no_spaces(full_name));

    if (cc_addr && *cc_addr) {
        out(eat_pl, "Cc:~%s", no_spaces(cc_addr));
        strcat(who_to, " ");
        strcat(who_to, cc_addr);
    }

    out(eat_pl, "Subject: Acknowledge");
    /*VLN out(eat_pl, "X-Loop: moderator@olytag.com");*/ /* UPDATE */
    out(eat_pl, "Bcc: moderator@olytag.com"); /* UPDATE */
    out(eat_pl, "");
    out(eat_pl, "     - Olympia order scanner -");
    out(eat_pl, "");
    if (pl) {
        out(eat_pl, "Hello, %s", box_name(pl));
    }
    out(eat_pl, "");
    /*
     *  Tue Jan  2 07:32:17 2001 -- Scott Turner
     *
     *  Warning about low balances.
     *
     */
    if (pl) {

        char *cmd = sout("%s -p %s -g tag%d -T %s > /dev/null",
                         options.accounting_prog,
                         box_code_less(pl),
                         game_number,
                         options.turn_charge);
        int result = system(cmd);

        if (result != -1 && result != 0) {
            out(eat_pl, "*********************************************************");
            out(eat_pl, "**                                                     **");
            out(eat_pl, "** Warning: Low account balance                        **");
            out(eat_pl, "**                                                     **");
            out(eat_pl, "** Your account has a low balance and you cannot       **");
            out(eat_pl, "** afford to pay for your next turn.                   **");
            out(eat_pl, "**                                                     **");
            out(eat_pl, "**                                                     **");
            out(eat_pl, "*********************************************************");
            report_account_out(pl, eat_pl);
            out(eat_pl, "*********************************************************");
        };
    };

    out(eat_pl, "%d queued, %d error%s.",
        n_queued, n_fail,
        n_fail == 1 ? "" : "s");
}


static void
include_orig(FILE *fp) {
    char *s, *c;

    out_alt_who = EAT_HEADERS;

    rewind(fp);
    while (s = getlin(fp)) {
        /*
         *  Tue Nov  7 08:23:19 2000 -- Scott Turner
         *
         *  Don't output the password in lines where it might appear.
         *
         */
        if (strstr(s, "begin") != (char *) NULL ||
            strstr(s, "BEGIN") != (char *) NULL) {
            /*
             *  begin ke4 "password"
             *
             *  Terminate at 2nd space.  Obviously can be easily fooled.
             *
             */
            c = strchr(s, ' '); // (char *) index(s, ' ');
            if (c != (char *) NULL) {
                c = strchr(c, ' '); // (char *) index(c, ' ');
                if (c != (char *) NULL) {
                    *c = 0;
                }
            };
        } else if (strstr(s, "password") != (char *) NULL ||
                   strstr(s, "PASSWORD") != (char *) NULL) {
            /*
             *  Terminate at first space.
             *
             */
            c = strchr(s, ' '); // (char *) index(s, ' ');
            if (c != (char *) NULL) {
                *c = 0;
            };
        };
        out(eat_pl, "%s", s);
    };
}


static void
show_pending() {

    out_alt_who = EAT_QUEUE;
    orders_template(eat_pl, pl);
}


int eat_queue_mode = 0;


static void
eat(char *fnam, int mail_now) {
    FILE *fp;
    int ret = 0;
    int cpp = 0;
    char fnam_cpp[LEN];

    init_eat_vars();
    eat_queue_mode = 1;

    fp = fopen(fnam, "r");

    if (fp == NULL) {
        fprintf(stderr, "can't open %s", fnam);
        perror("");
        return;
    }

    reply_addr = parse_reply(fp, &cpp);

    if (cpp) {
        char buf[LEN];
        char *s;

        /*
         *  We need to call the GCC preprocessor directly to avoid
         *  the fact that GCC wants a .c ending on a source file.  We
         *  want the GCC preprocessor because it let's us do -imacros.
         *
         */
        fclose(fp);
        sprintf(buf, "egrep -v '#include' %s > /tmp/cpp1.%d;"
                     "%s -P -imacros %s/defines /tmp/cpp1.%d"
                     " > /tmp/cpp.%d",
                fnam, get_process_id(),
                options.cpp,
                libdir, get_process_id(), get_process_id());
        system(buf);
        sprintf(fnam_cpp, "/tmp/cpp.%d", get_process_id());
        fp = fopen(fnam_cpp, "r");
        /*
         *  Mon Mar  1 12:28:58 1999 -- Scott Turner
         *
         *  How to handle an error?
         *
         */
        if (fp == NULL) {
            fprintf(stderr, "Cannot open cpp file!");
            fp = fopen(fnam, "r");
            if (fp == NULL) {
                fprintf(stderr, "can't open %s", fnam);
                perror("");
                return;
            }
        };
        /*
         *  Skip ahead to beginning of message.
         *
         */
        while (s = getlin(fp)) {
            if (!*s) { break; }
        };
    };

    if (reply_addr) {
        if (i_strncmp(reply_addr, "postmaster", 10) == 0 ||
            i_strncmp(reply_addr, "mailer-daemon", 13) == 0 ||
            i_strncmp(reply_addr, "mail-daemon", 11) == 0) {
            already_seen = TRUE;
        }

        unlink(sout("%s/log/%d", libdir, eat_pl));
        open_logfile_nondestruct();
        clear_know_rec(&(p_player(eat_pl)->output));
        out_path = MASTER;

        parse_and_munch(fp);
        eat_banner();

        if (pl) {
            show_pending();
        }

        include_orig(fp);

        out_alt_who = OUT_INCLUDE;
        gen_include_sup(eat_pl);    /* must be last */

        out_path = 0;
        out_alt_who = 0;

        if (pl) {
            unlink(sout("%s/orders/%d", libdir, pl));
            save_player_orders(pl);
            unlink(sout("%s/fact/%d", libdir, pl));
            write_player(pl);
        }

        close_logfile();

        if (mail_now) {
/* VLN
		  ret = system(sout("rep %s/log/%d | sendmail -t",
				libdir, eat_pl));
*/
            ret = system(sout("rep %s/log/%d | msmtp -t",
                              libdir, eat_pl));
            if (ret) {
                fprintf(stderr, "error: couldn't mail ack to %s\n",
                        who_to);
/* VLN
			fprintf(stderr, "command was: %s\n",
				sout("rep %s/log/%d | sendmail -t",
					libdir, eat_pl));
*/
                fprintf(stderr, "command was: %s\n",
                        sout("rep %s/log/%d | msmtp -t",
                             libdir, eat_pl));
                fprintf(stderr, "ret was = %d\n", ret);
                perror("");
            } else {
                /*
                 *  Let's not overwhelm the system with a bunch
                 *  of rapid-fire mail responses.
                 *
                 */
                sleep(5);
            };
        };

    }

    fclose(fp);

    if (cpp) {
        char buf[LEN];
        sprintf(buf, "/tmp/cpp1.%d", get_process_id());
        unlink(buf);
        sprintf(buf, "/tmp/cpp.%d", get_process_id());
        unlink(buf);
    };

    eat_queue_mode = 0;
}


static void
write_remind_list() {
    FILE *fp;
    char *fnam;
    int pl;
    struct entity_player *p;

    fnam = sout("%s/remind", libdir);
    fp = fopen(fnam, "w");
    if (fp == NULL) {
        fprintf(stderr, "can't write %s:", fnam);
        perror("");
        return;
    }

    loop_player(pl)
            {
                if (subkind(pl) != sub_pl_regular) {
                    continue;
                }

                p = rp_player(pl);
                if (p == NULL || p->sent_orders || p->dont_remind) {
                    continue;
                }

                if (p->email == NULL) {
                    fprintf(stderr, "player %s has no email address\n",
                            box_code(pl));
                    continue;
                }

                fprintf(fp, "%s\n", p->email);
            }
    next_player;

    fclose(fp);
}


int read_spool(int mail_now) {
    DIR *d;
    struct dirent *e;
    char fnam[LEN];
    int ret = TRUE;
    int did_one = FALSE;

    sprintf(fnam, "%s/spool", libdir);
    d = opendir(fnam);

    if (d == NULL) {
        fprintf(stderr, "read_spool: can't open %s: ", fnam);
        perror("");
        return FALSE;
    }

    while ((e = readdir(d)) != NULL) {
        if (strncmp(e->d_name, "stop", 4) == 0) {
            ret = FALSE;
            unlink(sout("%s/spool/%s", libdir, e->d_name));
        }

        if (*(e->d_name) == 'm') {
            sprintf(fnam, "%s/spool/%s", libdir, e->d_name);
            eat(fnam, mail_now);
            /*
             *  Leave the spooled file if we're not actually
             *  replying.
             *
             */
            if (mail_now) {
                unlink(fnam);
            };
            did_one = TRUE;
        }
    }

    closedir(d);

    if (did_one) {
        write_remind_list();
    }

    return ret;
}


void
eat_loop(int mail_now) {

    setbuf(stdout, NULL);
    makedir(sout("%s/orders", libdir), 0755);
    makedir(sout("%s/spool", libdir), 0777);
    chmod(sout("%s/spool", libdir), 0777);

    write_remind_list();

    while (read_spool(mail_now)) {
        sleep(10);
    }
}

int
v_format(struct command *c) {
    int i;
    int plyr;

    /*
     *  Make this work in the BEGIN section as well.
     *
     */
    if (c->who) {
        plyr = player(c->who);
    } else if (pl) {
        plyr = pl;
    } else {
        err(EAT_ERR, "BEGIN must come before EMAIL");
        out(eat_pl, "      Notab not set.");
        return TRUE;
    };

    /*
     *  Format can have:
     *
     *  CLEAR
     *  HTML
     *  TEXT
     *  RAW
     *  TAGS
     *  ALT
     *
     */
    for (i = 1; i < ilist_len(c->parse) && c->parse[i]; i++) {
        out_alt_who = EAT_OKAY;
        if (strcasecmp("HTML", c->parse[i]) == 0) {
            p_player(plyr)->format |= HTML;
            wout(c->who ? c->who : eat_pl, "HTML format added.");
        } else if (strcasecmp("CLEAR", c->parse[i]) == 0) {
            p_player(plyr)->format = 0;
            wout(c->who ? c->who : eat_pl, "All formats cleared (text only).");
        } else if (strcasecmp("TEXT", c->parse[i]) == 0) {
            p_player(plyr)->format |= TEXT;
            wout(c->who ? c->who : eat_pl, "TEXT format added.");
        } else if (strcasecmp("TAGS", c->parse[i]) == 0) {
            p_player(plyr)->format |= TAGS;
            wout(c->who ? c->who : eat_pl, "TAGS format added.");
        } else if (strcasecmp("ALT", c->parse[i]) == 0) {
            p_player(plyr)->format |= ALT;
            wout(c->who ? c->who : eat_pl, "ALT format added.");
        } else if (strcasecmp("RAW", c->parse[i]) == 0) {
            p_player(plyr)->format |= RAW;
            wout(c->who ? c->who : eat_pl, "RAW format added.");
        };
    };
    return TRUE;
}

int
do_rules_url(struct command *c) {
    char *cmd = c->parse[1];
    int plyr;

    /*
     *  Make this work in the BEGIN section as well.
     *
     */
    if (c->who) {
        plyr = player(c->who);
    } else if (pl) {
        plyr = pl;
    } else {
        err(EAT_ERR, "BEGIN must come before EMAIL");
        out(eat_pl, "      Notab not set.");
        return TRUE;
    };

    if (c->parse[2]) {
        if (strlen(c->parse[2]) > 255) {
            out_alt_who = EAT_OKAY;
            wout(c->who ? c->who : eat_pl,
                 "Rules HTML path too long.  Maximum length 255 chars.");
        } else {
            if (p_player(plyr)->rules_path != NULL) {
                my_free(p_player(plyr)->rules_path);
            }
            p_player(plyr)->rules_path = str_save(c->parse[2]);
            out_alt_who = EAT_OKAY;
            wout(c->who ? c->who : eat_pl,
                 "Rules HTML path \"%s\" set.", p_player(plyr)->rules_path);
        };
    } else {
        if (p_player(plyr)->rules_path != NULL) {
            my_free(p_player(plyr)->rules_path);
        }
        p_player(plyr)->rules_path = NULL;
        out_alt_who = EAT_OKAY;
        wout(c->who ? c->who : eat_pl,
             "Rules HTML path cleared.");
    };
    return TRUE;
};

int
do_db_url(struct command *c) {
    char *cmd = c->parse[1];
    int plyr;

    /*
     *  Make this work in the BEGIN section as well.
     *
     */
    if (c->who) {
        plyr = player(c->who);
    } else if (pl) {
        plyr = pl;
    } else {
        err(EAT_ERR, "BEGIN must come before EMAIL");
        out(eat_pl, "      Notab not set.");
        return TRUE;
    };

    if (c->parse[2]) {
        if (strlen(c->parse[2]) > 255) {
            out_alt_who = EAT_OKAY;
            wout(c->who ? c->who : eat_pl,
                 "DB HTML path too long.  Maximum length 255 chars.");
        } else {
            if (p_player(plyr)->db_path != NULL) {
                my_free(p_player(plyr)->db_path);
            }
            p_player(plyr)->db_path = str_save(c->parse[2]);
            out_alt_who = EAT_OKAY;
            wout(c->who ? c->who : eat_pl,
                 "DB HTML path \"%s\" set.", p_player(plyr)->db_path);
        };
    } else {
        if (p_player(plyr)->db_path != NULL) {
            my_free(p_player(plyr)->db_path);
        }
        p_player(plyr)->db_path = NULL;
        out_alt_who = EAT_OKAY;
        wout(c->who ? c->who : eat_pl, "DB HTML path cleared.");
    };
    return TRUE;
};

int
v_notab(struct command *c) {
    int plyr;

    /*
     *  Make this work in the BEGIN section as well.
     *
     */
    if (c->who) {
        plyr = player(c->who);
    } else if (pl) {
        plyr = pl;
    } else {
        err(EAT_ERR, "BEGIN must come before EMAIL");
        out(eat_pl, "      Notab not set.");
        return TRUE;
    };

    p_player(plyr)->notab = c->a;

    out_alt_who = EAT_OKAY;
    if (c->a) {
        if (c->who) {
            wout(c->who, "No TAB characters will appear in turn reports.");
        } else {
            wout(eat_pl, "No TAB characters will appear in turn reports.");
        }
    } else if (c->who) {
        wout(c->who, "TAB characters may appear in turn reports.");
    } else {
        wout(eat_pl, "TAB characters may appear in turn reports.");
    }
    return TRUE;
}

