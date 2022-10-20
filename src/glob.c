
#include    <stdio.h>
#include    "z.h"
#include    "oly.h"


char *from_host = "moderator@olytag.com (Olympia Moderator)";    /* UPDATE */
char *reply_host = "tagtest@olytag.com (Olympia Orders)";    /* UPDATE */

struct box **bx;        /* all possible entities */
int box_head[T_MAX];        /* heads of x_next_kind chain */
int sub_head[SUB_MAX];        /* heads of x_next_sub chain */

#if 0
struct entity_nation nations[MAX_NATIONS];  /* The nations */
int num_nations = 0;
#endif

char *libdir = "lib";
olytime sysclock;
int game_number;                /* we hope to have many :-) */
int indep_player = 100;        /* independent player */
int gm_player = 200;        /* The Fates */
int deserted_player = 201;    /* Deserted nobles */
int skill_player = 202;        /* skill listing */
int eat_pl = 203;        /* Order scanner */
int npc_pl = 206;        /* Subloc monster player */
int garr_pl = 207;        /* Garrison unit owner */
int combat_pl = 0;        /* Combat log */
int show_day = FALSE;
int post_has_been_run = FALSE;
int seed_has_been_run = FALSE;
int dist_sea_compute = FALSE;
int near_city_init = FALSE;
int cookie_init = FALSE;
int garrison_magic = 999;
int xsize = 100, ysize = 100;  /* Map size, set in "system" */
struct options_struct options;

int v_look(), v_stack(), v_unstack(), v_promote(), v_die();

int v_explore(), d_explore(), v_name(), v_banner(), v_notab();

int v_move(), d_move(), v_sail(), d_sail(), i_sail();

int v_give(), v_pay(), v_repair(), d_repair(), i_repair(), v_claim();

int v_swear(), v_form(), d_form(), v_use(), d_use(), i_use();

int v_teach();

int v_study(), d_study(), v_format();

#if 0
int v_acquire();
#endif

int v_wait(), d_wait(), i_wait(), v_flag(), v_discard(), v_guard();

int v_recruit(), v_make(), d_make(), i_make(), v_pillage(), d_pillage();

int v_attack(), v_behind(), v_bribe(), d_bribe();

int v_buy(), v_sell(), v_execute(), v_tell(), v_surrender();

int v_honor(), v_oath(), v_terrorize(), d_terrorize(), v_quit();

int v_build(), d_build(), v_quarry(), v_fish(), v_emote();

int v_post(), v_message(), v_rumor(), v_press();

int v_collect(), d_collect(), i_collect(), v_raze(), d_raze();

int v_wood(), v_yew(), v_catch(), v_mallorn(), v_stop();

int v_raise(), d_raise(), v_rally(), d_rally(), v_reclaim();

int v_incite(), v_forget(), v_garrison();

int v_fly(), d_fly(), v_sneak(), d_sneak();;
#if 0
int v_exhume(), d_exhume();
#endif

int v_admit(), v_hostile(), v_defend(), v_neutral(), v_att_clear();

int v_hide(), d_hide(), v_contact(), v_seek(), d_seek();

int v_opium(), v_get(), v_breed(), d_breed(), v_decree(), v_ungarrison();

int v_torture(), d_torture(), v_trance(), d_trance();

int v_fee(), v_ferry(), v_split();

int v_board(), v_unload();

#if 0
int v_bind_storm(), d_bind_storm();
#endif

int v_credit(), v_xyzzy(), v_plugh();

int v_fullname(), v_times();

int v_accept();

int v_enter(), v_exit(), v_north(), v_south(), v_east(), v_west();

int v_be(), v_listcmds(), v_poof(), v_see_all(), v_invent();

int v_add_item(), v_sub_item(), v_dump(), v_makeloc(), v_seed();

int v_lore(), v_know(), v_skills(), v_save(), v_postproc();

