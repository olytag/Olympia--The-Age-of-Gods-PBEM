/*
 *  EFFECT.C
 *  Mon Aug  5 12:40:07 1996 -- Scott Turner
 *
 *  Effects hang off of nobles, locations or structures (*) and have
 *  the following properties:
 *
 *    + have a type (generally equal to the skill used to create it).
 *    + have a duration in days.
 *    + have data: single integer.
 *
 *  This is used to implement things like a spell that gives a fortification
 *  a +25% resistance to attack, etc.
 *
 *  The actual effect structure is defined in oly.h
 *
 */
#include    <stdio.h>
#include    "z.h"
#include    "oly.h"

/*
 *  ADD_EFFECT
 *  Tue Aug  6 12:10:25 1996 -- Scott Turner
 *
 *  Add an effect to a thing.
 *
 */
int
add_effect(int what, int t, int st, int duration, int value) {
    int i;
    ilist *el = NULL;
    struct effect **e = NULL;
    struct effect *new;
    /*
     *  Validity checks.
     *
     */
    if (!valid_box(what)) { return 0; }
    /*
     *  Only characters, locations, and sublocations have
     *  effects on them.
     *
     */
    e = effects(what);
    el = (ilist *) &effects(what);

    /*
     *  Allocate and fill in the new effect.
     *
     */
    new = my_malloc(sizeof(*new));
    new->type = t;
    new->subtype = st;
    new->days = duration;
    new->data = value;
    /*
     *  Now append it to the effects list.
     *
     */
    ilist_append(el, (int) new);
    return 1;
};

/*
 *  DELETE_EFFECT
 *  Fri Sep 27 12:33:24 1996 -- Scott Turner
 *
 *  Delete the first effect of the given type.
 *
 */
void
delete_effect(int what, int type, int st) {
    int i;
    ilist *el = NULL;
    struct effect **e = NULL;

    /*
     *  Validity checks.
     *
     */
    if (!valid_box(what)) { return; }
    /*
     *  Only characters, locations, and sublocations have
     *  effects on them.
     *
     */
    e = effects(what);
    el = (ilist *) &effects(what);

    if (e == NULL) { return; }

    for (i = ilist_len(e) - 1; i >= 0; i--) {
        if (e[i]->type == type && (!st || e[i]->subtype == st)) {
            ilist_delete(el, i);
            return;
        };
    };
};

/*
 *  DELETE_ALL_EFFECTS
 *  Fri Sep 27 12:33:24 1996 -- Scott Turner
 *
 *  Delete all the effects of the given type.
 *
 */
void
delete_all_effects(int what, int type, int st) {
    int i;
    ilist *el = NULL;
    struct effect **e = NULL;

    /*
     *  Validity checks.
     *
     */
    if (!valid_box(what)) { return; }
    /*
     *  Only characters, locations, and sublocations have
     *  effects on them.
     *
     */
    e = effects(what);
    if (e == NULL) { return; }
    el = (ilist *) &effects(what);

    /*
     *  We go through this backwards so that ilist_delete doesn't
     *  break the rest of the loop.
     *
     */
    for (i = ilist_len(e) - 1; i >= 0; i--) {
        if (e[i]->type == type && (!st || e[i]->subtype == st)) {
            ilist_delete(el, i);
        };
    };
    return;
};

/*
 *  UPDATE_EFFECTS
 *  Tue Aug  6 12:05:04 1996 -- Scott Turner
 *
 *  Update something's effects for the passage of a day.
 *
 */
