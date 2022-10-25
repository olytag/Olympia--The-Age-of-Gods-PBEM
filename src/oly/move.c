// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    <string.h>
#include <stdlib.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"
#include "vectors/cs_list.h"
#include "vectors/effect_list.h"
#include "vectors/exit_view_list.h"
#include "vectors/item_ent_list.h"


static ilist ocean_chars = NULL;


void
departure_message(int who, struct exit_view *v) {
    char *to = "";
    char *with;
    char *desc;
    char *comma = "";

    assert(valid_box(who));

    if (!is_loc_or_ship(v->destination) ||
        !is_loc_or_ship(v->orig)) {
        return;
    }

    if (char_really_hidden(who)) {
        return;
    }

    if (loc_depth(v->orig) == LOC_province &&
        (weather_here(v->orig, sub_fog) || weather_here(v->orig, sub_fog)) &&
        is_priest(who) != sk_domingo) {
        return;
    }

    if (v->dest_hidden) {
        return;
    }

    desc = liner_desc(who);        /* consumes mucho souts */

    if (subloc(v->destination) == v->orig) {
        to = sout(" entered %s", box_name(v->destination));
    } else if (subloc(v->orig) == v->destination) {
        to = sout(" exited %s", box_name(v->orig));
    } else if (viewloc(v->orig) != viewloc(v->destination)) {
        if (v->direction >= 1 && v->direction <= 4) {
            to = sout(" went %s", full_dir_s[v->direction]);
        } else {
            to = sout(" left for %s", box_name(v->destination));
        }
    } else {
        return;
    }

    with = display_with(who);

    if (strchr(desc, ',')) {
        comma = ",";
    }

    if (!*with) {
        with = ".";
    }

    if (viewloc(v->orig) != viewloc(v->destination)) {
        int garr = garrison_here(v->orig);

        if (garr && garrison_notices(garr, who)) {
            show_to_garrison = TRUE;
        }
    }

    wout(v->orig, "%s%s%s%s", desc, comma, to, with);
    show_chars_below(v->orig, who);

    show_to_garrison = FALSE;
}


static void
arrival_message(int who, struct exit_view *v) {
    char *from = "";
    char *with;
    char *desc;
    char *comma = "";

    if (char_really_hidden(who)) {
        return;
    }
    /*
     *  Show nothing if foggy or misty here.
     *
     */
    if (loc_depth(v->destination) == LOC_province &&
        (weather_here(v->destination, sub_fog) ||
         weather_here(v->destination, sub_mist))) {
        return;
    }

    desc = liner_desc(who);        /* consumes mucho souts */

    if (v->orig_hidden == FALSE) {
        if (v->direction >= 1 && v->direction <= 4) {
            from = sout(" from the %s",
                        full_dir_s[exit_opposite[v->direction]]);
        } else {
            from = sout(" from %s", box_name(v->orig));
        }
    }

    with = display_with(who);

    if (strchr(desc, ',')) {
        comma = ",";
    }

    if (!*with) {
        with = ".";
    }

    if (viewloc(v->orig) != viewloc(v->destination)) {
        int garr = garrison_here(v->destination);

        if (garr) {
            if (garrison_notices(garr, who)) {
                show_to_garrison = TRUE;
            }

            if (garrison_spot_check(garr, who)) {
                indent += 3;
                wout(garr, "%s%s", desc, with);
                show_chars_below(garr, who);
                indent -= 3;
            }
        }
    }

    wout(v->destination, "%s%s arrived%s%s", desc, comma, from, with);
    show_chars_below(v->destination, who);

    show_to_garrison = FALSE;
}


/*
 *  Mark that we know both ends of a hidden road we're about to go through
 */

static void
discover_road(int who, int where, struct exit_view *v) {
    struct exit_view **l;
    int i;
    int j;

    l = exits_from_loc(who, v->destination);

    for (i = 0; i < ev_list_len(l); i++) {
        if (l[i]->road && l[i]->destination == where) {
            set_known(who, l[i]->road);
            set_known(who, v->road);

            loop_char_here(who, j)
                    {
                        set_known(j, l[i]->road);
                        set_known(j, v->road);
                    }
            next_char_here;
        }
    }
}


struct exit_view *
parse_exit_dir(struct command *c, int where, char *zero_arg) {
    struct exit_view **l;
    int i;
    int dir;

    l = exits_from_loc(c->who, where);

    if (valid_box(c->a) && subkind(c->a) != sub_region) {
        if (where == c->a) {
            if (zero_arg) {
                wout(c->who, "Already in %s.", box_name(where));
            }
            return FALSE;
        }
        /*
         *  Give priority to passable routes.  A secret passable route may
         *  parallel a visible impassable route.
         */
        {
            struct exit_view *ret = NULL;
            struct exit_view *impass_ret = NULL;

            for (i = 0; i < ev_list_len(l); i++) {
                if (l[i]->destination == c->a &&
                    (l[i]->hidden == FALSE || see_all(c->who))) {
                    if (l[i]->impassable) {
                        impass_ret = l[i];
                    } else {
                        ret = l[i];
                    }
                }
            }

            if (ret) {
                return ret;
            }
            if (impass_ret) {
                return impass_ret;
            }
        }

#if 0
        if (zero_arg)
          wout(c->who, "No visible route from %s to %s.",
           box_name(where),
           c->parse[1]);
#endif

        return NULL;
    }

    dir = lookup(full_dir_s, c->parse[1]);

    if (dir < 0) {
        dir = lookup(short_dir_s, c->parse[1]);
    }

    if (dir < 0) {
#if 0
        if (zero_arg)
          wout(c->who, "Unknown direction or destination '%s'.",
           c->parse[1]);
#endif
        return NULL;
    }

    for (i = 0; i < ev_list_len(l); i++) {
        if (l[i]->direction == dir &&
            (l[i]->hidden == FALSE || see_all(c->who))) {
            if (dir == DIR_IN && zero_arg) {
                wout(c->who, "(assuming '%s %s')",
                     zero_arg,
                     box_code_less(l[i]->destination));
            }

            return l[i];
        }
    }

    /*
     *  Wed Nov 18 18:28:23 1998 -- Scott Turner
     *
     *  Convert "move out" to "move up" if out isn't available and
     *  up is.
     *
     */
    if (dir == DIR_OUT) {
        for (i = 0; i < ev_list_len(l); i++) {
            if (l[i]->direction == DIR_UP &&
                (l[i]->hidden == FALSE || see_all(c->who))) {
                wout(c->who, "(assuming 'move up')");
                return l[i];
            }
        }
    }


    if (dir == DIR_IN) {
        for (i = 0; i < ev_list_len(l); i++) {
            if (l[i]->direction == DIR_DOWN &&
                (l[i]->hidden == FALSE || see_all(c->who))) {
                wout(c->who, "(assuming 'move down')");
                return l[i];
            }
        }
    }


#if 0
    if (zero_arg)
      wout(c->who, "No visible %s route from %s.",
       full_dir_s[dir], box_name(where));
#endif

    return NULL;
}

static void
kill_random_mount(int who) {
    struct item_ent *e;
    inventory_loop(who, e)
                {
                    if (item_ride_cap(e->item) >= 100) {
                        wout(who, "The forced ride kills one %s.", just_name(e->item));
                        sub_item(who, e->item, 1);
                        return;
                    };
                }
    inventory_next;
    assert(FALSE);
    /* NOT REACHED */
    exit(2);
};

#define FORCED_RIDE 2
#define FORCED_MARCH 1

int
move_exit_land(struct command *c, struct exit_view *v, int show) {
    struct weights w;
    int delay = v->distance;
    int terr;            /* destination terrain */
    int swamp = FALSE;        /* traveling in a swamp? */
    int mountains = FALSE;
    int val, i;
    int art;

    if (delay == 0) { return 0; }

    terr = subkind(province(v->destination));

    if (terr == sub_swamp ||
        subkind(v->destination) == sub_bog ||
        subkind(v->destination) == sub_pits) {
        swamp = TRUE;
    }

    if (terr == sub_mountain) { mountains = TRUE; }

    determine_stack_weights(c->who, &w, mountains);

    /*
     *  Are you riding?
     *
     */
    if (w.ride_cap >= w.ride_weight && !swamp) {
        /*
         *  You're riding.
         *
         */
        delay -= delay / 2;
        /*
         *  Are you also forcing the pace?
         *
         *  Tue May 25 06:45:58 1999 -- Scott Turner
         *
         *  Do not ignore ninjas, etc!
         *
         */
        if (get_effect(c->who, ef_forced_march, 0, 0) &&
            char_really_alone(c->who)) {
            delay -= delay / 2;
            v->forced_march = FORCED_RIDE;
        };
        /*
         *  Or perhaps you're doing a forced march?
         *
         */
    } else if (get_effect(c->who, ef_forced_march, 0, 0) &&
               char_really_alone(c->who)) {
        delay -= delay / 2;
        v->forced_march = FORCED_MARCH;
    } else {
        /*
         *  You're walking.
         *
         */
        if (w.land_weight > w.land_cap * 2) {
            wout(c->who, "%s is too overloaded to travel.",
                 box_name(c->who));
            wout(c->who, "You have a total of %d weight, and your maximum capacity is %d.", w.land_weight,
                 w.land_cap * 2);
            return -1;
        }
        /*
         *  In a swamp with animals?
         *
         */
        if (swamp && w.animals) {
            if (show) {
                wout(c->who, "Difficult terrain slows the animals.  "
                             "Travel will take an extra day.");
            }
            delay += 1;
        }

        /*
         *  Overloaded?
         *
         */
        if (w.land_weight > w.land_cap) {
            int ratio;
            int additional;

            ratio = (w.land_weight - w.land_cap) * 100 / w.land_cap;
            additional = delay * ratio / 100;

            if (show) {
                if (additional == 1) {
                    wout(c->who, "Excess inventory slows movement.  "
                                 "Travel will take an extra day.");
                } else if (additional > 1) {
                    wout(c->who, "Excess inventory slows movement.  "
                                 "Travel will take an extra %s days.",
                         nice_num(additional));
                }
            }

            delay += additional;
        }
    };

    /*
     *  The fast move effect speeds you by 2x if you're moving into
     *  your holy terrain.
     *
     */
    val = get_all_effects(c->who, ef_fast_move, 0, 0);
    if (val && is_holy_terrain(c->who, v->destination)) {
        int additional = delay / 2;
        if (show) {
            if (additional == 1) {
                wout(c->who, "Holy guidance speeds your travel by one day.");
            } else {
                wout(c->who, "Holy guidance speeds your travel by %d days.",
                     additional);
            };
        }
        delay -= additional;
        for (i = 0; i < val; i++) {
            delete_effect(c->who, ef_fast_move, 0);
        };
    };

    /*
     *  The slow move effect slows you by 2x.
     *
     *  Mon Oct  7 12:19:17 1996 -- Scott Turner
     *
     *  Shouldn't effect other priests...
     *
     */
    if (get_effect(v->destination, ef_slow_move, 0, 0)) {
        /*
         *  A priest?
         *
         */
        if (is_holy_terrain(c->who, v->destination)) {
            if (show) {
                wout(c->who, "You sense the power of %s obscuring the entrances to %s.",
                     god_name(is_priest(c->who)), box_name(v->destination));
            };
        } else {
            if (show) {
                if (delay == 1) {
                    wout(c->who, "A magical effect slows your movement by one day.");
                } else {
                    wout(c->who, "A magical effect slows your movement by %d days.",
                         delay);
                };
            }
            delay += delay;
        };
    };

    /*
     *  Tue Oct  6 18:10:55 1998 -- Scott Turner
     *
     *  Perchance you have an artifact.
     *
     */
    if (art = best_artifact(c->who, ART_FAST_TERR, v->destination, 0)) {
        delay -= rp_item_artifact(art)->param2;
        wout(c->who, "Your passage is magically sped by %s day%s.",
             nice_num(rp_item_artifact(art)->param2),
             rp_item_artifact(art)->param2 > 1 ? "s" : "");
    };

    if (delay < 0) { delay = 0; }

    return delay;
}


