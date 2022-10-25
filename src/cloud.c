// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    <unistd.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


int cloud_region = 0;


#define    SZ    4

/*
 *	%%%%
 *	%*%%	*=Nimbus
 *	%^%*	*=Aerovia	^=Mt. Olympus link
 *	*%%%	*=Stratos
 */
void
create_cloudlands() {
    int r, c;
    int map[SZ + 1][SZ + 1];
    int n;
    int i;
    int north, east, south, west;
    struct entity_loc *p;

    fprintf(stderr, "INIT: creating cloudlands\n");

/*
 *  Create region wrapper
 */

    cloud_region = new_ent(T_loc, sub_region);
    set_name(cloud_region, "Cloudlands");

/*
 *  Fill map[row,col] with locations.
 */

    for (r = 0; r <= SZ; r++) {
        for (c = 0; c <= SZ; c++) {
            n = new_ent(T_loc, sub_cloud);
            map[r][c] = n;
            set_name(n, "Cloud");
            set_where(n, cloud_region);
        }
    }

/*
 *  Set the NSEW exit routes for every map location
 */

    for (r = 0; r <= SZ; r++) {
        for (c = 0; c <= SZ; c++) {
            p = p_loc(map[r][c]);

            if (r == 0) {
                north = 0;
            } else {
                north = map[r - 1][c];
            }

            if (r == SZ) {
                south = 0;
            } else {
                south = map[r + 1][c];
            }

            if (c == SZ) {
                east = 0;
            } else {
                east = map[r][c + 1];
            }

            if (c == 0) {
                west = 0;
            } else {
                west = map[r][c - 1];
            }

            ilist_append(&p->prov_dest, north);
            ilist_append(&p->prov_dest, east);
            ilist_append(&p->prov_dest, south);
            ilist_append(&p->prov_dest, west);
        }
    }

    {
        int nimbus, aerovia, stratos;

        nimbus = new_ent(T_loc, sub_city);
        set_where(nimbus, map[1][1]);
        set_name(nimbus, "Nimbus");
        seed_city(nimbus);

        aerovia = new_ent(T_loc, sub_city);
        set_where(aerovia, map[2][3]);
        set_name(aerovia, "Aerovia");
        seed_city(aerovia);

        stratos = new_ent(T_loc, sub_city);
        set_where(stratos, map[3][0]);
        set_name(stratos, "Stratos");
        seed_city(stratos);
    }

/*
 *  Create gates to rings of stones at the four corners of the Cloudlands
 *
 *  Thu May 27 08:29:52 1999 -- Scott Turner
 *
 *  May not have rings of stones.
 *
 */

    {
        ilist l = NULL;
        int i;
        int gate1, gate2, gate3, gate4;

        loop_loc(i)
                {
                    if (subkind(i) == sub_stone_cir) {
                        ilist_append(&l, i);
                    }
                }
        next_loc;

        if (ilist_len(l) >= 4) {
            ilist_scramble(l);

            gate1 = new_ent(T_gate, 0);
            set_where(gate1, map[0][0]);
            p_gate(gate1)->to_loc = l[0];
            rp_gate(gate1)->seal_key = rnd(111, 999);

            gate2 = new_ent(T_gate, 0);
            set_where(gate2, map[SZ][0]);
            p_gate(gate2)->to_loc = l[1];
            rp_gate(gate2)->seal_key = rnd(111, 999);

            gate3 = new_ent(T_gate, 0);
            set_where(gate3, map[0][SZ]);
            p_gate(gate3)->to_loc = l[2];
            rp_gate(gate3)->seal_key = rnd(111, 999);

            gate4 = new_ent(T_gate, 0);
            set_where(gate4, map[SZ][SZ]);
            p_gate(gate4)->to_loc = l[3];
            rp_gate(gate4)->seal_key = rnd(111, 999);
        };

        ilist_reclaim(&l);
    }

    printf("Aerovia is in %s\n", box_name(map[2][3]));

/*
 *  Link a cloud to a Mt. Olympus below
 */

    {
        int i;
        int olympus = 0;
        struct entity_loc *p;

        loop_mountain(i)
                {
                    if (strcmp(name(i), "Mt. Olympus") == 0 || olympus == 0) {
                        olympus = i;
                        break;
                    }
                }
        next_mountain;

        if (olympus == 0) {
            fprintf(stderr,
                    "ERROR: Can't find mountain 'Mt. Olympus'\n");
            return;
        }

        p = p_loc(map[2][1]);
        while (ilist_len(p->prov_dest) < DIR_DOWN) {
            ilist_append(&p->prov_dest, 0);
        }
        p->prov_dest[DIR_DOWN - 1] = olympus;

        p = p_loc(olympus);
        while (ilist_len(p->prov_dest) < DIR_UP) {
            ilist_append(&p->prov_dest, 0);
        }
        p->prov_dest[DIR_UP - 1] = map[2][1];
    }
}