void update_effects(int what) {
    int i;
    ilist *el = NULL;
    struct effect **e = NULL;

    /*
     *  Validity checks.
     *
     */
    if (!valid_box(what)) { return; }
    /*
     *  Only characters, locations, and sublocations have
     *  effects on them.
     *
     */
    e = effects(what);
    el = (ilist *) &effects(what);
    /*
     *  Possibly no effects, in which case we're done.
     *
     */
    if (e == NULL) { return; }
    /*
     *  Otherwise, go through the list, decrement the "days" for each
     *  effect, and delete any that are now expired.  We go through
     *  the list backwards, because deleting removes elements and shifts
     *  the list.
     *
     */
    for (i = ilist_len(e) - 1; i >= 0; i--) {
        /*
         *  Special cases.
         *
         *  Fri Nov 29 10:44:00 1996 -- Scott Turner
         *
         *  Check hidden stuff every 30 days...
         */
        if (e[i]->type == ef_hide_money ||
            e[i]->type == ef_hide_item) {
            if (e[i]->days % 30 == 0 && rnd(1, 100) < (e[i]->days / 30)) {
                wout(what, "You have a nagging feeling you've forgotten something.");
                ilist_delete(el, i);
            };
            e[i]->days++;
            continue;
        };
        /*
         *  Mon May  5 15:27:53 1997 -- Scott Turner
         *  Disappearing golems.
         *
         */
        if (e[i]->type == ef_kill_dirt_golem) {
            if (e[i]->days < 2) {
                /*
                 *  Remove a dirt golem from this guy.
                 *
                 */
                if (consume_item(what, item_dirt_golem, 1)) {
                    wout(what, "A dirt golem suddenly crumbles to dust!");
                } else {
                    wout(gm_player, "No dirt golem available to crumble for %s.", box_name(what));
                };
            };
        };
        if (e[i]->type == ef_kill_flesh_golem) {
            if (e[i]->days < 2) {
                /*
                 *  Remove a flesh golem from this guy.
                 *
                 */
                if (consume_item(what, item_flesh_golem, 1)) {
                    wout(what, "A flesh golem suddenly crumbles to dust!");
                } else {
                    wout(gm_player, "No flesh golem available to crumble for %s.", box_name(what));
                };
            };
        };
        /*
         *  When tap health runs out, you suffer a wound.
         *
         */
        if (e[i]->type == ef_tap_wound && e[i]->days == 1) {
            wout(what, "You feel a aura-rending blast hit your body!");
            add_char_damage(what, e[i]->data, MATES);
        };

        /*
         *  Generic case.
         *
         */
        --e[i]->days;
        if (e[i]->days == 0) { ilist_delete(el, i); }
    };

    /*
     *  We might now have an empty list; if so we could reclaim it, but
     *  for the moment we leave it alone.
     *
     */
};

/*
 *  GET_EFFECT
 *  Tue Aug  6 12:05:34 1996 -- Scott Turner
 *
 *  Get the first effect of a type off of an effect list.
 *
 */
int get_effect(int what, int t, int st, int v) {
    int i;
    struct effect **e = NULL;
    /*
     *  Validity checks.
     *
     */
    if (!valid_box(what)) { return 0; }
    /*
     *  Only characters, locations, and sublocations have
     *  effects on them.
     *
     */
    e = effects(what);
    /*
     *  Possibly no effects, in which case we're done.
     *
     */
    if (e == NULL) { return 0; }
    /*
     *  Look for the effect.
     *
     */
    for (i = 0; i < ilist_len(e); i++) {
        if (e[i]->type == t &&
            (!st || e[i]->subtype == st) &&
            (!v || e[i]->data == v)) {
            return e[i]->data;
        }
    }

    return 0;
};

/*
 *  GET_ALL_EFFECTS
 *  Tue Aug  6 12:05:34 1996 -- Scott Turner
 *
 *  Like above, only sums multiple cumulative effects.
 *
 */
int get_all_effects(int what, int t, int st, int v) {
    int i, answer = 0;
    struct effect **e = NULL;
    /*
     *  Validity checks.
     *
     */
    if (!valid_box(what)) { return 0; }
    /*
     *  Only characters, locations, and sublocations have
     *  effects on them.
     *
     */
    e = effects(what);
    /*
     *  Possibly no effects, in which case we're done.
     *
     */
    if (e == NULL) { return 0; }
    /*
     *  Look for the effect.
     *
     */
    for (i = 0; i < ilist_len(e); i++) {
        if (e[i]->type == t &&
            (!st || e[i]->subtype == st) &&
            (!v || e[i]->data == v)) {
            answer += e[i]->data;
        }
    }

    return answer;
};
    
