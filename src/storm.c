// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    <string.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


#if 0

/*
 *  Wed Oct  9 14:20:52 1996 -- Scott Turner
 *
 *  Doesn't seem to exist in the game?
 *
 */

int
v_bind_storm(struct command *c)
{
    int storm = c->a;
    int ship = subloc(c->who);

    if (kind(storm) != T_storm || npc_summoner(storm) != c->who)
    {
        wout(c->who, "%s doesn't control any storm %s.",
                        box_name(c->who),
                        box_code(storm));
        return FALSE;
    }

    if (!is_ship(ship))
    {
        wout(c->who, "%s must be on a ship to bind the storm to.",
                    box_name(c->who));
        return FALSE;
    }

    if (!has_piety(c->who, 3))
        return FALSE;

    return TRUE;
}


int
d_bind_storm(struct command *c)
{
    int storm = c->a;
    int ship = subloc(c->who);
    int old;
    struct entity_subloc *p;

    if (kind(storm) != T_storm || npc_summoner(storm) != c->who)
    {
        wout(c->who, "%s doesn't control storm %s anymore.",
                        box_name(c->who),
                        box_code(storm));
        return FALSE;
    }

    if (!is_ship(ship))
    {
        wout(c->who, "%s is no longer on a ship.", box_name(c->who));
        return FALSE;
    }

    if (!use_piety(c->who, 3))
        return FALSE;

    old = storm_bind(storm);
    if (old)
    {
        p = rp_subloc(old);
        if (p)
            ilist_rem_value(&p->bound_storms, storm);
    }

    p_misc(storm)->bind_storm = storm;
    p = p_subloc(ship);
    ilist_append(&p->bound_storms, storm);

    wout(c->who, "Bound %s to %s.", box_name(storm), box_name(ship));
    return TRUE;
}
#endif

static void
move_storm(int storm, int dest) {
    int orig = subloc(storm);
    int sk = subkind(storm);
    int before, i;
    int owner;

    before = weather_here(dest, sk);

    set_where(storm, dest);

    owner = npc_summoner(storm);

    if (owner) {
        touch_loc_pl(player(owner), dest);
    }

    show_to_garrison = TRUE;

    if (weather_here(orig, sk) == 0) {
        switch (sk) {
            case sub_rain:
                wout(orig, "It has stopped raining.");
                break;

            case sub_wind:
                wout(orig, "It is no longer windy.");
                break;

            case sub_fog:
                wout(orig, "The fog has cleared.");
                break;

            case sub_mist:
                wout(orig, "The mist dissipates.");
                break;

            default:
                assert(FALSE);
        }
    }

    if (!before) {
        switch (sk) {
            case sub_rain:
                wout(dest, "It has begun to rain.");
                break;

            case sub_wind:
                wout(dest, "It has become quite windy.");
                break;

            case sub_fog:
                wout(dest, "It has become quite foggy.");
                break;

            case sub_mist:
                wout(dest, "A dank mist has risen from the ground.");
                break;
        }
    }

    show_to_garrison = FALSE;

    /*
     *  Tue Jan  7 12:25:57 1997 -- Scott Turner
     *
     *  A storm has potential to do damage to a ship only if it is a rain
     *  storm and there previously was no rain; this avoids hitting a ship
     *  multiply with the existing storms every time a new storm wanders
     *  by.
     *
     */
    if (before != sub_rain) loop_here(dest, i)
            {
                if (is_ship(i)) { ship_storm_check(i); }
            }next_here;
}

#if 0
void
move_bound_storms(int ship, int where)
{
    struct entity_subloc *p;
    int i;
    int storm;

    p = rp_subloc(ship);
    if (p == NULL)
        return;

    for (i = 0; i < ilist_len(p->bound_storms); i++)
    {
        storm = p->bound_storms[i];
        if (kind(storm) != T_storm)
        {
            ilist_delete(&p->bound_storms, storm);
            i--;
            continue;
        }

        move_storm(storm, where);
    }
}
#endif

