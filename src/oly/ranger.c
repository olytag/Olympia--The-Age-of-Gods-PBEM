// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


/*
 *  Find Food
 *
 *  Wed Jul  9 11:45:09 1997 -- Scott Turner
 *
 */
int
v_find_food(struct command *c) {
    if (has_item(province(c->who), item_peasant) >= 100) {
        wout(c->who, "You may only search for food in wilderness provinces.");
        return FALSE;
    };

    return TRUE;
}


int
d_find_food(struct command *c) {
    int food_found = 10;

    if (has_item(province(c->who), item_peasant) >= 100) {
        wout(c->who, "You may only search for food in wilderness provinces.");
        return FALSE;
    };

    /*
     *  Add the experience level of this guy into any effect...
     *
     */
    food_found = get_effect(c->who, ef_food_found, 0, 0);
    delete_effect(c->who, ef_food_found, 0);
    food_found += 10;
    if (subkind(province(c->who)) == sub_mountain) {
        food_found += skill_exp(c->who, sk_find_food) / 2;
    } else if (subkind(province(c->who)) == sub_desert) {
        food_found += skill_exp(c->who, sk_find_food) / 4;
    } else {
        food_found += skill_exp(c->who, sk_find_food);
    };

    if (!add_effect(c->who, ef_food_found, 0, 35, food_found)) {
        wout(c->who, "Through some odd circumstance you cannot find any food!");
        return FALSE;
    };

    return TRUE;
}
