// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


/*
 *  num   item           material
 *  ---   ----           --------
 *   72   longbow        yew [68]
 *   73   plate armor    iron [79]
 *   74   longsword      iron [79]
 *   75   pike           wood [77]
 *   85   crossbow       wood [77]
 */

/*
 *   num   kind          skill    input man        input item
 *   ---   -----------   -----    --------------   ----------------
 *    11   worker         none    peasant [10]
 *    19   sailor          120    peasant [10]
 *    21   crossbowman     121    peasant [10]     crossbow [85]
 *    12   soldier         121    peasant [10]
 *    16   pikeman         121    soldier [12]     pike [75]
 *    20   swordsman      9580    soldier [12]     longsword [74]
 *    14   knight         9580    swordsman [20]   warmount [53]
 *    15   elite guard    9580    knight [14]      plate armor [73]
 *    13   archer         9579    soldier [12]     longbow [72]
 *    22   elite archer   9579    archer [13]
 */

#define    WHERE_SHIP    (-1)

#ifndef OLY_FORWARD_struct_make
#define OLY_FORWARD_struct_make
struct make {
    int item;
    int inp1;
    int inp1_factor; /* # of inp1 needed to make 1 */
    int inp2;
    int inp2_factor; /* # of inp2 needed to make 1 */
    int inp3;
    int inp3_factor; /* # of inp3 needed to make 1 */
    int req_skill;
    int worker;      /* worker needed */
    char *got_em;
    int public;      /* does everyone see us make this */
    int where;       /* place required for production */
    int aura;        /* aura per unit required */
    int factor;      /* multiplying qty factor, usuall 1 */
    int days;        /* days to make each thing */
};
#endif //OLY_FORWARD_struct_make

