
extern char *top_order(int player, int who);
extern void pop_order(int player, int who);
extern void queue_order(int player, int who, char *s);
extern void prepend_order(int pl, int who, char *s);
extern void queue_stop(int pl, int who);
extern void load_orders();
extern void save_orders();

extern void delete_box(int n);
extern void change_box_kind(int n, int kind);
extern void alloc_box(int n, int kind, int sk);
extern int new_ent(int kind, int sk);
extern void list_pending_orders_sup(int who, int num, int show_empty);
extern int stop_order(int pl, int who);