static int
move_exit_fly(struct command *c, struct exit_view *v, int show) {
    struct weights w;
    int delay = v->distance;

    if (subkind(v->destination) == sub_under) {
        wout(c->who, "Cannot fly underground.");
        return -1;
    }

    if (delay > 3) {
        delay = 3;
    }

    determine_stack_weights(c->who, &w, FALSE);

    if (w.fly_cap < w.fly_weight) {
        wout(c->who, "%s is too overloaded to fly.",
             box_name(c->who));
        wout(c->who, "You have a total of %d weight, and your "
                     "maximum flying capacity is %d.", w.fly_weight, w.fly_cap);
        return -1;
    }

    return delay;
}


void
save_v_array(struct command *c, struct exit_view *v) {

#if 0
    c->v.direction = v->direction;
    c->v.destination = v->destination;
    c->v.road = v->road;
    c->v.dest_hidden = v->dest_hidden;
    c->v.distance = v->distance;
    c->v.orig = v->orig;
    c->v.orig_hidden = v->orig_hidden;
#endif
    c->b = v->direction;
    c->c = v->destination;
    c->d = v->road;
    c->e = v->dest_hidden;
    c->f = v->distance;
    c->g = v->orig;
    c->h = v->orig_hidden;
    c->i = v->seize;
    c->j = v->forced_march;
}


void
restore_v_array(struct command *c, struct exit_view *v) {

    bzero(v, sizeof(*v));

#if 0
    v->direction = c->v.direction;
    v->destination = c->v.destination;
    v->road = c->v.road;
    v->dest_hidden = c->v.dest_hidden;
    v->distance = c->v.distance;
    v->orig = c->v.orig;
    v->orig_hidden = c->v.orig_hidden;
#endif

    v->direction = c->b;
    v->destination = c->c;
    v->road = c->d;
    v->dest_hidden = c->e;
    v->distance = c->f;
    v->orig = c->g;
    v->orig_hidden = c->h;
    v->seize = c->i;
    v->forced_march = c->j;
}


static void
suspend_stack_actions(int who) {
    int i;

    loop_stack(who, i)
            {
                p_char(i)->moving = sysclock.days_since_epoch;
            }
    next_stack;
}


void
restore_stack_actions(int who) {
    int i;

    loop_stack(who, i)
            {
                p_char(i)->moving = 0;
            }
    next_stack;
}


void
clear_guard_flag(int who) {
    int pl;
    int i;

    if (kind(who) == T_char)
        p_char(who)->guard = FALSE;

    loop_char_here(who, i)
            {
                p_char(i)->guard = FALSE;
            }
    next_char_here;
}


static int
land_check(struct command *c, struct exit_view *v, int show) {
    int owner;        /* owner of loc we're moving into, if any */

    if (v->water) {
        if (show) {
            wout(c->who, "A sea-worthy ship is required "
                         "for travel across water.");
        }
        return FALSE;
    }

    if (v->impassable) {
        if (show) {
            wout(c->who, "That route is impassable.");
        }
        return FALSE;
    }

    if (v->in_transit) {
        if (show) {
            wout(c->who, "%s is underway.  Boarding is not "
                         "possible.", box_name(v->destination));
        }
        return FALSE;
    }

#if 0
    if (loc_depth(v->destination) == LOC_build &&
        (owner = building_owner(v->destination)) &&
        !will_admit(owner, c->who, v->destination) &&
        v->direction != DIR_OUT) {
      if (show) {
        wout(c->who, "%s refused to let us enter.",
         box_name(owner));
        wout(owner, "Refused to let %s enter.",
         box_name(c->who));
      }
      return FALSE;
    }
#endif

    return TRUE;
}


static int
can_move_here(int where, struct command *c) {
    struct exit_view *v;

    v = parse_exit_dir(c, where, NULL);

    if (v &&
        v->direction != DIR_IN &&
        land_check(c, v, FALSE) &&
        move_exit_land(c, v, FALSE) >= 0) {
        return TRUE;
    }

    return FALSE;
}


static int
can_move_at_outer_level(int where, struct command *c) {
    int outer;

    outer = subloc(where);
    while (loc_depth(outer) > LOC_region) {
        if (can_move_here(outer, c)) {
            return loc_depth(outer) - loc_depth(where);
        }
        outer = subloc(outer);
    }

    return 0;
}

static int
is_smuggling(int who, int ef) {
    return get_effect(who, ef, 0, 0);
};


static int
check_smuggling(int who, int ef, int sk) {
    /*
     *  How experienced is he?
     *
     */
    if (rp_skill_ent(who, sk)) {
        int exp = 50 + skill_exp(who, sk);
        /*
         *  Does he succeed?
         *
         */
        return (rnd(1, 100) < exp);
    };
    return FALSE;
};

static int
smuggle_savings(int who, int cost, int sk) {
    int reduce = (cost * rnd(50, 100)) / 100;
    if (reduce < 1) { reduce = 1; }
    if (reduce > cost) { reduce = cost; }
    /* Only increase 1x month? */
    add_skill_experience(who, sk);
    return reduce;
};

static int
smuggle_fine(int cost) {
    int fine = (cost * rnd(5, 13)) / 8;
    if (fine < 1) { fine = 1; }
    return fine;
};

static int
pay_fine(struct command *c, int fine, int ruler) {
    int total = stack_has_item(c->who, item_gold);

    if (total < fine) {
        (void) autocharge(c->who, total);
        gen_item(ruler, item_gold, total);
        wout(VECT, "Since you could not pay the entire fine, you are "
                   "questioned for 3 days and then released.");
        c->second_wait += 3;
        return FALSE;
    } else {
        (void) autocharge(c->who, fine);
        gen_item(ruler, item_gold, fine);
        wout(VECT, "You pay the fine and %s is detained for questioning for 3 days.",
             box_name(c->who));
        prepend_order(player(c->who), c->who, "wait time 3");
        return TRUE;
    };
    return TRUE;
}

