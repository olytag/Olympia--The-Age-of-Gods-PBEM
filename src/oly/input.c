// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "z.h"
#include "oly.h"
#include "forward.h"
#include "vectors/cs_list.h"
#include "vectors/effect_list.h"


#define        MAX_PRI        5

static int cur_pri;
int evening = 0;
static int cmd_wait = -1;
extern ilist wait_list;

static ilist load_q[MAX_PRI] = {NULL, NULL, NULL, NULL, NULL};
static ilist run_q = NULL;


/*
 *  Line parser.  Given an ilist of char *'s and a line,
 *  returns the ilist with pointers into the cut-up line.
 *
 *  Depends on pointer-to-int conversions being safe
 */

char **
parse_line(char **l, char *s) {
    char *p;
    char *prev;

    cs_list_clear( &l);

    while (*s) {
        while (*s && iswhite(*s)) {        /* eat whitespace */
            s++;
        }

        if (*s == '\0') {
            break;
        }

        if (*s == '"')        /* handle "double quoted" text */
        {
            s++;
            prev = s;
            while (*s && *s != '"') {
                s++;
            }
        } else if (*s == '\'')    /* handle 'single quoted' text */
        {
            s++;
            prev = s;
            while (*s && *s != '\'') {
                s++;
            }
        } else                /* unquoted argument */
        {
            prev = s;
            while (*s && !iswhite(*s)) {
                s++;
            }
        }

        if (*s) {
            *s++ = '\0';
        }

/*
 *  Strip leading and trailing whitespace from argument
 */

        while (*prev && iswhite(*prev)) {
            prev++;
        }

        for (p = prev; *p; p++) {}
        p--;

        while (p >= prev && iswhite(*p)) {
            *p = '\0';
            p--;
        }

        cs_list_append(&l, prev);
    }

    return l;
}


void
remove_comment(char *s) {

    while (*s) {
        while (*s == '"') {
            while (*s && *s != '"') {
                s++;
            }
            s++;
        }

        while (*s == '\'') {
            while (*s && *s != '\'') {
                s++;
            }
            s++;
        }

        if (*s == '#') {
            *s = '\0';
            break;
        }

        s++;
    }
}


void
remove_ctrl_chars(char *s) {

    while (*s) {
        if (*s & 0x80) {
            *s = *s & 0x7F;
        }
        if (*s < 32) {
            *s = ' ';
        }
        s++;
    }
}


int parse_arg(int who, char *s) {
    int n;

/*
 *  scode() will perform an atoi() if the string is a digits-only
 *  number, or a code_to_int() if the string is a location code.
 */

    n = scode(s);

    if (n < 0) {
        n = 0;
    }

    if (n == 0 && who &&
        subloc(who) &&
        fuzzy_strcmp(s, "garrison")) {
        n = garrison_here(subloc(who));
        if (n == 0) {
            n = garrison_magic;
        }
    }

    return n;
}


int fuzzy_find;        /* was string found with a fuzzy match? */

int
find_command(char *s) {
    int i;

    fuzzy_find = FALSE;

    if (!*s) {
        return -1;
    }

    for (i = 1; cmd_tbl[i].name; i++) {
        if (i_strcmp(cmd_tbl[i].name, s) == 0) {
            return i;
        }
    }

    for (i = 1; cmd_tbl[i].name; i++) {
        if (fuzzy_strcmp(cmd_tbl[i].name, s)) {
            fuzzy_find = TRUE;
            return i;
        }
    }

    return -1;
}


/*
 *  Call full oly_parse instead.  Don't call this.
 */

int
oly_parse_cmd(struct command *c, char *s) {

    c->cmd = 0;
    c->fuzzy = FALSE;
    c->use_skill = 0;

    while (iswhite(*s)) {
        s++;
    }

    remove_comment(s);
    remove_ctrl_chars(s);

    if (c->line) {
        my_free(c->line);
    }
    c->line = str_save(s);

    if (*s == '&') {
        c->conditional = 1;
        s++;
    } else if (*s == '?') {
        c->conditional = 2;
        s++;
    } else {
        c->conditional = 0;
    }

    if (c->parsed_line) {
        my_free(c->parsed_line);
    }
    c->parsed_line = str_save(s);

    c->parse = parse_line(c->parse, c->parsed_line);

    if (cs_list_len(c->parse) > 0) {
        int i;

        i = find_command(c->parse[0]);

        if (i > 0) {
            c->cmd = i;
            if (fuzzy_find) {
                c->fuzzy = TRUE;
            }
        } else {
            return FALSE;
        }
    }

    return TRUE;
}