struct make make_tbl[] =
        {

/*
 *  One-day things
 */

                {
                        item_blank_scroll,    item_lana_bark,     1, 0,                 1,
                        0,             0,
                        sk_alchemy,           0, "made",     FALSE, 0,          0, 1, 1
                },
                {
                        item_elite_arch,      item_archer,        1, item_elvish_arrow, 1,
                        0,             0,
                        sk_archery,           0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_angry_peasant,   item_peasant,       1, 0,                 1,
                        0,             0,
                        sk_train_angry,       0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_peasant,         item_angry_peasant, 1, 0,                 1,
                        0,             0,
                        sk_train_angry,       0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_archer,          item_soldier,       1, item_longbow,      1,
                        0,             0,
                        sk_archery,           0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_elite_guard,     item_knight,        1, 0,                 1,
                        0,             0,
                        sk_train_paladin,     0, "trained",  FALSE, sub_guild,  0, 1, 1
                },
                {
                        item_knight,          item_hvy_foot,      1, item_warmount,     1,
                        0,             0,
                        sk_train_knight,      0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_cavalier,        item_soldier,       1, item_mithril,      1,
                        item_warmount, 1,
                        sk_train_knight,      0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_blessed_soldier, item_soldier,       1, 0,                 1,
                        0,             0,
                        sk_religion,          0, "trained",  FALSE, sub_temple, 0, 1, 1
                },
                {
                        item_ghost_warrior, 0,                    1, 0,                 1,
                        0,             0,
                        sk_summon_ghost,      0, "summoned", FALSE, 0,          1, 2, 1
                },
                {
                        item_swordsman,       item_soldier,       1, item_longsword,    1,
                        0,             0,
                        sk_swordplay,         0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_hvy_foot,        item_swordsman,     1, item_plate,        1,
                        0,             0,
                        sk_train_armor,       0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_pirate,          item_sailor,        1, item_longsword,    1,
                        0,             0,
                        sk_swordplay,         0, "trained",  FALSE, WHERE_SHIP, 0, 1, 1
                },
                {
                        item_pikeman,         item_soldier,       1, item_pike,         1,
                        0,             0,
                        sk_combat,            0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_skirmisher,      item_peasant,       1, 0,                 1,
                        0,             0,
                        sk_combat,            0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_soldier,         item_skirmisher,    1, 0,                 1,
                        0,             0,
                        sk_combat_discipline, 0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_crossbowman,     item_soldier,       1, item_crossbow,     1,
                        0,             0,
                        sk_combat,            0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_hvy_xbowman,     item_soldier,       1, item_hvy_xbow,     1,
                        0,             0,
                        sk_combat,            0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_horse_archer,    item_archer,        1, item_riding_horse, 1,
                        0,             0,
                        sk_train_knight,      0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_sailor,          item_peasant,       1, 0,                 1,
                        0,             0,
                        sk_pilot_ship,        0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_worker,          item_peasant,       1, 0,                 1,
                        0,             0,
                        0,                    0, "trained",  FALSE, 0,          0, 1, 1
                },
                {
                        item_basket,        0,                    1, 0,                 1,
                        0,             0,
                        0,                    0, "made",     FALSE, 0,          0, 1, 1
                },
                {
                        item_pot,           0,                    1, 0,                 1,
                        0,             0,
                        0,                    0, "made",     FALSE, 0,          0, 1, 1
                },
                {
                        item_crossbow,        item_lumber,        1, 0,                 1,
                        0,             0,
                        sk_weaponsmith,       0, "made",     FALSE, 0,          0, 1, 1
                },
                {
                        item_hvy_xbow,        item_lumber,        1, item_iron,         1,
                        0,             0,
                        sk_weaponsmith,       0, "made",     FALSE, 0,          0, 1, 1
                },
                {
                        item_pike,            item_lumber,        1, 0,                 1,
                        0,             0,
                        sk_weaponsmith,       0, "made",     FALSE, 0,          0, 1, 1
                },
                {
                        item_longsword,       item_iron,          1, 0,                 1,
                        0,             0,
                        sk_weaponsmith,       0, "made",     FALSE, 0,          0, 1, 1
                },
                {
                        item_plate,           item_iron,          1, 0,                 1,
                        0,             0,
                        sk_weaponsmith,       0, "made",     FALSE, 0,          0, 1, 1
                },
                {
                        item_longbow,         item_yew,           1, 0,                 1,
                        0,             0,
                        sk_weaponsmith,       0, "made",     FALSE, 0,          0, 1, 1
                },
                {
                        item_drum,            item_mallorn_wood,  1, 0,                 1,
                        0,             0,
                        sk_summon_savage,     0, "made",     FALSE, 0,          0, 1, 1
                },
                {
                        item_leather,         item_ox,            1, 0,                 1,
                        0,             0,
                        0,                    0, "made",     FALSE, 0,          0, 1, 1
                },

/*
 *  Multi-day things
 */

                {
                        item_riding_horse,    item_wild_horse,    1, 0,                 1,
                        0,             0,
                        sk_train_wild,        0, "trained",  TRUE,  0,          0, 1, 5
                },
                {
                        item_warmount,        item_wild_horse,    1, 0,                 1,
                        0,             0,
                        sk_train_warmount,    0, "trained",  TRUE,  0,          0, 1, 10
                },
                {
                        item_new_wagon,       item_riding_horse,  2, item_lumber,       5,
                        0,             0,
                        sk_build_wagons,      0, "built",    TRUE,  0,          0, 1, 7
                },
                {
                        item_hvy_wagon,       item_riding_horse,  4, item_lumber,       10,
                        item_iron,     1,
                        sk_build_wagons,      0, "built",    TRUE,  0,          0, 1, 10
                },
                {
                        item_war_wagon,       item_riding_horse,  4, item_lumber,       10,
                        item_iron,     1,
                        sk_build_wagons,      0, "built",    TRUE,  0,          0, 1, 14
                },
                {
                        0,                  0,                    1, 0,                 1,
                        0,             0,
                        0,                    0, NULL, 0,           0,          0, 1, 1
                },
        };


static struct make *
find_make(int item) {
    int i;

    for (i = 0; make_tbl[i].item; i++) {
        if (make_tbl[i].item == item) {
            return &make_tbl[i];
        }
    }

    return NULL;
}


/*
 *  Make routine for things which take a day each
 */

