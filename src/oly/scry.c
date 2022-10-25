// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


void
scry_show_where(int who, int target) {

    if (crosses_ocean(who, target)) {
        out(who, "%s is in %s.", just_name(region(target)));
        return;
    }

    out(who, "%s is in %s.", box_name(province(target)));
}


int
cast_where(int who) {
    int where;

    where = char_proj_cast(who);

    if (is_loc_or_ship(where)) {
        return where;
    }

    return subloc(who);
}


int
reset_cast_where(int who) {
    int where;

    where = char_proj_cast(who);

    if (is_loc_or_ship(where)) {
        p_magic(who)->project_cast = 0;
        return where;
    }

    return subloc(who);
}


int
cast_check_char_here(int who, int target) {
    int where;
    int basic;
    int pl;

    basic = char_proj_cast(who);

    if (is_loc_or_ship(basic)) {
        where = basic;
    } else {
        where = subloc(who);
    }

#if 1
    p_magic(who)->project_cast = 0;
#endif

    if (crosses_ocean(where, who)) {
        wout(who, "Something seems to block your magic.");
        return FALSE;
    };

    if (kind(target) != T_char || where != subloc(target)) {
        wout(who, "%s is not a character in range of this cast.",
             box_code(target));
        return FALSE;
    }

    if (char_really_hidden(target)) {
        pl = player(who);
        if (pl == player(target)) {
            return TRUE;
        }

        if (contacted(target, who)) {
            return TRUE;
        }

        return FALSE;
    }

#if 0
    if (basic == where)
        p_magic(who)->project_cast = 0;
#endif

    return TRUE;
}


int
v_scry_region(struct command *c) {
    int targ_loc = c->a;
    int aura = c->b;

    if (!is_loc_or_ship(targ_loc)) {
        wout(c->who, "%s is not a location.", box_code(targ_loc));
        return FALSE;
    }

    if (crosses_ocean(targ_loc, c->who)) {
        wout(c->who, "Something seems to block your magic.");
        return FALSE;
    };

    if (c->b < 1) {
        c->b = 1;
    }
    aura = c->b;

    if (!check_aura(c->who, aura)) {
        return FALSE;
    }

    return TRUE;
}


void
alert_scry_attempt(int who, int where, char *t) {
    int n;
    int has_detect;
    char *source;

    loop_char_here(where, n)
            {
                has_detect = has_skill(n, sk_detect_scry);

                if (has_detect > exp_novice) {
                    source = box_name(who);
                } else {
                    source = "Someone";
                }

                if (has_detect) {
                    wout(n, "%s%s cast %s on this location.",
                         source, t, box_name(sk_scry_region));
                }

                if (has_detect >= exp_master) {
                    wout(n, "%s is in %s.", box_name(who),
                         char_rep_location(who));
                }
/*
 *  If another exp gradient is wanted, use box_name for the loc,
 *  then graduate to char_rep_location, since the latter gives
 *  more info
 */

            }
    next_char_here;
}


void
alert_palantir_scry(int who, int where) {
    int n;
    int has_detect;

    loop_char_here(where, n)
            {
                has_detect = has_skill(n, sk_detect_scry);

                if (has_detect < exp_master) {
                    continue;
                }

                wout(n, "%s used a palantir to scry this location.",
                     box_name(who));

                if (has_detect > exp_master) {
                    wout(n, "%s is in %s.", box_name(who),
                         char_rep_location(who));
                }
            }
    next_char_here;
}


void
alert_scry_generic(int who, int where) {
    int n;
    int has_detect;

    loop_char_here(where, n)
            {
                has_detect = has_skill(n, sk_detect_scry);

                if (has_detect) {
                    wout(n, "%s scried %s from %s.",
                         box_name(who),
                         box_name(where),
                         char_rep_location(who));
                }
            }
    next_char_here;
}

