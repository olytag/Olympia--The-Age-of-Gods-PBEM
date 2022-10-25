// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

/*
 *  Mon Oct 26 18:14:08 1998 -- Scott Turner
 *
 *  Revised Artifact Construction skills, using the new-style artifacts.
 *
 *  Detect Artifact
 *  Reveal Artifact
 *
 *  Mutate Artifact
 *  Destroy Artifact
 *
 *  Conceal Artifact
 *  Obscure Artifact
 *  Remove Obscurity
 *  Disguise Artifact?
 *  Remove Disguise?
 *
 *  Curse Artifact?
 *  Detect Cursed Artifact?
 *  Remove Curse?
 *
 *  Teleport Through Artifact
 *
 */
#include <stdio.h>
#include "z.h"
#include "oly.h"
#include "forward.h"
#include "vectors/cs_list.h"
#include "vectors/item_ent_list.h"


int
has_auraculum(int who) {
    int ac;

    ac = char_auraculum(who);

    if (ac && has_item(who, ac) > 0) {
        return ac;
    }

    return 0;
}


/*
 *  Maximum aura, innate plus the auraculum bonus
 */

int
max_eff_aura(int who) {
    int a;                /* aura */
    int ac;                /* auraculum */

    a = char_max_aura(who);
    if (ac = has_auraculum(who)) {
        a += p_item_artifact(ac)->param2;
    };

    {
        struct item_ent *e;
        int n;

        inventory_loop(who, e)
                    {
                        if (n = item_aura_bonus(e->item)) {
                            a += n;
                        }
                    }
        inventory_next;
    }

    return a;
}

/*
 *  Tue Oct 27 06:55:54 1998 -- Scott Turner
 *
 *  Modified for new-style artifacts.
 *
 */
int
v_forge_palantir(struct command *c) {
    if (!check_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };

    wout(c->who, "Attempt to create a palantir.");
    return TRUE;
}

int
d_forge_palantir(struct command *c) {
    int new;
    struct entity_item *p;
    struct item_magic *pm;

    new = create_unique_item(c->who, sub_magic_artifact);

    if (new < 0) {
        wout(c->who, "Spell failed.");
        return FALSE;
    }

    if (!charge_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };
    wout(c->who, "Used %s aura casting this spell.", nice_num(skill_aura(c->use_skill)));

    set_name(new, "Palantir");
    p = p_item(new);
    p->weight = 2;
    p_item_artifact(new)->type = ART_ORB;
    p_item_artifact(new)->param1 = 0;
    p_item_artifact(new)->param2 = 0;
    p_item_artifact(new)->uses = rnd(3, 9);

    wout(c->who, "Created %s.", box_name(new));
    log_output(LOG_SPECIAL, "%s created %s.", box_name(c->who), box_name(new));
    return TRUE;
}


/*
 *  v_use_palantir is replaced with v_art_orb.
 *
 */
int
v_use_palantir(struct command *c) {
    int item = c->a;
    int target = c->b;
    struct item_magic *p;

    if (!is_loc_or_ship(target)) {
        wout(c->who, "%s is not a location.", box_code(target));
        return FALSE;
    }

    p = rp_item_magic(item);

    if (p && p->one_turn_use) {
        wout(c->who, "The palantir may only be used once per month.");
        return FALSE;
    }

    wout(c->who, "Will attempt to view %s with the palantir.",
         box_code(target));

    c->wait = 7;

    return TRUE;
}


int
d_use_palantir(struct command *c) {
    int item = c->a;
    int target = c->b;

    if (!is_loc_or_ship(target)) {
        wout(c->who, "%s is not a location.", box_code(target));
        return FALSE;
    }

    if (loc_shroud(target)) {
        log_output(LOG_CODE, "Murky palantir result, who=%s, targ=%s",
                   box_code_less(c->who), box_code_less(target));
        wout(c->who, "Only murky, indistinct images are seen in "
                     "the palantir.");
        return FALSE;
    }

    log_output(LOG_CODE, "Palantir scry, who=%s, targ=%s",
               box_code_less(c->who), box_code_less(target));

    p_item_magic(item)->one_turn_use++;

    wout(c->who, "A vision of %s appears:", box_name(target));
    out(c->who, "");
    show_loc(c->who, target);

    alert_palantir_scry(c->who, target);

    return TRUE;
}