int
v_generic_make(struct command *c, int number, struct make *t) {
    int where = subloc(c->who);
    int days = -1;        /* as long as it takes to get number */

/*
 *  Don't run forever for non-resource limited production
 */

    if (days < 1 && number == 0 && !t->inp1 && !t->inp2 && !t->inp3) {
        days = (MONTH_DAYS + 1) - sysclock.day;
    }

    c->c = number;        /* number desired; 0 means all possible */
    c->d = 0;        /* number we have obtained so far */

    if (t->req_skill && has_skill(c->who, t->req_skill) < 1) {
        wout(c->who, "Requires %s.", box_name(t->req_skill));
        return FALSE;
    }

    if (t->worker && has_item(c->who, t->worker) < 1) {
        wout(c->who, "Need at least one %s.", box_name(t->worker));
        return FALSE;
    }

    if (t->inp1 && has_item(c->who, t->inp1) < t->inp1_factor) {
        wout(c->who, "Don't have enough %s.", plural_item_box(t->inp1, 2));
        return FALSE;
    }

    if (t->inp2 && has_item(c->who, t->inp2) < t->inp2_factor) {
        wout(c->who, "Don't have enough %s.", plural_item_box(t->inp2, 2));
        return FALSE;
    }

    if (t->inp3 && has_item(c->who, t->inp3) < t->inp3_factor) {
        wout(c->who, "Don't have enough %s.", plural_item_box(t->inp3, 2));
        return FALSE;
    }

    if (t->where == WHERE_SHIP) {
        if (!is_ship(where) && !is_ship_notdone(where)) {
            wout(c->who, "Must be on a ship.");
            return FALSE;
        }
    }

    if (t->where > 0 && subkind(where) != t->where) {
        wout(c->who, "Must be in a %s.", subkind_s[t->where]);
        return FALSE;
    }

    if (t->aura && char_cur_aura(c->who) < t->aura) {
        wout(c->who, "Need at least %d aura.", t->aura);
        return FALSE;
    }

    c->wait = days;
    return TRUE;
}


int
d_generic_make(struct command *c, struct make *t) {
    int number = c->c;
    int qty;
    int a;

    if (t->worker == item_worker) {
        qty = effective_workers(c->who);
    } else if (t->worker) {
        qty = has_item(c->who, t->worker);
    } else {
        qty = 1;
    };

    if (get_effect(c->who, ef_improve_make, 0, t->item)) {
        /*
         *  What this should do is double the numer of items
         *  you could make if you had the materials.
         *
         */
        qty *= 2;
        wout(c->who, "You are unusually productive.", box_name(c->who));
    };

    if (a = has_artifact(c->who, ART_TRAINING, t->item, 0, 0)) {
        qty *= 2;
        wout(c->who, "Training is unusually productive.", box_name(c->who));
    };

    if (t->inp1) {
        qty = min(qty, has_item(c->who, t->inp1) / t->inp1_factor);
    }

    if (t->inp2) {
        qty = min(qty, has_item(c->who, t->inp2) / t->inp2_factor);
    }

    if (t->inp3) {
        qty = min(qty, has_item(c->who, t->inp3) / t->inp3_factor);
    }

    if (t->aura) {
        qty = min(qty, char_cur_aura(c->who));
    }

    if (qty > 0) {
        if (number > 0 && (c->d + qty > number)) {
            qty = number - c->d;
        }

        assert(qty >= 0);

        if (t->inp1) {
            consume_item(c->who, t->inp1, qty * t->inp1_factor);
        }

        if (t->inp2) {
            consume_item(c->who, t->inp2, qty * t->inp2_factor);
        }

        if (t->inp3) {
            consume_item(c->who, t->inp3, qty * t->inp3_factor);
        }

        if (t->aura) {
            deduct_aura(c->who, t->aura);
        }

        gen_item(c->who, t->item, qty * t->factor);
        c->d += qty;

        if (t->req_skill) {
            add_skill_experience(c->who, t->req_skill);
        }

#if 1
/*
 *  We want to continue production as long as:
 *
 *	The specified number of days, if given, has not elapsed
 *	The specified number of items to make has not yet been produced
 *	We still have raw materials to continue production
 */

        if ((t->inp1 == 0 || has_item(c->who, t->inp1) > 0) &&
            (t->inp2 == 0 || has_item(c->who, t->inp2) > 0) &&
            (t->inp3 == 0 || has_item(c->who, t->inp3) > 0) &&
            (c->wait != 0) &&
            !(number > 0 && c->d >= number)) {
            return TRUE;        /* not done yet */
        }
#else
        if (c->wait != 0 && !(number > 0 && c->d >= number))
            return TRUE;		/* not done yet */
#endif
    }

    return i_generic_make(c, t);
}


int i_generic_make(struct command *c, struct make *t) {
    int where = subloc(c->who);

    out(c->who, "%s %s.", cap(t->got_em),
        just_name_qty(t->item, c->d * t->factor));

    if (t->public) {
        out(where, "%s %s %s.",
            box_name(c->who),
            t->got_em,
            just_name_qty(t->item, c->d));
    }

    c->wait = 0;

    if (c->d > 0 && c->d >= c->c) {
        return TRUE;
    }
    return FALSE;
}