static int
check_shield_artifact(int where) {
    int n;

    loop_all_here(where, n)
            {
                if (has_artifact(n, ART_SHIELD_PROV, 0, 0, 0)) { return TRUE; }
            }next_all_here;

    if (has_artifact(where, ART_SHIELD_PROV, 0, 0, 0)) { return TRUE; }

    return FALSE;
}

int
d_scry_region(struct command *c) {
    int targ_loc = c->a;
    int aura = c->b;

    if (!is_loc_or_ship(targ_loc)) {
        wout(c->who, "%s is no longer a valid location.",
             box_code(targ_loc));
        return FALSE;
    }

    if (crosses_ocean(targ_loc, c->who)) {
        wout(c->who, "Something seems to block your magic.");
        return FALSE;
    };

    if (!charge_aura(c->who, aura)) {
        return FALSE;
    }

    if (aura <= loc_shroud(province(targ_loc))) {
        wout(c->who, "%s is shrouded from your scry.",
             box_code(targ_loc));

        alert_scry_attempt(c->who, targ_loc, " unsuccessfully");

        return FALSE;
    }

    /*
     *  Sun Oct 11 18:26:34 1998 -- Scott Turner
     *
     *  Might be an artifact somewhere in the province.
     *
     */
    if (check_shield_artifact(targ_loc)) {
        wout(c->who, "A grey mist blocks your vision.");
        alert_scry_attempt(c->who, targ_loc, " unsuccessfully");
        return FALSE;
    };

    wout(c->who, "A vision of %s appears:", box_name(targ_loc));
    out(c->who, "");
    show_loc(c->who, targ_loc);

    alert_scry_attempt(c->who, targ_loc, "");

    return TRUE;
}


int
v_shroud_region(struct command *c) {
    int aura;
    int where = province(cast_where(c->who));

    if (crosses_ocean(where, c->who)) {
        wout(c->who, "Something seems to block your magic.");
        return FALSE;
    };

    if (c->a < 1) {
        c->a = 1;
    }
    aura = c->a;

    if (!has_piety(c->who, aura)) {
        wout(c->who, "You do not have that much piety.");
        return FALSE;
    };

    wout(c->who, "Attempt to create a magical shroud to conceal "
                 "%s from scry attempts.", box_code(where));

    reset_cast_where(c->who);
    c->b = where;

    return TRUE;
}


int
d_shroud_region(struct command *c) {
    int aura = c->a;
    struct entity_loc *p;
    int where = c->b;

    if (!use_piety(c->who, aura)) {
        wout(c->who, "You do not have that much piety.");
        return FALSE;
    };

    p = p_loc(where);
    p->shroud += aura * 2;

    wout(c->who, "%s is now cloaked with a strength %s location shroud.",
         box_name(where), nice_num(p->shroud));

    {
        int n;

        loop_char_here(where, n)
                {
                    int exp;

                    if (n == c->who) {
                        continue;
                    }

                    if (has_skill(n, sk_shroud_region)) {
                        wout(n, "%s cast %s here.  %s is now cloaked "
                                "with a strength %s location shroud.",
                             box_name(c->who),
                             box_name(sk_shroud_region),
                             box_code(where),
                             nice_num(p->shroud));
                    }
                }
        next_char_here;
    }

    return TRUE;
}


int
v_detect_scry(struct command *c) {

    if (!check_aura(c->who, 1)) {
        return FALSE;
    }

    wout(c->who, "Will practice location scry detection.");
    return TRUE;
}


int
d_detect_scry(struct command *c) {

    if (!charge_aura(c->who, 1)) {
        return FALSE;
    }

    return TRUE;
}


void
notify_loc_shroud(int where) {
    int who;
    struct entity_loc *p;

    p = rp_loc(where);

    if (p == NULL) {
        return;
    }

    loop_char_here(where, who)
            {
                int exp;

                if (has_skill(who, sk_shroud_region)) {
                    if (p->shroud > 0) {
                        wout(who, "The magical shroud over %s "
                                  "has diminished to %s aura.",
                             box_name(where),
                             nice_num(p->shroud));
                    } else {
                        wout(who, "The magical shroud over %s "
                                  "has dissipated.", box_name(where));
                    }
                }
            }
    next_char_here;
}


