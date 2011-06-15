
extern int loc_owner(int where);
extern void mark_loc_stack_known(int stack, int where);

extern void all_here(int who, ilist *l);
extern void all_char_here(int who, ilist *l);

extern int region(int who);
extern int province(int who);
extern int subloc(int who);
extern int viewloc(int who);

extern void add_to_here_list(int loc, int who);
extern void remove_from_here_list(int loc, int who);
extern void set_where(int who, int new_loc);
extern int in_here_list(int loc, int who);
extern int somewhere_inside(int a, int b);
extern int in_safe_now(int who);
extern int subloc_here(int where, int sk);
extern int count_loc_structures(int where, int a, int b);

#define	city_here(a)	subloc_here((a), sub_city)

