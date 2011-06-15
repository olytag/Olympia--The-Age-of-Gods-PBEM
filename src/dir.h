

struct exit_view {
	int direction;		/* which direction does the exit go */
	int destination;	/* where the exit goes */
	int orig;		/* loc we're coming from */
	int distance;		/* how far, in days */
	int impassable;		/* set if not possible to go there */
	int dest_hidden;	/* set if destination hidden */
	int orig_hidden;	/* set if origination or road is hidden */
	int hidden;		/* set if hidden destination unknown to us */
	int inside;		/* different region destinion is in */
	int road;		/* road entity number, if this is a road */
	int water;		/* is a water link */
	int in_transit;		/* is link to a ship that is moving? */
	int magic_barrier;	/* a magical barrier prevents travel */
	int hades_cost;		/* Gate Spirit of Hades fee to enter */
  int seize;  /* Whether we're seizing in an attack. */
  int forced_march; /* A forced ride? */
};

extern void list_exits(int who, int where);
extern struct exit_view **exits_from_loc(int who, int where);
extern void determine_map_edges();
extern void dir_assert();
extern int exit_distance(int, int);
extern void find_hidden_exit(int who, struct exit_view **l, int which);
extern int count_hidden_exits(struct exit_view **l);
extern void list_sailable_routes(int who, int ship);
extern int hidden_count_to_index(int which, struct exit_view **l);
extern int has_ocean_access(int where);
extern int location_direction(int where, int dir);

/*
 *  From move.c:
 */

extern struct exit_view *
	parse_exit_dir(struct command *c, int where, char *zero_arg);


#define	DIR_NSEW(a)		((a) >= DIR_N && (a) <= DIR_W)

#define	RAND	1
#define	LAND	1
#define	WATER	2

extern struct exit_view **exits_from_loc_nsew(int, int);
extern struct exit_view **exits_from_loc_nsew_select(int, int, int, int);