/*
 *  Look up command and tokenize arguments
 */

int
oly_parse(struct command *c, char *s) {

    c->a = 0;
    c->b = 0;
    c->c = 0;
    c->d = 0;
    c->e = 0;
    c->f = 0;
    c->g = 0;
    c->h = 0;

    if (!oly_parse_cmd(c, s)) {
        return FALSE;
    }

    switch (min(cs_list_len(c->parse), 9)) {
        case 9:
            c->h = parse_arg(c->who, c->parse[8]);
        case 8:
            c->g = parse_arg(c->who, c->parse[7]);
        case 7:
            c->f = parse_arg(c->who, c->parse[6]);
        case 6:
            c->e = parse_arg(c->who, c->parse[5]);
        case 5:
            c->d = parse_arg(c->who, c->parse[4]);
        case 4:
            c->c = parse_arg(c->who, c->parse[3]);
        case 3:
            c->b = parse_arg(c->who, c->parse[2]);
        case 2:
            c->a = parse_arg(c->who, c->parse[1]);
    }

    return TRUE;
}


void
cmd_shift(struct command *c) {

    if (cs_list_len(c->parse) > 1) {
/*
 *  Deleted argument need not be freed, since it's just a
 *  pointer into another string.  It was never allocated
 *  itself.
 */

        cs_list_delete(&c->parse, 1);
    }

    c->a = c->b;
    c->b = c->c;
    c->c = c->d;
    c->d = c->e;
    c->e = c->f;
    c->f = c->g;
    c->g = c->h;

    if (numargs(c) >= 8) {
        c->h = parse_arg(c->who, c->parse[8]);
    } else {
        c->h = 0;
    }
}


int
check_allow(struct command *c, char *allow) {
    int t;

    if (allow == NULL) {
        return TRUE;
    }

    if (immediate && strchr(allow, 'i') != NULL) {
        return TRUE;
    }

#if 0
    if (allow == NULL || immediate)
        return TRUE;			/* don't check */
#endif

    switch (bx[c->who]->kind) {
        case T_player:
            t = 'p';
            break;

        case T_char:
            t = restricted_control(c->who);
            if (t == 0) {
                t = 'c';
            }
            break;

        default:
            fprintf(stderr, "check_allow: bad kind: %s\n",
                    box_name(c->who));
            assert(FALSE);
    }

    if (strchr(allow, 'm') != NULL && player(c->who) == gm_player) {
        return TRUE;
    }

    if (strchr(allow, t) == NULL) {
        wout(c->who, "%s may not issue that order.", box_name(c->who));
        return FALSE;
    }

    return TRUE;
}


static int
get_command(int who, char *s) {
    int fact;        /* who controls us now? */
    char *order;

    if (!valid_box(who) || kind(who) == T_deadchar || subkind(who) == sub_dead_body) {
        return FALSE;
    }

    fact = player(who);
    order = top_order(fact, who);

/*
 *  If we don't have any orders, then fail.
 */

    if (order == NULL) {
        return FALSE;
    }

/*
 *  Update the player's last turn field so we know if he's been
 *  playing or not.
 */

    {
        struct entity_player *p;

        p = p_player(fact);
        p->last_order_turn = sysclock.turn;
    }

    assert(strlen(order) < LEN);    /* maybe not a valid assert */

    strncpy(s, order, LEN - 1);
    s[LEN - 1] = '\0';

    pop_order(fact, who);

    return TRUE;
}


static int
exec_precedence(int who) {
    int n;
    int stack_depth = 0;
    int pos;

    if (kind(who) != T_char) {
        return 0;
    }

    for (n = who; n; n = stack_parent(n), stack_depth++) {}

    pos = here_pos(who);

    assert(stack_depth < 100);
    assert(pos < 10000);

    return stack_depth * 10000 + pos;
}


static int exec_comp(const void *q1, const void *q2) {
    int *a = (int *)q1;
    int *b = (int *)q2;

    return bx[*a]->temp - bx[*b]->temp;
}