int
v_destroy_art(struct command *c) {
    int item = c->a;

    if (has_item(c->who, item) < 1) {
        wout(c->who, "%s does not have %s.",
             box_name(c->who),
             box_code(item));
        return FALSE;
    }

    if (!is_artifact(item)) {
        wout(c->who, "Cannot destroy %s with this spell.",
             box_name(item));
        return FALSE;
    }

    if (!check_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };

    wout(c->who, "Attempt to destroy %s.", box_name(item));
    return TRUE;
}

int
d_destroy_art(struct command *c) {
    int item = c->a;
    int aura;

    if (has_item(c->who, item) < 0) {
        wout(c->who, "%s does not have %s.",
             box_name(c->who),
             box_code(item));
        return FALSE;
    }

    if (!is_artifact(item)) {
        wout(c->who, "Cannot destroy %s with this spell.",
             box_name(item));
        return FALSE;
    }

    if (!charge_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };
    wout(c->who, "Used %s aura casting this spell.", nice_num(skill_aura(c->use_skill)));

    aura = rnd(1, 20);
    if (rp_item_artifact(item)->param2 & CA_N_MELEE ||
        rp_item_artifact(item)->param2 & CA_N_MISSILE ||
        rp_item_artifact(item)->param2 & CA_N_SPECIAL ||
        rp_item_artifact(item)->param2 & CA_M_MELEE ||
        rp_item_artifact(item)->param2 & CA_M_SPECIAL ||
        rp_item_artifact(item)->param2 & CA_N_MELEE_D ||
        rp_item_artifact(item)->param2 & CA_N_MISSILE_D ||
        rp_item_artifact(item)->param2 & CA_N_SPECIAL_D ||
        rp_item_artifact(item)->param2 & CA_M_MELEE_D ||
        rp_item_artifact(item)->param2 & CA_M_MISSILE_D ||
        rp_item_artifact(item)->param2 & CA_M_SPECIAL_D) {
        aura = rp_item_artifact(item)->param1 / 5;
    }
    if (rp_item_artifact(item)->type == ART_AURACULUM) {
        aura = rp_item_artifact(item)->param2 / 2;
    }
    if (rp_item_artifact(item)->type == ART_ORB) {
        aura = rnd(1, 8);
    };

    if (aura > 20) { aura = 20; }

    add_aura(c->who, aura);
    wout(c->who, "You gain %s aura from the destroyed artifact.",
         nice_num(char_cur_aura(c->who)));

    log_output(LOG_SPECIAL, "%s destroyed %s.",
               box_name(c->who), box_name(item));

    destroy_unique_item(c->who, item);

    return 0; // todo: should this return something?
};

int
v_mutate_art(struct command *c) {
    int item = c->a;

    if (has_item(c->who, item) < 1) {
        wout(c->who, "%s does not have %s.",
             box_name(c->who),
             box_code(item));
        return FALSE;
    }

    if (!is_artifact(item)) {
        wout(c->who, "Cannot mutate %s with this spell.",
             box_name(item));
        return FALSE;
    }

    if (rp_item_artifact(item)->type == ART_COMBAT ||
        rp_item_artifact(item)->type == ART_AURACULUM ||
        rp_item_artifact(item)->type == ART_ORB) {
        wout(c->who, "%s is not a mutable artifact.",
             box_name(item));
        return FALSE;
    };

    if (!check_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };

    return TRUE;
}