int
v_dispel_region(struct command *c) {
    int targ_loc = province(c->a);

    if (!is_loc_or_ship(targ_loc)) {
        wout(c->who, "%s is not a location.", box_code(targ_loc));
        return FALSE;
    }

    if (!check_aura(c->who, 3)) {
        return FALSE;
    }

    wout(c->who, "Attempt to dispel any magical shroud over %s.",
         box_name(targ_loc));

    return TRUE;
}


int
d_dispel_region(struct command *c) {
    int targ_loc = province(c->a);
    struct entity_loc *p;

    if (!is_loc_or_ship(targ_loc)) {
        wout(c->who, "%s is no longer a valid location.",
             box_code(targ_loc));
        return FALSE;
    }

    p = rp_loc(targ_loc);

    if (p && p->shroud > 0) {
        if (!charge_aura(c->who, 3)) {
            return FALSE;
        }

        wout(c->who, "Removed an aura %s magical shroud from %s.",
             nice_num(p->shroud),
             box_name(targ_loc));
        p->shroud = 0;
        notify_loc_shroud(targ_loc);
    } else {
        wout(c->who, "%s was not magically shrouded.",
             box_name(targ_loc));
    }

    return TRUE;
}


void
show_item_where(int who, int target) {
    int owner;
    int prov;

    assert(kind(target) == T_item);

    owner = item_unique(target);
    assert(owner);

    prov = province(owner);

    if (prov == owner) {
        wout(who, "%s is in %s.", box_name(target), box_name(prov));
        return;
    }

    if (subkind(owner) == sub_graveyard) {
        wout(who, "%s is buried in %s, in %s.",
             box_name(target),
             box_name(owner),
             box_name(prov));
        return;
    }

    wout(who, "%s is held by %s, in %s.",
         box_name(target), box_name(owner), box_name(prov));
}


int
v_locate_char(struct command *c) {
    int target = c->a;
    int aura = c->b;

    if (c->b < 1) {
        c->b = 1;
    }
    aura = c->b;

    if (!check_aura(c->who, aura)) {
        return FALSE;
    }

    wout(c->who, "Attempt to locate %s.", box_code(target));

    return TRUE;
}


int
d_locate_char(struct command *c) {
    int target = c->a;
    int aura = c->b;
    int chance;

    if (kind(target) != T_char && subkind(target) != sub_dead_body) {
        wout(c->who, "%s is not a character.", box_code(target));
        charge_aura(c->who, 1);
        return FALSE;
    }

    if (crosses_ocean(target, c->who)) {
        wout(c->who, "Something seems to block your magic.");
        charge_aura(c->who, 1);
        return FALSE;
    };

    if (!charge_aura(c->who, aura)) {
        return FALSE;
    }

    switch (aura) {
        case 1:
            chance = 50;
            break;

        case 2:
            chance = 75;
            break;

        case 3:
            chance = 90;
            break;

        default:
            assert(FALSE);
    }

    if (rnd(1, 100) > chance) {
        wout(c->who, "Character location failed.");
        return FALSE;
    }

    if (subkind(target) == sub_dead_body) {
        show_item_where(c->who, target);
    } else {
        wout(c->who, "%s is in %s.", box_name(target),
             char_rep_location(target));
    }

    return TRUE;
}


