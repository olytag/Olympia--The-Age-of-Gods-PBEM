
#include    <math.h>
#include    <stdio.h>
#include    "z.h"
#include    "oly.h"


int
list_gates_here(int who, int where, int show_dest) {
    int gate;
    int first = TRUE;
    char *sealed;
    char *dest;

    loop_gates_here(where, gate)
                {
                    if (first) {
                        out(who, "Gates here:");
                        indent += 3;
                        first = FALSE;
                    }

                    if (gate_seal(gate)) {
                        sealed = ", sealed";
                    } else {
                        sealed = "";
                    }

                    if (show_dest) {
                        dest = sout(", to %s", box_name(gate_dest(gate)));
                    } else {
                        dest = "";
                    }

                    out(who, "%s%s%s", box_name(gate), sealed, dest);
                    set_known(who, gate);
                }
    next_gate_here;

    if (first) {
        out(who, "There are no gates here.");
        return FALSE;
    }

    indent -= 3;
    return TRUE;
}


int
province_gate_here(int where) {
    int i;
    int gate = 0;

    loop_all_here(where, i)
            {
                if (kind(i) != T_gate) {
                    continue;
                }

                gate = i;
                break;
            }
    next_all_here;

    return gate;
}


static int
list_province_gates(int who, int where) {
    int gate;

    gate = province_gate_here(where);

    if (gate) {
        wout(who, "A gate exists somewhere in this province.");
    }

    return gate;
}


static void
list_nearby_gates(int who, int where) {
    int dist = gate_dist(where);

    if (dist == 0) {
        wout(who, "There are no nearby gates.");
    } else {
        wout(who, "The nearest gate is %s province%s away.",
             nice_num(dist), add_s(dist));
    }
}


static int
check_gate_here(int who, int gate) {

    if (kind(gate) != T_gate || subloc(gate) != subloc(who)) {
        wout(who, "There is no gate %s here.", box_code(gate));
        return FALSE;
    }

    return TRUE;
}


int
v_detect_gates(struct command *c) {

    if (!check_aura(c->who, 1)) {
        return FALSE;
    }

    return TRUE;
}


int
d_detect_gates(struct command *c) {
    int ret;

    if (!charge_aura(c->who, 1)) {
        return FALSE;
    }

    /*
     *  Tue Sep 29 07:22:38 1998 -- Scott Turner
     *
     *  Permit farcasting.
     *
     */
    /* ret = list_gates_here(c->who, subloc(c->who), TRUE); */
    ret = list_gates_here(c->who, cast_where(c->who), TRUE);


    if (ret == FALSE) {
        ret = list_province_gates(c->who, province(cast_where(c->who)));
    }

    if (ret == FALSE) {
        list_nearby_gates(c->who, province(cast_where(c->who)));
    }

    reset_cast_where(c->who);

    return TRUE;
}


void
do_jump(int who, int dest, int gate, int backwards) {
    struct entity_gate *p;

    leave_stack(who);
    wout(who, "Successful jump to %s.", box_name(dest));
    move_stack(who, dest);

#ifdef NOTYET
    departure message?
    arrival message?
#endif

    clear_guard_flag(who);

    if (gate) {
        assert(kind(gate) == T_gate);
        p = rp_gate(gate);

        if (p && kind(p->notify_jumps) == T_char) {
            wout(p->notify_jumps, "%s has jumped%s through %s.",
                 box_name(who),
                 backwards ? " backwards" : "",
                 box_name(gate));
        }
    }
}


int
v_jump_gate(struct command *c) {
    int gate = c->a;
    int key = c->b;
    int sealed;
    int dest;
    int cost;
    struct weights w;

    if (!check_gate_here(c->who, gate)) {
        return FALSE;
    }

    set_known(c->who, gate);

    sealed = gate_seal(gate);
    dest = gate_dest(gate);

    if (sealed > 0 && key == 0) {
        wout(c->who, "The gate is sealed.");
        return FALSE;
    }

    if (sealed > 0 && key != sealed) {
        wout(c->who, "Incorrect gate key.  Jump fails.");
        return TRUE;
    }

    determine_stack_weights(c->who, &w, FALSE);
    cost = (int) ceil((double) w.total_weight / 250.0);

    if (!charge_aura(c->who, cost)) {
        return FALSE;
    }

    assert(valid_box(dest));
    assert(is_loc_or_ship(dest));

    do_jump(c->who, dest, gate, FALSE);
    wout(c->who, "Cost %s aura to jump %s weight.",
         nice_num(cost), nice_num(w.total_weight));

    return TRUE;
}


