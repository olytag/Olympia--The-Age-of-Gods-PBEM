// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


/*
 *  is b somewhere inside of a?
 */

int
somewhere_inside(int a, int b) {

    if (a == b) {
        return FALSE;
    }

    while (b > 0) {
        b = loc(b);
        if (a == b) {
            return TRUE;
        }
    }

    return FALSE;
}


static void
add_here(int who, ilist *l) {
    int i;
    struct loc_info *p;

    assert(valid_box(who));

    ilist_append(l, who);

    p = rp_loc_info(who);
    assert(p != NULL);

    for (i = 0; i < ilist_len(p->here_list); i++) {
        add_here(p->here_list[i], l);
    }
}


void
all_here(int who, ilist *l) {
    int i;
    struct loc_info *p;

    assert(valid_box(who));

    ilist_clear(l);

    p = rp_loc_info(who);

    if (p == NULL) {
        return;
    }

    for (i = 0; i < ilist_len(p->here_list); i++) {
        add_here(p->here_list[i], l);
    }
}


static void
add_char_here(int who, ilist *l) {
    int i;
    struct loc_info *p;

    assert(valid_box(who));

    ilist_append(l, who);

    p = rp_loc_info(who);
    assert(p != NULL);

    for (i = 0; i < ilist_len(p->here_list); i++) {
        if (kind(p->here_list[i]) == T_char) {
            add_char_here(p->here_list[i], l);
        }
    }
}


void
all_char_here(int who, ilist *l) {
    int i;
    struct loc_info *p;

    assert(valid_box(who));

    ilist_clear(l);

    p = rp_loc_info(who);

    if (p == NULL) {
        return;
    }

    for (i = 0; i < ilist_len(p->here_list); i++) {
        if (kind(p->here_list[i]) == T_char) {
            add_char_here(p->here_list[i], l);
        }
    }
}


void
all_stack(int who, ilist *l) {
    int i;
    struct loc_info *p;

    assert(valid_box(who));

    ilist_clear(l);
    ilist_append(l, who);

    p = rp_loc_info(who);

    if (p == NULL) {
        return;
    }

    for (i = 0; i < ilist_len(p->here_list); i++) {
        if (kind(p->here_list[i]) == T_char) {
            add_char_here(p->here_list[i], l);
        }
    }
}


/*
 *  Return the ultimate region a character is in
 */

int
region(int who) {

    while (who > 0 &&
           (kind(who) != T_loc || loc_depth(who) != LOC_region)) {
        who = loc(who);
    }

    return who;
}


/*
 *  Return the ultimate province a character is in
 */

int
province(int who) {
    if (item_unique(who)) { who = item_unique(who); }

    while (who > 0 &&
           (kind(who) != T_loc || loc_depth(who) != LOC_province)) {
        who = loc(who);
    }

    return who;
}


/*
 *  Return the immediate location a character is in, irrespective
 *  of who or what we are stacked with.  This may be a sublocation
 *  such as a city, or a province.
 */

int
subloc(int who) {

    do {
        who = loc(who);
    } while (who > 0 && kind(who) != T_loc && kind(who) != T_ship);

    return who;
}


/*
 *  Step out from a location until we get to the appropriate viewing
 *  level.  Currently, provinces see into everything except cities.
 *
 *  I have added graveyards and faery hills to the viewloc hierarchy
 *  so that players will be able to see the hidden links into Faery
 *  and Hades when they are inside these locations.  Otherwise, they
 *  must carefully note the message given when they find the hidden
 *  route.
 */

int
viewloc(int who) {

    while (who > 0 &&
           loc_depth(who) != LOC_province &&
           subkind(who) != sub_guild &&
           subkind(who) != sub_city &&
           subkind(who) != sub_graveyard &&
           subkind(who) != sub_faery_hill) {
        who = loc(who);
    }

    return who;
}


int
in_safe_now(int who) {

    do {
        if (safe_haven(who)) {
            return TRUE;
        }

        who = loc(who);
    } while (who > 0);

    return FALSE;
}


void
add_to_here_list(int loc, int who) {

    assert(!in_here_list(loc, who));
    ilist_append(&(p_loc_info(loc)->here_list), who);
    assert(in_here_list(loc, who));
}


void
remove_from_here_list(int loc, int who) {

    assert(in_here_list(loc, who));
    ilist_rem_value(&(rp_loc_info(loc)->here_list), who);
    /*
     *  Mon Apr 20 18:08:44 1998 -- Scott Turner
     *
     *  Thanks to Rich's nice encapsulation, this is the only
     *  place we have to worry about someone being the last person
     *  out of a subloc...so here's where we reset the fees.
     *
     *  Anyway, that's plan :-).
     *
     */
    if (!ilist_len(rp_loc_info(loc)->here_list) &&
        rp_subloc(loc)) {
        rp_subloc(loc)->control.weight = 0;
        rp_subloc(loc)->control.nobles = 0;
        rp_subloc(loc)->control.men = 0;

        rp_subloc(loc)->control2.weight = 0;
        rp_subloc(loc)->control2.nobles = 0;
        rp_subloc(loc)->control2.men = 0;
    };

    assert(!in_here_list(loc, who));
}


void
set_where(int who, int new_loc) {
    int old_loc;

/*
 *  This check could be expanded to make sure that new_loc is
 *  not anywhere inside of who, by walking up new_loc to the top
 *  and making sure we don't go through who
 */

    assert(who != new_loc);

    old_loc = loc(who);

    if (old_loc > 0) {
        remove_from_here_list(old_loc, who);
    }

    if (new_loc > 0) {
        add_to_here_list(new_loc, who);
    }

    p_loc_info(who)->where = new_loc;

#if 0
    if (is_loc_or_ship(loc(who)))
        assert(!is_prisoner(who));
#endif
}


/*
 *  Mark that each member of a stack (or a ship) has visited a location
 */

void
mark_loc_stack_known(int stack, int where) {
    int i;

    if (kind(stack) == T_char) {
        set_known(stack, where);
    }

    loop_char_here(stack, i)
            {
                if (!is_prisoner(i)) {
                    set_known(i, where);
                }
            }
    next_char_here;
}


int
in_here_list(int loc, int who) {
    struct loc_info *p;

    p = rp_loc_info(loc);

    if (p == NULL) {
        return FALSE;
    }

    return (ilist_lookup(p->here_list, who) != -1);
}


int
subloc_here(int where, int sk) {
    int i;
    int ret = 0;

    loop_here(where, i)
            {
                if (kind(i) == T_loc && subkind(i) == sk) {
                    ret = i;
                    break;
                }
            }
    next_here;

    return ret;
}


int
count_loc_structures(int where, int a, int b) {
    int sum = 0;
    int i;

    loop_here(where, i)
            {
                if (kind(i) == T_loc && (subkind(i) == a || subkind(i) == b)) {
                    sum++;
                }
            }
    next_here;

    return sum;
}


#if 0
static int
province_owner(int where)
{
    int prov = province(where);
    int city;
    int castle;

    city = city_here(prov);

    castle = subloc_here(prov, sub_castle);
    if (castle == 0 && city)
        castle = subloc_here(city, sub_castle);

    if (castle)
        return first_character(castle);

    return 0;
}
#endif


int
building_owner(int where) {

    assert(loc_depth(where) == LOC_build);
    return first_character(where);
}

