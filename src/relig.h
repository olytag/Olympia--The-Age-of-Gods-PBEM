/*
 *  Thu Sep  5 12:12:38 1996 -- Scott Turner
 *
 *  Externs for relig.c
 *
 */
extern int is_member(int who, int relig);
extern int use_piety(int who, int amount);
extern int has_piety(int who, int amount);
extern int priest_in_stack(int who, int which);
extern int do_trap_attack(int who, int num_attacks, int chance);
extern int is_holy_terrain(int n, int where);