static void
sort_load_queue(ilist l) {
    int i;

    for (i = 0; i < ilist_len(l); i++) {
        bx[l[i]]->temp = exec_precedence(l[i]);
    }

    qsort(l, ilist_len(l), sizeof(int), exec_comp);
}


/*
 *  We want priority n-1 commands to finish before priority n
 *  commands.  We must fish the priority out of the command
 *  structure and give it msb status in the sorting tag.
 */

static void
sort_run_queue(ilist l) {
    int i;
    int pri;
    struct command *c;

    for (i = 0; i < ilist_len(l); i++) {
        c = rp_command(l[i]);
        assert(c);

        pri = c->pri;

        bx[l[i]]->temp = pri * 1000000 + exec_precedence(l[i]);
    }

    qsort(l, ilist_len(l), sizeof(int), exec_comp);
}


static void
set_state(struct command *c, int state, int new_pri) {

    switch (c->state) {
        case RUN:
            ilist_rem_value_uniq(&run_q, c->who);
            break;

        case LOAD:
            ilist_rem_value_uniq(&load_q[c->pri], c->who);
            break;
    }

    c->state = state;

    switch (c->state) {
        case RUN:
            ilist_append(&run_q, c->who);
            break;

        case LOAD:
            ilist_append(&load_q[new_pri], c->who);
            break;
    }
}


/*
 *  Load the command structure with a new command.
 *  Returns TRUE or FALSE depending on whether a command was present
 *  to load.
 *
 *  Sets c->state:
 *
 *	DONE	no more commands remain in the queue
 *	LOAD	command loaded and ready to run
 *	ERROR	player command has an error.
 *
 *	Note that both LOAD and ERROR states are passed to do_command.
 *	do_commmand will report error states to the player.
 */

static int
load_command(struct command *c) {
    char buf[LEN];

    assert(c != NULL);

    if (!get_command(c->who, buf)) {
        set_state(c, DONE, 0);
        return FALSE;
    }

    if (oly_parse(c, buf)) {
        int pri = cmd_tbl[c->cmd].pri;

        set_state(c, LOAD, pri);

        c->pri = pri;
        c->wait = cmd_tbl[c->cmd].time;
        c->poll = cmd_tbl[c->cmd].poll;
        c->days_executing = 0;
    } else {
        set_state(c, ERROR, 0);
    }

    return TRUE;
}


void
command_done(struct command *c) {

    if (immediate) {
        set_state(c, DONE, 0);
        return;
    }

    if (load_command(c))        /* sets c->state */
    {
        if (c->pri < cur_pri) {
            cur_pri = c->pri;
        }
    }
}


int finish_command(struct command *c) {
    extern int cmd_wait;

    assert(c != NULL);
    assert(valid_box(c->who));

    if (kind(c->who) == T_deadchar) {
        command_done(c);
        return FALSE;
    }

/*
 *  Characters stacked under units engaged in movement have
 *  their commands suspended until they get to their destination,
 *  except for wait completion checks.
 *
 */

    if (char_gone(c->who) &&
        stack_leader(c->who) != c->who &&
        c->cmd != cmd_wait) {
        return TRUE;
    }

    if (c->wait > 0) {
        c->wait--;
    }

    if (c->wait > 0 && c->debug == sysclock.day) {
        out(c->who, "finish_command called twice, wait=%d", c->wait);
    }
    c->debug = sysclock.day;

/*
 *  Call the finish routine once, when the command is done waiting,
 *  or every evening if the poll flag is set.
 */

    if (c->wait <= 0 || c->poll) {
        if (cmd_tbl[c->cmd].finish != NULL && !c->inhibit_finish) {
            c->status = (*cmd_tbl[c->cmd].finish)(c);
        }
    }

    if (c->state == RUN && (c->status == FALSE || c->wait == 0)) {
        command_done(c);
    }

    return c->status;
}

/*
 *  Thu Oct 24 15:52:04 1996 -- Scott Turner
 *
 *  If there's any urchin spies on this guy, report his doings.
 *
 */
void
do_urchin_spies(struct command *c) {
    struct effect *e;

    loop_effects(c->who, e)
            {
                if (e->type == ef_urchin_spy &&
                    e->data == subloc(c->who) &&
                    valid_box(e->subtype) &&
                    e->data == subloc(e->subtype)) {
                    wout(e->subtype, "An urchin reports that %s does \"%s\".",
                         box_name(c->who),
                         c->line);
                };
            }
    next_effect;
};