int
new_storm(int new, int sk, int aura, int where) {
    int before, i;

    assert(sk == sub_rain || sk == sub_wind || sk == sub_fog || sk == sub_mist);
    assert(loc_depth(where) == LOC_province);

    before = weather_here(where, sk);

    if (new == 0) {
        new = new_ent(T_storm, sk);

        if (new <= 0) {
            return -1;
        }
    }

    p_misc(new)->storm_str = aura;
    set_where(new, where);

    show_to_garrison = TRUE;

    if (!before) {
        switch (sk) {
            case sub_rain:
                wout(where, "It has begun to rain.");
                break;

            case sub_wind:
                wout(where, "It has become quite windy.");
                break;

            case sub_fog:
                wout(where, "It has become quite foggy.");
                break;

            case sub_mist:
                wout(where, "The province is shrouded in a dank mist.");
                break;
        }
    }

    show_to_garrison = FALSE;

    /*
     *  Tue Jan  7 12:25:57 1997 -- Scott Turner
     *
     *  A storm has potential to do damage to a ship only if it is a rain
     *  storm and there previously was no rain; this avoids hitting a ship
     *  multiply with the existing storms every time a new storm wanders
     *  by.
     *
     */
    if (before != sub_rain) loop_here(where, i)
            {
                if (is_ship(i)) { ship_storm_check(i); }
            }next_here;

    return TRUE;
}


void
storm_report(pl)
        int pl;
{
    int first = TRUE;
    int owner;
    int where;
    int i;

    loop_storm(i)
            {
                owner = npc_summoner(i);

                if (owner == 0 || player(owner) != pl) {
                    continue;
                }

                where = province(i);

                /*
                 *  Tue Jun 10 11:54:48 1997 -- Scott Turner
                 *
                 *  Check for crossing water...
                 *
                 */
                if (crosses_ocean(owner, i)) { continue; }

                if (first) {
                    if (options.output_tags) {
                        out(pl, "<tag type=storm_report pl=%d>", pl);
                    }
                    out(pl, "");
                    out(pl, "%5s  %4s  %5s  %4s  %s",
                        "storm", "kind", "owner", "loc", "strength");
                    out(pl, "%5s  %4s  %5s  %4s  %s",
                        "-----", "----", "-----", "----", "--------");

                    first = FALSE;
                }

                if (options.output_tags) {
                    out(pl, "<tag type=storm storm=%d kind=%s owner=%d "
                            "loc=%d strength=%d>",
                        i, subkind_s[subkind(i)], owner, where,
                        storm_strength(i));
                }

                out(pl, "%5s  %4s  %5s  %4s     %s",
                    box_code_less(i),
                    subkind_s[subkind(i)],
                    box_code_less(owner),
                    box_code_less(where),
                    comma_num(storm_strength(i)));

                tagout(pl, "</tag type=storm>");
            }
    next_storm;

    if (!first) { tagout(pl, "</tag type=storm_report>"); }
}


void
dissipate_storm(int storm, int show) {
    int owner;
    struct entity_misc *p;
    int where = subloc(storm);
    int ship;

    assert(kind(storm) == T_storm);

    owner = npc_summoner(storm);

    /*
     *  Tue Jun 10 11:56:04 1997 -- Scott Turner
     *
     *  Same region...
     *
     */
    if (owner && kind(owner) == T_char &&
        region(owner) == region(storm)) {
        wout(owner, "%s has dissipated.", box_name(storm));
    }

    if (show) {
        int sk = subkind(storm);

        if (weather_here(where, sk) == 0) {
            switch (sk) {
                case sub_rain:
                    wout(where, "It has stopped raining.");
                    break;

                case sub_wind:
                    wout(where, "It is no longer windy.");
                    break;

                case sub_fog:
                    wout(where, "The fog has cleared.");
                    break;

                case sub_mist:
                    wout(where, "The mist has dissipated.");
                    break;

                default:
                    assert(FALSE);
            }
        }
    }

    set_where(storm, 0);

    p = p_misc(storm);

    if (p->npc_home && p->npc_cookie) {
        gen_item(p->npc_home, p->npc_cookie, 1);
    }

#if 0
    if (ship = storm_bind(storm))
    {
        struct entity_subloc *p;

        p = rp_subloc(ship);
        if (p)
            ilist_rem_value(&p->bound_storms, storm);

        rp_misc(storm)->bind_storm = 0;
    }
#endif

    delete_box(storm);
}


int
weather_here(int where, int sk) {
    int i;
    int sum = 0;

    if (loc_depth(where) == LOC_build) {
        return 0;
    }

    where = province(where);

    if (!where) { return 0; }

    loop_here(where, i)
            {
                if (kind(i) == T_storm && subkind(i) == sk) {
                    sum += storm_strength(i);
                }
            }
    next_here;

    return sum;
}