int calc_entrance_fee(struct loc_control_ent *control, struct command *c, int ruler) {
    int w_cost = 0, m_cost = 0, n_cost = 0;
    vector_stack(c->who, 1);

    if (control->weight) {
        struct weights w;
        determine_stack_weights(c->who, &w, FALSE);
        w_cost += (control->weight * w.land_weight) / 1000;

        if (is_smuggling(c->who, ef_smuggle_goods)) {
            if (check_smuggling(c->who, ef_smuggle_goods, sk_smuggle_goods)) {
                /*
                 *  Success.
                   *
                   */
                int reduce = smuggle_savings(c->who, w_cost, sk_smuggle_goods);
                if (reduce) {
                    indent += 3;
                    wout(c->who, "You smuggle %s worth of goods unseen through the gates.",
                         gold_s(reduce));
                    indent -= 3;
                };
                w_cost -= reduce;
            } else if (rnd(1, 4) == 1) {
                /*
                   *  Caught smuggling.
                   *
                   */
                int fine = smuggle_fine(w_cost);
                indent += 3;
                wout(VECT, "You are caught attempting to smuggle and fined %s!",
                     gold_s(fine));
                indent -= 3;
                if (!pay_fine(c, fine, ruler)) {
                    return -1;
                };
            };
        };

        if (w_cost) {
            wout(c->who, "Entrance fee is %s on %d weight.", gold_s(w_cost),
                 w.land_weight);
        }
    }

    if (control->men) {
        m_cost = (control->men * count_stack_any(c->who)) / 100;

        if (is_smuggling(c->who, ef_smuggle_men)) {
            if (check_smuggling(c->who, ef_smuggle_men, sk_smuggle_men)) {
                /*
                 *  Success.
                 *
                 */
                int reduce = smuggle_savings(c->who, m_cost, sk_smuggle_men);
                if (reduce) {
                    indent += 3;
                    wout(c->who, "You smuggle %s worth of men unseen through the gates.",
                         gold_s(reduce));
                    indent -= 3;
                };
                m_cost -= reduce;
            } else if (rnd(1, 4) == 1) {
                /*
                   *  Caught smuggling.
                   *
                   */
                int fine = smuggle_fine(w_cost);
                indent += 3;
                wout(VECT, "You are caught attempting to smuggle and fined %s!",
                     gold_s(fine));
                indent -= 3;
                if (!pay_fine(c, fine, ruler)) {
                    return -1;
                };
            };
        };

        if (m_cost) {
            wout(c->who, "Entrance fee is %s on %d men/beasts.",
                 gold_s(m_cost),
                 count_stack_any(c->who));
        }
    }

    if (control->nobles) {
        n_cost = control->nobles * count_stack_units(c->who);

        if (is_smuggling(c->who, ef_smuggle_men)) {
            if (check_smuggling(c->who, ef_smuggle_men, sk_smuggle_men)) {
                /*
                 *  Success.
                   *
                   */
                int reduce = smuggle_savings(c->who, n_cost, sk_smuggle_men);
                if (reduce) {
                    indent += 3;
                    wout(c->who, "You smuggle %s worth of nobles unseen through the gates.",
                         gold_s(reduce));
                    indent -= 3;
                };
                n_cost -= reduce;
            } else if (rnd(1, 4) == 1) {
                /*
                   *  Caught smuggling.
                   *
                   */
                int fine = smuggle_fine(w_cost);
                indent += 3;
                wout(VECT, "You are caught attempting to smuggle and fined %s!",
                     gold_s(fine));
                indent -= 3;
                if (!pay_fine(c, fine, ruler)) {
                    return -1;
                };
            };
        };

        if (n_cost) {
            wout(c->who, "Entrance fee is %s on %d nobles.",
                 gold_s(n_cost),
                 count_stack_units(c->who));
        }
    }

    return (w_cost + m_cost + n_cost);
};

int
charge_entrance_fees(int who, int ruler, int cost) {
    extern int gold_fees;

    if (!autocharge(who, cost)) {
        wout(VECT, "Can't afford %s in fees to enter, so you are turned away.",
             gold_s(cost));
        return FALSE;
    };
    gen_item(ruler, item_gold, cost);
    wout(ruler, "Received %s in entrance fees from %s.",
         gold_s(cost), box_name(who));
    wout(VECT, "%s took %s in entrance fees from us.",
         box_name(ruler),
         gold_s(cost));
    gold_fees += cost;
    return TRUE;
};

/*
 *  Thu Mar 20 11:44:30 1997 -- Scott Turner
 *
 *  Who is immediately in control of this location?
 *
 */
int
controls_loc(int where) {
    if (!valid_box(where) ||
        (kind(where) != T_loc && kind(where) != T_ship)) {
        return FALSE;
    }

    if (kind(where) == T_loc && loc_depth(where) == LOC_province) {
        return garrison_here(where);
    } else if (subkind(where) == sub_city) {
        return garrison_here(province(where));
    } else {
        return first_character(where);
    };
};

int
player_controls_loc(int where) {
    if (!valid_box(where) ||
        (kind(where) != T_loc && kind(where) != T_ship)) {
        return FALSE;
    }

    if (kind(where) == T_loc && loc_depth(where) == LOC_province) {
        return player(province_admin(where));
    } else if (subkind(where) == sub_city) {
        return player(province_admin(province(where)));
    } else {
        return player(first_character(where));
    };
};

/*
 *  Tue Dec 29 11:30:19 1998 -- Scott Turner
 *
 *  Check to see if <who> can join <g>.
 *
 */
int
can_join_guild(int who, int g) {
    struct entity_skill *p;
    int i;

    /*
     *  Can't already belong to a guild.
     *
     */
    if (guild_member(who)) {
        wout(who, "%s is already a guild member.", box_name(who));
        return FALSE;
    };

    /*
     *  Priests can only join their strength guild.
     *
     */
    if (is_priest(who) &&
        g != rp_relig_skill(is_priest(who))->strength) {
        wout(who, "You may only join the %s Guild.",
             box_name(rp_relig_skill(is_priest(who))->strength));
        return FALSE;
    };
#if 0
    /*
     *  Obvious magicians can't join any guild.
     *
     */
    if (is_magician(who) && !char_hide_mage(who)) {
      wout(who,"Magicians are not welcome here.");
      return FALSE;
    };
#endif

    /*
     *  You need to already know all the learnable and researchable
     *  skills in this school.
     *
     */
    p = rp_skill(g);
    if (p == NULL) { return FALSE; }

    for (i = 0; i < ilist_len(p->offered); i++) {
        if (!has_skill(who, p->offered[i])) {
            wout(who, "You must master all available skills before entering a guild.");
            return FALSE;
        }
    };
    for (i = 0; i < ilist_len(p->research); i++) {
        if (!has_skill(who, p->research[i])) {
            wout(who, "You must master all available skills before entering a guild.");
            return FALSE;
        }
    };
    return TRUE;
};

/*
 *  Tue Dec 29 11:35:49 1998 -- Scott Turner
 *
 *  Join a guild.
 *
 */
int
join_guild(int who, int g) {
    if (kind(g) != T_skill || skill_school(g) != g) { return FALSE; }
    if (!can_join_guild(who, g)) { return FALSE; }
    wout(who, "%s joins the %s Guild.", just_name(who), just_name(g));
    rp_char(who)->guild = g;
    return TRUE;
};

static int
check_guilds(struct command *c, struct exit_view *v) {
    struct entity_skill *p;
    int i;

    /*
     *  Temples
     *
     *  Only a priest of the appropriate religion can move into
     *  a temple (but this doesn't prevent you from ATTACKing it or
     *  RAZEing it).
     *
     */
    if (is_temple(v->destination) &&
        is_temple(v->destination) != is_priest(c->who)) {
        wout(c->who, "Only a priest of %s may enter %s.",
             box_name(is_temple(v->destination)),
             box_name(v->destination));
        return FALSE;
    };

    /*
     *  Guilds
     *
     *  Temples are really just guilds, so we do the same thing here.
     *
     */
    if (is_guild(v->destination) &&
        is_guild(v->destination) != guild_member(c->who)) {
        return join_guild(c->who, is_guild(v->destination));
    };

    /*
     *  Otherwise, you may enter...
     *
     */
    return TRUE;
};

void
do_actual_move(struct command *c, struct exit_view *v, int delay) {
    int attack = (strcasecmp(c->parse[0], "attack") == 0);

    v->distance = delay;
    c->wait = delay;

    save_v_array(c, v);
    leave_stack(c->who);

    if ((delay - attack) > 0) {
        wout(VECT, "Travel to %s will take %s day%s.",
             box_name(v->destination),
             nice_num(delay),
             delay == 1 ? "" : "s");
    }

    suspend_stack_actions(c->who);
    clear_guard_flag(c->who);

    if ((delay - attack) > 1) {
        prisoner_movement_escape_check(c->who);
    }

    departure_message(c->who, v);

};

/*
 *  Sun Mar 30 18:40:12 1997 -- Scott Turner
 *
 *  Do an attack after failing a move.
 *
 */
void
do_move_attack(struct command *c, struct exit_view *v) {
    char buf[256];
    sprintf(buf, "attack %d", v->destination);
    prepend_order(player(c->who), c->who, buf);
};

/*
 *  Sun Mar 30 20:21:19 1997 -- Scott Turner
 *
 *  Move permitted?
 *
 *  Mon May 24 12:52:20 1999 -- Scott Turner
 *
 *  Moves into sublocs now have to check their size.
 */
int
move_permitted(struct command *c, struct exit_view *v) {
    if (v->hades_cost) {
        int n = count_stack_any(c->who);
        int cost, j;

        loop_stack(c->who, j)
                {
                    if (has_artifact(j, ART_PROT_HADES, 0, 0, 0)) {
                        wout(j, "Enter, daemon lord!");
                        n--;
                    };
                }next_stack;

        if (n < 0) { n = 0; }
        cost = v->hades_cost * n;

        if (cost) {
            if (!autocharge(c->who, cost)) {
                wout(c->who, "Can't afford %s to enter Hades.",
                     gold_s(cost));
                return FALSE;
            }
            wout(c->who, "The Gatekeeper Spirit of Hades took "
                         "%s from us.", gold_s(cost));
        };

        log_output(LOG_SPECIAL, "%s (%s) tries to enter Hades",
                   box_name(player(c->who)),
                   box_name(c->who));
    };

    /*
     *  Check and/or join guilds.
     *
     */
    if (!check_guilds(c, v)) { return FALSE; }

    /*
     *  If the destination is a subloc with an entrance size,
     *  then we'd better be that size.
     *
     */
    if (entrance_size(v->destination)) {
        /*
         *  Use the special count function to also count ninjas
         *  and angels.
         *
         */
        int n = count_stack_any_real(c->who, FALSE, FALSE);
        if (n > entrance_size(v->destination)) {
            wout(c->who, "The entrance will pass only %s at a time.",
                 nice_num(entrance_size(v->destination)));
            return FALSE;
        };
    };

    return TRUE;
};

/*
 *  Tue Sep  7 14:17:06 1999 -- Scott Turner
 *
 *  You can sneak into a province, but not anything smaller.
 *
 *  Mon Jan 15 10:25:18 2001 -- Scott Turner
 *
 *  Whoops, you'd better also be hidden!
 */
static int
can_sneak(int who, int where) {
    return (char_hidden(who) &&
            province(where) == where &&
            char_alone_stealth(who));
};

/*
 *  Mon Jan 15 10:43:50 2001 -- Scott Turner
 *
 *  This checks to see if "who" can enter "dest" peacefully from "from".
 *
 */
