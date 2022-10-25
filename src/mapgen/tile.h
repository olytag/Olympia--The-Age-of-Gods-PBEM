// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_TILE_H
#define OLYTAG_TILE_H


#include "vectors/ilist.h"
#include "vectors/rlist.h"

struct tile {
    char save_char;
    long region;
    char *name;
    long terrain;
    long hidden;
    long city;
    long mark;
    long inside;
    long color;      /* map coloring for */
    long row, col;   /* map tile we're inside */
    long depth;

    long safe_haven;
    long sea_lane;
    long uldim_flag;
    long summerbridge_flag;
    long region_boundary;
    long major_city;

    ilist subs;

    ilist gates_dest;  /* gates from here */
    ilist gates_num;   /* gates from here */
    ilist gates_key;

    rlist roads;
};

typedef struct tile tile_t;


#endif //OLYTAG_TILE_H
