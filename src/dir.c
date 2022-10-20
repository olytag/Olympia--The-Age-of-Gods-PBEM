
#include    <stdio.h>
#include    "z.h"
#include    "oly.h"


int max_map_row = 0;
int max_map_col = 0;
int max_map_init = FALSE;


/*
 *  location_direction depends upon an understanding of the layout
 *  of the map with respect to entity numbers.
 *
 *  In other words, the entity number of a region determines where
 *  it is on the map.
 *
 *  Here is how:
 *
 *	  (r,c)
 *	--------------------
 *	| (1,1)		(1,99)
 *	|
 *	|
 *	| (n,1)		(n,99)
 *
 *
 *  Entity 10101 corresponds to (1,1).  [10199] corresponds to (1,99).
 *
 *  Note that for player convenience an alternate method of representing
 *  location entity numbers may be used, i.e. 'aa'-> 101, 'ab' -> 102,
 *  so [aa01] -> [10101], [ab53] -> [10253].
 */

int
region_row(int where) {
    int row;

    row = (where / 100) % 100;

    return row;
}


int
region_col(int where) {
    int col;

    col = where % 100;

    return col;
}


int
rc_to_region(int row, int col) {
    int reg;

    assert(row >= 0 && row < 100);
    assert(col >= 0 && col < 100);

    reg = 10000 + (row * 100) + col;

    return reg;
}


void
determine_map_edges() {
    int i;
    int row, col;

    loop_province(i)
                {
                    if (region(i) == faery_region ||
                        region(i) == hades_region ||
                        region(i) == cloud_region) {
                        continue;
                    }

                    row = region_row(i);
                    col = region_col(i);

                    if (row > max_map_row) {
                        max_map_row = row;
                    }

                    if (col > max_map_col) {
                        max_map_col = col;
                    }
                }
    next_province;

    max_map_init = TRUE;
}


int
los_province_distance(int a, int b) {
    int ra, rb, r1, r2;
    int ca, cb, c1, c2;
    int d1, d2;

    a = province(a);
    b = province(b);

    ra = region_row(a) - 1;
    rb = region_row(b) - 1;
    ca = region_col(a) - 1;
    cb = region_col(b) - 1;

    r1 = min(ra, rb);
    r2 = max(ra, rb);
    c1 = min(ca, cb);
    c2 = max(ca, cb);

    /* no wrap N-S */
    /*
     *  Mon Apr 10 10:06:53 2000 -- Scott Turner
     *
     *  Well, we do have N-S wrapping now.  How does that
     *  change this?
     *
     *  Hmm, looks like I already put this fix in :-)
     *
     */

    d1 = r2 - r1;
    if (d1 > (r1 + max_map_row) - r2) {
        d1 = (r1 + max_map_row) - r2;
    }

    d2 = c2 - c1;
    if (d2 > (c1 + max_map_col) - c2) {
        d2 = (c1 + max_map_col) - c2;
    }

#if 1
    return d1 + d2;        /* since there is no diagonal movement */
#else
    d1 *= d1;
    d2 *= d2;

    return my_sqrt(d1 + d2);
#endif
}


void
dir_assert() {
    int row, col, reg;

    row = 1;
    col = 1;
    reg = rc_to_region(row, col);
    assert(reg == 10101);
    assert(region_row(reg) == 1);
    assert(region_col(reg) == 1);

    row = 99;
    col = 99;
    reg = rc_to_region(row, col);
    assert(reg == 19999);
    assert(region_row(reg) == 99);
    assert(region_col(reg) == 99);

    row = 57;
    col = 63;
    reg = rc_to_region(row, col);
    assert(reg == 15763);
    assert(region_row(reg) == 57);
    assert(region_col(reg) == 63);
}


/*
 *  Return the region immediately adjacent to <location>
 *  in direction <dir>
 *
 *  Returns 0 if there is no adjacent location in the given
 *  direction.
 *
 *  Cache version.
 */

int
location_direction(int where, int dir) {
    struct entity_loc *p;

    dir--;

    p = rp_loc(where);
    if (p == NULL || dir >= ilist_len(p->prov_dest)) {
        return 0;
    }

    return p->prov_dest[dir];
}


