

extern struct skill_ent *p_skill_ent(int who, int skill);
extern struct skill_ent *rp_skill_ent(int who, int skill);
extern int has_skill(int who, int skill);
extern int has_subskill(int who, int subskill);
extern void set_skill(int who, int skill, int know);

extern int skill_school(int sk);
extern void list_skills(int who, int num, char *prefix);

extern int learn_skill(int who, int sk);
extern char *exp_s(int level);
extern int forget_skill(int who, int skill);

#define NOT_TAUGHT 0
#define TAUGHT_SPECIFIC -1
#define TAUGHT_GENERIC -2
#define TAUGHT_STUDYPOINTS -3