int
v_bar_loc(struct command *c) {
    int aura;
    int where;

    where = cast_where(c->who);

    if (kind(where) != T_loc) {
        wout(c->who, "%s is not a location.", box_code(where));
        return FALSE;
    }

    if (in_safe_now(where)) {
        wout(c->who, "Can't put a barrier around a safe haven.");
        return FALSE;
    }

    if (loc_depth(where) < LOC_subloc) {
        wout(c->who, "Can't put a barrier around %s.", box_code(where));
        return FALSE;
    }

    if (crosses_ocean(where, c->who)) {
        wout(c->who, "Something seems to block your magic.");
        return FALSE;
    };

    if (c->a < 1) {
        c->a = 1;
    }
    if (c->a > 4) {
        c->a = 4;
    }
    aura = c->a * c->a;

    if (!check_aura(c->who, aura)) {
        return FALSE;
    }

    if (loc_barrier(where) > 0) {
        wout(c->who, "%s already has a barrier.",
             box_name(where));
        return FALSE;
    }

    if (no_barrier(where) > 0) {
        wout(c->who, "You cannot cast another barrier on %s.",
             box_name(where));
        return FALSE;
    }

    c->d = where;
    reset_cast_where(c->who);

    wout(c->who, "Create a magical barrier over %s.", box_name(where));
    return TRUE;
}


int
d_bar_loc(struct command *c) {
    int aura = c->a * c->a;
    int where = c->d;
    struct entity_loc *p;
    int old_val;

    if (kind(where) != T_loc) {
        wout(c->who, "%s is not a location.", box_code(where));
        return FALSE;
    }

    if (in_safe_now(where)) {
        wout(c->who, "Can't put a barrier around a safe haven.");
        return FALSE;
    }

    if (loc_depth(where) < LOC_subloc) {
        wout(c->who, "Can't put a barrier around %s.", box_code(where));
        return FALSE;
    }

    if (loc_barrier(where) > 0) {
        wout(c->who, "%s already has a barrier.",
             box_name(where));
        return FALSE;
    }

    if (no_barrier(where) > 0) {
        wout(c->who, "You cannot cast another barrier on %s.",
             box_name(where));
        return FALSE;
    }

    if (!charge_aura(c->who, aura)) {
        return FALSE;
    }

    add_effect(where, ef_magic_barrier, 0, c->a * 30, 1);

    wout(c->who, "Cast a %s aura barrier over %s.",
         nice_num(c->a),
         box_name(where));

    return TRUE;
}


int
v_unbar_loc(struct command *c) {
    int aura;
    int where;
    struct exit_view *v;

    if (*(c->parse[1]) == '0') {
        where = cast_where(c->who);
    } else {
        v = parse_exit_dir(c, cast_where(c->who),
                           sout("use %d", sk_unbar_loc));

        if (v == NULL) {
            return FALSE;
        }

        where = v->destination;
    }

    if (crosses_ocean(where, c->who)) {
        wout(c->who, "Something seems to block your magic.");
        return FALSE;
    };

    if (c->b < 1) {
        c->b = 1;
    }
    if (c->b > 4) {
        c->b = 4;
    }
    aura = c->b;

    if (!check_aura(c->who, aura)) {
        return FALSE;
    }

    if (loc_barrier(where) == 0) {
        wout(c->who, "There is no barrier over %s.", box_name(where));
        return FALSE;
    }

    c->d = where;
    reset_cast_where(c->who);

    return TRUE;
}


int
d_unbar_loc(struct command *c) {
    int aura = c->b;
    int where = c->d;
    struct entity_loc *p;
    int old_val;
    int chance;

    if (kind(where) != T_loc) {
        wout(c->who, "%s is not a location.", box_code(where));
        return FALSE;
    }

    if (loc_barrier(where) == 0) {
        wout(c->who, "There is no barrier over %s.", box_name(where));
        return FALSE;
    }

    if (!charge_aura(c->who, aura)) {
        return FALSE;
    }

    switch (aura) {
        case 1:
            chance = 10;
            break;

        case 2:
            chance = 25;
            break;

        case 3:
            chance = 50;
            break;

        case 4:
            chance = 75;
            break;

        default:
            assert(FALSE);
    }

    if (rnd(1, 100) > chance) {
        wout(c->who, "Attempt to remove barrier fails.");
        return FALSE;
    }

    wout(c->who, "The barrier over %s has been removed.", box_name(where));
    wout(where, "The barrier over %s has dissipated.", box_name(where));
    delete_effect(where, ef_magic_barrier, 0);
    add_effect(where, ef_inhibit_barrier, 0, 4, 1);

    return TRUE;
}


