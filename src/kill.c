// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


/*
 *  Wed Jun 11 14:26:47 1997 -- Scott Turner
 *
 *  Functions for killing off a character.  Transitioning it
 *  through a dead body to a dead soul to nothing.  Properly handles
 *  npc characters.
 *
 *  That's the theory, anyway.
 *
 */
int dead_body_np = TRUE;

int
nearby_grave(int where) {
    struct entity_loc *p;
    int i;
    static ilist l = NULL;

    where = province(where);
    p = rp_loc(where);

    if (p && p->near_grave) {
        return p->near_grave;
    }

    log_output(LOG_CODE, "%s has no nearby grave", box_name(where));

    ilist_clear(&l);
    loop_subkind(sub_graveyard, i)
            {
                ilist_append(&l, i);
            }
    next_subkind;

    assert(ilist_len(l) > 0);

    ilist_scramble(l);

    return l[rnd(0, ilist_len(l) - 1)];
}

void
remove_follower(int who) {
    /*
     *  Wed Jun 11 15:32:10 1997 -- Scott Turner
     *
     *  When a body finally is destroyed for good, we need to free
     *  up all the priest related "stuff".
     *
     */
    if (rp_char(who) && rp_char(who)->religion.priest) {
        /*
         *  You are no longer following someone.
         *
         */
        rp_char(who)->religion.priest = 0;

        /*
         *  Remove it from the list of followers.
         *
         */
        if (valid_box(rp_char(who)->religion.priest) &&
            is_priest(rp_char(who)->religion.priest)) {
            wout(rp_char(who)->religion.priest,
                 "An angel informs you that your follower %s~%s has passed on.",
                 rp_misc(who)->save_name, box_code(who));
            ilist_rem_value(&(rp_char(rp_char(who)->religion.priest)->religion.followers), who);
        };
    };
};

/*
 *  Wed Jun 11 15:29:07 1997 -- Scott Turner
 *
 *  Destroy what you get, based on what it is.
 *
 */
static void
convert_to_nothing(int cur_pl, int who) {
    char *name;
    /*
     *  If it has an old lord, and can receive the old NPs, then
     *  return them.
     *
     */
    int pl = body_old_lord(who) || player(who);

    /*
     *  Figure out a name.  It may not be save_name if the bugger
     *  never became a corpse.
     *
     */
    if (rp_misc(who) && rp_misc(who)->save_name) {
        name = rp_misc(who)->save_name;
    } else {
        name = box_name(who);
    }

    if (valid_box(pl) && dead_body_np) {
        if (options.death_nps == 1) {
            add_np(pl, 1);
            out(pl, "%s~%s has passed on.  Gained 1 NP.",
                name, box_code(who));
        } else if (options.death_nps == 2) {
            int num = nps_invested(who);
            add_np(pl, num);
            out(pl, "%s~%s has passed on.  Gained %s NP%s.",
                name, box_code(who), nice_num(num), num == 1 ? "" : "s");
        };
        out(pl, "%s~%s has passed on.", name, box_code(who));
    } else if (valid_box(pl)) {
        out(pl, "The soul of %s~%s has passed on.  No NPs reclaimed.",
            name, box_code(who));
    };

    /*
     *  Perhaps he was a follower.
     *
     */
    remove_follower(who);

    /*
     *  If he was a priest, free up all his followers.
     *
     */
    if (rp_char(who) && is_priest(who)) {
        int i;
        loop_followers(who, i)
                {
                    if (valid_box(i) && rp_char(i)) {
                        wout(i, "An angel informs you that your priest %s has passed on.",
                             name);
                        rp_char(i)->religion.priest = 0;
                    };
                }next_follower;
        ilist_reclaim(&rp_char(who)->religion.followers);
    };

    /*
     *  Now we actually destroy the object.
     *
     */
    if (item_unique(who)) {
        destroy_unique_item(item_unique(who), who);
    } else {
        if (kind(who) == T_char && player(who)) {
            set_lord(who, 0, LOY_UNCHANGED, 0);
        }
        set_where(who, 0);
        delete_box(who);
    };
};

/*
 *  Thu May 29 12:10:18 1997 -- Scott Turner
 *
 *  Turn something into a lost soul.  Assumes your character
 *  name is in save_name.
 *
 *  Wed Apr 29 11:19:44 1998 -- Scott Turner
 *
 *  Need to fix the "unique item" status of this object when we
 *  un-bodify-it :-)
 */