int
peaceful_enter(int who, int from, int where) {
    int ruler = controls_loc(where);
    int pl = player_controls_loc(where);
    struct loc_control_ent *control = NULL;

    /*
     *  Mon Mar 17 11:52:50 1997 -- Scott Turner
     *
     *  If someone rules here and is hostile to you, you won't
     *  be admitted.
     *
     */
    if (ruler &&
        is_hostile(ruler, who) &&
        !can_sneak(who, where)) {
        return FALSE;
    };

    /*
     *  Thu Mar 20 12:33:41 1997 -- Scott Turner
     *
     *  If the border is closed and you aren't on the admit list,
     *  then you are refused admission.  However, there needs to
     *  actually be someone ruling the location.
     *
     *  Mon Mar  1 10:04:12 1999 -- Scott Turner
     *
     *  There's a problem here, in that this is checking "where", which
     *  is probably not the right thing!
     *
     *  Mon May 17 08:10:56 1999 -- Scott Turner
     *
     *  Don't prevent people from leaving a mine if the border to the
     *  enclosing province is closed.
     *
     *  Tue Sep  7 14:19:17 1999 -- Scott Turner
     *
     *  Let concealed people sneak into provinces.
     *
     */
    if (pl &&
        ruler &&
        ((rp_loc(where) &&
          rp_loc(where)->control.closed) ||
         (rp_subloc(where) &&
          rp_subloc(where)->control.closed)) &&
        !somewhere_inside(where, from) &&
        subkind(from) != sub_mine &&
        subkind(from) != sub_mine_notdone &&
        !will_admit(pl, who, ruler) &&
        !can_sneak(who, where)) {
        return FALSE;
    };
    return TRUE;
};

/*
 * Sun Mar 30 20:00:15 1997 -- Scott Turner
 *
 *  Check to see if a move can be accomplished w/o a fight.
 *
 */
int
check_peaceful_move(struct command *c, struct exit_view *v) {
    int ruler = 0;
    int pl; // todo: pl is uninitialized; we said no bug fixes
    struct loc_control_ent *control = NULL;
    int cost = 0, payee = 0;
    int where = subloc(c->who);

    if (!peaceful_enter(c->who, v->orig, v->destination)) {
        wout(c->who, "You are refused admission to %s.",
             box_name(v->destination));
        return FALSE;
    };

#if 0
    /*
     *  Mon Mar 17 11:52:50 1997 -- Scott Turner
     *
     *  If someone rules here and is hostile to you, you won't
     *  be admitted.
     *
     */
    ruler = controls_loc(v->destination);
    if (ruler && is_hostile(ruler,c->who) &&
        !can_sneak(c->who, v->destination)) {
      wout(c->who,"%s refused you admission to %s.",
       box_name(ruler),box_name(v->destination));
      return FALSE;
    };

    /*
     *  Thu Mar 20 12:33:41 1997 -- Scott Turner
     *
     *  If the border is closed and you aren't on the admit list,
     *  then you are refused admission.  However, there needs to
     *  actually be someone ruling the location.
     *
     *  Mon Mar  1 10:04:12 1999 -- Scott Turner
     *
     *  There's a problem here, in that this is checking "where", which
     *  is probably not the right thing!
     *
     *  Mon May 17 08:10:56 1999 -- Scott Turner
     *
     *  Don't prevent people from leaving a mine if the border to the
     *  enclosing province is closed.
     *
     *  Tue Sep  7 14:19:17 1999 -- Scott Turner
     *
     *  Let concealed people sneak into provinces.
     *
     */
    pl = player_controls_loc(v->destination);
    if (pl &&
        ruler &&
        rp_loc(v->destination) &&
        rp_loc(v->destination)->control.closed &&
        !somewhere_inside(v->destination, v->orig) &&
        subkind(v->orig) != sub_mine &&
        subkind(v->orig) != sub_mine_notdone &&
        !will_admit(pl, c->who, ruler) &&
        !can_sneak(c->who, v->destination)) {
      wout(c->who,"%s refused you admission to %s.",
       box_name(ruler),box_name(v->destination));
      return FALSE;
    } else if (pl &&
           ruler &&
           rp_subloc(v->destination) &&
           rp_subloc(v->destination)->control.closed &&
           !somewhere_inside(v->destination, v->orig) &&
           subkind(v->orig) != sub_mine &&
           subkind(v->orig) != sub_mine_notdone &&
           !will_admit(pl, c->who, ruler) &&
           !can_sneak(c->who, v->destination)) {
      wout(c->who,"%s refused you admission to %s.",
       box_name(ruler),box_name(v->destination));
      return FALSE;
    };
#endif

    /*
     *  Mon Mar 17 11:56:23 1997 -- Scott Turner
     *
     *  Entrance fees to locations.
     *
     *  Subloc fee if:
     *
     *   1 -- you're not coming from inside the subloc
     *   2 -- fee is set.
     *   3 -- subloc has an owner
     *   4 -- owner is a different player (will_admit)
     *   4 -- owner is not admitting you free.
     *
     */
    if (rp_subloc(v->destination) &&
        !somewhere_inside(v->destination, v->orig) &&
        (rp_subloc(v->destination)->control.men ||
         rp_subloc(v->destination)->control.weight ||
         rp_subloc(v->destination)->control.nobles) &&
        pl &&
        !will_admit(pl, c->who, v->destination) &&
        !can_sneak(c->who, v->destination)) {
        control = &rp_subloc(v->destination)->control;
        /*
         *  Loc fee if:
         *
         *   1 -- fee is set.
         *   2 -- not coming from inside
         *   2 -- loc has a garrison
         *   3 -- castle is occupied.
         *   4 -- you don't rule here.
         *   5 -- coming from a loc and not a subloc
         *   6 -- orig is not administered by same as this loc
         *   7 -- you're not admitted free.
         *
         */
    } else if (rp_loc(v->destination) &&
               (rp_loc(v->destination)->control.men ||
                rp_loc(v->destination)->control.weight ||
                rp_loc(v->destination)->control.nobles) &&
               pl && /* This also checks for a controlled garrison */
               !will_admit(pl, c->who, v->destination) &&
               !may_rule_here(c->who, v->destination) &&
               kind(v->orig) == T_loc &&
               loc_depth(v->orig) == LOC_province &&
               province_admin(v->destination) !=
               province_admin(v->orig) &&
               !can_sneak(c->who, v->destination)) {
        control = &rp_loc(v->destination)->control;
    };

    if (control && ruler && (control->men ||
                             control->nobles ||
                             control->weight)) {
        if (!will_pay(c->who)) {
            wout(VECT, "%s is charging for entrance and you are not "
                       "entering any location charging for entrance.",
                 box_name(v->destination));
            return FALSE;
        };
        cost = calc_entrance_fee(control, c, ruler);
        if (cost < 0) {
            return FALSE;
        } else if (will_pay(c->who) < cost) {
            wout(VECT, "Refused to pay %s to enter %s,"
                       " so you are turned away.",
                 gold_s(cost), box_name(v->destination));
            return FALSE;
        } else if (!charge_entrance_fees(c->who, ruler, cost)) {
            return FALSE;
        };
    };
    return TRUE;
};

#if 0
/*
 *  Sun Mar 30 18:37:03 1997 -- Scott Turner
 *
 *  Check for an "attack" flag and prepend the appropriate
 *  order if turned away...
 *
 */
int
v_move(struct command *c)
{
    struct exit_view *v;
    int delay;
    int where = subloc(c->who);
    int check_outer = TRUE;
    int prepend = 0;

    if (numargs(c) < 1)
    {
        wout(c->who, "Specify direction or destination to MOVE.");
        return FALSE;
    }

    while (numargs(c) > 0)
    {
      v = parse_exit_dir(c, where, "move");

      if (v) {
        check_outer = FALSE;
            if (land_check(c, v, TRUE)) break;
        v = NULL;
      }

      cmd_shift(c);
    }

    vector_stack(c->who, TRUE);

    if (v == NULL && check_outer && can_move_at_outer_level(where, c))
    {
        c->a = subloc(where);
        v = parse_exit_dir(c, where, NULL);
        assert(v);

        if (v)
        {
            assert(move_exit_land(c, v, FALSE) >= 0);
            wout(c->who, "(assuming 'move out' first)");
            prepend = 1;
        }
    }

    if (v == NULL)
        return FALSE;

    delay = move_exit_land(c, v, TRUE);

    if (delay < 0)
        return FALSE;

    if (!move_permitted(c,v) ||
        !check_peaceful_move(c,v)) {
      return FALSE;
    };

    /*
     *  Actual movement; encapsulated so it can also be
     *  called from v_attack.
     *
     */
    do_actual_move(c,v,delay);

    if (prepend) prepend_order(player(c->who), c->who, c->line);
    return TRUE;
}
#endif


void
touch_loc_after_move(int who, int where) {
    int pl;
    int i;

    if (kind(who) == T_char) {
        touch_loc(who);
    }

    loop_char_here(who, i)
            {
                if (!is_prisoner(i)) {
                    touch_loc(i);
                }
            }
    next_char_here;
}


void
move_stack(int who, int where) {

    assert(kind(who) == T_char);

    if (!in_faery(subloc(who)) && in_faery(where)) {
        log_output(LOG_SPECIAL, "%s enters Faery at %s.",
                   box_name(who), box_name(where));
    }

    set_where(who, where);
    mark_loc_stack_known(who, where);
    touch_loc_after_move(who, where);
    update_weather_view_locs(who, where);
    clear_contacts(who);

#ifndef NEW_TRADE
    if (subkind(where) == sub_city ||
        (subkind(where) == sub_guild &&
         rp_subloc(where)->guild == sk_trading))
    {
        int i;

        loop_stack(who, i)
        {
            match_trades(i);
        }
        next_stack;
    }
#endif

    if (subkind(where) == sub_ocean) {
        struct entity_char *p;

        p = p_char(who);

        if (p->time_flying == 0) {
            p->time_flying++;
            ilist_append(&ocean_chars, who);
        }
    }

    if (subkind(where) != sub_ocean) {
        struct entity_char *p;

        p = rp_char(who);
        if (p && p->time_flying) {
            p->time_flying = 0;
            ilist_rem_value(&ocean_chars, who);
        }
    }

#if 0
    /*
     *  Don't need new bandits "popping" up.
     *
     */
    if (loc_depth(where) == LOC_province &&
        subkind(where) != sub_ocean &&
        has_item(where, item_peasant) < 100 &&
        !in_hades(where) &&
        !in_faery(where) &&
        !in_clouds(where))
        wilderness_attack_check(who, where);
#endif
}