int
d_mutate_art(struct command *c) {
    int item = c->a;
    int aura, new;

    if (has_item(c->who, item) < 0) {
        wout(c->who, "%s does not have %s.",
             box_name(c->who),
             box_code(item));
        return FALSE;
    }

    if (!is_artifact(item)) {
        wout(c->who, "Cannot mutate %s with this spell.",
             box_name(item));
        return FALSE;
    }

    if (rp_item_artifact(item)->type == ART_COMBAT ||
        rp_item_artifact(item)->type == ART_AURACULUM ||
        rp_item_artifact(item)->type == ART_ORB) {
        wout(c->who, "%s is not a mutable artifact.",
             box_name(item));
        return FALSE;
    };

    if (!charge_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };
    wout(c->who, "Used %s aura casting this spell.", nice_num(skill_aura(c->use_skill)));

    new = create_random_artifact(c->who);
    wout(c->who, "%s mutates into %s!", box_name(item), box_name(new));
    destroy_unique_item(c->who, item);
    return TRUE;
};

/*
 *  Tue Oct 27 11:26:28 1998 -- Scott Turner
 *
 *  Conceal artifacts.
 *
 */
int
v_conceal_arts(struct command *c) {
    int target = c->a;

    if (!cast_check_char_here(c->who, target)) {
        wout(c->who, "Cannot cast on that target.");
        return FALSE;
    };

    if (!check_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };

    return TRUE;
}

int
d_conceal_arts(struct command *c) {
    int target = c->a;

    if (!cast_check_char_here(c->who, target)) {
        wout(c->who, "Cannot cast on that target.");
        return FALSE;
    };

    if (!charge_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };
    wout(c->who, "Used %s aura casting this spell.", nice_num(skill_aura(c->use_skill)));

    if (!add_effect(target, ef_conceal_artifacts, 0, 30, 1)) {
        wout(c->who, "For some odd reason, your spell fails.");
        return FALSE;
    };
    wout(c->who, "%s now has artifacts concealed for 30 days.",
         box_name(target));

    reset_cast_where(c->who);
    return TRUE;
};

/*
 *  Tue Oct 27 11:26:28 1998 -- Scott Turner
 *
 *  Reveal artifacts.
 *
 */
int
v_reveal_arts(struct command *c) {
    int target = c->a;

    if (!cast_check_char_here(c->who, target)) {
        wout(c->who, "Cannot cast on that target.");
        return FALSE;
    };

    if (!check_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };

    return TRUE;
}

int
d_reveal_arts(struct command *c) {
    int target = c->a;
    int num = 0;
    struct item_ent *e;

    if (!cast_check_char_here(c->who, target)) {
        wout(c->who, "Cannot cast on that target.");
        return FALSE;
    };

    if (!charge_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };
    wout(c->who, "Used %s aura casting this spell.", nice_num(skill_aura(c->use_skill)));

    if (get_effect(target, ef_conceal_artifacts, 0, 0)) {
        wout(c->who, "%s is carrying no artifacts.", box_name(target));
        return TRUE;
    };

    inventory_loop(target, e)
                {
                    if (item_unique(e->item) &&
                        is_artifact(e->item)) {
                        wout(c->who, "%s is carrying an artifact %s.", box_name(target),
                             box_name(e->item));
                        num++;
                    };
                }
    inventory_next;

    if (!num) {
        wout(c->who, "%s is carrying no artifacts.", box_name(target));
        return TRUE;
    };

    reset_cast_where(c->who);
    return TRUE;
};

/*
 *  Tue Oct 27 12:19:41 1998 -- Scott Turner
 *
 *  Deep identify ignores obscurity.
 *
 */
int
v_deep_identify(struct command *c) {
    int target = c->a;

    if (!charge_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };
    wout(c->who, "Used %s aura casting this spell.", nice_num(skill_aura(c->use_skill)));

    if (!valid_box(target) ||
        !is_artifact(target) ||
        !has_item(c->who, target) ||
        get_effect(target, ef_obscure_artifact, 0, 0)) {
        wout(c->who, "You are unable to identify that item.");
        return TRUE;
    };

    artifact_identify("You study the aura of this artifact "
                      "and identify it as: ", c);

    return 0; // todo: should this return something?
};