void
do_command(struct command *c) {

    assert(c != NULL);

    if (!immediate) {
        if (options.output_tags < 1) {
            out(c->who, "> %s", c->line);
        } else {
            char *rest = strchr(c->line, ' ');
            if (rest == NULL || *rest == '\0') {
                out(c->who, "> <tag type=command name=%s>%s</tag type=command>",
                    cmd_tbl[c->cmd].name,
                    c->parse[0]);
            } else {
                out(c->who, "> <tag type=command name=%s>%s</tag type=command>%s",
                    cmd_tbl[c->cmd].name,
                    c->parse[0], rest);
            }
        };

        if (c->fuzzy) {
            out(c->who, "(assuming you meant '%s')",
                cmd_tbl[c->cmd].name);
        }
    }

    if (c->state == ERROR) {
        out(c->who, "Unrecognized command.");
        c->status = FALSE;
    } else if (!check_allow(c, cmd_tbl[c->cmd].allow)) {
        c->status = FALSE;
    } else if (cmd_tbl[c->cmd].start == NULL) {
        out(c->who, "Unimplemented command.");
        c->status = FALSE;
    } else {
/*
 *  Increment count of commands started this turn
 */
        p_player(player(c->who))->cmd_count++;

        set_state(c, RUN, 0);

        assert(c->days_executing == 0);

        c->debug = 0;
        c->inhibit_finish = FALSE;
        c->status = (*cmd_tbl[c->cmd].start)(c);

        /*
         *  Thu Oct 24 15:48:47 1996 -- Scott Turner
         *
         *  Let the urchins who are spying on you report
         *  back to their masters.
         *
         */
        do_urchin_spies(c);
    }

    if (!c->status) {
        command_done(c);
    } else if (c->wait == 0 && c->state == RUN) {
        c->status = finish_command(c);
    }

#ifndef NEW_TRADE
    if (ilist_len(trades_to_check) > 0)
        check_validated_trades();
#endif
}


void
init_load_sup(int who) {
    struct command *c;

    c = rp_command(who);

/*
 *  All characters should have a command structure.
 *  Create one if they don't.
 */

    if (c == NULL) {
        c = p_command(who);
        c->who = who;
        c->state = DONE;
    }

    assert(who == c->who);

    switch (c->state) {
        case LOAD:
            ilist_append(&load_q[c->pri], c->who);
            break;

        case RUN:
            ilist_append(&run_q, c->who);
            break;

        case DONE:
            load_command(c);
            break;

        default:
            assert(FALSE);
    }
}


void
initial_command_load() {
    int i;

    loop_char(i)
            {
                init_load_sup(i);
            }
    next_char;

    loop_player(i)
            {
                init_load_sup(i);
            }
    next_player;
}


int
min_pri_ready() {
    int pri;
    int i;
    struct command *c;

    for (pri = 0; pri < MAX_PRI; pri++) {
        for (i = 0; i < ilist_len(load_q[pri]); i++) {
            c = rp_command(load_q[pri][i]);

            assert(c);
            assert(c->state == LOAD);
            assert(c->pri == pri);

            if (!is_prisoner(c->who) &&
                !char_moving(c->who) &&
                c->second_wait == FALSE) {
                return pri;
            }
        }
    }

    return 99;
}


void
init_wait_list() {
    int i;
    struct command *c;

    loop_char(i)
            {
                c = rp_command(i);

                if (c && c->state == RUN && c->cmd == cmd_wait) {
                    ilist_append(&wait_list, i);
                }
            }
    next_char;
}


void
check_all_waits() {
    int i;
    struct command *c;

    for (i = 0; i < ilist_len(wait_list); i++) {
        c = rp_command(wait_list[i]);
        if (c && c->state == RUN && c->cmd == cmd_wait) {
            assert(c->wait == -1);
            finish_command(c);
        }
    }
}


static int auto_attack_flag;