static void
convert_to_soul(int pl, int who) {
    char new_name[120];
    int dest = 0, i, sum = 0;

    assert(kind(who) == T_char ||
           (kind(who) == T_item && subkind(who) == sub_dead_body));

    /*
     *  Remove this thing from wherever it was at.
     *
     */
    if (kind(who) == T_char) {
        set_where(who, 0);
    } else if (item_unique(who)) {
        sub_item(item_unique(who), who, 1);
        p_item(who)->who_has = 0;
    } else {
        assert(FALSE);
    };

    /*
     *  Change it into the appropriate kind of thing.
     *
     */
    change_box_kind(who, T_char);
    change_box_subkind(who, sub_lost_soul);

    /*
     *  Set it's name correctly.
     *
     */
    if (p_misc(who) && !p_misc(who)->save_name) {
        p_misc(who)->save_name = bx[who]->name;
        bx[who]->name = NULL;
    };
    sprintf(new_name, "Lost soul of %s", p_misc(who)->save_name);
    if (bx[who]->name) { my_free(bx[who]->name); }
    bx[who]->name = str_save(new_name);

    p_item(who)->weight = item_weight(item_peasant);
    if (p_item(who)->plural_name) { my_free(p_item(who)->plural_name); }
    p_item(who)->plural_name = str_save("lost souls");

    /*
     *  Save the old lord properly, if possible.
     *
     */
    if (!rp_misc(who)->old_lord && player(who)) {
        rp_misc(who)->old_lord = player(who);
    };

    /*
     *  Make sure it won't fight.
     *
     */
    rp_char(who)->break_point = 100;  /* Don't fight */


    /*
     *  Select a location from all the provinces in Hades.
     *
     */
    loop_province(i)
                {
                    if (!in_hades(i)) { continue; }
                    if (province_subloc(i, sub_city)) { continue; }
                    sum++;
                    if (rnd(1, sum) == 1) {
                        dest = i;
                    };
                }next_province;
    assert(dest);

    set_where(who, dest);
    set_lord(who, indep_player, LOY_UNCHANGED, 0);  /* will this work? */
}

/*
 *  Wed Jun 11 15:11:40 1997 -- Scott Turner
 *
 *  Convert to a dead body.
 *
 *  Tue May 18 06:59:34 1999 -- Scott Turner
 *
 *  This should now drop the body wherever it was.  The only place
 *  we don't want to drop a body is an ocean province.
 *
 */
static void
convert_to_body(int pl, int who) {
    int where = province(who);
#if 0
    int grave = nearby_grave(where);
#endif
    struct entity_item *p;

    /*
     *  Should be a character.
     *
     */
    assert(kind(who) == T_char && !subkind(who));

    /*
     *  Let's not drop this into the ocean.
     *
     */
    if (subkind(where) == sub_ocean) {
        where = find_nearest_land(where);
        assert(where && subkind(where) != sub_ocean);
    };

#if 0
    /*
     * If we couldn't find a nearby grave, we might have to go
     * directly to being a lost soul.
     *
     */
    if (!grave) {
      convert_to_soul(pl, who);
      return;
    };
#endif

    /*
     *  Save the old "lord"
     *
     */
    p_misc(who)->old_lord = pl;

    /*
     *  Remove it from the player's list of units...
     *
     */
    set_lord(who, 0, LOY_UNCHANGED, 0);

    /*
     *  Remove this from the world.
     *
     */
    set_where(who, 0);

    /*
     *  Make it a dead body.
     *
     */
    change_box_kind(who, T_item);
    change_box_subkind(who, sub_dead_body);

    /*
     *  Name it appropriately.
     *
     */
    p_misc(who)->save_name = bx[who]->name;
    bx[who]->name = str_save("dead body");
    p_item(who)->plural_name = str_save("dead bodies");
    p_item(who)->weight = item_weight(item_peasant);

    /*
     *  Finally, stick it in the grave.
     *
     */
    hack_unique_item(who, where);
};

/*
 *  Wed Jun 11 14:34:06 1997 -- Scott Turner
 *
 *  This function should take who to kill, inherit, and
 *  eventual status (body, soul, or nothing).  It should be able to
 *  transition anything to any of those states.
 *
 *  Tue May 18 06:54:02 1999 -- Scott Turner
 *
 *  Simplifying death:
 *	- Upon death, body is dropped where killed.
 *	- Bodies decay for 12 months before disintegrating.
 *
 */