/*
 *  Tue Oct 27 11:26:28 1998 -- Scott Turner
 *
 *  Obscure an artifact.
 *
 */
int
v_obscure_art(struct command *c) {
    int target = c->a;

    if (!has_item(c->who, target)) {
        wout(c->who, "You must possess an artifact to obscure it.");
        return FALSE;
    };

    if (!check_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };

    return TRUE;
}

int
d_obscure_art(struct command *c) {
    int target = c->a;

    if (!has_item(c->who, target)) {
        wout(c->who, "You must possess an artifact to obscure it.");
        return FALSE;
    };

    if (!charge_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };
    wout(c->who, "Used %s aura casting this spell.", nice_num(skill_aura(c->use_skill)));

    if (!add_effect(target, ef_obscure_artifact, 0, -1, 1)) {
        wout(c->who, "For some odd reason, your spell fails.");
        return FALSE;
    };

    wout(c->who, "%s is now permanently obscured.",
         box_name(target));

    return TRUE;
};

/*
 *  Tue Oct 27 11:26:28 1998 -- Scott Turner
 *
 *  Remove an obscurity.
 *
 */
int
v_unobscure_art(struct command *c) {
    int target = c->a;

    if (!has_item(c->who, target)) {
        wout(c->who, "You must possess an artifact to remove an obscurity.");
        return FALSE;
    };

    if (!check_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };

    return TRUE;
}

int
d_unobscure_art(struct command *c) {
    int target = c->a;

    if (!has_item(c->who, target)) {
        wout(c->who, "You must possess an artifact to remove an obscurity.");
        return FALSE;
    };

    if (!charge_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };
    wout(c->who, "Used %s aura casting this spell.", nice_num(skill_aura(c->use_skill)));

    delete_effect(target, ef_obscure_artifact, 0);

    wout(c->who, "Remove obscurity cast upon %s.",
         box_name(target));

    return TRUE;
};

/*
 *  Tue Oct 27 11:34:53 1998 -- Scott Turner
 *
 *  This function is needed for detecting artifacts.
 *  Ignore concealed artifacts.
 */
int
find_nearest_artifact(int who) {
    int distance = 9999, d, i;
    int where = province(who);

    if (region(where) == faery_region ||
        region(where) == hades_region ||
        region(where) == cloud_region) {
        return -1;
    }

    loop_artifact(i)
            {
                if (region(item_unique(i)) == faery_region ||
                    region(item_unique(i)) == hades_region ||
                    region(item_unique(i)) == cloud_region) {
                    continue;
                }
                /*
                 *  Might be concealed.
                 *
                 */
                if (item_unique(i) &&
                    get_effect(item_unique(i), ef_conceal_artifacts, 0, 0)) {
                    continue;
                }
                d = los_province_distance(item_unique(i), where);
                if (d < distance) {
                    distance = d;
                };
            }next_artifact;

    return distance;
};

/*
 *  Tue Oct 27 11:26:28 1998 -- Scott Turner
 *
 *  Detect artifacts.
 *
 */
int
v_detect_arts(struct command *c) {
    if (region(c->who) == faery_region ||
        region(c->who) == hades_region ||
        region(c->who) == cloud_region) {
        wout(c->who, "Your magic does not work in this place.");
        return FALSE;
    };

    if (!check_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };

    return TRUE;
}

int
d_detect_arts(struct command *c) {
    int distance;

    if (region(c->who) == faery_region ||
        region(c->who) == hades_region ||
        region(c->who) == cloud_region) {
        wout(c->who, "Your magic does not work in this place.");
        return FALSE;
    };

    if (!charge_aura(c->who, skill_aura(c->use_skill))) {
        return FALSE;
    };
    wout(c->who, "Used %s aura casting this spell.", nice_num(skill_aura(c->use_skill)));

    distance = find_nearest_artifact(c->who);
    if (distance == -1) {
        wout(c->who, "The nearest artifact is very distant.");
    } else {
        wout(c->who, "The nearest artifact is %s province%s away.",
             nice_num(distance), distance == 1 ? "" : "s");
    };

    return TRUE;
};

