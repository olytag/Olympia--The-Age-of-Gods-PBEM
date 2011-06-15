
#include	<stdio.h>
#include	"z.h"
#include	"oly.h"


int
new_potion(int who)
{
	char *s;
	int new;
	struct item_magic *p;

	new = create_unique_item(who, 0);

	if (new < 0)
		return -1;

	switch (rnd(1,2))
	{
	case 1:
		s = "Magic potion";
		break;

	case 2:
		s = "Strange potion";
		break;

	default:
		assert(FALSE);
	}

	set_name(new, s);
	p = p_item_magic(new);
	p->creator = who;
	p_item(new)->weight = 1;

	wout(who, "Produced one %s", box_name(new));

	return new;
}


int
d_brew_slave(struct command *c)
{
	int new;

	new = new_potion(c->who);

	if (new < 0)
	{
		wout(c->who, "Attempt to brew potion failed.");
		return FALSE;
	}

	p_item_magic(new)->use_key = use_slave_potion;

	return TRUE;
}


int
d_brew_death(struct command *c)
{
	int new;

	new = new_potion(c->who);

	if (new < 0)
	{
		wout(c->who, "Attempt to brew potion failed.");
		return FALSE;
	}

	p_item_magic(new)->use_key = use_death_potion;

	return TRUE;
}


int
v_brew(struct command *c)
{

	return TRUE;
}


int
d_brew_heal(struct command *c)
{
	int new;

	new = new_potion(c->who);

	if (new < 0)
	{
		wout(c->who, "Attempt to brew potion failed.");
		return FALSE;
	}

	p_item_magic(new)->use_key = use_heal_potion;

	return TRUE;
}

int
d_brew_weightlessness(struct command *c)
{
	int new;

	new = new_potion(c->who);

	if (new < 0)
	{
		wout(c->who, "Attempt to brew potion failed.");
		return FALSE;
	}

	p_item_magic(new)->use_key = use_weightlessness_potion;

	return TRUE;
}

int
d_brew_fiery(struct command *c)
{
	int new;

	new = new_potion(c->who);

	if (new < 0)
	{
		wout(c->who, "Attempt to brew potion failed.");
		return FALSE;
	}

	p_item_magic(new)->use_key = use_fiery_potion;

	return TRUE;
}

int
v_use_heal(struct command *c)
{
	int item = c->a;

	assert(kind(item) == T_item);

	wout(c->who, "%s drinks the potion...", just_name(c->who));

	if (char_health(c->who) == 100)
	{
		wout(c->who, "Nothing happens.");
		destroy_unique_item(c->who, item);
		return TRUE;
	}

	wout(c->who, "%s is immediately healed of all wounds!",
				just_name(c->who));

	p_char(c->who)->sick = FALSE;
	rp_char(c->who)->health = 100;

	destroy_unique_item(c->who, item);

	return TRUE;
}


int
v_use_death(struct command *c)
{
	int item = c->a;

	assert(kind(item) == T_item);

	wout(c->who, "%s drinks the potion...", just_name(c->who));
	destroy_unique_item(c->who, item);

	wout(c->who, "It's poison!");

	p_char(c->who)->sick = TRUE;
	add_char_damage(c->who, 50, MATES);

	return TRUE;
}

int
v_use_fiery(struct command *c)
{
	int item = c->a;

	assert(kind(item) == T_item);

	wout(c->who, "%s drinks the potion...", just_name(c->who));
	destroy_unique_item(c->who, item);

	wout(c->who, "It burns horribly!");

	add_char_damage(c->who, 10+rnd(1,10), MATES);

	return TRUE;
}

int
v_use_slave(struct command *c)
{
	int item = c->a;
	int creator;

	assert(kind(item) == T_item);
	creator = item_creator(item);

/*
 *  NOTYET:
 *	should be log_code
 *	must take into account different loyalties
 *	percentage chance?
 *
 *	5	0
 *	4	15	or death?
 *	3	30
 *	2	60
 *	1	90
 */

	log_output(LOG_SPECIAL, "%s drinks a slavery potion to %s\n",
				box_name(c->who), box_name(creator));

	wout(c->who, "%s drinks the potion...", just_name(c->who));

	if (!valid_box(creator) || kind(creator) != T_char ||
	    get_effect(c->who, ef_guard_loyalty, 0, 0)) {
		wout(c->who, "Nothing happens.");
	} else if (unit_deserts(c->who, creator, TRUE, LOY_contract, 250)) {
	  wout(c->who, "%s is suddenly overcome with an irresistible "
	       "desire to serve %s.",
	       just_name(c->who),
	       box_name(creator));
	} else {
	  wout(c->who, "Nothing happens.");
	};

	destroy_unique_item(c->who, item);
	return TRUE;
}


int
v_lead_to_gold(struct command *c)
{
	int amount = c->a;
	int qty;

	if (has_item(c->who, item_farrenstone) < 1)
	{
	    wout(c->who, "Requires %s.", box_name_qty(item_farrenstone, 1));
	    return FALSE;
	}

	qty = has_item(c->who, item_lead);

	if (amount == 0)
		amount = qty;

	if (amount > qty)
		amount = qty;

	qty = min(qty, 20);

	if (qty == 0)
	{
		wout(c->who, "Don't have any %s.", box_name(item_lead));
		return FALSE;
	}

	c->d = qty;

	return TRUE;
}


int
d_lead_to_gold(struct command *c)
{
	int qty = c->d;
	int has = has_item(c->who, item_lead);
	extern int gold_lead_to_gold;

	if (has_item(c->who, item_farrenstone) < 1)
	{
	    wout(c->who, "Requires %s.", box_name_qty(item_farrenstone, 1));
	    return FALSE;
	}

	if (has < qty)
		qty = has;

	if (qty == 0)
	{
		wout(c->who, "Don't have any %s.", box_name(item_lead));
		return FALSE;
	}

	wout(c->who, "Turned %s into %s.", just_name_qty(item_lead, qty),
				just_name_qty(item_gold, qty*10));

	consume_item(c->who, item_lead, qty);
	consume_item(c->who, item_farrenstone, 1);
	gen_item(c->who, item_gold, qty * 10);
	gold_lead_to_gold += 100;

	return TRUE;
}

int
v_use_weightlessness(struct command *c)
{
  int item = c->a;

  assert(kind(item) == T_item);

  wout(c->who, "%s drinks the potion...", just_name(c->who));

  if (get_effect(c->who, ef_weightlessness, 0, 0)) {
    wout(c->who, "%s is already weightless.", just_name(c->who));
    destroy_unique_item(c->who, item);
    return TRUE;
  };

  if (!add_effect(c->who, ef_weightlessness, 0, 7, 1)) {
    wout(c->who,"Oddly enough, the potion has no effect.");
  } else {
    wout(c->who,"%s feels himself become weightless!", just_name(c->who));
  };
  destroy_unique_item(c->who, item);

  return TRUE;
}