int
v_summon_rain(struct command *c) {
    int aura = c->a;
    int where;


    where = province(cast_where(c->who));
    c->d = where;

    if (crosses_ocean(where, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    if (aura < 3) {
        c->a = aura = 3;
    }

    if (!may_cookie_npc(c->who, where, item_rain_cookie)) {
        return FALSE;
    }

    if (!has_piety(c->who, aura)) {
        wout(c->who, "You haven't the aura for that prayer.");
        return FALSE;
    };

    return TRUE;
}


int
d_summon_rain(struct command *c) {
    int aura = c->a;
    int where = c->d;
    int new;
    char *name = c->parse[2];

    if (!may_cookie_npc(c->who, where, item_rain_cookie)) {
        return FALSE;
    }

    if (crosses_ocean(where, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    if (!use_piety(c->who, aura)) {
        wout(c->who, "You haven't the aura for that prayer.");
        return FALSE;
    };


    new = do_cookie_npc(c->who, where, item_rain_cookie, where);

    if (new <= 0) {
        wout(c->who, "Failed to summon a storm.");
        return FALSE;
    }

    reset_cast_where(c->who);

    if (name && *name) {
        set_name(new, name);
    }

    new_storm(new, sub_rain, aura * 2, where);

    wout(c->who, "Summoned %s.", box_name_kind(new));

    return TRUE;
}


int
v_summon_wind(struct command *c) {
    int aura = c->a;
    int where;

    where = province(cast_where(c->who));
    c->d = where;

    if (crosses_ocean(where, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    if (aura < 3) {
        c->a = aura = 3;
    }

    if (!may_cookie_npc(c->who, where, item_wind_cookie)) {
        return FALSE;
    }

    if (!has_piety(c->who, aura)) {
        wout(c->who, "You haven't the aura for that prayer.");
        return FALSE;
    };

    return TRUE;
}


int
d_summon_wind(struct command *c) {
    int aura = c->a;
    char *name = c->parse[2];
    int where = c->d;
    int new;

    if (!may_cookie_npc(c->who, where, item_wind_cookie)) {
        return FALSE;
    }

    if (crosses_ocean(where, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    if (!use_piety(c->who, aura)) {
        wout(c->who, "You haven't the aura for that prayer.");
        return FALSE;
    };

    new = do_cookie_npc(c->who, where, item_wind_cookie, where);

    if (new <= 0) {
        wout(c->who, "Failed to summon a storm.");
        return FALSE;
    }

    reset_cast_where(c->who);

    if (name && *name) {
        set_name(new, name);
    }

    new_storm(new, sub_wind, aura * 2, where);

    wout(c->who, "Summoned %s.", box_name_kind(new));

    return TRUE;
}


int
v_summon_fog(struct command *c) {
    int aura = c->a;
    int where;

    where = province(cast_where(c->who));
    c->d = where;

    if (crosses_ocean(where, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    if (aura < 3) {
        c->a = aura = 3;
    }

    if (!may_cookie_npc(c->who, where, item_fog_cookie)) {
        return FALSE;
    }

    if (!has_piety(c->who, aura)) {
        wout(c->who, "You haven't the aura for that prayer.");
        return FALSE;
    };

    return TRUE;
}


int
d_summon_fog(struct command *c) {
    int aura = c->a;
    char *name = c->parse[2];
    int where = c->d;
    int new;

    if (!may_cookie_npc(c->who, subloc(c->who), item_fog_cookie)) {
        return FALSE;
    }

    if (crosses_ocean(where, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    if (!use_piety(c->who, aura)) {
        wout(c->who, "You haven't the aura for that prayer.");
        return FALSE;
    };

    new = do_cookie_npc(c->who, where, item_fog_cookie, where);

    if (new <= 0) {
        wout(c->who, "Failed to summon a storm.");
        return FALSE;
    }

    reset_cast_where(c->who);

    if (name && *name) {
        set_name(new, name);
    }

    new_storm(new, sub_fog, aura * 2, where);

    wout(c->who, "Summoned %s.", box_name_kind(new));

    return TRUE;
}


static struct exit_view *
parse_storm_dir(struct command *c, int storm) {
    int where = subloc(storm);
    struct exit_view **l;
    int i;
    int dir;

    l = exits_from_loc_nsew(c->who, where);

    if (valid_box(c->a)) {
        if (where == c->a) {
            wout(c->who, "%s is already in %s.",
                 box_name(storm), box_name(where));
            return FALSE;
        }

        {
            struct exit_view *ret = NULL;

            for (i = 0; i < ilist_len(l); i++) {
                if (l[i]->destination == c->a) {
                    ret = l[i];
                }
            }

            if (ret) {
                return ret;
            }
        }

        wout(c->who, "No route from %s to %s.",
             box_name(where),
             c->parse[1]);

        return NULL;
    }

    dir = lookup(full_dir_s, c->parse[1]);

    if (dir < 0) {
        dir = lookup(short_dir_s, c->parse[1]);
    }

    if (dir < 0) {
        wout(c->who, "Unknown direction or destination '%s'.",
             c->parse[1]);
        return NULL;
    }

    if (!DIR_NSEW(dir)) {
        wout(c->who, "Direction must be N, S, E or W.");
        return FALSE;
    }

    for (i = 0; i < ilist_len(l); i++) {
        if (l[i]->direction == dir) {
            return l[i];
        }
    }

    wout(c->who, "No %s route from %s.",
         full_dir_s[dir], box_name(where));
    return NULL;
}


int
v_direct_storm(struct command *c) {
    int storm = c->a;
    struct exit_view *v;
    int dest;

    if (kind(storm) != T_storm || npc_summoner(storm) != c->who) {
        wout(c->who, "You don't control any storm %s.",
             box_code(storm));
        return FALSE;
    }

    cmd_shift(c);

    v = parse_storm_dir(c, storm);

    if (v == NULL) {
        return FALSE;
    }

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    if (loc_depth(v->destination) != LOC_province) {
        wout(c->who, "Can't direct storm to %s.",
             box_code(v->destination));
        return FALSE;
    }

    dest = v->destination;
    p_misc(storm)->storm_move = dest;
    p_misc(storm)->npc_dir = v->direction;

    wout(c->who, "%s will move to %s at month end.",
         box_name(storm), box_name(dest));

    return TRUE;
}


int
v_dissipate(struct command *c) {
    int storm = c->a;
    int where;
    char here_s[LEN];

    if (kind(storm) != T_storm || npc_summoner(storm) != c->who) {
        wout(c->who, "You don't control any storm %s.",
             box_code(storm));
        return FALSE;
    }

    where = province(cast_where(c->who));

    if (where == province(subloc(c->who))) {
        strcpy(here_s, "here");
    } else {
        sprintf(here_s, "in %s", box_name(where));
    }

    c->d = where;

    if (subloc(storm) != where) {
        wout(c->who, "%s is not %s.", box_name(storm), here_s);
        return FALSE;
    }

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    return TRUE;
}


int
d_dissipate(struct command *c) {
    int storm = c->a;
    int where = c->d;
    char here_s[LEN];
    struct entity_misc *p;

    if (kind(storm) != T_storm || npc_summoner(storm) != c->who) {
        wout(c->who, "You don't control any storm %s.",
             box_code(storm));
        return FALSE;
    }

    if (where == province(subloc(c->who))) {
        strcpy(here_s, "here");
    } else {
        sprintf(here_s, "in %s", box_name(where));
    }

    if (subloc(storm) != where) {
        wout(c->who, "%s is not %s.", box_name(storm), here_s);
        return FALSE;
    }

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    p = p_misc(storm);

    /* add_aura(c->who,p->storm_str/2); */
    add_piety(c->who, p->storm_str / 2, 1);
    p->storm_str = 0;

    dissipate_storm(storm, TRUE);
    out(c->who, "Current piety is now %s.", nice_num(rp_char(c->who)->religion.piety));

    return TRUE;
}


int
v_renew_storm(struct command *c) {
    int storm = c->a;
    int aura = c->b;
    int where;
    char here_s[LEN];

    if (kind(storm) != T_storm) {
        wout(c->who, "%s is not a storm.", box_code(storm));
        return FALSE;
    }

    if (aura < 1) {
        c->b = aura = 1;
    }

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    if (!has_piety(c->who, aura)) {
        wout(c->who, "You haven't the aura for that prayer.");
        return FALSE;
    };

    where = province(cast_where(c->who));

    if (where == province(subloc(c->who))) {
        strcpy(here_s, "here");
    } else {
        sprintf(here_s, "in %s", box_name(where));
    }

    c->d = where;

    if (subloc(storm) != where) {
        wout(c->who, "%s is not %s.", box_name(storm), here_s);
        return FALSE;
    }

    return TRUE;
}


int
d_renew_storm(struct command *c) {
    int storm = c->a;
    int aura = c->b;
    int where = c->d;
    char here_s[LEN];
    struct entity_misc *p;

    if (kind(storm) != T_storm) {
        wout(c->who, "%s is not a storm.", box_code(storm));
        return FALSE;
    }

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    if (where == province(subloc(c->who))) {
        strcpy(here_s, "here");
    } else {
        sprintf(here_s, "in %s", box_name(where));
    }

    if (subloc(storm) != where) {
        wout(c->who, "%s is not %s.", box_name(storm), here_s);
        return FALSE;
    }

    if (!use_piety(c->who, aura)) {
        wout(c->who, "You haven't the aura for that prayer.");
        return FALSE;
    };

    p = p_misc(storm);
    p->storm_str += aura * 2;

    out(c->who, "%s is now strength %s.",
        box_name(storm), comma_num(p->storm_str));

    return TRUE;
}


int
v_lightning(struct command *c) {
    int storm = c->a;
    int target = c->b;
    int where;

    if (kind(storm) != T_storm || npc_summoner(storm) != c->who) {
        wout(c->who, "You don't control any storm %s.",
             box_code(storm));
        return FALSE;
    }

    if (subkind(storm) != sub_rain) {
        wout(c->who, "%s is not a rain storm.", box_name(storm));
        return FALSE;
    }

    where = subloc(storm);

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    if (kind(target) != T_char && !is_loc_or_ship(target)) {
        wout(c->who, "%s is not a valid target.", box_code(target));
        return FALSE;
    }

    if (is_loc_or_ship(target) && loc_depth(target) != LOC_build) {
        wout(c->who, "%s is not a valid target.", box_code(target));
        return FALSE;
    }

    if (subloc(target) != where) {
        wout(c->who, "Target %s isn't in the same place as the storm.",
             box_code(target));
        return FALSE;
    }

    if (in_safe_now(target)) {
        wout(c->who, "Not allowed in a safe haven.");
        return FALSE;
    }

    return TRUE;
}


int
d_lightning(struct command *c) {
    int storm = c->a;
    int target = c->b;
    int aura = c->c;
    int where;
    struct entity_misc *p;

    if (kind(storm) != T_storm || npc_summoner(storm) != c->who) {
        wout(c->who, "You don't control any storm %s.",
             box_code(storm));
        return FALSE;
    }

    if (subkind(storm) != sub_rain) {
        wout(c->who, "%s is not a rain storm.", box_name(storm));
        return FALSE;
    }

    where = subloc(storm);

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    if (kind(target) != T_char && !is_loc_or_ship(target)) {
        wout(c->who, "%s is not a valid target.", box_code(target));
        return FALSE;
    }

    if (is_loc_or_ship(target) && loc_depth(target) != LOC_build) {
        wout(c->who, "%s is not a valid target.", box_code(target));
        return FALSE;
    }

    if (subloc(target) != where) {
        wout(c->who, "Target %s isn't in the same place as the storm.",
             box_code(target));
        return FALSE;
    }

    if (in_safe_now(target)) {
        wout(c->who, "Not allowed in a safe haven.");
        return FALSE;
    }

    p = p_misc(storm);

    if (aura == 0) {
        aura = p->storm_str;
    }

    if (aura > p->storm_str) {
        aura = p->storm_str;
    }

    p->storm_str -= aura;

    wout(c->who, "%s strikes %s with a lightning bolt!",
         box_name(storm), box_name(target));

    vector_clear();
    vector_add(where);
    vector_add(target);

    if (is_loc_or_ship(target)) {
        /*
         *  Protection provided by priests of Eres
         *
         */
        if (priest_in_stack(target, sk_eres)) {
            wout(VECT, "Lightning narrowly misses %s!", box_name(target));
        } else {
            wout(VECT, "%s was struck by lightning!",
                 box_name(target));
            add_structure_damage(target, aura);
        };
    } else {
        if (is_priest(target) == sk_eres) {
            wout(VECT, "Lightning strikes %s but does no harm!",
                 box_name(target));
        } else {
            wout(VECT, "%s was struck by lightning!",
                 box_name(target));
            add_char_damage(target, aura, MATES);
        };
    };

    if (p->storm_str <= 0) {
        dissipate_storm(storm, TRUE);
    }

    return TRUE;
}


#if 0
int
v_list_storms(struct command *c)
{

  if (!has_piety(c->who, 1)) {
    wout(c->who, "You haven't the aura for that prayer.");
    return FALSE;
  };

    return TRUE;
}


int
d_list_storms(struct command *c)
{
    int where;
    int i;
    int first = TRUE;
    char here_s[LEN];

    if (!use_piety(c->who, 1)) {
      wout(c->who, "You haven't the aura for that prayer.");
      return FALSE;
    };

    where = province(reset_cast_where(c->who));

    if (crosses_ocean(where,c->who)) {
      wout(c->who,"Something seems to block your magic.");
      return FALSE;
    };

    if (where == province(subloc(c->who)))
        strcpy(here_s, "here");
    else
        sprintf(here_s, "in %s", box_name(where));

    loop_here(where, i)
    {
        if (kind(i) != T_storm)
            continue;

        if (first)
        {
            wout(c->who, "Storms %s:", here_s);
            indent += 3;
            first = FALSE;
        }

        wout(c->who, "%s", liner_desc(i));
    }
    next_here;

    if (first)
        wout(c->who, "There are no storms %s.", here_s);
    else
        indent -= 3;

    return TRUE;
}
#endif


int
v_seize_storm(struct command *c) {
    int storm = c->a;
    int where;
    char here_s[LEN];

    if (kind(storm) != T_storm) {
        wout(c->who, "%s isn't a storm.", box_code(storm));
        return FALSE;
    }

    if (npc_summoner(storm) == c->who) {
        wout(c->who, "You already control %s.", box_name(storm));
        return FALSE;
    }

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    where = province(cast_where(c->who));

    if (where == province(subloc(c->who))) {
        strcpy(here_s, "here");
    } else {
        sprintf(here_s, "in %s", box_name(where));
    }

    c->d = where;

    if (subloc(storm) != where) {
        wout(c->who, "%s is not %s.", box_name(storm), here_s);
        return FALSE;
    }

    return TRUE;
}


int
d_seize_storm(struct command *c) {
    int storm = c->a;
    int where = c->d;
    char here_s[LEN];
    int owner;

    if (kind(storm) != T_storm) {
        wout(c->who, "%s isn't a storm.", box_code(storm));
        return FALSE;
    }

    owner = npc_summoner(storm);

    if (owner && owner == c->who) {
        wout(c->who, "You already control %s.", box_name(storm));
        return FALSE;
    }

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    if (where == province(subloc(c->who))) {
        strcpy(here_s, "here");
    } else {
        sprintf(here_s, "in %s", box_name(where));
    }

    if (subloc(storm) != where) {
        wout(c->who, "%s is not %s.", box_name(storm), here_s);
        return FALSE;
    }

    vector_clear();
    vector_add(c->who);
    if (owner) {
        vector_add(owner);
    }

    wout(VECT, "%s seized control of %s!",
         box_name(c->who), box_name(storm));

    p_misc(storm)->summoned_by = c->who;

    return TRUE;
}


int
v_death_fog(struct command *c) {
    int storm = c->a;
    int target = c->b;
    int where;

    if (kind(storm) != T_storm || npc_summoner(storm) != c->who) {
        wout(c->who, "You don't control any storm %s.",
             box_code(storm));
        return FALSE;
    }

    if (subkind(storm) != sub_fog) {
        wout(c->who, "%s is not a fog.", box_name(storm));
        return FALSE;
    }

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    where = subloc(storm);

    if (kind(target) != T_char) {
        wout(c->who, "%s is not a valid target.", box_code(target));
        return FALSE;
    }

    if (in_safe_now(target)) {
        wout(c->who, "Not allowed in a safe haven.");
        return FALSE;
    }

    if (subloc(target) != where) {
        wout(c->who, "Target %s isn't in the same place as the fog.",
             box_code(target));
        return FALSE;
    }

    return TRUE;
}


static char *
fog_excuse() {

    switch (rnd(1, 3)) {
        case 1:
            return "wandered off in the fog and were lost.";
        case 2:
            return "choked to death in the poisonous fog.";
        case 3:
            return "disappeared in the fog.";
        default:
            assert(FALSE);
    }
}


int
d_death_fog(struct command *c) {
    int storm = c->a;
    int target = c->b;
    int aura = c->c;
    int where;
    struct entity_misc *p;
    int save_aura;
    int aura_used;
    struct item_ent *e;

    if (kind(storm) != T_storm || npc_summoner(storm) != c->who) {
        wout(c->who, "You don't control any storm %s.",
             box_code(storm));
        return FALSE;
    }

    if (subkind(storm) != sub_fog) {
        wout(c->who, "%s is not a fog.", box_name(storm));
        return FALSE;
    }

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    where = subloc(storm);

    if (kind(target) != T_char) {
        wout(c->who, "%s is not a valid target.", box_code(target));
        return FALSE;
    }

    if (subloc(target) != where) {
        wout(c->who, "Target %s isn't in the same place as the fog.",
             box_code(target));
        return FALSE;
    }

    p = p_misc(storm);

    aura = min(aura, p->storm_str);

    loop_inv(target, e)
                {
                    /*
                     *   Quit if we've run out of aura.
                     *
                     */
                    if (aura < 1) { break; }

                    if (item_attack(e->item) > 0 &&
                        item_attack(e->item) + item_defense(e->item) +
                        item_missile(e->item) < 25) {
                        /*
                         *  Kill some or all of these...
                         *
                         */
                        int amount = min(has_item(target, e->item), aura);
                        consume_item(target, e->item, amount);
                        wout(target, "%s %s %s.", cap(nice_num(amount)),
                             plural_item_name(e->item, amount),
                             fog_excuse());
                        aura_used += amount;
                    };
                }next_inv;

    if (aura_used == 0) {
        wout(c->who, "%s has no vulnerable men.",
             box_name(target));
        return FALSE;
    };

    wout(c->who, "Killed %s men of %s.",
         nice_num(aura_used),
         aura_used == 1 ? "man" : "men",
         box_name(target));

    p->storm_str -= aura_used;

    if (p->storm_str <= 0) {
        dissipate_storm(storm, TRUE);
    }

    return TRUE;
}


int
v_banish_corpses(struct command *c) {
    int target = c->a;
    int i;
    int sum = 0;

    sum = has_item(target, item_corpse);

    if (sum == 0) {
        wout(c->who, "There are no %s here.",
             plural_item_name(item_corpse, 2));
        return FALSE;
    }

    return TRUE;
}


int
d_banish_corpses(struct command *c) {
    int target = c->a;
    int max_aura = c->b;
    int i;
    int sum = 0;
    int n;
    int max;

    sum = has_item(target, item_corpse);

    if (sum == 0) {
        wout(c->who, "There are no %s here.",
             plural_item_name(item_corpse, 2));
        return FALSE;
    }

    if (is_priest(c->who)) {
        max = rp_char(c->who)->religion.piety;
    } else {
        max = char_cur_aura(c->who);
    };

    if (max == 0) {
        wout(c->who, "You lack the strength to challenge the undead.");
        return FALSE;
    };

    if (max < sum) { sum = max; }

    if (max_aura && (sum > max_aura)) { sum = max_aura; }

    if (is_priest(c->who)) {
        use_piety(c->who, sum);
    } else {
        charge_aura(c->who, sum);
    };

    wout(c->who, "Banished %s %s.", comma_num(sum),
         plural_item_name(item_corpse, sum));
    wout(province(c->who), "%s banished %s %s!",
         box_name(c->who), comma_num(sum),
         plural_item_name(item_corpse, sum));

    consume_item(target, item_corpse, sum);
    wout(i, "%s banished our %s!", box_name(c->who),
         plural_item_name(item_corpse, n));

    return TRUE;
}


int
v_fierce_wind(struct command *c) {
    int storm = c->a;
    int target = c->b;
    int where;

    if (!has_holy_symbol(c->who)) {
        wout(c->who, "You must have a holy symbol to use fierce wind.");
        return FALSE;
    };

    if (!has_holy_plant(c->who)) {
        wout(c->who, "You must have a holy plant to use fierce wind.");
        return FALSE;
    };

    if (kind(storm) != T_storm || npc_summoner(storm) != c->who) {
        wout(c->who, "You don't control any storm %s.",
             box_code(storm));
        return FALSE;
    }

    if (subkind(storm) != sub_wind) {
        wout(c->who, "%s is not a rain storm.", box_name(storm));
        return FALSE;
    }

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    where = subloc(storm);

    if (!is_loc_or_ship(target) || loc_depth(target) != LOC_build) {
        wout(c->who, "%s is not a valid target.", box_code(target));
        return FALSE;
    }

    if (subloc(target) != where) {
        wout(c->who, "Target %s isn't in the same place as the storm.",
             box_code(target));
        return FALSE;
    }

    return TRUE;
}


int
d_fierce_wind(struct command *c) {
    int storm = c->a;
    int target = c->b;
    int aura = c->c;
    int where;
    struct entity_misc *p;

    if (!has_holy_symbol(c->who)) {
        wout(c->who, "You must have a holy symbol to use fierce wind.");
        return FALSE;
    };

    if (!has_holy_plant(c->who)) {
        wout(c->who, "You must have a holy plant to use fierce wind.");
        return FALSE;
    };

    if (kind(storm) != T_storm || npc_summoner(storm) != c->who) {
        wout(c->who, "You don't control any storm %s.",
             box_code(storm));
        return FALSE;
    }

    if (subkind(storm) != sub_rain) {
        wout(c->who, "%s is not a rain storm.", box_name(storm));
        return FALSE;
    }

    if (crosses_ocean(storm, c->who)) {
        wout(c->who, "Something seems to block your prayer.");
        return FALSE;
    };

    where = subloc(storm);

    if (!is_loc_or_ship(target) || loc_depth(target) != LOC_build) {
        wout(c->who, "%s is not a valid target.", box_code(target));
        return FALSE;
    }

    if (subloc(target) != where) {
        wout(c->who, "Target %s isn't in the same place as the storm.",
             box_code(target));
        return FALSE;
    }

    if (!use_piety(c->who, skill_piety(c->use_skill))) {
        wout(c->who, "You don't have the piety required to use that prayer.");
        return FALSE;
    };

    /*
     *  Use up 1 holy plant.
     *
     */
    move_item(c->who, 0, holy_plant(c->who), 1);

    p = p_misc(storm);

    if (aura == 0) {
        aura = p->storm_str;
    }

    if (p->storm_str > aura) {
        aura = p->storm_str;
    }

    p->storm_str -= aura;

    vector_clear();
    vector_add(where);
    vector_add(target);
    vector_add(c->who);
    /*
     *  Protection provided by priests of Eres
     *
     */
    if (priest_in_stack(target, sk_eres)) {
        wout(VECT, "A fierce wind blows around %s but does no harm!",
             box_name(target));
    } else {
        wout(VECT, "%s is buffeted by a fierce wind!", box_name(target));
        add_structure_damage(target, aura);
    };

    if (p->storm_str <= 0) {
        dissipate_storm(storm, TRUE);
    }

    return TRUE;
}


static void
create_some_storms(int num, int kind) {
    static ilist l = NULL;
    int i;

    ilist_clear(&l);

    loop_province(i)
                {
                    if (in_clouds(i) || in_hades(i) || in_faery(i)) {
                        continue;
                    }

                    if (subkind(i) == sub_mine_shaft) { continue; }

                    if (weather_here(i, kind)) {
                        continue;
                    }

                    ilist_append(&l, i);
                }
    next_province;

    ilist_scramble(l);

    for (i = 0; i < ilist_len(l) && i < num; i++) {
        new_storm(0, kind, rnd(2, 3), l[i]);
    }

    ilist_reclaim(&l);
}


void
natural_weather() {
    int nprov = nprovinces();
    int n;

/*
 *  One natural storm per 4 (formerly 16) provinces.
 *  Half of storms made each month.
 *  Called four times per month.
 */

    n = nprov / 4 / 2 / 4;

    switch (oly_month(sysclock)) {
        case 0:    /* Fierce winds */
            create_some_storms(n, sub_fog);
            create_some_storms(n, sub_wind);
            break;

        case 1:    /* Snowmelt */
            create_some_storms(n, sub_fog);
            create_some_storms(n, sub_rain);
            break;

        case 2:    /* Blossom bloom */
            break;

        case 3:    /* Sunsear */
            create_some_storms(n, sub_rain);
            break;

        case 4:    /* Thunder and rain */
            create_some_storms(n * 2, sub_rain);
            break;

        case 5:    /* Harvest */
            break;

        case 6:    /* Waning days */
#if 1
            create_some_storms(n, sub_rain);
            create_some_storms(n, sub_fog);
            create_some_storms(n, sub_rain);
#endif
            break;

        case 7:    /* Dark night */
            create_some_storms(n, sub_wind);
            break;

        default:
            assert(FALSE);
    }
}


static void
update_weather_view_loc_sup(int who, int where) {
    int pl;

    pl = player(who);
    assert(valid_box(pl));

    set_bit(&(p_player(pl)->weather_seen), where);
}


void
update_weather_view_locs(int stack, int where) {
    int i;

    where = province(where);

    if (kind(stack) == T_char && weather_mage(stack)) {
        update_weather_view_loc_sup(stack, where);
    }

    loop_char_here(stack, i)
            {
                if (!is_prisoner(i) && weather_mage(i)) {
                    update_weather_view_loc_sup(i, where);
                }
            }
    next_char_here;
}


void
init_weather_views() {
    int who;

    loop_char(who)
            {
                if (weather_mage(who)) {
                    update_weather_view_loc_sup(who, province(who));
                }
            }
    next_char;
}


int
can_see_weather_here(int who, int where) {
    int pl = player(who);
    struct entity_player *p;

    assert(valid_box(pl));

    where = province(where);

    p = rp_player(pl);
    if (p == NULL) {
        return FALSE;
    }

    return test_bit(p->weather_seen, where);
}

