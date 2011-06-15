/*
 *  Advanced sorcery
 */

#include	<stdio.h>
#include	"z.h"
#include	"oly.h"


int
v_trance(struct command *c)
{

	if (has_skill(c->who, sk_trance) < 1)
	{
		wout(c->who, "Requires knowledge of %s.", box_name(sk_trance));
		return FALSE;
	}

	return TRUE;
}


int
d_trance(struct command *c)
{
	struct char_magic *p;

	p = p_magic(c->who);

	p->cur_aura = max_eff_aura(c->who);
	wout(c->who, "Current aura is now %d.", p->cur_aura);

	if (char_health(c->who) < 100 || char_sick(c->who))
	{
		p_char(c->who)->sick = FALSE;
		rp_char(c->who)->health = 100;

		wout(c->who, "%s is fully healed.", box_name(c->who));
	}

	return TRUE;
}


int
v_teleport_item(struct command *c)
{

	return TRUE;
}


/*
 *  give <who> <what> [qty] [have-left]
 */

int
d_teleport_item(struct command *c)
{
	int target = c->a;
	int item = c->b;
	int qty = c->c;
	int have_left = c->d;
	int ret;
	int aura;

	if (kind(target) != T_char)
	{
		wout(c->who, "%s is not a character.", box_code(target));
		return FALSE;
	}

	if (is_prisoner(target))
	{
		wout(c->who, "Prisoners may not be given anything.");
		return FALSE;
	}

	if (kind(item) != T_item)
	{
		wout(c->who, "%s is not an item.", box_code(item));
		return FALSE;
	}

	if (has_item(c->who, item) < 1)
	{
		wout(c->who, "%s does not have any %s.", box_name(c->who),
						box_code(item));
		return FALSE;
	}

	if (rp_item(item)->ungiveable) {
	  wout(c->who,"You cannot teleport %s to another noble.",
	       plural_item_name(item, 2));
	  return FALSE;
	};

	if (crosses_ocean(target,c->who)) {
	  wout(c->who,"Something seems to block your magic.");
	  return FALSE;
	};

	qty = how_many(c->who, c->who, item, qty, have_left);

	if (qty <= 0)
		return FALSE;

	aura = 3 + item_weight(item) * qty / 250;

	if (!check_aura(c->who, aura))
		return FALSE;

	if (!will_accept(target, item, c->who, qty))
		return FALSE;

	charge_aura(c->who, aura);

	ret = move_item(c->who, target, item, qty);
	assert(ret);

	wout(c->who, "Teleported %s to %s.",
				just_name_qty(item, qty),
				box_name(target));

	wout(target, "%s teleported %s to us.",
				box_name(c->who),
				just_name_qty(item, qty));

	return TRUE;
}


/*
 *  Mon May  5 12:25:36 1997 -- Scott Turner
 *
 *  Create iron golem.  Decays after one year.
 *
 */
int
v_create_iron_golem(struct command *c)
{
  wout(c->who, "Begin construction of a iron golem.");
  return TRUE;
}

int
d_create_iron_golem(struct command *c)
{
  int new;

  if (!charge_aura(c->who, skill_piety(c->use_skill))) return FALSE;
  gen_item(c->who, item_iron_golem, 1);
  wout(c->who, "You have created a iron golem.");

  return TRUE;
}
