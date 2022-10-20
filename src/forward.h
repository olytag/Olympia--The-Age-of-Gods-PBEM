// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_FORWARD_H
#define OLYTAG_FORWARD_H

#include <stdio.h>
#include "types.h"

void add_new_players();
void add_skill_experience(int who, int skill);
void all_stack(int who, ilist *l);
int autocharge(int who, int total);
int building_owner(int ship);
int calc_beast_limit(int who, int boolean);
int calc_man_limit(int who, int boolean);
void call_init_routines();
void change_box_subkind(int where, int kind);
void character_report();
void check_db();
int check_skill(int who, int skill);
void check_win_conditions();
void clear_all_att(int who);
void clear_contacts(int who);
void close_logfile();
void cmd_shift(struct command *c);
int contacted(int target, int who);
int count_stack_any(int who);
int count_stack_any_real(int who, int ignore_ninjas, int ignore_angels);
void create_monster_stack(int item, int qty, int where);
int create_random_artifact(int gm_player);
void determine_output_order();
void do_times();
void eat_loop(int mail_now);
void extract_stacked_unit(int who);
void file_lock(char *name, int fd);
int find_command(char *cmd);
int first_character(int where);
void flush_unit_orders(int old_pl, int who);
int garrison_here(int where);
int garrison_spot_check(int garr, int target);
void gen_include_section();
void glob_init();
void gm_report(int gm_player);
void gm_show_all_skills(int skill_player, int boolean);
void ilist_test();
void immediate_commands();
void init_load_sup(int who);
void init_lower();
void init_random();
void init_spaces();
int is_hostile(int ruler, int who);
int is_port_city(int where);
int item_ride_cap(int item);
void list_order_templates();
int load_cmap_players();
void load_db();
int loc_depth(int i);
int loc_target(int target);
void lock_tag();
void log_output(int code, char *format, ...);
void mail_reports();
int makedir(const char *path, int mode);
void move_stack(int who, int where);
int near_rocky_coast(int loc);
int nearby_grave(int province);
void new_player_top(int mail_now);
int nps_invested(int who);
int oly_parse(struct command *c, char *cmd);
void open_logfile();
void open_times();
void out(int who, char *format, ...);
void player_banner();
void player_ent_info();
void player_report();
void post_month();
void pre_month();
void print_map(FILE *fp);
void prisoner_movement_escape_check(int who);
void process_orders();
int province_admin(int i);
void queue(int who, char *format, ...);
int rnd(int low, int high);
void read_spool(int mail_now);
int region_col(int i);
int region_row(int i);
int regular_combat(int, int, int, int);
void save_box(FILE *fp, int a);
void save_db();
void save_logdir();
void scan_char_skill_lore();
int select_attacker(int who, int target);
void set_bit(sparse *kr, int i);
void setup_html_all();
void setup_html_dir(int pl);
void set_html_pass(int pl);
void ship_storm_check(int ship);
void show_lore_sheets();
int ship_cap(int ship);
char *sout(char *format, ...);
void stage(char *s);
int sub_item(int where, int item, int something);
void summary_report();
int test_bit(sparse kr, int i);
void test_random();
void v_identify(struct command *c);
int will_admit(int pl, int who, int ruler);
void wiout(int who, int ind, char *format, ...);
void wout(int who, char *format, ...);
void write_factions();
void write_forwards();
void write_email();
void write_nations_lists();
void write_player_list();
void write_totimes();
void update_weather_view_locs(int who, int where);
int v_move_attack(struct command *c);

// mdhender: temporary declarations to appease compiling on Windows with gcc
void bcopy(const void *src, void *dest, size_t n);
void bzero(void *s, size_t n);
long int random(void);;
void srandom(unsigned int seed);

#endif //OLYTAG_FORWARD_H