/*
 *  Traps
 *
 *  Encodes some common traps...
 *
 */
struct trap_struct traps[] =
        {
                {sk_eres,
                        sk_eres,
                        10,
                           50,
                        "water elemental",
                        "A water elemental eyes you warily as you arrive.",
                        "You see an odd spot of water in the ocean below.",
                        "A water elemental attacks as you enter the province!"},

                {sk_dol,
                        sk_dol,
                        10,
                           50,
                        "snake",
                        "A nest of snakes rattles menacingly as you pass by.",
                        "You spot a nest of snakes in the province below.",
                        "A nest of snakes attacks as you enter the province!"},

                {sk_anteus,
                        sk_anteus,
                        10,
                           50,
                        "boulder",
                        "A precarious pile of boulders shifts slightly as you pass by.",
                        "You spot a strange pile of boulders in the province below.",
                        "A boulder trap collapses on you as you enter the province!"},

                {sk_timeid,
                        sk_timeid,
                        10,
                           50,
                        "deadfall",
                        "A dead tree shifts slightly as you pass by.",
                        "You spot an odd-looking dead tree in the province below.",
                        "A deadfall drops on you as you enter the province!"},

                {sk_kireus,
                        sk_kireus,
                        10,
                           50,
                        "quicksand",
                        "A pool of quicksand gurgles as you pass by.",
                        "You spot a patch of odd-colored sand in the province below.",
                        "You stumble into a patch of quicksand as you enter the province!"},

                {0, 0,  0, 0, NULL, NULL, NULL, NULL}

        };

/*
 *  Thu Sep 26 13:13:24 1996 -- Scott Turner
 *
 *  Check to see if there are any effects hanging on this province that
 *  will effect the arriving stack.
 *
 */
void
check_arrival_effects(int who, int where, int flying) {
    int val, i, num;
    struct effect *e;

    vector_stack(who, TRUE);
    /*
     *  Generic Trap Check
     *
     *  If there is a generic trap on this location (ef_religion_trap), then
     *  we use the subtype as an index into traps[] to figure out exactly what
     *  should happen.
     *
     */
    // todo: cast to struct effect * from int
    loop_effects(where, e)
            {
                /*
                 *  Look for religion traps.
                 *
                 */
                if (e->type == ef_religion_trap) {
                    /*
                     *  Look up the subtype in traps[]
                     *
                     */
                    for (i = 0; traps[i].type && traps[i].type != e->subtype; i++) {}
                    /*
                     *  Find something?
                     *
                     */
                    if (traps[i].type) {
                        /*
                         *  Implement this trap.
                         *
                         */
                        if (priest_in_stack(who, traps[i].religion)) {
                            wout(VECT, traps[i].ignored);
                            continue;
                        };
                        /*
                         *  Ignored if you're flying.
                         *
                         */
                        if (flying) {
                            if ((rnd(1, 4) > 2)) {
                                wout(VECT, traps[i].flying);
                            };
                            continue;
                        };
#ifdef HERO
                        /*
                         *  Wed Nov 25 13:05:44 1998 -- Scott Turner
                         *
                         *  You might be a hero with "acute senses"
                         *
                         */
                        if (rnd(1,100) < min(80, 15 + 2*skill_exp(who, sk_acute_senses))) {
                          wout(VECT, traps[i].flying);
                          continue;
                        };
#endif // HERO

                        /*
                         *  Otherwise you get attacked.  Note that the ship
                         *  is the "who" being passed in.
                         *
                         */
                        wout(VECT, traps[i].attack);
                        do_trap_attack(who, traps[i].num_attacks, traps[i].attack_chance);
                        delete_effect(where, e->type, e->subtype);
                    };
                };
            }next_effect;
};


#if 0
int
d_move(struct command *c)
{
    struct exit_view vv;
    struct exit_view *v = &vv;

    restore_v_array(c, v);

    if (v->road)
        discover_road(c->who, subloc(c->who), v);

    vector_stack(c->who, TRUE);
    wout(VECT, "Arrival at %s.", box_name(v->destination));
    if (loc_depth(v->destination) == LOC_province &&
        viewloc(subloc(c->who)) != viewloc(v->destination) &&
        weather_here(v->destination, sub_fog))
    {
        wout(VECT, "The province is blanketed in fog.");
    }

    if (loc_depth(v->destination) == LOC_province &&
        viewloc(subloc(c->who)) != viewloc(v->destination) &&
        weather_here(v->destination, sub_mist))
    {
        wout(VECT, "The province is covered with a dank mist.");
    }

    check_arrival_effects(c->who, v->destination, 0);

    restore_stack_actions(c->who);

#if 0
/*
 *  Stackmates who are executing commands have gotten a free day.
 *  Force a one evening delay into their command completion loop.
 */

    if (v->distance > 0)
    {
        int i;

        loop_char_here(c->who, i)
        {
            struct command *nc = rp_command(i);

            if (nc->wait != 0)
                nc->move_skip = TRUE;
        }
        next_char_here;
    }
#endif

    move_stack(c->who, v->destination);

    if (viewloc(v->orig) != viewloc(v->destination))
        arrival_message(c->who, v);

    return TRUE;
}
#endif

void
init_ocean_chars() {
    int i;
    int where;

    loop_char(i)
            {
                where = subloc(i);

                if (subkind(where) == sub_ocean) {
                    ilist_append(&ocean_chars, i);
                }
            }
    next_char;
}


void
check_ocean_chars() {
    int i;
    int where;
    struct entity_char *p;
    int who;
    static ilist l = NULL;

    ilist_clear(&l);
    l = ilist_copy(ocean_chars);

    for (i = 0; i < ilist_len(l); i++) {
        who = l[i];
        where = subloc(who);
        p = p_char(who);

        if (!alive(who) || subkind(where) != sub_ocean) {
            p->time_flying = 0;
            ilist_rem_value(&ocean_chars, who);
            continue;
        }

        p->time_flying++;

        if (p->time_flying <= 15) {
            continue;
        }

        if (stack_parent(who)) {
            continue;
        }

/*
 *  Flying stack plunges into the sea.
 */

        vector_stack(who, TRUE);
        wout(VECT, "Flight can no longer be maintained.  %s "
                   "plunges into the sea.", box_name(who));

        kill_stack_ocean(who);
    }
    ilist_reclaim(&l);

}


static int
fly_check(struct command *c, struct exit_view *v) {

    return TRUE;
}


static int
can_fly_here(int where, struct command *c) {
    struct exit_view *v;

    v = parse_exit_dir(c, where, NULL);

    if (v && v->direction != DIR_IN && move_exit_fly(c, v, FALSE) >= 0) {
        return TRUE;
    }

    return FALSE;
}


static int
can_fly_at_outer_level(int where, struct command *c) {
    int outer;

    outer = subloc(where);
    while (loc_depth(outer) > LOC_region) {
        if (can_fly_here(outer, c)) {
            return loc_depth(outer) - loc_depth(where);
        }
        outer = subloc(outer);
    }

    return 0;
}

/*
 *  Wed Mar 26 08:21:51 1997 -- Scott Turner
 *
 *  Note that flying avoids all control -- closed borders and/or fees.
 *
 */
int
v_fly(struct command *c) {
    struct exit_view *v;
    int delay;
    int where = subloc(c->who);
    int check_outer = TRUE;
    int prepend = 0;

    if (numargs(c) < 1) {
        wout(c->who, "Specify direction or destination to FLY.");
        return FALSE;
    }

    while (numargs(c) > 0) {
        v = parse_exit_dir(c, where, "fly");

        if (v) {
            check_outer = FALSE;

            if (fly_check(c, v)) {
                break;
            }
            v = NULL;
        }

        cmd_shift(c);
    }

    if (v == NULL && check_outer && can_fly_at_outer_level(where, c)) {
        c->a = subloc(where);
        v = parse_exit_dir(c, where, NULL);
        assert(v);

        if (v) {
            assert(move_exit_fly(c, v, FALSE) >= 0);
            wout(c->who, "(assuming 'fly out' first)");
            prepend = 1;
        }
    }

    if (v == NULL) {
        return FALSE;
    }

    delay = move_exit_fly(c, v, TRUE);

    if (delay < 0) {
        return FALSE;
    }

    /*
     *  Check and/or join guilds.
     *
     */
    if (!check_guilds(c, v)) { return FALSE; }

    v->distance = delay;
    c->wait = delay;

    save_v_array(c, v);
    leave_stack(c->who);

    if (delay > 0) {
        vector_stack(c->who, TRUE);
        wout(VECT, "Flying to %s will take %s day%s.",
             box_name(v->destination),
             nice_num(delay),
             delay == 1 ? "" : "s");
    }

    suspend_stack_actions(c->who);
    clear_guard_flag(c->who);

    departure_message(c->who, v);

    if (prepend) { prepend_order(player(c->who), c->who, c->line); }
    return TRUE;
}