int
v_reverse_jump(struct command *c) {
    int gate = c->a;
    int key = c->b;
    int sealed;
    int dest;
    int cost;
    struct weights w;

    if (kind(gate) != T_gate || gate_dest(gate) != subloc(c->who)) {
        wout(c->who, "No gate %s leads here.", box_code(gate));
        return FALSE;
    }

    sealed = gate_seal(gate);
    dest = subloc(gate);

    if (sealed > 0 && key == 0) {
        wout(c->who, "The gate is sealed.");
        return FALSE;
    }

    if (sealed > 0 && key != sealed) {
        wout(c->who, "Incorrect gate key.  Jump fails.");
        return TRUE;
    }

    determine_stack_weights(c->who, &w, FALSE);
    cost = 2 * ceil(w.total_weight / 250.0);

    if (!charge_aura(c->who, cost)) {
        return FALSE;
    }

    assert(valid_box(dest));
    assert(is_loc_or_ship(dest));

    do_jump(c->who, dest, gate, TRUE);
    wout(c->who, "Cost %s aura to jump %s weight.",
         nice_num(cost), nice_num(w.total_weight));

    return TRUE;
}


int
v_reveal_key(struct command *c) {
    if (!check_aura(c->who, 10)) {
        return FALSE;
    }

    wout(c->who, "Attempt to learn the key for %s.", box_name(c->a));
    return TRUE;
}


int
d_reveal_key(struct command *c) {
    int gate = c->a;
    int sealed;
    int where = cast_where(c->who);

    if (!charge_aura(c->who, 10)) {
        return FALSE;
    }

    reset_cast_where(c->who);

    if (kind(gate) != T_gate || subloc(gate) != where) {
        wout(c->who, "There is no gate %s at %s.", box_code(gate),
             box_code(where));
        return FALSE;
    };

    sealed = gate_seal(gate);

    if (sealed == 0) {
        wout(c->who, "%s is not sealed.", box_name(gate));
        return FALSE;
    }

    wout(c->who, "The key to %s is: %d", box_name(gate), sealed);
    return TRUE;
}


int
v_seal_gate(struct command *c) {
    int gate = c->a;
    int key = c->b;
    int sealed;

    if (!check_aura(c->who, 6)) {
        return FALSE;
    }

    if (key == 0) {
        wout(c->who, "Must specify a key to seal the gate with.");
        return FALSE;
    }

    if (key > 999) {
        wout(c->who, "The key must be between 1 and 999.");
        return FALSE;
    }

    return TRUE;
}


int
d_seal_gate(struct command *c) {
    int gate = c->a;
    int key = c->b;
    int sealed;
    int where = cast_where(c->who);

    if (!charge_aura(c->who, 6)) {
        return FALSE;
    }

    reset_cast_where(c->who);

    if (kind(gate) != T_gate || subloc(gate) != where) {
        wout(c->who, "There is no gate %s at %s.", box_code(gate),
             box_code(where));
        return FALSE;
    };

    sealed = gate_seal(gate);

    if (sealed != 0) {
        wout(c->who, "%s has been sealed by someone else.",
             box_name(gate));
        return FALSE;
    }

    p_gate(gate)->seal_key = key;

    wout(c->who, "Sealed %s with key %d.", box_name(gate), key);
    return TRUE;
}


static void
unseal_gate(int who, int gate) {
    struct entity_gate *p;

    assert(kind(gate) == T_gate);

    p = p_gate(gate);
    p->seal_key = 0;

    if (kind(p->notify_unseal) == T_char) {
        wout(p->notify_unseal, "%s has been unsealed by %s.",
             box_name(gate), box_name(who));
    }

    p->notify_unseal = 0;
}


int
v_unseal_gate(struct command *c) {
    if (!check_aura(c->who, 3)) {
        return FALSE;
    }

    if (c->b == 0) {
        wout(c->who, "Must specify a key to unseal the gate.");
        return FALSE;
    }

    return TRUE;
}


