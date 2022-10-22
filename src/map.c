// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    <time.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


/*
 *  Thu Nov 12 12:22:52 1998 -- Scott Turner
 *
 *  Some cute routines to print out a view of the map.
 *
 */
#define MAX_X 100
#define MAX_Y 100
static char map[MAX_X][MAX_Y];


/*
 *  Thu Nov 12 12:24:20 1998 -- Scott Turner
 *
 *  Fill the map with characters corresponding to the provinces.
 *
 */
int
load_cmap() {
    int i, x, y;

    if (xsize >= MAX_X) { return 0; }
    if (ysize >= MAX_Y) { return 0; }

    for (x = 0; x < xsize; x++) {
        for (y = 0; y < ysize; y++) {
            map[x][y] = '#';
        }
    }  /* Impassable */

    loop_kind(T_loc, i)
            {
                if (loc_depth(i) != LOC_province) continue;
                if (region(i) == faery_region || region(i) == hades_region ||
                    region(i) == cloud_region)
                    continue;
                x = region_col(i);
                y = region_row(i);
                switch (subkind(i)) {
                    case sub_ocean:
                        map[x][y] = ' ';
                        break;
                    case sub_forest:
                        map[x][y] = '%';
                        break;
                    case sub_plain:
                    case sub_island:
                        map[x][y] = '.';
                        break;
                    case sub_mountain:
                        map[x][y] = '^';
                        break;
                    case sub_mine_shaft:
                        map[x][y] = '0';
                        break;
                    case sub_desert:
                        map[x][y] = '-';
                        break;
                    case sub_swamp:
                        map[x][y] = 's';
                        break;
                    default:
                        fprintf(stderr, "Unknown subtype: %d.\n", subkind(i));
                        map[x][y] = '?';
                        break;
                }
        next_kind;
    };
    return 1;
};

void
print_map(FILE *fp) {
    int x, y;

    if (xsize >= MAX_X) { return; }
    if (ysize >= MAX_Y) { return; }

    for (y = 0; y < ysize; y++) {
        for (x = 0; x < xsize; x++) {
            putc(map[x][y], fp);
        }
        putc('\n', fp);
    };
}


/*
 *  Thu Nov 12 12:42:41 1998 -- Scott Turner
 *
 *  Indicate where players are...
 *
 */
int
load_cmap_players() {
    int i, x, y, count, j;

    if (xsize >= MAX_X) { return 0; }
    if (ysize >= MAX_Y) { return 0; }

    loop_kind(T_loc, i)
            {
                if (loc_depth(i) != LOC_province) { continue; }
                if (region(i) == faery_region || region(i) == hades_region ||
                    region(i) == cloud_region) {
                    continue;
                }
                x = region_col(i);
                y = region_row(i);

                count = 0;
                loop_all_here(i, j)
                        {
                            if (kind(j) == T_char && !is_real_npc(j)) { count++; }
                        }next_all_here;

                if (!count) { continue; }

                if (count < 10) {
                    map[x][y] = '0' + count;
                } else {
                    map[x][y] = '*';
                    printf("%s at (%d, %d) has %d nobles.\n",
                           box_name(i), x, y, count);
                };
            }next_kind;
    return 1;
};
