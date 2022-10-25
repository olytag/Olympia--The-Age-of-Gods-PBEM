// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

/*
 * Tue Nov 24 13:19:23 1998 -- Scott Turner
 *
 * Heroism: Skills to make an individual noble an adventurer.
 * Many suggested by Jeremy Maiden (JeremyM@ctxuk.citrix.com)
 *
 * re Survive Fatal Wound (return NP version)
 * of Defense
 * of Swordplay
 * 
 * of Avoid wounds (decrease amount of wounds by %) +
 * of Avoid Illness (reduce chance of illness) 
 * of Improved Recovery (recover from illness)
 * of Personal Fight to the Death (don't quit at first wound) +
 * of Forced Marching (improved foot speed) +
 * re Acute Senses (avoid traps) +
 * re Improved Explore (subdivided for terrain types?) +
 * re Uncanny Accuracy (archery to the back rank) +
 * re Extra Attacks (increase # of attacks each combat round) +
 * re Blinding Speed (1 attack during Special Attack round)
 *
 * Skills marked with (+) only usable if you are not commanding men.
 *
 */
#include    <stdio.h>
#include    <unistd.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


/*
 *  Tue Nov 24 13:25:09 1998 -- Scott Turner
 *
 *  Extra attacks.  Get an extra attack after every 8 weeks of practice.
 *
 *  I don't think we actually need to have a v_ and a d_ for this,
 *  since using "practice"  should do the trick.  Just need to have
 *  the skill set up appropriately in Lib/skill.  Note practice cost 
 *  of $50.
 *
 */


/*
 *  Tue Nov 24 13:36:28 1998 -- Scott Turner
 *
 *  Avoid wounds.  Get -% to wound damage up to a maximum of
 *  -80%, -2% per week practiced.
 *
 *  I don't think we actually need to have a v_ and a d_ for this,
 *  since using "practice"  should do the trick.  Just need to have
 *  the skill set up appropriately in Lib/skill.  Note practice cost 
 *  of $50.
 *
 */

/*
 *  Tue Nov 24 13:43:44 1998 -- Scott Turner
 *
 *  Avoid illness is exactly the same as avoid wounds.  Get 2% to
 *  avoid illness up to a maximum of 80%, 2% per week practiced.
 *
 */

/*
 *  Wed Nov 25 07:03:49 1998 -- Scott Turner
 *
 *  Improved Recovery gives you an additional +1% to recover per 
 *  week for each level of expertise, up to a maximum of +50%
 *
 */

/*
 *  Wed Nov 25 07:05:07 1998 -- Scott Turner
 *
 *  Personal Fight to the Death allows you to set the level of wounds
 *  that will cause you to quit a fight, rather than automatically
 *  quit after the first wound.
 */
int
v_personal_fight_to_death(struct command *c) {
    int flag = c->a;

    if (flag < 0) {
        flag = 0;
    }
    if (flag > 100) {
        flag = 100;
    }

    p_char(c->who)->personal_break_point = flag;
    wout(c->who, "%s will now leave battle when he has "
                 "%d health remaining", box_name(c->who), flag);

    return TRUE;
}

/*
 *  Wed Nov 25 07:13:12 1998 -- Scott Turner
 *
 *  Forced Marching makes your next move be at riding speed, at the 
 *  cost of that many days travel in health.
 *  
 */
int
v_forced_march(struct command *c) {
    /*  Has a $10 cost */
    if (!charge(c->who, 10)) {
        wout(c->who, "Can't afford %s to prepare for a forced march.",
             gold_s(10));
        return FALSE;
    }

    if (get_effect(c->who, ef_forced_march, 0, 0)) {
        wout(c->who, "You are already prepared for a forced march.");
    } else if (!add_effect(c->who, ef_forced_march, 0, -1, 1)) {
        wout(c->who, "For some reason, you cannot prepare for a forced march!");
        return FALSE;
    } else {
        wout(c->who, "Now prepared to make the next move as a forced march.");
    };
    return TRUE;
}

/*
 *  Wed Nov 25 07:24:43 1998 -- Scott Turner
 *
 *  Acute Senses gives you a 2% chance to avoid a trap in a province
 *  for every week's practice to a maximum of 80%.
 *
 */

/*
 *  Wed Nov 25 07:24:43 1998 -- Scott Turner
 *
 *  Improved Explore gives a +1% chance to explore for each week's 
 *  practice, up to a maximum of +50%.
 *
 */

/*
 *  Wed Nov 25 07:30:01 1998 -- Scott Turner
 *
 *  Uncanny Accuracy permits the hero to target his missile attacks
 *  against any missile troops in the back row of the opponent.  Is this 
 *  easily doable?
 *
 */

/*
 *  Wed Nov 25 07:31:44 1998 -- Scott Turner
 *
 *  Blinding Speed gives the Hero a melee attack during the special 
 *  attacks phase.
 *
 */