int
v_proj_cast(struct command *c) {
    int to_where, curr_where;
    int aura;

    if (c->a == 0) {
        c->a = subloc(c->who);
    }
    to_where = c->a;

    if (!is_loc_or_ship(to_where)) {
        wout(c->who, "%s is not a location.", box_code(to_where));
        return FALSE;
    }

    if (in_safe_now(to_where)) {
        wout(c->who, "Magic may not be projected to safe havens.");
        return FALSE;
    }

    curr_where = cast_where(c->who);

    if (crosses_ocean(curr_where, c->who)) {
        wout(c->who, "Something seems to block your magic.");
        return FALSE;
    };

    aura = los_province_distance(curr_where, to_where) + 1;
    c->d = aura;
    assert(aura >= 0 && aura < 100);

    if (crosses_ocean(to_where, c->who)) {
        wout(c->who, "Something seems to block your magic.");
        return FALSE;
    };

/*
 *  Don't needlessly give away the exact distance with
 *  check_aura.
 */

    if (char_cur_aura(c->who) < aura) {
        wout(c->who, "Not enough current aura.");
        return FALSE;
    }

    wout(c->who, "Attempt to project next cast to %s.",
         box_name(to_where));

    reset_cast_where(c->who);

    return TRUE;
}


int
d_proj_cast(struct command *c) {
    int to_where = c->a;
    int aura = c->d;
    struct char_magic *p;

    if (!is_loc_or_ship(to_where)) {
        wout(c->who, "%s is not a valid location.",
             box_code(to_where));
        return FALSE;
    }

    if (crosses_ocean(to_where, c->who)) {
        wout(c->who, "Something seems to block your magic.");
        return FALSE;
    };

    if (!charge_aura(c->who, aura)) {
        return FALSE;
    }

    if (subloc(c->who) != to_where && loc_shroud(province(to_where))) {
        wout(c->who, "%s is protected with a magical shroud.",
             box_name(to_where));
        wout(c->who, "Spell fails.");
        return FALSE;
    }

    p = p_magic(c->who);
    p->project_cast = to_where;

    wout(c->who, "Next cast will be based from %s.", box_name(to_where));

    return TRUE;
}


int
v_save_proj(struct command *c) {

    if (!valid_box(char_proj_cast(c->who))) {
        wout(c->who, "No projected cast state is active.");
        return FALSE;
    }

    if (!check_aura(c->who, 3)) {
        return FALSE;
    }

    wout(c->who, "Attempt to save projected cast state.");
    return TRUE;
}


int
d_save_proj(struct command *c) {
    int new;
    struct char_magic *p;
    struct item_magic *im;

    if (!charge_aura(c->who, 3)) {
        return FALSE;
    }

    new = new_potion(c->who);

    p = p_magic(c->who);
    im = p_item_magic(new);

    im->use_key = use_proj_cast;
    im->project_cast = p->project_cast;

    p->project_cast = 0;

    return TRUE;
}


int
v_use_proj_cast(struct command *c) {
    int item = c->a;
    struct item_magic *im;

    assert(kind(item) == T_item);

    wout(c->who, "%s drinks the potion...", just_name(c->who));

    im = rp_item_magic(item);

    if (im == NULL ||
        !is_loc_or_ship(im->project_cast) ||
        !is_magician(c->who)) {
        destroy_unique_item(c->who, item);
        wout(c->who, "Nothing happens.");
        return FALSE;
    }

    p_magic(c->who)->project_cast = im->project_cast;

    wout(c->who, "Project next cast to %s.",
         char_rep_location(im->project_cast));
    destroy_unique_item(c->who, item);

    return TRUE;
}