int v_los(), v_kill(), v_take_pris(), v_ct(), v_fix(), v_fix2();

int v_seedmarket(), v_relore();

int v_seedorc();

int v_make_artifact();

int v_tax();

int v_think();

int v_border();

int v_move_attack(), d_move_attack();

int v_practice(), d_practice();

int v_maxpay();

int v_identify();

/*
 *  Religion
 *  Wed Aug 14 14:36:51 1996 -- Scott Turner
 *
 */
int v_dedicate(), d_dedicate();

int v_dedicate_tower(), d_dedicate_tower();

int v_nationlist();

/*
 *  Wed Dec 27 12:49:34 2000 -- Scott Turner
 *
 *  Command commenters.
 *
 */
char *buy_comment(struct command *c);

char *give_comment(struct command *c);

char *get_comment(struct command *c);

char *study_comment(struct command *c);

char *admit_comment(struct command *c);

void admit_check(struct command *c);

char *default_comment(struct command *c);

char *move_comment(struct command *c);

char *attack_comment(struct command *c);

char *accept_comment(struct command *c);

char *drop_comment(struct command *c);

char *catch_comment(struct command *c);

char *collect_comment(struct command *c);

char *attitude_comment(struct command *c);

void quit_check(struct command *c);

/*
 *  Allow field:
 *
 *	c	character
 *	p	player entity
 *	i	immediate mode only (debugging/maintenance)
 *	r	restricted -- for npc units under control
 *	g	garrison
 *	m	Gamemaster only
 */

struct cmd_tbl_ent cmd_tbl[] = {
/*
allow  name         start         finish      intr      time poll pri
num_args_req max_args arg_types[] cmd_checker
 */