static char *verbs[] = {"", "died", "transmigrated", "permanently ascended"};

void
kill_char(int who, int inherit, int status) {
    int where = subloc(who);
    int pl = player(who);
    int a;

    assert(kind(who) == T_char ||
           (kind(who) == T_item && subkind(who) == sub_dead_body));

    /*
     *  Tue May 18 06:56:44 1999 -- Scott Turner
     *
     *  Treat S_soul as S_nothing...
     *
     */
    if (status == S_soul) { status = S_nothing; }

    /*
     *  Don't do anything to a noble who has survival_fatal.
     *
     */
    if (!char_melt_me(who) && survive_fatal(who)) { return; }

    /*
     *  If you're being melted, you should go to nothing no matter what.
     *
     */
    if (char_melt_me(who)) { status = S_nothing; }

#if 0
    /*
     *  The only non-character thing that can transition is a dead
     *  body; everything else becomes nothing.
     *
     */
    if (kind(who) != T_char) {
      if (subkind(who) == sub_dead_body)
        status = S_soul;
      else
        status = S_nothing;
    } else {
      /*
       *  Anything with a subkind has to go to nothing.
       *
       */
      if (subkind(who)) status = S_nothing;
    };
#endif

    if (kind(who) != T_char || subkind(who)) { status = S_nothing; }

    /*
     *  Regardless of what is eventually going to happen to you,
     *  the following things must be done:
     *
     */

    /*
     *  Inform you and your stack parent what has happened, or whoever
     *  is holding you, if you're a unique item.
     *
     */
    if (subkind(who) == sub_garrison) {
        wout(who, "Garrison disbanded.");
    } else if (item_unique(who) && rp_char(item_unique(who))) {
        wout(item_unique(who), "*** %s has %s ***", just_name(who), verbs[status]);
    } else if (p_char(who)) {
        int sp;
        p_char(who)->prisoner = FALSE;
        wout(who, "*** %s has %s ***", just_name(who), verbs[status]);

        sp = stack_parent(who);
        if (sp) { wout(sp, "%s has %s.", box_name(who), verbs[status]); }

        p_char(who)->prisoner = TRUE;        /* suppress output */
    } else {
        assert(FALSE);  /* You should be either a character or a unique item. */
    };

    /*
     *  Inform the death log.
     *
     */
    log_output(LOG_DEATH, "%s %s in %s.",
               box_name(who), verbs[status], char_rep_location(who));

    /*
     *  If the dying thing has any possessions, inherit them out.
     *
     */
    if (kind(who) == T_char) {
        take_unit_items(who, inherit, TAKE_SOME);
    }

    /*
     *  If the dying thing is the leader of a stack and is
     *  moving, we need to restore the stacks actions!
     *
     */
    if (kind(who) == T_char && stack_leader(who) == who && char_moving(who)) {
        restore_stack_actions(who);
    }

    /*
     *  If the dying thing is in a stack, then remove it from the
     *  stack.
     *
     */
    if (kind(who) == T_char) {
        extract_stacked_unit(who);
    }

    /*
     *  Fix its orders.
     *
     */
    if (kind(who) == T_char && player(who)) {
        flush_unit_orders(player(who), who);
        interrupt_order(who);
    };

    /*
     *  Delete all its aura/piety if has any.
     *
     */
    if (rp_magic(who))
        rp_magic(who)->cur_aura = 0;

    /*
     *  If it's newly dead, then it's not a prisoner.
     *
     */
    if (rp_char(who)) {
        rp_char(who)->prisoner = FALSE;
    };

    /*
     *  If it can cheat death, then let it.
     *
     */
    if (!char_melt_me(who) && has_skill(who, sk_transcend_death)) {
        log_output(LOG_SPECIAL, "%s transcends death", box_name(who));
        log_output(LOG_SPECIAL, "...%s moved to %s",
                   box_name(who), box_name(hades_pit));
        p_char(who)->prisoner = FALSE;
        p_char(who)->sick = FALSE;
        p_char(who)->health = 100;
        move_stack(who, hades_pit);
        wout(who, "%s appears at %s.",
             box_name(who), box_name(hades_pit));
        return;
    };

    /*
     *  It might have an artifact.
     *
     */
    if (!char_melt_me(who) && (a = has_artifact(who, ART_RESTORE, 0, 0, 1))) {
        int where = pick_starting_city(nation(who), 0);
        log_output(LOG_SPECIAL, "%s uses a Restore Life artifact %s.",
                   box_name(who), box_name(a));
        log_output(LOG_SPECIAL, "Transferring %s to %s.", box_name(who),
                   box_name(where));
        p_char(who)->prisoner = FALSE;
        p_char(who)->sick = FALSE;
        p_char(who)->health = 100;
        move_stack(who, where);
        wout(who, "%s appears at %s.",
             box_name(who), box_name(where));
        /*
         *  Now fix up the artifact.
         *
         */
        rp_item_artifact(a)->uses--;
        if (!rp_item_artifact(a)->uses) {
            wout(who, "%s vanishes.", box_name(a));
            destroy_unique_item(who, a);
        };
        return;
    };


    /*
     *  It might be a "token item", meaning we have to take care
     *  of resetting the token, etc.
     */
    {
        int token_item = our_token(who);
        struct entity_player *token_pl;
        int who_has;

        if (token_item) {
            who_has = item_unique(token_item);
            token_pl = p_player(token_item);

            ilist_rem_value(&token_pl->units, who);

            if (!char_melt_me(who))
                p_item_magic(token_item)->token_num--;

            if (item_token_num(token_item) <= 0) {
                if (player(who_has) == sub_pl_regular) {
                    wout(who_has, "%s vanishes.",
                         box_name(token_item));
                }
                destroy_unique_item(who_has, token_item);
            }
            assert(status == S_nothing);  /* Should be disappearing? */
        }
    }

#if 0
    /*
     *  If the unit is going to change players, it needs
     *  to update it's loyalties.
     *
     */
    if (kind(who) == T_char)
      (void) unit_deserts(who, 0, TRUE, LOY_UNCHANGED, 0);
#endif

    /*
     *  Put back the cookie...does nothing if inappropriate.
     *
     */
    put_back_cookie(who);

    switch (status) {
        case S_body:
            p_char(who)->death_time = sysclock;    /* record time of death */
            convert_to_body(pl, who);
            break;
        case S_soul:
            assert(FALSE);
            convert_to_soul(pl, who);
            break;
        case S_nothing:
            convert_to_nothing(pl, who);
            break;
        default:
            assert(FALSE);
    };

}