int
d_unseal_gate(struct command *c) {
    int gate = c->a;
    int key = c->b;
    int sealed;
    int where = cast_where(c->who);

    if (!charge_aura(c->who, 3)) {
        return FALSE;
    }

    reset_cast_where(c->who);

    if (kind(gate) != T_gate || subloc(gate) != where) {
        wout(c->who, "There is no gate %s at %s.", box_code(gate),
             box_code(where));
        return FALSE;
    };

    sealed = gate_seal(gate);

    if (sealed == 0) {
        wout(c->who, "The gate is not sealed.");
        return FALSE;
    }

    if (key != sealed) {
        wout(c->who, "Incorrect gate key.  Unseal fails.");
        return TRUE;
    }

    unseal_gate(c->who, gate);

    wout(c->who, "Successfully unsealed %s.", box_name(gate));

    return TRUE;
}


int
v_notify_unseal(struct command *c) {
    if (c->b == 0) {
        wout(c->who, "Must specify the gate seal.");
        return FALSE;
    }

    if (!check_aura(c->who, 5)) {
        return FALSE;
    }

    return TRUE;
}


int
d_notify_unseal(struct command *c) {
    int gate = c->a;
    int key = c->b;
    int sealed;
    int where = cast_where(c->who);

    if (!charge_aura(c->who, 5)) {
        return FALSE;
    }

    reset_cast_where(c->who);

    if (kind(gate) != T_gate || subloc(gate) != where) {
        wout(c->who, "There is no gate %s at %s.", box_code(gate),
             box_code(where));
        return FALSE;
    };

    sealed = gate_seal(gate);

    if (sealed == 0) {
        wout(c->who, "The gate is not sealed.");
        return FALSE;
    }

    if (key != sealed) {
        wout(c->who, "Incorrect gate key.  Spell fails.");
        return TRUE;
    }

    assert(kind(gate) == T_gate);
    p_gate(gate)->notify_unseal = c->who;

    wout(c->who, "Notification spell successfully cast.");
    return TRUE;
}


int
v_rem_seal(struct command *c) {
    if (!check_aura(c->who, 8)) {
        return FALSE;
    }

    return TRUE;
}


int
d_rem_seal(struct command *c) {
    int gate = c->a;
    int sealed;
    int where = cast_where(c->who);

    if (!charge_aura(c->who, 8)) {
        return FALSE;
    }

    reset_cast_where(c->who);

    if (kind(gate) != T_gate || subloc(gate) != where) {
        wout(c->who, "There is no gate %s at %s.", box_code(gate),
             box_code(where));
        return FALSE;
    };

    sealed = gate_seal(gate);

    if (sealed == 0) {
        wout(c->who, "The gate is not sealed.");
        return FALSE;
    }

    unseal_gate(c->who, gate);

    wout(c->who, "Unsealed %s.", box_name(gate));

    return TRUE;
}


int
v_notify_jump(struct command *c) {
    if (!check_aura(c->who, 6)) {
        return FALSE;
    }

    return TRUE;
}


int
d_notify_jump(struct command *c) {
    int gate = c->a;
    int where = cast_where(c->who);

    if (!charge_aura(c->who, 6)) {
        return FALSE;
    }

    reset_cast_where(c->who);

    if (kind(gate) != T_gate || subloc(gate) != where) {
        wout(c->who, "There is no gate %s at %s.", box_code(gate),
             box_code(where));
        return FALSE;
    };

    p_gate(gate)->notify_jumps = c->who;

    wout(c->who, "Notification spell successfully cast.");
    return TRUE;
}


int
v_teleport(struct command *c) {
    int dest = c->a;
    int cost;
    struct weights w;

    if (!is_loc_or_ship(dest)) {
        wout(c->who, "There is no location %s.", c->parse[1]);
        return FALSE;
    }

    determine_stack_weights(c->who, &w, FALSE);
    cost = w.total_weight / 250;

    if (has_item(c->who, item_gate_crystal) < 1) {
        wout(c->who, "Teleportation requires %s.",
             box_name_qty(item_gate_crystal, 1));
        return FALSE;
    }

    if (!charge_aura(c->who, cost)) {
        return FALSE;
    }

    consume_item(c->who, item_gate_crystal, 1);

    do_jump(c->who, dest, 0, FALSE);

    return TRUE;
}