int
exit_distance(int loc1, int loc2) {
    int dist;
    int w_d;    /* where depth */
    int d_d;    /* dest depth */

    if (subkind(loc1) == sub_hades_pit || subkind(loc2) == sub_hades_pit) {
        return 28;
    }

    if (subkind(loc1) == sub_mine_shaft || subkind(loc1) == sub_mine_shaft_notdone ||
        subkind(loc2) == sub_mine_shaft || subkind(loc2) == sub_mine_shaft_notdone) {
        return 0;
    }

    if (loc_depth(loc1) > loc_depth(loc2)) {
        int tmp;

        tmp = loc1;
        loc1 = loc2;
        loc2 = tmp;
    }

    w_d = loc_depth(loc1);
    d_d = loc_depth(loc2);

    if (d_d == LOC_build) {
        return 0;
    }
    if (d_d == LOC_subloc) {
        return 1;
    }

/*
 *  water-land links are distance=1
 */

    if (subkind(loc1) == sub_ocean && subkind(loc2) != sub_ocean) {
        return 2;
    }

    if (subkind(loc1) != sub_ocean && subkind(loc2) == sub_ocean) {
        return 2;
    }

/*
 *  Linked sublocs between regions
 */

    if (province(loc1) != province(loc2)) {
        loc1 = province(loc1);
        loc2 = province(loc2);
    }

    switch (subkind(loc2)) {
        case sub_ocean:
            if (loc_sea_lane(loc1) && loc_sea_lane(loc2)) {
                dist = 5;
            } else {
                dist = 6;
            }
            break;

        case sub_mountain:
            dist = 10;
            break;
        case sub_forest:
            dist = 8;
            break;
        case sub_swamp:
            dist = 14;
            break;
        case sub_desert:
            dist = 8;
            break;
        case sub_plain:
            dist = 7;
            break;
        case sub_under:
            dist = 7;
            break;
        case sub_cloud:
            dist = 7;
            break;

        default:
            fprintf(stderr,
                    "exit_distance: subkind=%s, loc1=%d, loc2=%d, "
                    "w_d=%d, d_d=%d\n",
                    subkind_s[subkind(loc2)], loc1, loc2, w_d, d_d);
            assert(FALSE);
    }

    return dist;
}


int
is_port_city(int where) {
    int p;
    int n, s, e, w;

    if (subkind(where) != sub_city) {
        return FALSE;
    }

    assert(loc_depth(where) == LOC_subloc);

    p = province(where);

    if (subkind(p) == sub_mountain) {
        return FALSE;
    }

    n = location_direction(p, DIR_N);
    s = location_direction(p, DIR_S);
    e = location_direction(p, DIR_E);
    w = location_direction(p, DIR_W);

    if ((n && subkind(n) == sub_ocean) ||
        (s && subkind(s) == sub_ocean) ||
        (e && subkind(e) == sub_ocean) ||
        (w && subkind(w) == sub_ocean)) {
        return TRUE;
    }

    return FALSE;
}


int
province_has_port_city(where)
        int where;
{
    int i;
    int ret = 0;

    assert(loc_depth(where) == LOC_province);

    loop_here(where, i)
            {
                if (subkind(i) == sub_city && is_port_city(i)) {
                    ret = i;
                    break;
                }
            }
    next_here;

    return ret;
}


#if 0
static int
summer_uldim_open_now()
{
    extern int month_done;

    if (oly_month(sysclock) >= 3 && oly_month(sysclock) <= 6)
        return TRUE;

    if (oly_month(sysclock) == 2 && month_done)
        return TRUE;

    return FALSE;
}
#endif