int
v_forge_aura(struct command *c) {
    int aura;

    if (char_auraculum(c->who)) {
        wout(c->who, "%s may only be used once.",
             box_name(c->use_skill));
        return FALSE;
    }

    if (c->a < 1) {
        c->a = 1;
    }
    aura = c->a;

    if (!check_aura(c->who, aura)) {
        return FALSE;
    }

    if (aura > char_max_aura(c->who)) {
        wout(c->who, "The specified amount of aura exceeds the "
                     "maximum aura level of %s.", box_name(c->who));
        return FALSE;
    }

    wout(c->who, "Attempt to forge an auraculum.");
    return TRUE;
}

static void
notify_others_auraculum(int who, int item) {
    int n;

    loop_char(n)
            {
                if (n != who && is_magician(n) && has_auraculum(n)) {
                    wout(n, "%s has constructed an auraculum.",
                         box_name(who));
                }
            }
    next_char;

    log_output(LOG_SPECIAL, "%s created %s, %s.",
               box_name(who),
               box_name(item),
               subkind_s[subkind(item)]);
}

/*
 *  Mon Oct 26 10:31:46 1998 -- Scott Turner
 *
 *  Modified to create a new-style artifact.
 *
 */
int
d_forge_aura(struct command *c) {
    int aura = c->a;
    char *new_name;
    int new;
    struct entity_item *p;
    struct char_magic *cm;

    if (aura > char_max_aura(c->who)) {
        wout(c->who, "The specified amount of aura exceeds the "
                     "maximum aura level of %s.", box_name(c->who));
        return FALSE;
    }

    if (!charge_aura(c->who, aura)) {
        return FALSE;
    }

    if (numargs(c) < 2) {
        switch (rnd(1, 3)) {
            case 1:
                new_name = "Gold ring";
                break;
            case 2:
                new_name = "Wooden staff";
                break;
            case 3:
                new_name = "Jeweled crown";
                break;
            default:
                assert(FALSE);
        }
    } else {
        new_name = str_save(c->parse[2]);
    };

    new = create_unique_item(c->who, sub_magic_artifact);

    if (new < 0) {
        wout(c->who, "Spell failed.");
        return FALSE;
    }

    set_name(new, new_name);

    p = p_item(new);
    p->weight = rnd(1, 3);

    p_item_artifact(new)->type = ART_AURACULUM;
    p_item_artifact(new)->param1 = c->who;  /* creator */
    p_item_artifact(new)->param2 = aura * 2;  /* aura */

    cm = p_magic(c->who);
    cm->auraculum = new;
    cm->max_aura -= aura;

    wout(c->who, "Created %s.", box_name(new));
    notify_others_auraculum(c->who, new);

    learn_skill(c->who, sk_adv_sorcery);

    return TRUE;
}

int
v_forge_art_x(struct command *c) {
    int aura = c->a;
    int rare_item;

    if (aura < 1) {
        c->a = aura = 1;
    }
    if (aura > 20) {
        c->a = aura = 20;
    }

    if (!check_aura(c->who, aura)) {
        return FALSE;
    }

    if (!can_pay(c->who, 500)) {
        wout(c->who, "Requires %s.", gold_s(500));
        return FALSE;
    }

    switch (c->use_skill) {
        case sk_forge_weapon:
        case sk_forge_armor:
            rare_item = item_mithril;
            break;

        case sk_forge_bow:
            rare_item = item_mallorn_wood;
            break;

        default:
            assert(FALSE);
    }
    c->d = rare_item;

    if (!has_item(c->who, rare_item) >= 1) { // todo: should be !(... >= 1), maybe?
        wout(c->who, "Requires %s.", box_name_qty(rare_item, 1));
        return FALSE;
    }

    return TRUE;
}


