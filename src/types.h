// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_TYPES_H
#define OLYTAG_TYPES_H

// mdhender: this header file contains type declarations copied from oly.h.
//           they've been copied here to allow forward.h to avoid warnings.
//           i feel bad about all the pre-processor hacks this causes.

#ifndef OLY_FORWARD_schar
#define OLY_FORWARD_schar
typedef signed char schar;
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