static void
add_province_exit(int who, int where, int dest, int dir, struct exit_view ***l) {
    struct exit_view *v;
    int n;

    assert(valid_box(dest));

    v = my_malloc(sizeof(*v));

    if ((is_ship_either(where) && ship_gone(where)) ||
        (is_ship_either(dest) && ship_gone(dest))) {
        v->in_transit = TRUE;
    }

    if (is_ship_either(where) && subkind(dest) == sub_ocean) {
        v->impassable = TRUE;
    }

    if (subkind(where) == sub_ocean && !is_ship_either(dest)) {
        v->water = TRUE;
    }

    if (subkind(dest) == sub_ocean) {
        v->water = TRUE;
    }

/*
 *  if land->water && land has a city, then impassable
 */

    if (loc_depth(where) == LOC_province &&
        subkind(dest) == sub_ocean &&
        province_has_port_city(where)) {
        v->impassable = TRUE;
    }

/*
 *  Can't go into collapsed mines
 */

    if (subkind(dest) == sub_mine_collapsed) {
        v->impassable = TRUE;
    }

/*
 *  if water->land && land has a city, then dest = the city
 */

    if (subkind(where) == sub_ocean &&        /* from ocean */
        subkind(dest) != sub_ocean &&        /* to land */
        subkind(dest) != sub_mountain &&        /* no mountain ports */
        loc_depth(dest) == LOC_province &&        /* and not islands */
        (n = province_has_port_city(dest))) {
#if 0
        dest = n;
#else
        v->impassable = TRUE;
        add_province_exit(who, where, n, dir, l);
#endif
    }

/*
 *  if water-mountain, then impassable
 */

    if ((subkind(where) == sub_mountain && subkind(dest) == sub_ocean) ||
        (subkind(where) == sub_ocean && subkind(dest) == sub_mountain)) {
        v->impassable = TRUE;
    }

/*
 *  if surface-cloud, then impassable (except by FLYing)
 */

    if ((dir == DIR_UP || dir == DIR_DOWN) &&
        (subkind(where) == sub_cloud || subkind(dest) == sub_cloud)) {
        v->impassable = TRUE;
    }

#if 0
    /*
     *  If Uldim mountains, then impassable
     */

        if ((dir == DIR_N && uldim(where) == 1) ||
            (dir == DIR_S && uldim(where) == 2))
        {
            v->impassable = TRUE;
        }

    /*
     *  Uldim pass and Summerbridge are passable part of the year
     */

        if ((dir == DIR_N && (uldim(where) == 4 || summerbridge(where) == 1)) ||
            (dir == DIR_S && (uldim(where) == 3 || summerbridge(where) == 2)))
        {
            if (!summer_uldim_open_now())
                v->impassable = TRUE;
        }
#endif

    v->orig = where;
    v->destination = dest;
    v->direction = dir;
    v->distance = exit_distance(where, dest);

    if (loc_hidden(where)) {
        v->orig_hidden = TRUE;
    }

    if (loc_hidden(dest)) {
        v->dest_hidden = TRUE;
    }

/*
 *  Don't make Out routes be hidden.  The character may have poofed
 *  into a building, and it's unreasonable not to know how to leave.
 */

    if (loc_hidden(dest) && !test_known(who, dest) && dir != DIR_OUT) {
        v->hidden = TRUE;
    }

    if (region(where) != region(dest)) {
        v->inside = region(dest);

        if (!in_hades(where) && in_hades(dest)) {
            v->hades_cost = 100;
        }
    }

/*
 *  If the destination location is protected by a magical barrier,
 *  then don't allow travel.
 */

    if (loc_barrier(dest) && dir != DIR_OUT) {
        v->impassable = TRUE;
        v->magic_barrier = TRUE;
    }

    ilist_append((ilist *) l, (int) v);
}


static void
extra_routes(int who, int where, struct exit_view ***l) {
    int i;
    int dest;
    struct exit_view *v;

    loop_here(where, i)
            {
                if (kind(i) == T_road) {
                    dest = road_dest(i);
                    assert(valid_box(dest));

                    v = my_malloc(sizeof(*v));
                    v->orig = where;
                    v->destination = dest;
                    v->distance = exit_distance(where, dest);
                    v->road = i;

/*
 *  Surface-cloud links are impassable (except by flying)
 */

                    if ((subkind(where) == sub_mountain && subkind(dest) == sub_cloud) ||
                        (subkind(where) == sub_cloud && subkind(dest) == sub_mountain)) {
                        v->impassable = TRUE;
                    }

                    if (road_hidden(i)) {
                        v->orig_hidden = TRUE;
                        v->dest_hidden = TRUE;
                    }

                    if (road_hidden(i) && !test_known(who, i)) {
                        v->hidden = TRUE;
                    }

                    if (region(where) != region(dest)) {
                        v->inside = region(dest);
                    }

                    if (subkind(where) == sub_ocean ||
                        subkind(dest) == sub_ocean) {
                        v->water = TRUE;
                    }

                    ilist_append((ilist *) l, (int) v);
                }
            }
    next_here;
}