/*
 *  Sat Mar 15 20:46:49 1997 -- Scott Turner
 *
 *  Disconnect two locs..
 *
 */
static
void disconnect(int here, int there) {
    int i;
    struct entity_loc *p1 = p_loc(here);
    assert(p1);
    for (i = 0; i < ilist_len(p1->prov_dest); i++) {
        if (p1->prov_dest[i] == there) {
            fprintf(stderr, "Disconnecting %s from %s.\n",
                    box_name(here), box_name(there));
            p1->prov_dest[i] = 0;
            return;
        }
    };
    return;
};

/*
 *  Fri Mar 14 10:19:11 1997 -- Scott Turner
 *
 *  Float the cloudlands around.
 *
 */
void
float_cloudlands() {
    int map[SZ + 1][SZ + 1];
    int i, row = 0, col = 0, x, y, x_off, y_off, r, where, nx, ny;

    tags_off();
    fprintf(stderr, "Floating cloudlands.\n");
    /*
     *  To fill in "map" we take advantage of how the locations
     *  were added to the Cloudlands region.
     *
     */
    loop_here(cloud_region, i)
            {
                assert(!(row > SZ));
                map[row][col++] = i;
                if (col > SZ) {
                    col = 0;
                    ++row;
                };
            }next_here;

    /*
     *  Now let's break all connections between the Cloudlands
     *  and any non-cloudlands regions.
     *
     */
    for (row = 0; row <= SZ; row++) {
        for (col = 0; col <= SZ; col++) {
            for (i = 0; i < ilist_len(rp_loc(map[row][col])->prov_dest); i++) {
                int tmp = rp_loc(map[row][col])->prov_dest[i];
                if (tmp && region(tmp) != cloud_region) {
                    /*
                     *  Disconnect...
                     *
                     */
                    disconnect(map[row][col], tmp);
                    disconnect(tmp, map[row][col]);
                };
            }
        }
    };

    /*
     *  Now we need to determine where over the map the cloudlands
     *  is.  This is actually stored in the Cloudlands region in that
     *  regions "where" box.  Although a region isn't actually anywhere,
     *  that's a convenient fiction for us.  It holds the Oly location
     *  of map[0][0].
     *
     */
    y = region_row(subloc(cloud_region));
    x = region_col(subloc(cloud_region));

    /*
     *  Thu May 11 06:39:50 2000 -- Scott Turner
     *
     *  This causes a problem if there's a hole in the map right
     *  here (when we try to save the DB).  So let's loop and make
     *  sure we don't use an offset that causes a hole.
     */
    do {
        /*
         *  Now calculate the offsets.
         *
         */
        y_off = rnd(1, 7) - 3;
        x_off = rnd(1, 7) - 3;

        /*
         *  Check for "wrap".
         *
         *  Wed Oct  7 19:25:13 1998 -- Scott Turner
         *
         *  This really needs to be the size of the world!
         */
        x = (x + x_off + xsize) % xsize;
        y = (y + y_off + ysize) % ysize;

        /*
         *  So now change the "where" of the cloud_region.  We don't
         *  use "set_where" since it's not really there :-).
         *
         */
        p_loc_info(cloud_region)->where = rc_to_region(y, x);
    } while (!valid_box(p_loc_info(cloud_region)->where));

    fprintf(stderr, "Cloudlands floating (%d, %d).\n", x_off, y_off);
    /*
     *  Now go through and recalculate where each of the cloudlands
     *  is over, and make connections to mountains as appropriate.
     *
     */
    for (row = 0; row <= SZ; row++) {
        for (col = 0; col <= SZ; col++) {
            /*
             *  Check for "wrap".
             *
             */
            nx = (x + col + xsize) % xsize;
            ny = (y + row + ysize) % ysize;

            where = rc_to_region(ny, nx);
            /*
             *  Possibly a hole in the map?
             *
             */
            if (!valid_box(where)) { continue; }

            if (subkind(where) == sub_mountain) {
                /*
                 *  Create the connection -- both ways.
                 *
                 */
                fprintf(stderr, "Creating connection between %s and %s.\n",
                        box_name(map[row][col]), box_name(where));
                connect_locations(map[row][col], DIR_DOWN, where, DIR_UP);
            } else {
                out(where, "Strangely solid clouds cover the sky.");
            };
        }
    };
    tags_on();
};