void
restore_dead_body(int owner, int who) {
    struct entity_misc *pm;
    struct entity_item *pi;
    struct entity_char *pc;
    int sub_item();

    log_output(LOG_CODE, "dead body revived: who=%s, owner=%s, player=%s",
               box_code_less(who),
               box_code_less(owner),
               box_code_less(player(owner)));

    /*
     *  If it's an item (body), remove it...
     *
     *  Thu May 17 07:33:23 2001 -- Scott Turner
     *
     *  Owner is not necessarily the holder of the body, since
     *  you can now resurrect at a distance.
     *
     */
    if (kind(who) == T_item) {
        int ret;

        ret = sub_item(item_unique(who), who, 1);
        assert(ret);

        p_item(who)->who_has = 0;
    };


    change_box_kind(who, T_char);
    change_box_subkind(who, 0);

    pm = p_misc(who);
    pi = p_item(who);
    pc = p_char(who);

    pi->weight = 0;
    my_free(pi->plural_name);
    pi->plural_name = NULL;

    if (pm->save_name && *pm->save_name) {
        set_name(who, pm->save_name);
        my_free(pm->save_name);
        pm->save_name = NULL;
    }

    pc->health = 100;
    pc->sick = FALSE;
    pc->break_point = 50;

    set_where(who, owner);

    if (kind(pm->old_lord) == T_player) {
        wout(pm->old_lord, "%s has been brought back to life.",
             box_name(who));

#if 0
        if (pm->old_lord != player(owner))
          p_char(who)->prisoner = TRUE;
#endif

        set_lord(who, pm->old_lord, LOY_UNCHANGED, 0);
    } else {
        set_lord(who, indep_player, LOY_UNCHANGED, 0);
    }

    pm->old_lord = 0;

    wout(owner, "%s stacks beneath us.", box_name(who));
}