/*
 *  Exits from a province
 *
 *	cycle through the possible directions, checking each
 *	add all inner locations
 */

static void
province_exits(int who, int where, struct exit_view ***l) {
    int dir;
    int n;

    for (dir = 1; dir <= DIR_DOWN; dir++) {
        n = location_direction(where, dir);
        if (n) {
            add_province_exit(who, where, n, dir, l);
        }
    }
}


static void
province_sub_exits(int who, int where, struct exit_view ***l) {
    int i;
    struct entity_subloc *p;

    loop_here(where, i)
            {
                if (is_loc_or_ship(i)) {
                    add_province_exit(who, where, i, DIR_IN, l);
                }
            }
    next_here;

    /*
     *  Mon Dec  9 15:22:12 1996 -- Scott Turner
     *
     *  All links are "open" now.
     *
     */
    p = rp_subloc(where);

    if (p) {
        for (i = 0; i < ilist_len(p->link_from); i++) {
            if (p->link_from[i]) {
                add_province_exit(who, where, p->link_from[i], DIR_IN, l);
            }
        }
    }

}


static void
subloc_exits(int who, int where, struct exit_view ***l) {
    int dir;
    int n;
    int i;
    struct entity_subloc *p;

    if (is_port_city(where)) {
        int p;

        p = province(where);

        for (dir = 1; dir <= 4; dir++) {
            n = location_direction(p, dir);
            if (n && subkind(n) == sub_ocean) {
                add_province_exit(who, where, n, dir, l);
            }
        }
    }

    loop_here(where, i)
            {
                if (is_loc_or_ship(i)) {
                    add_province_exit(who, where, i, DIR_IN, l);
                }
            }
    next_here;

    /*
     *  Mon Dec  9 14:42:04 1996 -- Scott Turner
     *
     *  Faery hills may be "floating" with no outer location.
     *
     *  Wed Jun 24 21:06:47 1998 -- Scott Turner
     *
     *  Huh? So you can't exit out into nowhere...
     *
     */
    if (loc(where) && subkind(loc(where)) != sub_region) {
        add_province_exit(who, where, loc(where), DIR_OUT, l);
    }

    /*
     *  Mon Dec  9 15:22:12 1996 -- Scott Turner
     *
     *  All links are "open" now.
     *
     */
    p = rp_subloc(where);

    if (p) /*  && loc_link_open(where)) */
    {
        for (i = 0; i < ilist_len(p->link_to); i++) {
            add_province_exit(who, where, p->link_to[i], 0, l);
        }
    }
}


/*
 *  If we're on a ship at sea and there are other ships in
 *  the same location, add boarding routes between the ships
 */

static void
ship_exits(int who, int ship, struct exit_view ***l) {
    int i;
    int outer_loc;

    assert(is_ship_either(ship));
    outer_loc = loc(ship);

/*
 *  We want to maintain the link to the other ships, even after
 *  they have left.  Attacking another ship requires the link to
 *  be in place, and we want to allow combat against a ship that
 *  may have left.  Therefore, we add the ship link, even if
 *  ship_gone(ship) is true.
 */

/*
 *  Exit from ship to location it is in
 */

    add_province_exit(who, ship, outer_loc, DIR_OUT, l);

#if 0
    if (subkind(outer_loc) != sub_ocean)
        return;
#endif

    loop_here(outer_loc, i)
            {
                if (i != ship && is_ship_either(i)) {
                    add_province_exit(who, ship, i, 0, l);
                }
            }
    next_here;
}


