// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_TYPES_H
#define OLYTAG_TYPES_H

// mdhender: this header file contains type declarations copied from oly.h.
//           they've been copied here to allow forward.h to avoid warnings.
//           i feel bad about all the pre-processor hacks this causes.

#ifndef OLY_FORWARD_struct_harvest
#define OLY_FORWARD_struct_harvest
struct harvest {
    int item;
    int skill;
    int worker;
    int chance;       /* chance to get one each day, if nonzero */
    char *got_em;
    char *none_now;
    char *none_ever;
    char *task_desc;
    int public;       /* 3rd party view, yes/no */
    int piety;        /* Does it use piety? */
};
#endif //OLY_FORWARD_struct_harvest

#ifndef OLY_FORWARD_ilist
#define OLY_FORWARD_ilist
typedef int *ilist;
#endif //OLY_FORWARD_ilist

#ifndef OLY_FORWARD_loc_control_ent
#define OLY_FORWARD_loc_control_ent
struct loc_control_ent {
    int closed;
    int nobles, men, weight;
};
#endif

#ifndef OLY_FORWARD_struct_make
#define OLY_FORWARD_struct_make
struct make {
    int item;
    int inp1;
    int inp1_factor; /* # of inp1 needed to make 1 */
    int inp2;
    int inp2_factor; /* # of inp2 needed to make 1 */
    int inp3;
    int inp3_factor; /* # of inp3 needed to make 1 */
    int req_skill;
    int worker;      /* worker needed */
    char *got_em;
    int public;      /* does everyone see us make this */
    int where;       /* place required for production */
    int aura;        /* aura per unit required */
    int factor;      /* multiplying qty factor, usuall 1 */
    int days;        /* days to make each thing */
};
#endif //OLY_FORWARD_struct_make

#ifndef OLY_FORWARD_schar
#define OLY_FORWARD_schar
typedef signed char schar;
#endif

#ifndef OLY_FORWARD_struct_skill_ent
#define OLY_FORWARD_struct_skill_ent
struct skill_ent {
    int skill;
    int days_studied;  /* days studied * TOUGH_NUM */
    int experience;    /* experience level with skill */
    char know;         /* SKILL_xxx */
/*
 *  Not saved:
 */
    char exp_this_month; /* flag for add_skill_experience() */
};
#endif //OLY_FORWARD_struct_skill_ent

#ifndef OLY_FORWARD_sparse
#define OLY_FORWARD_sparse
typedef ilist sparse;
#endif

#ifndef OLY_FORWARD_struct_command
#define OLY_FORWARD_struct_command
struct command {
    int who;                      /* entity this is under (redundant) */
    int wait;                     /* time until completion */
    int cmd;                      /* index into cmd_tbl */

    int use_skill;                /* skill we are using, if any */
    int use_ent;                  /* index into use_tbl[] for skill usage */
    int use_exp;                  /* experience level at using this skill */
    int days_executing;           /* how long has this command been running */

    int a, b, c, d, e, f, g, h, i, j; /* command arguments */

    char *line;                   /* original command line */
    char *parsed_line;            /* cut-up line, pointed to by parse */
    char **parse;                 /* ilist of parsed arguments */

    schar state;                  /* LOAD, RUN, ERROR, DONE */
    schar status;                 /* success or failure */
    schar poll;                   /* call finish routine each day? */
    schar pri;                    /* command priority or precedence */
    schar conditional;            /* 0=none 1=last succeeded 2=last failed */
    schar inhibit_finish;         /* don't call d_xxx */

    schar fuzzy;                  /* command matched fuzzy -- not saved */
    schar second_wait;            /* delay resulting from auto attacks -- saved */
    struct wait_arg **wait_parse; /* not saved */
    schar debug;                  /* debugging check -- not saved */
};
#endif

#endif //OLYTAG_TYPES_H