int
d_forge_art_x(struct command *c) {
    int new;
    int aura = c->a;
    int rare_item = c->d;
    char *new_name;
    struct item_magic *pm;

    if (!check_aura(c->who, aura)) {
        return FALSE;
    }

    if (!charge(c->who, 500)) {
        wout(c->who, "Requires %s.", gold_s(500));
        return FALSE;
    }

    if (!has_item(c->who, rare_item) >= 1) { // todo: should be !(... >= 1), maybe?
        wout(c->who, "Requires %s.", box_name_qty(rare_item, 1));
        return FALSE;
    }

    charge_aura(c->who, aura);
    charge(c->who, 500);
    consume_item(c->who, rare_item, 1);

    new = create_unique_item(c->who, sub_magic_artifact);
    p_item(new)->weight = 10;
    p_item_artifact(new)->type = ART_COMBAT;

    switch (c->use_skill) {
        case sk_forge_weapon:
            rp_item_artifact(new)->param2 = CA_N_MELEE;
            rp_item_artifact(new)->param1 = aura * 5;
            new_name = "enchanted sword";
            break;

        case sk_forge_armor:
            rp_item_artifact(new)->param2 = CA_N_MELEE_D | CA_N_MISSILE_D | CA_N_SPECIAL_D;
            rp_item_artifact(new)->param1 = aura * 5;
            new_name = "enchanted armor";
            break;

        case sk_forge_bow:
            rp_item_artifact(new)->param2 = CA_N_MISSILE;
            rp_item_artifact(new)->param1 = aura * 5;
            new_name = "enchanted bow";
            break;

        default:
            assert(FALSE);
    }

    if (numargs(c) >= 2 && c->parse[2] && *(c->parse[2])) {
        new_name = c->parse[2];
    }

    set_name(new, new_name);
    wout(c->who, "Created %s.", box_name(new));

    return TRUE;
}

int
new_suffuse_ring(int who) {
    int new;
    int ni;
    int lore;

    new = create_unique_item(who, sub_magic_artifact);
    set_name(new, "Golden ring");
    p_item(new)->weight = 1;
    p_item_artifact(new)->type = ART_DESTROY;
    rp_item_artifact(new)->uses = 1;
    rp_item_artifact(new)->param1 = random_beast(0);
    return new;
}

/* Temporary */

static ilist orb_used_this_month = NULL;

int
v_use_orb(struct command *c) {
    int item = c->a;
    int target = c->b;
    int where = 0;
    int owner;
    struct item_magic *p;

    if (ilist_lookup(orb_used_this_month, item) >= 0) {
        wout(c->who, "The orb may only be used once per month.");
        wout(c->who, "Only murky, indistinct images are seen in the orb.");
        return FALSE;
    }

    ilist_append(&orb_used_this_month, item);

    if (rnd(1, 3) == 1) {
        wout(c->who, "Only murky, indistinct images are seen in the orb.");
        return FALSE;
    }

    switch (kind(target)) {
        case T_loc:
        case T_ship:
            where = province(target);
            break;

        case T_char:
            where = province(target);
            break;

        case T_item:
            if (owner = item_unique(target)) {
                where = province(owner);
            }
            break;
    }

    if (where == 0) {
        wout(c->who, "The orb is unsure what location is meant to be scried.");
    } else if (loc_shroud(where)) {
        wout(c->who, "The orb is unable to penetrate a shroud over %s.",
             box_name(where));
    } else {
        wout(c->who, "A vision of %s appears:", box_name(where));
        show_loc(c->who, where);
        alert_scry_generic(c->who, where);
    }

    p = p_item_magic(item);

    p->orb_use_count--;
    if (p->orb_use_count <= 0) {
        wout(c->who, "After the vision fades, the orb grows "
                     "dark, and shatters.  The orb is gone");
        destroy_unique_item(c->who, item);
    }

    return TRUE;
}