/*
 *  Make routine for things which take more than a day each
 */

int
v_second_make(struct command *c, int number, struct make *t) {
    int where = subloc(c->who);

    c->c = number;        /* number desired; 0 means all possible */
    c->d = 0;        /* number we have obtained so far */

    if (t->req_skill && has_skill(c->who, t->req_skill) < 1) {
        wout(c->who, "Requires %s.", box_name(t->req_skill));
        return FALSE;
    }

    if (t->inp1 && has_item(c->who, t->inp1) < t->inp1_factor) {
        wout(c->who, "Don't have enough %s.", plural_item_box(t->inp1, 2));
        return FALSE;
    }

    if (t->inp2 && has_item(c->who, t->inp2) < t->inp2_factor) {
        wout(c->who, "Don't have enough %s.", plural_item_box(t->inp2, 2));
        return FALSE;
    }

    if (t->inp3 && has_item(c->who, t->inp3) < t->inp3_factor) {
        wout(c->who, "Don't have enough %s.", plural_item_box(t->inp3, 2));
        return FALSE;
    }

    c->wait = t->days;
    c->poll = FALSE;

    if (t->req_skill) {
        c->use_exp = has_skill(c->who, t->req_skill);
        experience_use_speedup(c);
    }

    return TRUE;
}


int
d_second_make(struct command *c, struct make *t) {
    int number = c->c;

    if (t->inp1 && has_item(c->who, t->inp1) < t->inp1_factor) {
        wout(c->who, "Don't have %s.", box_name_qty(t->inp1, t->inp1_factor));
        return FALSE;
    }

    if (t->inp2 && has_item(c->who, t->inp2) < t->inp2_factor) {
        wout(c->who, "Don't have %s.", box_name_qty(t->inp2, t->inp2_factor));
        return FALSE;
    }

    if (t->inp3 && has_item(c->who, t->inp3) < t->inp3_factor) {
        wout(c->who, "Don't have %s.", box_name_qty(t->inp2, t->inp2_factor));
        return FALSE;
    }

    if (t->inp1) {
        consume_item(c->who, t->inp1, t->inp1_factor);
    }

    if (t->inp2) {
        consume_item(c->who, t->inp2, t->inp2_factor);
    }

    if (t->inp3) {
        consume_item(c->who, t->inp3, t->inp3_factor);
    }

    gen_item(c->who, t->item, 1);

    out(c->who, "%s %s.", cap(t->got_em), just_name_qty(t->item, 1));

    if (t->public) {
        out(subloc(c->who), "%s %s %s.",
            box_name(c->who),
            t->got_em,
            just_name_qty(t->item, 1));
    }

    c->d++;

    if (c->d < c->c) {
        c->wait = t->days;
    }

    if (t->req_skill) {
        add_skill_experience(c->who, t->req_skill);
    }

    return TRUE;
}


int
v_make(struct command *c) {
    int item = c->a;
    int number = c->b;
    struct make *t;

    t = find_make(item);

    if (t == NULL) {
        wout(c->who, "Don't know how to make %s.",
             box_code(item));
        return FALSE;
    }

    if (t->days == 1) {
        return v_generic_make(c, number, t);
    } else {
        return v_second_make(c, number, t);
    }
}


int
d_make(struct command *c) {
    int item = c->a;
    struct make *t;

    t = find_make(item);

    if (t == NULL) {
        out(c->who, "Internal error.");
        log_output(LOG_CODE, "d_make: t is NULL, who=%d", c->who);
        return FALSE;
    }

    if (t->days == 1) {
        return d_generic_make(c, t);
    } else {
        return d_second_make(c, t);
    }
}


int
i_make(struct command *c) {
    int item = c->a;
    struct make *t;

    t = find_make(item);

    if (t == NULL) {
        out(c->who, "Internal error.");
        log_output(LOG_CODE, "i_make: t is NULL, who=%d", c->who);
        return FALSE;
    }

    if (t->days == 1) {
        return i_generic_make(c, t);
    } else {
        return TRUE;
    }
}


int
v_use_train_riding(struct command *c) {
    int ret;

    ret = oly_parse(c,
                    sout("make %s %d", box_code_less(item_riding_horse), c->a));
    assert(ret);

    return v_make(c);
}


int
v_use_train_war(struct command *c) {
    int ret;

    ret = oly_parse(c,
                    sout("make %s %d", box_code_less(item_warmount), c->a));
    assert(ret);

    return v_make(c);
}

