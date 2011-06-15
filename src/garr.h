
#define		RANK_lord	5
#define		RANK_knight	6
#define		RANK_baron	7
#define		RANK_count	8
#define		RANK_earl	9
#define		RANK_marquess	10
#define		RANK_duke	11
#define		RANK_king	20

extern int top_ruler(int n);
extern void garrison_gold();
extern char *rank_s(int who);
extern void touch_garrison_locs();
extern void determine_noble_ranks();
extern int may_rule_here(int who, int where);
extern ilist players_who_rule_here(int where);
extern int garrison_notices(int garr, int target);
