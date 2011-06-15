
extern char *liner_desc(int n);
extern char *display_with(int who);
extern char *display_owner(int who);
extern char *show_loc_header(int where);
extern void show_chars_below(int who, int n);
extern void show_owner_stack(int who, int n);

extern int show_display_string;
extern void turn_end_loc_reports();
extern int any_chars_here(int where);
extern char *loc_civ_s(int where);