static void
building_exits(int who, int where, struct exit_view ***l) {
    int i;

    if (is_ship_either(where)) {
        ship_exits(who, where, l);
    } else {
        add_province_exit(who, where, loc(where), DIR_OUT, l);
    }

    loop_here(where, i)
            {
                if (is_loc_or_ship(i)) {
                    add_province_exit(who, where, i, DIR_IN, l);
                }
            }
    next_here;
}


struct exit_view **
exits_from_loc(int who, int where) {
    static struct exit_view **l = NULL;
    int i;

    for (i = 0; i < ilist_len(l); i++) {
        my_free(l[i]);
    }

    ilist_clear((ilist *) &l);

    switch (loc_depth(where)) {
        case LOC_province:
            province_exits(who, where, &l);
            province_sub_exits(who, where, &l);
            break;

        case LOC_subloc:
            subloc_exits(who, where, &l);
            break;

        case LOC_build:
            building_exits(who, where, &l);
            break;

        default:
            fprintf(stderr, "where=%d, depth=%d\n",
                    where, loc_depth(where));
            assert(FALSE);
    }

    extra_routes(who, where, &l);        /* add secret hidden roads */

    return l;
}


struct exit_view **
exits_from_loc_nsew(int who, int where) {
    static struct exit_view **l = NULL;
    int i;

    if (loc_depth(where) != LOC_province) {
        return NULL;
    }

    for (i = 0; i < ilist_len(l); i++) {
        my_free(l[i]);
    }

    ilist_clear((ilist *) &l);

    province_exits(who, where, &l);

    return l;
}


struct exit_view **
exits_from_loc_nsew_select(int who, int where, int land, int rand) {
    struct exit_view **l;
    static struct exit_view **ret = NULL;
    int i;

    if (loc_depth(where) != LOC_province) {
        return NULL;
    }

    ilist_clear((ilist *) &ret);
    l = exits_from_loc_nsew(who, where);

    for (i = 0; i < ilist_len(l); i++) {
        if (((land & LAND) && !l[i]->water) ||
            ((land & WATER) && l[i]->water)) {
            ilist_append((ilist *) &ret, (int) l[i]);
        }
    }

    if (rand) {
        ilist_scramble((ilist) ret);
    }

    return ret;
}


/*
 *  Returns:
 *
 *	0	no ocean access
 *	1	ocean access, but impassable
 *	2	passable ocean access (but may be hidden)
 */

int
has_ocean_access(int where) {
    struct exit_view **l;
    int i;
    int ret = 0;

    l = exits_from_loc(0, where);

    for (i = 0; i < ilist_len(l); i++) {
        if (l[i]->water) {
            if (l[i]->impassable) {
                if (ret == 0) {
                    ret = 1;
                }
            } else {
                ret = 2;
            }
        }
    }

    return ret;
}


