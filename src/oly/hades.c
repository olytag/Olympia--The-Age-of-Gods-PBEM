// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    <unistd.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


int hades_region = 0;
int hades_pit = 0;        /* Pit of Hades */
int hades_player = 0;


#define    SZ    6        /* SZ x SZ is size of Hades */


void
create_hades() {
    int r, c;
    int map[SZ + 1][SZ + 1];
    int n;
    int i;
    int north, east, south, west;
    struct entity_loc *p;

    fprintf(stderr, "INIT: creating hades\n");

/*
 *  Create region wrapper for Hades
 */

    hades_region = new_ent(T_loc, sub_region);
    set_name(hades_region, "Hades");

/*
 *  Create the King of Hades player
 */

    assert(hades_player == 0);

    hades_player = 205;
    alloc_box(hades_player, T_player, sub_pl_npc);
    set_name(hades_player, "King of Hades");
    p_player(hades_player)->password = str_save(DEFAULT_PASSWORD);

/*
 *  Fill map[row,col] with locations.
 */

    for (r = 0; r <= SZ; r++) {
        for (c = 0; c <= SZ; c++) {
            n = new_ent(T_loc, sub_under);
            map[r][c] = n;
            set_name(n, "Hades");
            set_where(n, hades_region);
            p_loc(n)->hidden = TRUE;

            set_known(hades_player, n);
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

/*
 *  Place a city in the center of the map, with the Pit of Hades inside
 *  the city.  This is the only place where Necromancy is taught.
 */

    {
        int city;
        int pit;
        struct entity_subloc *p;

        city = new_ent(T_loc, sub_city);
        set_where(city, map[SZ / 2][SZ / 2]);
        set_name(city, "City of the Dead");
        set_known(hades_player, city);

        p = p_subloc(city);
        ilist_append(&p->teaches, sk_necromancy);

        pit = new_ent(T_loc, sub_hades_pit);
        set_where(pit, city);
        set_name(pit, "Pit of Hades");
        set_known(hades_player, pit);

        hades_pit = pit;
    }

/*
 *
 *  Dual-link every graveyard from the world into one of the
 *  Hades locations except the center one containing the pit.
 */

    {
        ilist l = NULL;
        struct entity_subloc *p;

        loop_loc(i)
                {
                    if (subkind(i) == sub_graveyard) {
                        ilist_append(&l, i);
                        set_known(hades_player, i);
                    }
                }
        next_loc;

        ilist_scramble(l);

        i = 0;
        while (i < ilist_len(l)) {
            for (r = 0; r <= SZ && i < ilist_len(l); r++) {
                for (c = 0; c <= SZ && i < ilist_len(l); c++) {
                    if (r == SZ / 2 && c == SZ / 2) {
                        continue;
                    }

                    p = p_subloc(l[i]);
                    ilist_append(&p->link_to, map[r][c]);

                    /*
                    p->link_when = -1;
                    p->link_open = -1;
                    */

                    p = p_subloc(map[r][c]);
                    ilist_append(&p->link_from, l[i]);

                    i++;
                }
            }
        }

        ilist_reclaim(&l);
    }

    printf("hades loc is %s\n", box_name(map[1][1]));
}


static void
create_hades_nasty() {
    int new;
    struct loc_info *p;
    int where;

    p = rp_loc_info(hades_region);
    assert(p);

    where = p->here_list[rnd(0, ilist_len(p->here_list) - 1)];

    switch (rnd(1, 5)) {
        case 1:
            new = new_char(sub_ni, item_spirit, where, 100, hades_player,
                           LOY_npc, 0, "Tortured spirits");

            if (new < 0) {
                return;
            }

            p_char(new)->npc_prog = PROG_balrog;
            gen_item(new, item_spirit, rnd(25, 75));

            break;

        case 2:
            new = new_char(sub_ni, item_spirit, where, 100, hades_player,
                           LOY_npc, 0, "Ghostly presence");

            if (new < 0) {
                return;
            }

            p_char(new)->npc_prog = PROG_balrog;
            p_char(new)->attack = 100;
            rp_char(new)->defense = 100;
            break;

        case 3:
            new = new_char(sub_ni, item_lesser_demon, where,
                           100, hades_player,
                           LOY_npc, 0, "Lesser Demon");

            if (new < 0) {
                return;
            }

            p_char(new)->npc_prog = PROG_balrog;
            p_char(new)->attack = 250;
            rp_char(new)->defense = 250;
            gen_item(new, item_spirit, rnd(50, 150));
            break;

        case 4:
            new = new_char(sub_ni, item_greater_demon, where,
                           100, hades_player,
                           LOY_npc, 0, "Greater Demon");

            if (new < 0) {
                return;
            }

            p_char(new)->npc_prog = PROG_balrog;
            p_char(new)->attack = 500;
            rp_char(new)->defense = 500;

            gen_item(new, item_spirit, rnd(100, 250));
            break;

        case 5:
            new = create_new_beasts(where, sub_undead);
            p_char(new)->npc_prog = PROG_balrog;
            fprintf(stderr, "Created undead in Hades: %s.\n",
                    box_name(new));
            break;

        default:
            assert(FALSE);
    }

    queue(new, "wait time 0");
#if 0
    init_load_sup(new);   /* make ready to execute commands immediately */
#endif
}


#if 0
static void
auto_hades_sup(int who)
{
    int i;
    int where = subloc(who);
    int queued_something = FALSE;

    loop_here(where, i)
    {
        if (kind(i) != T_char || subkind(player(i)) != sub_pl_regular)
            continue;

        if (has_skill(i, sk_transcend_death) && char_alone(i))
            continue;

        queued_something = TRUE;

        queue(who, "attack %s", box_code_less(i));
    }
    next_here;

    if (!queued_something)
        npc_move(who);
}
#endif

void
auto_hades() {
    int i;
    int n_hades = 0;

    loop_units(hades_player, i)
            {
                n_hades++;
            }
    next_unit;

    while (n_hades < 25) {
        create_hades_nasty();
        n_hades++;
    }

#if 0
    loop_units(hades_player, i)
    {
        auto_hades_sup(i);
    }
    next_unit;
#endif

}

