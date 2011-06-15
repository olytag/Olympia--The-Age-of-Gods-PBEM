
extern int max_eff_aura(int who);				/* art.c */
extern int has_auraculum(int who);				/* art.c */
extern void touch_loc_pl(int pl, int where);			/* day.c */
extern void touch_loc(int who);					/* day.c */
extern void add_city_trade(int, int, int, int, int, int);	/* buy.c */
extern int distance(int orig, int dest, int gate);		/* seed.c */
extern void interrupt_order(int who);				/* input.c */
extern int may_cookie_npc(int who, int where, int cookie);	/* npc.c */
extern int do_cookie_npc(int, int, int, int);			/* npc.c */
extern int weather_here(int where, int sk);			/* storm.c */
extern void show_char_inventory(int who, int num, char *prefix);/* report.c */
extern char **parse_line(char **l, char *s);			/* input.c */
extern int unit_maint_cost(int who, int towho);			/* day.c */
extern char *wield_s(int who);					/* combat.c */
extern int can_see_weather_here(int who, int where);		/* storm.c */
extern void queue_lore(int who, int num, int anyway);		/* lore.c */
extern void create_mine_info(int mine);				/* mining.c */
extern struct entity_mine *get_mine_info(int where);            /* mining.c */
extern void copy_file(char *file1, char *file2);                /* main.c */
extern int nation(int who);                                     /* perm.c */