void
start_phase() {
    int i, j;
    int pri;
    struct command *c;
    ilist l;

    while (1) {
        pri = cur_pri = min_pri_ready();

/*
 *  Auto-attacks (declare hostile) should occur once per day.
 *  We must make sure they don't run before a command of lesser
 *  priority than attack.
 */

        if (auto_attack_flag && pri >= 3) {
            check_all_auto_attacks();
            auto_attack_flag = FALSE;
        }

        if (pri == 99) {
            return;
        }

        sort_load_queue(load_q[pri]);
        l = ilist_copy(load_q[pri]);

        for (j = 0; j < ilist_len(l); j++) {
            i = l[j];

            c = rp_command(i);
            assert(c != NULL);

            if (c->state == LOAD &&
                c->pri == pri &&
                !is_prisoner(i) &&
                !char_moving(i) &&
                c->second_wait == FALSE) {
                do_command(c);
                check_all_waits();

                if (pri != cur_pri) {
                    break;
                }
            }
        }
        ilist_reclaim(&l);
    }
}


static void
evening_phase() {
    int i, j;
    struct command *c;
    ilist l;

    evening = 1;

    sort_run_queue(run_q);

    l = ilist_copy(run_q);
    for (j = 0; j < ilist_len(l); j++) {
        i = l[j];

        c = rp_command(i);
        assert(c);

        if (c->state != RUN) {
            continue;
        }

        if (c->second_wait) {
            continue;
        }

        assert(c->state == RUN);

        c->days_executing++;
        finish_command(c);

#ifndef NEW_TRADE
        if (ilist_len(trades_to_check) > 0)
            check_validated_trades();
#endif // NEW_TRADE
    }
    ilist_reclaim(&l);

    check_all_waits();

    evening = 0;
}


/*
 *  repeat until we don't queue anything new:
 *  	for every character not doing something
 *  		check for a command, but avoid
 *		IDLE commands induced by wait events
 *
 *  check for satisfied wait events
 *
 *  repeat until we don't queue anything new:
 *  	for every character not doing something
 *		check for a command, including IDLE
 *
 *  "evening" command completion check phase
 */

static void
daily_command_loop() {

    auto_attack_flag = TRUE;
    start_phase();
    evening_phase();
    clear_second_waits();
}


static void
process_player_orders() {
    int pl;
    struct command *c;

    loop_player(pl)
            {
                c = rp_command(pl);

                if (c == NULL) {
                    continue;
                }

/*
 *  pl can switch to T_deleted as a result of a quit order
 */
                while (kind(pl) == T_player && c->state == LOAD) {
                    do_command(c);

#if 0
                    /*
                     *  All player orders should be zero time
                     */

                                assert(c->state != RUN);
#endif
                }
            }
    next_player;
}


/*
 *  Interrupt an executing order, if any.
 */

void
interrupt_order(int who) {
    struct command *c;

    if (stack_leader(who) == who) {
        restore_stack_actions(who);
    }    /* not moving anymore */

    c = rp_command(who);

    if (c == NULL)
        return;

    if (c->state == RUN) {
        if (cmd_tbl[c->cmd].interrupt != NULL)
            c->status = (*cmd_tbl[c->cmd].interrupt)(c);

        command_done(c);
        assert(c->state != RUN);
    }
}


static void
process_interrupted_units() {
    int who;
    struct command *who_c;

    loop_char(who)
            {
                if (!stop_order(player(who), who)) {
                    continue;
                }

                pop_order(player(who), who);
                out(who, "> stop");

                who_c = rp_command(who);

                if (who_c && who_c->cmd != 0 && who_c->wait != 0) {
                    out(who, "Interrupt current order.");
                    interrupt_order(who);
                } else {
                    out(who, "No order is currently executing.");
                }
            }
    next_char;
}


int month_done = FALSE;

void
process_orders() {

    stage("process_orders()");

    cmd_wait = find_command("wait");
    assert(cmd_wait > 0);

    init_locs_touched();
    init_weather_views();
    olytime_turn_change(&sysclock);

#if 0
    player_accounting();
#endif

    init_wait_list();
    init_collect_list();
    queue_npc_orders();
    initial_command_load();
    ping_garrisons();
#if 0
    check_token_units();			/* XXX/NOTYET -- temp fix */
#endif

    process_interrupted_units();        /* happens on day 0 */
    process_player_orders();
    scan_char_item_lore();

    stage("");

    while (sysclock.day < MONTH_DAYS) {
        olytime_increment(&sysclock);

        if (sysclock.day == 1) {
            match_all_trades();
        }

        print_dot('.');

        daily_command_loop();
        daily_events();
    }
    fprintf(stderr, "\n");

    month_done = TRUE;
}