        {"", "", NULL, NULL, NULL, 0, 0, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
#if 0
        {"m",  "acquire",   v_acquire,    NULL,       NULL,       1,  0,  3,
         0, 0, {0, 0, 0, 0, 0}, NULL},
#endif
        {"cpr", "accept", v_accept, NULL, NULL, 0, 0, 0,
         1, 3, {0, 0, CMD_qty, 0, 0}, accept_comment, NULL},
        {"cpr", "admit", v_admit, NULL, NULL, 0, 0, 0,
         1, 0, {0, 0, 0, 0, 0}, admit_comment, admit_check},
        {"cr", "attack", v_move_attack, d_move_attack, NULL, -1, 0, 3,
         1, 0, {0, 0, 0, 0, 0}, attack_comment, NULL},
        {"cr", "banner", v_banner, NULL, NULL, 0, 0, 1,
         1, 2, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "behind", v_behind, NULL, NULL, 0, 0, 1,
         1, 1, {0, 0, 0, 0, 0}, NULL, NULL},
#if 0
        {"c",  "bind",      v_bind_storm, d_bind_storm, NULL,     7,  0,  3,
         0, 0, {0, 0, 0, 0, 0}, NULL},
        {"c",  "board",     v_board,      NULL,       NULL,       0,  0,  2,
         0, 0, {0, 0, 0, 0, 0}, NULL},
#endif
        {"c", "border", v_border, NULL, NULL, 0, 0, 0,
         2, 2, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "breed", v_breed, d_breed, NULL, 7, 0, 3,
         2, 2, {CMD_item, CMD_item, 0, 0, 0}, NULL, NULL},
        {"c", "bribe", v_bribe, d_bribe, NULL, 7, 0, 3,
         2, 3, {CMD_unit, CMD_gold, 0, 0, 0}, NULL, NULL},
        {"c", "build", v_build, d_build, NULL, -1, 1, 3,
         1, 4, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "buy", v_buy, NULL, NULL, 0, 0, 1,
         1, 4, {CMD_item, CMD_qty, 0, 0, 0}, buy_comment, NULL},
        {"c", "catch", v_catch, NULL, NULL, -1, 1, 3,
         0, 2, {CMD_qty, CMD_days, 0, 0, 0}, catch_comment, NULL},
        {"c", "claim", v_claim, NULL, NULL, 0, 0, 1,
         1, 2, {CMD_item, CMD_qty, 0, 0, 0}, NULL, NULL},
        {"c", "collect", v_collect, d_collect, i_collect, -1, 1, 3,
         1, 3, {CMD_item, CMD_qty, CMD_days, 0, 0}, collect_comment, NULL},
        {"cr", "contact", v_contact, NULL, NULL, 0, 0, 0,
         1, 1, {CMD_unit, 0, 0, 0, 0}, NULL, NULL},
        {"m", "credit", v_credit, NULL, NULL, 0, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "decree", v_decree, NULL, NULL, 0, 0, 0,
         2, 2, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "dedicate", v_dedicate, d_dedicate, NULL, 7, 0, 3,
         1, 1, {CMD_unit, 0, 0, 0, 0}, default_comment, NULL},
        {"cpr", "default", v_att_clear, NULL, NULL, 0, 0, 0,
         0, 0, {CMD_unit, 0, 0, 0, 0}, attitude_comment, NULL},
        {"cpr", "defend", v_defend, NULL, NULL, 0, 0, 0,
         0, 0, {CMD_unit, 0, 0, 0, 0}, attitude_comment, NULL},
        {"c", "die", v_die, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "discard", v_discard, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "drop", v_discard, NULL, NULL, 0, 0, 1,
         1, 3, {CMD_item, CMD_qty, 0, 0, 0}, drop_comment, NULL},
        {"m", "emote", v_emote, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "execute", v_execute, NULL, NULL, 0, 0, 1,
         0, 1, {CMD_unit, 0, 0, 0, 0}, default_comment, NULL},
#if 0
        {"c",  "exhume",    v_exhume,     d_exhume,   NULL,       7,  0,  , 3,
         0, 0, {0, 0, 0, 0, 0}, NULL},
#endif // , NULL 0
        {"c", "explore", v_explore, d_explore, NULL, 7, 0, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "fee", v_fee, NULL, NULL, 0, 0, 1,
         2, 3, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "ferry", v_ferry, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "fish", v_fish, NULL, NULL, -1, 1, 3,
         0, 2, {0, CMD_days, 0, 0, 0}, NULL, NULL},
        {"cr", "flag", v_flag, NULL, NULL, 0, 0, 1,
         1, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "fly", v_fly, d_fly, NULL, -1, 1, 2,
         1, 0, {0, 0, 0, 0, 0}, move_comment, NULL},
        {"c", "forget", v_forget, NULL, NULL, 0, 0, 1,
         1, 1, {CMD_skill, 0, 0, 0, 0}, default_comment, NULL},
        {"c", "form", v_form, d_form, NULL, 7, 0, 3,
         2, 2, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cp", "format", v_format, NULL, NULL, 0, 0, 1,
         1, 2, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "garison", v_garrison, NULL, NULL, 1, 0, 3,
         1, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "garrison", v_garrison, NULL, NULL, 1, 0, 3,
         1, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "get", v_get, NULL, NULL, 0, 0, 1,
         2, 4, {CMD_unit, CMD_item, CMD_qty, 0, 0}, get_comment, NULL},
        {"cr", "give", v_give, NULL, NULL, 0, 0, 1,
         2, 4, {CMD_unit, CMD_item, CMD_qty, 0, 0}, give_comment, NULL},
        {"cr", "go", v_move_attack, d_move_attack, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, move_comment, NULL},
        {"c", "guard", v_guard, NULL, NULL, 0, 0, 1,
         1, 1, {0, 0, 0, 0, 0}, default_comment, NULL},
        {"c", "guild", v_dedicate_tower, d_dedicate_tower, NULL, 7, 0, 3,
         1, 1, {CMD_skill, 0, 0, 0, 0}, default_comment, NULL},
        {"c", "hide", v_hide, d_hide, NULL, 3, 0, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "honor", v_honor, NULL, NULL, 0, 0, 3,
         1, 1, {CMD_gold, 0, 0, 0, 0}, NULL, NULL},
        {"c", "honour", v_honor, NULL, NULL, 1, 0, 3,
         1, 1, {CMD_gold, 0, 0, 0, 0}, NULL, NULL},
        {"cpr", "hostile", v_hostile, NULL, NULL, 0, 0, 0,
         0, 0, {CMD_unit, 0, 0, 0, 0}, attitude_comment, NULL},
        {"c", "identify", v_identify, NULL, NULL, 0, 0, 3,
         1, 1, {CMD_item, 0, 0, 0, 0}, default_comment, NULL},
        {"c", "incite", v_incite, NULL, NULL, 7, 0, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "make", v_make, d_make, i_make, -1, 1, 3,
         1, 2, {CMD_item, 0, 0, 0, 0}, NULL, NULL},
        {"c", "mallorn", v_mallorn, NULL, NULL, -1, 1, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cp", "message", v_message, NULL, NULL, 1, 0, 3,
         2, 2, {0, CMD_unit, 0, 0, 0}, NULL, NULL},
        {"cr", "move", v_move_attack, d_move_attack, NULL, -1, 0, 2,
         1, 0, {0, 0, 0, 0, 0}, move_comment, NULL},
        {"cpr", "maxpay", v_maxpay, NULL, NULL, 0, 0, 1,
         0, 1, {CMD_gold, 0, 0, 0, 0}, NULL, NULL},
        {"cpr", "name", v_name, NULL, NULL, 0, 0, 1,
         1, 2, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cp", "nationlist", v_nationlist, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cpr", "neutral", v_neutral, NULL, NULL, 0, 0, 0,
         0, 0, {CMD_unit, 0, 0, 0, 0}, attitude_comment, NULL},
        {"cp", "notab", v_notab, NULL, NULL, 0, 0, 1,
         1, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "oath", v_oath, NULL, NULL, 1, 0, 3,
         1, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "opium", v_opium, NULL, NULL, -1, 1, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "pay", v_pay, NULL, NULL, 0, 0, 1,
         1, 3, {CMD_unit, CMD_gold, 0, 0, 0}, NULL, NULL},
        {"cr", "pillage", v_pillage, d_pillage, NULL, 7, 1, 3,
         0, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "post", v_post, NULL, NULL, 1, 0, 3,
         1, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "practice", v_practice, d_practice, NULL, 7, 0, 3,
         1, 1, {CMD_practice, 0, 0, 0, 0}, study_comment, NULL},
        {"cp", "press", v_press, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "promote", v_promote, NULL, NULL, 0, 0, 1,
         1, 1, {CMD_unit, 0, 0, 0, 0}, default_comment, NULL},
        {"c", "quarry", v_quarry, NULL, NULL, -1, 1, 3,
         0, 2, {0, CMD_days, 0, 0, 0}, NULL, NULL},
        {"p", "quit", v_quit, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, quit_check},
        {"c", "raise", v_raise, d_raise, NULL, 7, 0, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "rally", v_rally, d_rally, NULL, 7, 0, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "raze", v_raze, d_raze, NULL, -1, 1, 3,
         0, 2, {0, CMD_days, 0, 0, 0}, default_comment, NULL},
        {"cpr", "realname", v_fullname, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "reclaim", v_reclaim, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "recruit", v_recruit, NULL, NULL, -1, 1, 3,
         0, 1, {CMD_days, 0, 0, 0, 0}, NULL, NULL},
        {"c", "repair", v_repair, d_repair, i_repair, -1, 1, 3,
         0, 1, {CMD_days, 0, 0, 0, 0}, NULL, NULL},
        {"cp", "rumor", v_rumor, NULL, NULL, 0, 0, 1,
         0, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cp", "rumors", v_rumor, NULL, NULL, 0, 0, 1,
         0, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "sail", v_sail, d_sail, i_sail, -1, 0, 4,
         1, 0, {0, 0, 0, 0, 0}, move_comment, NULL},
        {"c", "sell", v_sell, NULL, NULL, 0, 0, 1,
         3, 4, {CMD_item, CMD_qty, 0, 0, 0}, buy_comment, NULL},
        {"cr", "seek", v_seek, d_seek, NULL, 7, 1, 3,
         0, 1, {CMD_unit, 0, 0, 0, 0}, default_comment, NULL},
        {"c", "sneak", v_sneak, d_sneak, NULL, 3, 0, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cp", "split", v_split, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "stack", v_stack, NULL, NULL, 0, 0, 1,
         1, 1, {CMD_unit, 0, 0, 0, 0}, default_comment, NULL},
        {"c", "stone", v_quarry, NULL, NULL, -1, 1, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "study", v_study, d_study, NULL, 7, 1, 3,
         1, 2, {CMD_skill, 0, 0, 0, 0}, study_comment, NULL},
/* {"c",  "surrender", v_surrender,  NULL,       NULL,       1,  0,  1}, ,
 0, 0, {0, 0, 0, 0, 0}, NULL, NULL*/
        {"c", "swear", v_swear, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "take", v_get, NULL, NULL, 0, 0, 1,
         2, 4, {CMD_unit, CMD_item, 0, 0, 0}, get_comment, NULL},
        {"cp", "tax", v_tax, NULL, NULL, 0, 0, 1,
         3, 3, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "teach", v_teach, NULL, NULL, 7, 1, 2,
         1, 2, {CMD_skill, CMD_days, 0, 0, 0}, default_comment, NULL},
        {"cp", "tell", v_tell, NULL, NULL, 0, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "think", v_think, NULL, NULL, 1, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cp", "times", v_times, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "train", v_make, d_make, i_make, -1, 1, 3,
         1, 2, {CMD_item, CMD_qty, 0, 0, 0}, NULL, NULL},
        {"c", "trance", v_trance, d_trance, NULL, 28, 0, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "terrorize", v_terrorize, d_terrorize, NULL, 7, 0, 3,
         2, 2, {CMD_unit, 0, 0, 0, 0}, NULL, NULL},
        {"c", "torture", v_torture, d_torture, NULL, 7, 0, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "unload", v_unload, NULL, NULL, 0, 0, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "ungarrison", v_ungarrison, NULL, NULL, 1, 0, 3,
         0, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "unstack", v_unstack, NULL, NULL, 0, 0, 1,
         0, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "use", v_use, d_use, i_use, -1, 1, 3,
         1, 0, {CMD_use, 0, 0, 0, 0}, study_comment, NULL},
        {"crm", "wait", v_wait, d_wait, i_wait, -1, 1, 1,
         1, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "wood", v_wood, NULL, NULL, -1, 1, 3,
         0, 2, {0, CMD_days, 0, 0, 0}, NULL, NULL},
        {"cr", "xyzzy", v_xyzzy, NULL, NULL, 0, 0, 3,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"c", "yew", v_yew, NULL, NULL, -1, 1, 3,
         0, 2, {0, CMD_days, 0, 0, 0}, NULL, NULL},

        {"cr", "north", v_north, NULL, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "n", v_north, NULL, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "s", v_south, NULL, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "south", v_south, NULL, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "east", v_east, NULL, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "e", v_east, NULL, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "west", v_west, NULL, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "w", v_west, NULL, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "enter", v_enter, NULL, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "exit", v_exit, NULL, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "in", v_enter, NULL, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cr", "out", v_exit, NULL, NULL, -1, 0, 2,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},

        {"", "begin", NULL, NULL, NULL, 0, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"", "unit", NULL, NULL, NULL, 0, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"", "email", NULL, NULL, NULL, 0, 0, 0,
         1, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"", "vis_email", NULL, NULL, NULL, 0, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"", "end", NULL, NULL, NULL, 0, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"", "flush", NULL, NULL, NULL, 0, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"", "lore", NULL, NULL, NULL, 0, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"", "passwd", NULL, NULL, NULL, 0, 0, 0,
         0, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"", "password", NULL, NULL, NULL, 0, 0, 0,
         0, 1, {0, 0, 0, 0, 0}, NULL, NULL},
        {"", "players", NULL, NULL, NULL, 0, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"", "resend", NULL, NULL, NULL, 0, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"", "option", NULL, NULL, NULL, 0, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"cpr", "stop", v_stop, NULL, NULL, 0, 0, 0,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},

        {"i", "look", v_look, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "l", v_look, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "ct", v_ct, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "be", v_be, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "additem", v_add_item, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "subitem", v_sub_item, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "artifact", v_make_artifact, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "h", v_listcmds, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "dump", v_dump, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "i", v_invent, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "fix", v_fix, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "fix2", v_fix2, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "kill", v_kill, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "los", v_los, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"m", "relore", v_relore, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "sk", v_skills, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "know", v_know, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "seed", v_seed, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "seedorc", v_seedorc, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "seedmarket", v_seedmarket, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "sheet", v_lore, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "poof", v_poof, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "postproc", v_postproc, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "save", v_save, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "seeall", v_see_all, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "tp", v_take_pris, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {"i", "makeloc", v_makeloc, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL},
        {NULL, NULL, NULL, NULL, NULL, 0, 0, 1,
         0, 0, {0, 0, 0, 0, 0}, NULL, NULL}

};


char *kind_s[] = {
        "deleted",            /* T_deleted */
        "player",            /* T_player */
        "char",                /* T_char */
        "loc",                /* T_loc */
        "item",                /* T_item */
        "skill",            /* T_skill */
        "gate",                /* T_gate */
        "road",                /* T_road */
        "deadchar",            /* T_deadchar */
        "ship",                /* T_ship */
        "post",                /* T_post */
        "storm",            /* T_storm */
        "unform",            /* T_unform */
        "lore",                /* T_lore */
        "nation",            /* T_nation */
        NULL
};

char *subkind_s[] = {
        "<no subkind>",
        "ocean",            /* sub_ocean */
        "forest",            /* sub_forest */
        "plain",            /* sub_plain */
        "mountain",            /* sub_mountain */
        "desert",            /* sub_desert */
        "swamp",            /* sub_swamp */
        "underground",            /* sub_under */
        "faery hill",            /* sub_faery_hill */
        "island",            /* sub_island */
        "ring of stones",        /* sub_stone_cir */
        "mallorn grove",        /* sub_mallorn_grove */
        "bog",                /* sub_bog */
        "cave",                /* sub_cave */
        "city",                /* sub_city */
        "lair",                /* sub_lair */
        "graveyard",            /* sub_graveyard */
        "ruins",            /* sub_ruins */
        "field",            /* sub_battlefield */
        "enchanted forest",        /* sub_ench_forest */
        "rocky hill",            /* sub_rocky_hill */
        "circle of trees",        /* sub_tree_cir */
        "pits",                /* sub_pits */
        "pasture",            /* sub_pasture */
        "oasis",            /* sub_oasis */
        "yew grove",            /* sub_yew_grove */
        "sand pit",            /* sub_sand_pit */
        "sacred grove",            /* sub_sacred_grove */
        "poppy field",            /* sub_poppy_field */
        "temple",            /* sub_temple */
        "galley",            /* sub_galley */
        "roundship",            /* sub_roundship */
        "castle",            /* sub_castle */
        "galley-in-progress",        /* sub_galley_notdone */
        "roundship-in-progress",    /* sub_roundship_notdone */
        "ghost ship",            /* sub_ghost_ship */
        "temple-in-progress",        /* sub_temple_notdone */
        "inn",                /* sub_inn */
        "inn-in-progress",        /* sub_inn_notdone */
        "castle-in-progress",        /* sub_castle_notdone */
        "mine",                /* sub_mine */
        "mine-in-progress",        /* sub_mine_notdone */
        "scroll",            /* sub_scroll */
        "magic",            /* sub_magic */
        "palantir",            /* sub_palantir */
        "auraculum",            /* sub_auraculum */
        "tower",            /* sub_tower */
        "tower-in-progress",        /* sub_tower_notdone */
        "pl_system",            /* sub_pl_system */
        "pl_regular",            /* sub_pl_regular */
        "region",            /* sub_region */
        "pl_savage",            /* sub_pl_savage */
        "pl_npc",            /* sub_pl_npc */
        "collapsed mine",        /* sub_mine_collapsed */
        "ni",                /* sub_ni */
        "demon lord",            /* sub_demon_lord */
        "dead body",            /* sub_dead_body */
        "fog",                /* sub_fog */
        "wind",                /* sub_wind */
        "rain",                /* sub_rain */
        "pit",                /* sub_hades_pit */
        "artifact",            /* sub_artifact */
        "pl_silent",            /* sub_pl_silent */
        "npc_token",            /* sub_npc_token */
        "garrison",            /* sub_garrison */
        "cloud",            /* sub_cloud */
        "raft",                /* sub_raft */
        "raft-in-progress",        /* sub_raft_notdone */
        "suffuse_ring",            /* sub_suffuse_ring */
        "religion",                     /* sub_religion */
        "holy symbol",                /* sub_holy_symbol */
        "mist",                /* sub_mist */
        "book",                /* sub_book */
        "guild",                /* sub_market */
        "trade_good",                /* sub_trade_good */
        "city-in-progress",        /* sub_city_notdone */
        "ship",                /* sub_ship */
        "ship-in-progress",        /* sub_ship_notdone */
        "mine-shaft",            /* sub_mine_shaft */
        "mine-shaft-in-progress",    /* sub_mine_shaft_notdone */
        "orc-stronghold",        /* sub_orc_stronghold */
        "orc-stronghold-in-progress",    /* sub_orc_stronghold_notdone */
        "Staff-of-the-Sun",        /* sub_special_staff */
        "lost_soul",            /* sub_lost_soul */
        "undead",            /* sub_undead */
        "pen-crown",            /* sub_pen_crown */
        "animal-part",            /* sub_animal_part */
        "magical-artifact",        /* sub_magic_artifact */

        NULL
};

char *short_dir_s[] = {
        "<no dir>",
        "n",
        "e",
        "s",
        "w",
        "u",
        "d",
        "i",
        "o",
        NULL
};

char *full_dir_s[] = {
        "<no dir>",
        "north",
        "east",
        "south",
        "west",
        "up",
        "down",
        "in",
        "out",
        NULL
};

int exit_opposite[] = {
        0,
        DIR_S,
        DIR_W,
        DIR_N,
        DIR_E,
        DIR_DOWN,
        DIR_UP,
        DIR_OUT,
        DIR_IN,
        0
};

char *loc_depth_s[] = {
        "<no depth",
        "region",
        "province",
        "subloc",
        NULL
};

char *month_names[] = {
        "Fierce winds",
        "Snowmelt",
        "Blossom bloom",
        "Sunsear",
        "Thunder and rain",
        "Harvest",
        "Waning days",
        "Dark night",
        NULL
};


void
glob_init() {
    int i;

#if 0
    for (i = 0; i < MAX_BOXES; i++)
        bx[i] = NULL;
#endif

    for (i = 0; i < T_MAX; i++) {
        box_head[i] = 0;
    }

    bx = my_malloc(sizeof(*bx) * MAX_BOXES);
}