static void
list_exit_extras(int who, struct exit_view *v) {

    if (v->magic_barrier) {
        indent += 3;
#if 0
        wout(who, "A magical barrier surrounds %s.",
                box_name(v->destination));
        wout(who, "Entry to %s is prevented by a magical barrier.",
                box_name(v->destination));
#endif
        tagout(who, "<tag type=loc_barrier id=%d>", v->destination);
        wout(who, "A magical barrier prevents entry.");
        tagout(who, "</tag type=loc_barrier>");
        indent -= 3;
    }

    if (v->hades_cost) {
        indent += 3;
        wiout(who, 1, "\"Notice to mortals, from the Gatekeeper "
                      "Spirit of Hades: 100 gold/head is removed "
                      "from any stack taking this road.\"");
        indent -= 3;
    }

    if (rp_loc(v->destination) &&
        garrison_here(v->destination)) {
        indent += 3;

        tagout(who, "<tag type=loc_garr id=%d garr=%d>",
               v->destination, garrison_here(v->destination));
        wout(who, "%s%s",
             liner_desc(garrison_here(v->destination)),
             display_with(garrison_here(v->destination)));
        tagout(who, "</tag type=loc_garr>");
        indent -= 3;
    };

    if (rp_loc(v->destination) &&
        rp_loc(v->destination)->control.closed &&
        province_admin(v->destination)) {
        indent += 6;
        tagout(who, "<tag type=border_closed id=%d>", v->destination);
        wout(who, "Border closed.");
        tagout(who, "</tag type=border_closed>");
        indent -= 6;
    };

    if (rp_loc(v->destination)) {
        tagout(who, "<tag type=fees id=%d nobles=%d weight=%d men=%d>",
               v->destination, rp_loc(v->destination)->control.nobles,
               rp_loc(v->destination)->control.weight,
               rp_loc(v->destination)->control.men);
    }

    if (rp_loc(v->destination) &&
        rp_loc(v->destination)->control.nobles &&
        province_admin(v->destination)) {
        indent += 6;
        wout(who, "Fee of %s per noble to enter.",
             gold_s(rp_loc(v->destination)->control.nobles));
        indent -= 6;
    };

    if (rp_loc(v->destination) &&
        rp_loc(v->destination)->control.men &&
        province_admin(v->destination)) {
        indent += 6;
        wout(who, "Fee of %s per 100 men to enter.",
             gold_s(rp_loc(v->destination)->control.men));
        indent -= 6;
    };

    if (rp_loc(v->destination) &&
        rp_loc(v->destination)->control.weight &&
        province_admin(v->destination)) {
        indent += 6;
        wout(who, "Fee of %s per 1000 weight to enter.",
             gold_s(rp_loc(v->destination)->control.weight));
        indent -= 6;
    };

    if (rp_loc(v->destination)) { tagout(who, "</tag type=fees>"); }
}


/*
 *	East, swamp, to Athens [aa59], 15 days
 */

/*
 *  We don't list routes to ships that are in_transit
 *
 *  We create them anyway so the player can get a useful error
 *  message if he attempts to traverse them.  Something like "That
 *  ship has left."
 */

static void
list_exits_sup(int who, int where, struct exit_view *v, char *first) {
    char *ret = "";
    char *s;

#if 0
    if (v->in_transit)
        return;
#endif

    if (v->hidden && !see_all(who)) {
        return;
    }

    if (first && *first) {
        out(who, "%s", first);
        indent += 3;

        *first = '\0';
    }

    if (v->direction > 0) {
        ret = comma_append(ret, full_dir_s[v->direction]);
    }

    s = name(v->destination);

    if (s && *s && !is_ship_either(v->destination)) {
        ret = comma_append(ret, subkind_s[subkind(v->destination)]);
    }

    ret = comma_append(ret, sout("to %s", box_name(v->destination)));

    if (v->inside) {
        s = name(v->inside);
        if (s && *s) {
            ret = comma_append(ret, s);
        }
    }

    if (v->dest_hidden) {
        ret = comma_append(ret, "hidden");
    }

    if (v->impassable) {
        ret = comma_append(ret, "impassable");
    } else {
        ret = comma_append(ret,
                           sout("%d~day%s", v->distance, add_s(v->distance)));
    }

    tagout(who, "<tag type=exit id=%d dir=%d dest=%d inside=%d "
                "hidden=%d impassable=%d distance=%d>",
           where,
           v->direction,
           v->destination,
           v->inside,
           v->dest_hidden,
           v->impassable,
           v->distance);


    wout(who, "%s", cap(ret));

    list_exit_extras(who, v);

    tagout(who, "</tag type=exit>");
}


static void
list_road_sup(int who, int where, struct exit_view *v, char *first) {
    char *dist;
    char *hid = "";

#if 0
    if (v->in_transit)
        return;
#endif

    if (v->hidden && !see_all(who)) {
        return;
    }

    if (v->dest_hidden) {
        hid = "hidden, ";
    }

    if (first && *first) {
        out(who, "");
        out(who, "%s", first);
        *first = '\0';
        indent += 3;
    }

    if (v->impassable || v->in_transit) {
        dist = "impassable";
    } else {
        dist = sout("%d~day%s", add_ds(v->distance));
    }

    tagout(who, "<tag type=exit id=%d dir=%d dest=%d inside=%d "
                "hidden=%d impassable=%d distance=%d>",
           where,
           v->direction,
           v->destination,
           v->inside,
           v->dest_hidden,
           v->impassable,
           v->distance);

    out(who, "%s, to %s, %s%s",
        just_name(v->road),
        box_name(v->destination),
        hid,
        dist);

    list_exit_extras(who, v);
    tagout(who, "</tag type=exit>");
}