int
d_fly(struct command *c) {
    struct exit_view vv;
    struct exit_view *v = &vv;
    struct weights w;

    restore_v_array(c, v);
    restore_stack_actions(c->who);
    /*
     *  Can he still fly?
     *
     */
    determine_stack_weights(c->who, &w, FALSE);

    if (w.fly_cap < w.fly_weight) {
        wout(c->who, "%s is too overloaded to fly.",
             box_name(c->who));
        wout(c->who, "You have a total of %d weight, and your "
                     "maximum flying capacity is %d.", w.fly_weight, w.fly_cap);
        return FALSE;
    };

    if (c->wait != 0) { return TRUE; }


    if (v->road) {
        discover_road(c->who, subloc(c->who), v);
    }

    vector_stack(c->who, TRUE);
    wout(VECT, "Arrival at %s.", box_name(v->destination));

    if (loc_depth(v->destination) == LOC_province &&
        viewloc(subloc(c->who)) != viewloc(v->destination) &&
        weather_here(v->destination, sub_fog)) {
        wout(VECT, "The province is blanketed in fog.");
    }

    if (loc_depth(v->destination) == LOC_province &&
        viewloc(subloc(c->who)) != viewloc(v->destination) &&
        weather_here(v->destination, sub_mist)) {
        wout(VECT, "The province is covered with a dank mist.");
    }

    check_arrival_effects(c->who, v->destination, 1);


#if 0
    /*
     *  Stackmates who are executing commands have gotten a free day.
     *  Force a one evening delay into their command completion loop.
     */

        if (v->distance > 0)
        {
            int i;

            loop_char_here(c->who, i)
            {
                struct command *nc = rp_command(i);

                if (nc->wait != 0)
                    nc->move_skip = TRUE;
            }
            next_char_here;
        }
#endif

    move_stack(c->who, v->destination);
    arrival_message(c->who, v);

    return TRUE;
}


/*
 *  Synonym for 'move out'
 */

int
v_exit(struct command *c) {
    int ret;

    ret = oly_parse(c, "move out");
    assert(ret);

    return v_move_attack(c);
}


int
v_enter(struct command *c) {
    int ret;

    if (numargs(c) < 1) {
        ret = oly_parse(c, "move in");
        assert(ret);
        return v_move_attack(c);
    }

    ret = oly_parse(c, sout("move %s", c->parse[1]));
    assert(ret);

    return v_move_attack(c);
}


int
v_north(struct command *c) {
    int ret;

    ret = oly_parse(c, "move north");
    assert(ret);

    return v_move_attack(c);
}


int
v_south(struct command *c) {
    int ret;

    ret = oly_parse(c, "move south");
    assert(ret);

    return v_move_attack(c);
}


int
v_east(struct command *c) {
    int ret;

    ret = oly_parse(c, "move east");
    assert(ret);

    return v_move_attack(c);
}


int
v_west(struct command *c) {
    int ret;

    ret = oly_parse(c, "move west");
    assert(ret);

    return v_move_attack(c);
}


void
check_captain_loses_sailors(int qty, int target, int inform) {
    static int cmd_sail = -1;
    int where = subloc(target);
    struct command *c;
    int hands_short = 0;
    int before, now;
    int should_have;
    int penalty;

/* This is broken */
    return;

    if (cmd_sail < 0) {
        cmd_sail = find_command("sail");
        assert(cmd_sail > 0);
    }

    if (!is_ship(subloc(target))) {
        return;
    }

    c = rp_command(target);

    if (c == NULL || c->state != RUN || c->cmd != cmd_sail) {
        return;
    }

    now = has_item(target, item_sailor) + has_item(target, item_pirate);
    before = now + qty;

    switch (subkind(where)) {
        case sub_galley:
            should_have = 14;
            break;

        case sub_roundship:
            should_have = 8;
            break;

        default:
            fprintf(stderr, "kind is %d\n", subkind(where));
            assert(FALSE);
    }

    if (now >= should_have) {
        return;
    }        /* still have enough sailors */

    if (before > should_have)
        before = should_have;

    penalty = before - now;

    assert(penalty > 0);

    vector_clear();
    vector_add(target);
    if (inform && target != inform)
        vector_add(inform);

    if (penalty == 1)
        wout(VECT, "Loss of crew will cause travel to take an extra day.");
    else
        wout(VECT, "Loss of crew will cause travel to take %s extra days.",
             nice_num(penalty));

    assert(c->wait > 0);
    c->wait += penalty;

    log_output(LOG_SPECIAL, "Loss of sailors incurs penalty for %s.",
               box_code(player(target)));
}


static int
move_exit_water(struct command *c, struct exit_view *v, int ship, int show) {
    int delay = v->distance;
    int hands_short = 0;        /* how many hands we are short */
    int n;
    char *s;
    int sail_time = v->distance, row_time = v->distance;
    int wind_bonus = 0;
    int where = subloc(ship);
    struct entity_ship *sp = rp_ship(ship);
    int ports, sails;

    switch (subkind(ship)) {

        case sub_roundship:
            n = has_item(c->who, item_sailor) + has_item(c->who, item_pirate);
            if (n < 8) {
                hands_short = 8 - n;

                if (hands_short == 1) { s = "day"; }
                else { s = sout("%s days", nice_num(hands_short)); }

                if (show) {
                    wout(c->who, "The crew of a roundship is eight "
                                 "sailors, but you have %s.  Travel will "
                                 "take an extra %s.",
                         n == 0 ? "none" : nice_num(n), s);
                }

                if (weather_here(where, sub_wind) &&
                    delay > 1) {
                    wind_bonus = 1;
                    if (show) {
                        wout(c->who, "Favorable winds speed our progress.");
                    }
                };
                delay = delay + hands_short - wind_bonus;
            }
            break;

        case sub_galley:
            n = has_item(c->who, item_sailor) + has_item(c->who, item_pirate);
            /* n += has_item(c->who, item_slave); */

            if (n < 14) {
                hands_short = 14 - n;

                if (hands_short == 1) { s = "day"; }
                else { s = sout("%s days", nice_num(hands_short)); }

                if (show) {
                    wout(c->who, "The crew of a galley is fourteen "
                                 "pirates or sailors, but you have %s.  "
                                 "Travel will take an extra %s.",
                         n == 0 ? "none" : nice_num(n), s);
                }
            }
            delay = delay + hands_short - wind_bonus;
            break;

            /*
             *  Fri Jan  3 11:54:06 1997 -- Scott Turner
             *
             *  For the general-purpose ship, we calculate a row time and a
             *  sail time and then use the lesser of the two.
             *
             */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
        case sub_ship:
            if (!sp) { return sail_time; }
            /*
             *  Sailors available.
             *
             */
            n = has_item(c->who, item_sailor) + has_item(c->who, item_pirate)
                - MAX((sp->hulls / 20) * 2, 2);
            /*
             *  Can we row?
             *
             */


            /*
             *  How many manned ports do we have?
             *
             */
            ports = MIN(sp->ports, n / ROWERS_PER_PORT);

            /*
             *  Do we have more or fewer ports than we need?
             *
             */
            if ((ports * ROWERS_PER_PORT) < sp->hulls) {
                /*
                 *  Need at least one oarsmen per unit hull.
                 *
                 */
                row_time = 1000;
            } else if (ports > sp->hulls) {
                /*
                 *  Save one day per each multiple of sp->hulls over 1
                 *
                 */
                row_time -= (ports / sp->hulls) - 1;
            } else if (ports < sp->hulls) {
                row_time += sp->hulls - ports;
            };

            if (row_time < 1) { row_time = 1; }
            /*
             *  Can we sail?
             *
             */

            /*
             *  How many sails available?
             *
             */
            sails = MIN(sp->sails, n);

            /*
             *  Do we have more or fewer sails than we need?
             *
             */
            if (sails < sp->hulls / 2) {
                /*
                 *  Need at least 1 sail per 2 hull to move at all.
                 *
                 */
                sail_time = 1000;
            } else if (sails > 2 * sp->hulls) {
                /*
                 *  Save one day for each full sail per unit hull
                 *
                 */
                sail_time -= ((int) ((sails - 2 * sp->hulls) / sp->hulls)) * 2;
            } else if (sails < 2 * sp->hulls) {
                sail_time += 2;
            };

            /*
             *  Wind bonus
             *
             */
            if (weather_here(where, sub_wind) && sail_time > 1 && sail_time != 1000) {
                sail_time--;
                if (show) { wout(c->who, "Favorable winds speed our progress."); }
            };
            if (sail_time < 1) { sail_time = 1; }

            /*
             *  Which is it?
             *
             */
            if (sail_time == 1000 && row_time == 1000) {
                if (show) {
                    wout(c->who, "The ship is unable to be rowed or sailed at this time.");
                }
                return -1;
            } else if (sail_time < row_time) {
                if (show) { wout(c->who, "The sails are unfurled."); }
                return sail_time;
            } else {
                if (show) { wout(c->who, "The rowers dip their oars to the cadence of the drums."); }
                return row_time;
            };
            break;

        default:
            fprintf(stderr, "subkind = %d\n", subkind(ship));
            assert(FALSE);
    }

    return delay;
}


static void
sail_depart_message(int ship, struct exit_view *v) {
    char *to = "";
    char *desc;
    char *comma = "";

    desc = liner_desc(ship);

    if (v->dest_hidden == FALSE) {
        to = sout(" for %s.", box_name(v->destination));
    }

    if (strchr(desc, ',')) {
        comma = ",";
    }

    wout(v->orig, "%s%s departed%s", desc, comma, to);
}


static void
sail_arrive_message(int ship, struct exit_view *v) {
    char *from = "";
    char *desc;
    char *comma = "";
    char *with;

    desc = liner_desc(ship);

    if (v->orig_hidden == FALSE) {
        from = sout(" from %s", box_name(v->orig));
    }

    if (strchr(desc, ',')) {
        comma = ",";
    }

    with = display_owner(ship);

    if (!*with) {
        with = ".";
    }

    show_to_garrison = TRUE;

    wout(v->destination, "%s%s arrived%s%s", desc, comma, from, with);
    show_owner_stack(v->destination, ship);

    show_to_garrison = FALSE;
}


static int
sail_check(struct command *c, struct exit_view *v, int show) {

    if (!v->water) {
        if (show) {
            wout(c->who,
                 "There is no water route in that direction.");
        }
        return FALSE;
    }

    if (v->impassable) {
        if (show) {
            wout(c->who, "That route is impassable.");
        }
        return FALSE;
    }

    return TRUE;
}


