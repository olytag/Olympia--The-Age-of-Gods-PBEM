// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_FORWARD_H
#define OLYTAG_FORWARD_H

#include <stdio.h>

void add_new_players();
void call_init_routines();
void character_report();
void check_db();
void check_win_conditions();
void close_logfile();
int create_random_artifact(int gm_player);
void determine_output_order();
void do_times();
void eat_loop(int mail_now);
void gen_include_section();
void glob_init();
void gm_report(int gm_player);
void gm_show_all_skills(int skill_player, int boolean);
void ilist_test();
void immediate_commands();
void init_lower();
void init_random();
void init_spaces();
void list_order_templates();
int load_cmap_players();
void load_db();
int loc_depth(int i);
void lock_tag();
void mail_reports();
int makedir(const char *path, int mode);
void new_player_top(int mail_now);
void open_logfile();
void open_times();
void player_banner();
void player_ent_info();
void player_report();
void post_month();
void pre_month();
void print_map(FILE *fp);
void process_orders();
int province_admin(int i);
void read_spool(int mail_now);
int region_col(int i);
int region_row(int i);
int regular_combat(int, int, int, int);
void save_box(FILE *fp, int a);
void save_db();
void save_logdir();
void scan_char_skill_lore();
void setup_html_all();
void setup_html_dir(int pl);
void set_html_pass(int pl);
void show_lore_sheets();
char *sout(char *format, ...);
void stage(char *s);
void summary_report();
void test_random();
void v_identify(struct command *c);
void wout(int who, char *format, ...);
void write_factions();
void write_forwards();
void write_email();
void write_nations_lists();
void write_player_list();
void write_totimes();

#endif //OLYTAG_FORWARD_H