void
list_exits(int who, int where) {
    struct exit_view **l;
    int i;
    char first[LEN];

    tagout(who, "<tag type=exits loc=%d>", where);

    l = exits_from_loc(who, where);

/*
 *  direction may be zero for roads and secret passages
 */

    sprintf(first, "Routes leaving %s: ", just_name(where));

    for (i = 0; i < ilist_len(l); i++) {
        if (l[i]->road == 0 &&
            (l[i]->direction != DIR_IN || see_all(who) == 2)) {
            list_exits_sup(who, where, l[i], first);
        }
    }

    for (i = 0; i < ilist_len(l); i++) {
        if (l[i]->road) {
            list_road_sup(who, where, l[i], first);
        }
    }

    if (*first) {
#if 0
        wout(who, "No known routes leaving %s", box_name(where));
#else
        if (is_ship_either(where)) {
            wout(who, "No current exits from %s", box_name(where));
        } else {
            wout(who, "No known routes leaving %s", box_name(where));
        }
#endif
    } else {
        indent -= 3;
    }

    tagout(who, "</tag type=exits>");

}


void
list_sailable_routes(int who, int ship) {
    char first[LEN];
    int outer_loc;
    struct exit_view **l;
    int i;

    if (!is_ship_either(ship)) {
        return;
    }

    outer_loc = loc(ship);
    l = exits_from_loc(who, outer_loc);

    sprintf(first, "Ocean routes:");

    tagout(who, "<tag type=sail_routes id=%d>", outer_loc);

    for (i = 0; i < ilist_len(l); i++) {
        if (l[i]->direction > 0 &&
            (l[i]->direction != DIR_IN || see_all(who) == 2) &&
            l[i]->water) {
            list_exits_sup(who, outer_loc, l[i], first);
        }
    }

    for (i = 0; i < ilist_len(l); i++) {
        if (l[i]->road && l[i]->water) {
            list_road_sup(who, outer_loc, l[i], first);
        }
    }

    if (*first) {
        out(who, "No visible sailable routes");
    } else {
        indent -= 3;
    }

    out(who, "");
    tagout(who, "</tag type=sail_routes>");
}


int
count_hidden_exits(struct exit_view **l) {
    int sum = 0;
    int i;

    for (i = 0; i < ilist_len(l); i++) {
        if (l[i]->hidden) {
            sum++;
        }
    }

    return sum;
}


int
hidden_count_to_index(int which, struct exit_view **l) {
    int i;

    for (i = 0; i < ilist_len(l); i++) {
        if (l[i]->hidden) {
            which--;
        }

        if (which <= 0) {
            assert(l[i]->hidden);
            return i;
        }
    }

    assert(FALSE);
}


void
find_hidden_exit(int who, struct exit_view **l, int which) {
    int where = subloc(who);

    if (is_ship(where)) {
        where = subloc(where);
    }

    assert(valid_box(who));
    assert(which < ilist_len(l));
    assert(l[which]->hidden);

    if (l[which]->road) {
        wout(who, "A hidden route has been found in %s!", box_name(where));
        out(who, "");

        set_known(who, l[which]->road);
        l[which]->hidden = FALSE;

        indent += 3;
        list_road_sup(who, subloc(who), l[which], NULL);
        indent -= 3;
    } else if (l[which]->direction == DIR_IN) {
        wout(who, "A hidden inner location has been found in %s!", box_name(where));
        out(who, "");

        set_known(who, l[which]->destination);
        l[which]->hidden = FALSE;

        indent += 3;
        wout(who, "%s", liner_desc(l[which]->destination));
        indent -= 3;
    } else {
        wout(who, "A hidden route has been found in %s!", box_name(where));
        out(who, "");

        set_known(who, l[which]->destination);
        l[which]->hidden = FALSE;

        indent += 3;
        list_exits_sup(who, subloc(who), l[which], NULL);
        indent -= 3;
    }


    out(who, "");
}