static int
can_sail_here(int where, struct command *c, int ship) {
    struct exit_view *v;

    v = parse_exit_dir(c, where, NULL);

    if (v && v->direction != DIR_IN &&
        sail_check(c, v, FALSE) &&
        move_exit_water(c, v, ship, FALSE) >= 0) {
        return TRUE;
    }

    return FALSE;
}


static int
can_sail_at_outer_level(int ship, int where, struct command *c) {
    int outer;

    if (ship_cap(ship)) {
        int loaded;

        loaded = ship_weight(ship) * 100 / ship_cap(ship);

        if (loaded > 100) {
            wout(c->who, "%s is too overloaded to sail.",
                 box_name(ship));
            return FALSE;
        }
    }

    outer = subloc(where);
    while (loc_depth(outer) > LOC_region) {
        if (can_sail_here(outer, c, ship)) {
            return loc_depth(outer) - loc_depth(where);
        }
        outer = subloc(outer);
    }

    return 0;
}


int
v_sail(struct command *c) {
    struct exit_view *v;
    int delay;
    int ship = subloc(c->who);
    struct entity_ship *s = rp_ship(ship);
    int outer_loc, rocky_coast;
    int check_outer = TRUE;
    int result;

    if (!is_ship(ship)) {
        if (is_ship_notdone(ship)) {
            wout(c->who, "%s is not yet completed.",
                 box_name(ship));
        } else {
            wout(c->who, "Must be on a sea-worthy ship to sail.");
        }
        return FALSE;
    }

    if (building_owner(ship) != c->who) {
        wout(c->who, "Only the captain of a ship may sail.");
        return FALSE;
    }

    if (has_skill(c->who, sk_pilot_ship) <= 0) {
        wout(c->who, "Knowledge of %s is required to sail.",
             box_name(sk_pilot_ship));
        return FALSE;
    }

    if (numargs(c) < 1) {
        wout(c->who, "Specify direction or destination to sail.");
        return FALSE;
    }

    outer_loc = subloc(ship);
    /*
     *  Wed Jan  8 12:07:09 1997 -- Scott Turner
     *
     *  We have to do this *before* the parse_exit_dir below, because
     *  they both use a static local in exits_from_loc, and since v is
     *  saved here, bad things happen...
     *
     */
    rocky_coast = near_rocky_coast(outer_loc);
    while (numargs(c) > 0) {
        v = parse_exit_dir(c, outer_loc, "sail");

        if (v) {
            check_outer = FALSE;

            if (sail_check(c, v, TRUE)) {
                break;
            }
            v = NULL;
        }

        cmd_shift(c);
    }

    if (v == NULL && check_outer &&
        can_sail_at_outer_level(ship, outer_loc, c)) {
        c->a = subloc(outer_loc);
        v = parse_exit_dir(c, outer_loc, NULL);
        assert(v);

        if (v) {
            assert(move_exit_water(c, v, ship, FALSE) >= 0);
            wout(c->who, "(assuming 'sail out' first)");
            prepend_order(player(c->who), c->who, c->line);
        }
    }

    if (v == NULL) {
        wout(c->who, "No valid arguments to sail.");
        return FALSE;
    };

    if (ship_cap(ship)) {
        int loaded;

        loaded = ship_weight(ship) * 100 / ship_cap(ship);

        if (loaded > 100) {
            wout(c->who, "%s is too overloaded to sail.",
                 box_name(ship));
            return FALSE;
        }
    }

    assert(!v->in_transit);

    delay = move_exit_water(c, v, ship, TRUE);

    if (delay < 0) {
        return FALSE;
    }

    /*
     *  Tue Jan  7 15:28:02 1997 -- Scott Turner
     *
     *  Possibility of problems if you're sailing out of a deep
     *  water province and you don't have Deep Sea Navigation.
     *
     *  Tue Dec 15 08:29:33 1998 -- Scott Turner
     *
     *  The Pen don't get lost.
     *
     */
    if (rocky_coast == 3 &&
        has_skill(c->who, sk_deep_sea) <= 0 &&
        !(nation(first_character(ship)) &&
          strncmp(rp_nation(nation(first_character(ship)))->name, "Pen", 3) == 0) &&
        !has_artifact(c->who, ART_SAFE_SEA, 0, 0, 0) &&
        rnd(1, 3) == 1) {
        wout(c->who, "The lack of landmarks to sail by confuses you.");
        result = rnd(1, 100) - (priest_in_stack(c->who, sk_eres) * 10);
        if (result <= 40) {
            int tmp = rnd(3, 10);
            wout(c->who, "It takes %s days to reorient yourself.",
                 nice_num(tmp));
            delay += tmp;
        } else if (result <= 80) {
            /*
             *  Lost; so move like an NPC (randomly).
             *
             */
            struct exit_view **choices =
                    exits_from_loc_nsew_select(c->who, outer_loc, WATER, 1);

            if (ev_list_len(choices)) {
                if (choices[0]->destination != v->destination) {
                    wout(c->who, "Somehow you sail off in an unintended direction.");
                    v = choices[0];
                    delay = move_exit_water(c, v, ship, TRUE);
                };
            };
        } else if (result <= 95) {
            wout(c->who, "You sail into rough seas that damage the ship.");
            add_structure_damage(ship, rnd(1, 20));
        } else {
            wout(c->who, "In your confusion you somehow swamp the ship!");
            add_structure_damage(ship, 100);
        };
    };

    /*
     *  Wed Jan  8 12:08:48 1997 -- Scott Turner
     *
     *  Ship might have been destroyed!
     *
     */
    if (!valid_box(ship)) { return FALSE; }

    c->wait = delay;
    v->distance = delay;

    save_v_array(c, v);

    if (delay > 0) {
        vector_char_here(c->who);
        vector_add(c->who);

        wout(VECT, "Sailing to %s will take %s day%s.",
             box_name(v->destination),
             nice_num(delay),
             delay == 1 ? "" : "s");
    }

    sail_depart_message(ship, v);

/*
 *  Mark the in_transit field with the daystamp of the beginning
 *  of the voyage
 */

    p_subloc(ship)->moving = sysclock.days_since_epoch;

    if (ferry_horn(ship))        /* clear ferry horn signal */
        p_magic(ship)->ferry_flag = 0;

    return TRUE;
}

/*
 *  Tue Oct  6 18:01:38 1998 -- Scott Turner
 *
 *  This now must be polled for deep sea effects!
 *
 */
int
d_sail(struct command *c) {
    int ship = subloc(c->who);
    struct exit_view vv;
    struct exit_view *v = &vv;

    assert(is_ship(ship));            /* still a ship */
    assert(building_owner(ship) == c->who);    /* still captain */

    restore_v_array(c, v);

    if (v->road) {
        discover_road(c->who, subloc(ship), v);
    }

    vector_char_here(ship);
    wout(VECT, "Arrival at %s.", box_name(v->destination));

    if (loc_depth(v->destination) == LOC_province &&
        viewloc(subloc(ship)) != viewloc(v->destination) &&
        weather_here(v->destination, sub_fog)) {
        wout(VECT, "The province is blanketed in fog.");
    }

    if (loc_depth(v->destination) == LOC_province &&
        viewloc(subloc(ship)) != viewloc(v->destination) &&
        weather_here(v->destination, sub_mist)) {
        wout(VECT, "The province is covered in a dank mist.");
    }

    p_subloc(ship)->moving = 0;        /* no longer moving */
    set_where(ship, v->destination);
    mark_loc_stack_known(ship, v->destination);
#if 0
    move_bound_storms(ship, v->destination);
#endif
    if (ferry_horn(ship))        /* clear ferry horn signal */
        p_magic(ship)->ferry_flag = 0;

    touch_loc_after_move(ship, v->destination);
    check_arrival_effects(ship, v->destination, 0);
    sail_arrive_message(ship, v);

    if (c->use_skill == 0) {
        add_skill_experience(c->who, sk_pilot_ship);
    }

    ship_storm_check(ship);  /* Might destroy ship? */

    return TRUE;
}


/*
 *  If sailing is interrupted, we must zero subloc.moving
 *  to indicate that the ship is no longer in transit.
 */

int i_sail(struct command *c) {
    int ship = subloc(c->who);

    assert(is_ship(ship));

    p_subloc(ship)->moving = 0;

    if (ferry_horn(ship))        /* clear ferry horn signal */
        p_magic(ship)->ferry_flag = 0;

    return TRUE;
}

/*
 *  Thu Apr  3 19:42:14 1997 -- Scott Turner
 *
 *  This version doesn't look for locations.
 *
 */
static int
select_target_local(struct command *c) {
    int target = c->a;
    int where = subloc(c->who);

    if (kind(target) == T_char) {
        if (!check_char_here(c->who, target)) {
            return 0;
        }

        if (loc_depth(where) == LOC_province &&
            weather_here(where, sub_fog) &&
            !contacted(target, c->who)) {
            wout(c->who, "That target is not visible in the fog.");
            return 0;
        };

        if (is_prisoner(target)) {
            wout(c->who, "Cannot attack prisoners.");
            return 0;
        }

        if (c->who == target) {
            wout(c->who, "Can't attack oneself.");
            return 0;
        }

        if (stack_leader(c->who) == stack_leader(target)) {
            wout(c->who, "Can't attack a member of the same stack.");
            return 0;
        }

        return stack_leader(target);
    }

    return 0;
}

static
int attack_okay(struct command *c, int target) {
    int n, attacker, targ_who;

    if (stack_leader(c->who) != c->who) {
        wout(c->who, "Only the stack leader may initiate combat.");
        return FALSE;
    }

    if (target <= 0) {
        wout(c->who, "You must specify a target to attack.");
        return FALSE;
    }

    attacker = select_attacker(c->who, target);
    if (attacker <= 0) {
        wout(c->who, "You must specify a target to attack.");
        return FALSE;
    };

    if (is_loc_or_ship(target)) {
        targ_who = loc_target(target);
    } else {
        targ_who = target;
    };

    /*
     *  Target should be a character?
     *
     */
    if (kind(targ_who) != T_char) {
        wout(c->who, "Nothing there to attack.");
        return FALSE;
    };

    if (!is_real_npc(c->who) && player(c->who) == player(targ_who)) {
        wout(c->who, "Units in the same faction may not engage "
                     "in combat.");
        return FALSE;
    };

    /*
     *  Could be gone?
     *
     *  Mon Oct  5 18:58:37 1998 -- Scott Turner
     *
     *  Note difference from char_gone!
     *
     */
#if 0
    if (!is_loc_or_ship(target) && char_gone(target)) {
        wout(c->who, "%s has already left.",box_name(target));
        return FALSE;
      };

    if (is_ship(target) && ship_gone(target)) {
        wout(c->who, "The ship slips away before you can attack it!");
        return FALSE;
      };

    if (subloc(c->who) != subloc(target)) {
        wout(c->who, "%s is not here.",box_name(target));
        return FALSE;
    };
#endif

    if (char_really_hidden(target) && !contacted(target, c->who)) {
        wout(c->who, "%s is not here.", box_name(target));
        return FALSE;
    };

    if (is_prisoner(targ_who)) {
        wout(c->who, "Cannot attack prisoners.");
        return 0;
    }

    if (c->who == targ_who) {
        wout(c->who, "Can't attack oneself.");
        return 0;
    }

    if (stack_leader(c->who) == stack_leader(targ_who)) {
        wout(c->who, "Can't attack a member of the same stack.");
        return 0;
    }

    /*
     *  Can't attack because we need an item?
     *
     */
    n = only_defeatable(targ_who);
    if (n && !has_item(c->who, n)) {
        wout(c->who, "To defeat %s you need %s.", box_name(targ_who),
             box_name(n));
        return FALSE;
    };

    /*
     *  If the destination is a subloc with an entrance size,
     *  then we'd better be that size.
     *
     */
    if (entrance_size(target)) {
        /*
         *  Use the special count function to also count ninjas
         *  and angels.
         *
         */
        int n = count_stack_any_real(c->who, FALSE, FALSE);
        if (n > entrance_size(target)) {
            wout(c->who, "The entrance will pass only %s at a time.",
                 nice_num(entrance_size(target)));
            return FALSE;
        };
    };

    return TRUE;
};

/*
 *  Tue Apr  1 12:22:42 1997 -- Scott Turner
 *
 *  So far doesn't have
 *     - "no seize" flag for attack use
 *     - non-location targets
 *
 *  Fri Dec  3 06:10:20 1999 -- Scott Turner
 *
 *  Need to add in restrictions for entrance size.  We need to add
 *  a check in move_permitted (which will stop peaceful moves through
 *  too small an opening) and in attack_okay (which will stop attacks
 *  through too small an opening).
 *
 *  Wed Nov  1 13:48:40 2000 -- Scott Turner
 *
 *  Fold in flying.
 *
 */
int
v_move_attack(struct command *c) {
    struct exit_view *v, tmp_v;
    int delay;
    int where = subloc(c->who);
    int check_outer = TRUE;
    int prepend = 0;
    int attack = 0, target = 0, seize = 0, fly = 0;

    /*
     *  Check for the flag that says not to seize the slot.
     *
     */
    seize = 1;
    if (numargs(c) && atoi(c->parse[numargs(c)]) == 1) {
        seize = 0;
    };

    if (numargs(c) < 1) {
        wout(c->who, "Specify direction or destination to %s.",
             c->parse[0]);
        return FALSE;
    }

    if (!strcasecmp(c->parse[0], "attack")) {
        attack = 1;
    }

    if (!strcasecmp(c->parse[0], "fly")) {
        fly = 1;
    }

    while (numargs(c) > 0) {
        v = parse_exit_dir(c, where, c->parse[0]);

        if (v) {
            check_outer = FALSE;
            /*
             *  Accept this move if:
             *
             *  (1) we can get there overland.
             *  (2) we're attacking OR
             *      we're peaceful and this move is permitted.
             *
             *  In d_move, we'll avoid making an unpermitted move after
             *  an attack.  So we can attack into somewhere we can't move,
             *  we just can't follow up by occupying that location.
             *
             *  Wed Nov  1 13:51:53 2000 -- Scott Turner
             *
             *  Updated for flying.
             */
            if ((fly && fly_check(c, v)) ||
                (!fly && land_check(c, v, TRUE)) &&
                (attack || (move_permitted(c, v) && check_peaceful_move(c, v)))) {
                break;
            }
            v = NULL;
        }

        /*
         *  Maybe he's trying to attack someone here?
         *
         */
        target = select_target_local(c);
        if (target) {
            /*
             *  V is only needed till the end of the routine.
             *
             */
            v = &tmp_v;
            bzero(&tmp_v, sizeof(tmp_v));
            v->destination = target;
            v->distance = 0;
        };

        cmd_shift(c);
    }

    if (v == NULL) {
        if (attack) {
            wout(c->who, "No target found!", c->parse[0]);
        } else {
            wout(c->who, "No valid arguments to %s.", c->parse[0]);
        }
        return FALSE;
    };

    v->seize = seize;  /* Hacky place to keep it :-) */

    vector_stack(c->who, TRUE);

    /*
     *  If v->distance = 0, this returns 0, so it works for the phony
     *  attack exit_view.
     *
     */
    delay = move_exit_land(c, v, TRUE);

    if (delay < 0) {
        return FALSE;
    }

    /*
     *  An attack costs 1 day to prepare.
     *
     */
    if (attack) { ++delay; }

    /*
     *  Attack might not be okay?
     *
     */
    if (attack && !attack_okay(c, v->destination)) {
        /*
         *  In that case, let's attempt this as a peaceful move.
         *
         *  Tue Sep 21 12:54:48 1999 -- Scott Turner
         *
         *  No, let's fail.  The player can always get this functionality
         *  with an attack/move comibination.
         */
        return FALSE;
    };

    /*
     *  Actual movement; encapsulated so it can also be
     *  called from v_attack.
     *
     */
    do_actual_move(c, v, delay);

    return TRUE;
}

int
d_move_attack(struct command *c) {
    struct exit_view vv;
    struct exit_view tmp_v;
    struct exit_view *v = &vv;
    int attack = 0, attacker, target, result = 0;
    int delay = 0;

    if (!strcasecmp(c->parse[0], "attack")) {
        attack = 1;
    }

    restore_v_array(c, v);
    restore_stack_actions(c->who);

    if (!valid_box(v->destination)) {
        wout(c->who, "Your destination no longer exists!");
        return FALSE;
    };

    /*
     *  Maybe you are now overloaded?
     *
     */
    delay = move_exit_land(c, v, FALSE);

    if (delay < 0) {
        return FALSE;
    }

    /*
     *  Check for a forced march.
     *
     */
    if (v->forced_march == FORCED_MARCH) {
        wout(c->who, "Forced march costs %s health point%s.",
             nice_num(c->f),
             c->f == 1 ? "" : "s");
        add_char_damage(c->who, c->f, MATES);
    } else if (v->forced_march == FORCED_RIDE) {
        if (rnd(1, 100) < (c->f * 20)) {
            kill_random_mount(c->who);
        };
    };

    /*
     *  Remove the effect if it should have been used.
     *
     */
    if (c->f > 0) { delete_effect(c->who, ef_forced_march, 0); }

    if (attack) {
        /*
         *  Attack might not be okay?
         *
         *  Tue Sep 21 12:45:31 1999 -- Scott Turner
         *
         *  If the attack is "not okay", then you should just automatically
         *  win the battle, since what this means is that while you were moving
         *  the enemy did something to make your attack irrelevant -- like
         *  ungarrisoning the province.
         *
         */
        target = v->destination;
        if (!target) {
            wout(c->who, "You didn't specify anyone to attack.");
            return FALSE;
        };

        /*
         *  If the attack is okay, then go ahead and run the battle.  If
         *  it isn't okay, then you "win".
         *
         */
        if (attack_okay(c, target)) {
            attacker = select_attacker(c->who, target);
            result = regular_combat(attacker, target, v->seize, c->who);
            if (!result) { return FALSE; }
        };

        /*
         *  Seize the slot?
         */
        if (!v->seize) { return TRUE; }
    };

    if (!is_loc_or_ship(v->destination)) {
        return TRUE;
    };

    if (!move_permitted(c, v)) {
        wout(c->who, "You cannot move into that location.");
        return FALSE;
    };

    if (v->road) {
        discover_road(c->who, subloc(c->who), v);
    }

    vector_stack(c->who, TRUE);
    wout(VECT, "Arrival at %s.", box_name(v->destination));
    if (loc_depth(v->destination) == LOC_province &&
        viewloc(subloc(c->who)) != viewloc(v->destination) &&
        weather_here(v->destination, sub_fog)) {
        wout(VECT, "The province is blanketed in fog.");
    }

    if (loc_depth(v->destination) == LOC_province &&
        viewloc(subloc(c->who)) != viewloc(v->destination) &&
        weather_here(v->destination, sub_mist)) {
        wout(VECT, "The province is covered with a dank mist.");
    }

    check_arrival_effects(c->who, v->destination, 0);

    move_stack(c->who, v->destination);

    if (attack && result == A_WON) { promote(c->who, 0); }

    if (viewloc(v->orig) != viewloc(v->destination)) {
        arrival_message(c->who, v);
    }

    return TRUE;
}

int
v_maxpay(struct command *c) {
    int m = c->a;

    if (!rp_char(c->who)) {
        wout(c->who, "Oddly enough, you cannot use the maxpay command.");
        return FALSE;
    };

    rp_char(c->who)->pay = m;
    wout(c->who, "Maximum amount paid to enter a location set to %s.",
         nice_num(m));

    return TRUE;
};
