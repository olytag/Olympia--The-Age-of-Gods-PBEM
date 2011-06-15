

#include	<stdio.h>
#include	<string.h>
#include	"z.h"
#include	"oly.h"



/*
 *  Entity coding system
 *
 *  range	      extent	use
 *  1-9999	       9999	reserved (items, skills)
 *
 *  10,000-19,999    10,000	provinces		(CCNN: AA00-DV99)
 *  20,000-26,759      6760	player entities		(CCN)
 *  26,760-33,519      6760	lucky characters	(CNC)
 *  33,520-36,119      2600     lucky locs		(CNN)
 *  36,120-102,400   66,279	sublocs, runoff		(CCNC)
 */


static char *letters = "abcdefghijklmnopqrstuvwxyz";


char *
int_to_code(int l)
{
	int n, a, b, c;

	if (l < 10000)
	{
		return sout("%d", l);
	}

	if (l < 20000)			/* CCNN */
	{
		l -= 10000;

		n = l % 100;
		l /= 100;

		a = l % 26;
		b = l / 26;

		return sout("%c%c%02d", letters[b], letters[a], n);
	}

	if (l < 26760)			/* CCN */
	{
		l -= 20000;

		n = l % 10;
		l /= 10;

		a = l % 26;
		b = l / 26;

		return sout("%c%c%d", letters[b], letters[a], n);
	}

	if (l < 33520)			/* CNC */
	{
		l -= 26760;

		n = l % 26;
		l /= 26;

		a = l % 10;
		b = l / 10;

		return sout("%c%d%c", letters[b], a, letters[n]);
	}

	if (l < 36120)			/* CNN */
	{
		l -= 33520;

		n = l % 10;
		l /= 10;

		a = l % 10;
		b = l / 10;

		return sout("%c%d%d", letters[b], a, n);
	}

	{				/* CCNC */
		l -= 36120;

		a = l % 26;
		l /= 26;

		b = l % 10;
		l /= 10;

		c = l % 26;
		l /= 26;

		return sout("%c%c%d%c", letters[l], letters[c], b, letters[a]);
	}
}


int
code_to_int(char *s)
{
	char a, b, c, d;

	if (s == NULL) return 0;

	if (isdigit(*s))
		return atoi(s);

	if (!isalpha(*s))
		return 0;

	switch (strlen(s))
	{
	case 3:
		if (isdigit(*(s+1)) && isalpha(*(s+2)))		/* CNC */
		{
			a = tolower(*s) - 'a';
			b = *(s+1) - '0';
			c = tolower(*(s+2)) - 'a';

			return a * 260 + b * 26 + c + 26760;
		}

		if (isalpha(*(s+1)) && isdigit(*(s+2)))		/* CCN */
		{
			a = tolower(*s) - 'a';
			b = tolower(*(s+1)) - 'a';
			c = *(s+2) - '0';

			return a * 260 + b * 10 + c + 20000;
		}

		if (isdigit(*(s+1)) && isdigit(*(s+2)))		/* CNN */
		{
			a = tolower(*s) - 'a';
			b = *(s+1) - '0';
			c = *(s+2) - '0';

			return a * 100 + b * 10 + c + 33520;
		}

		return 0;

	case 4:
		if (isalpha(*(s+1)) && isdigit(*(s+2)) && isdigit(*(s+3)))
		{
			a = tolower(*s) - 'a';
			b = tolower(*(s+1)) - 'a';
			c = *(s+2) - '0';
			d = *(s+3) - '0';


			return a * 2600 + b * 100 + c * 10 + d + 10000;
		}

		if (isalpha(*(s+1)) && isdigit(*(s+2)) && isalpha(*(s+3)))
		{
			a = tolower(*s) - 'a';
			b = tolower(*(s+1)) - 'a';
			c = *(s+2) - '0';
			d = tolower(*(s+3)) - 'a';


			return a * 6760 + b * 260 + c * 26 + d + 36120;
		}
		return 0;

	default:
		return 0;
	}
}


int
scode(char *s)
{
	int n;

	if (*s == '[' || *s == '(')
		s++;

	return code_to_int(s);
}


char *
name(int n)
{

	assert(valid_box(n));

	if (bx[n]->name)
		return bx[n]->name;

	return "";
}


void
set_name(int n, char *s)
{

	assert(valid_box(n));

	if (bx[n]->name)
		my_free(bx[n]->name);

	bx[n]->name = str_save(s);

	{
		char *t;

		for (t = bx[n]->name; *t; t++)
		{
			switch (*t)
			{
			case '[':
				*t = '{';
				break;

			case ']':
				*t = '}';
				break;
			}
		}
	}
}

void
set_banner(int n, char *s)
{
	struct entity_misc *p;

	p = p_misc(n);

	if (p->display)
	{
		my_free(p->display);
		p->display = NULL;
	}

	if (s && *s)
	{
		if (strlen(s) > 50)
			s[50] = '\0';
		p->display = str_save(s);
	}
}


char *
display_name(int n)
{
	char *s;
	int i;

	if (!valid_box(n))
		return "";

	s = name(n);

	if (s && *s)
		return s;

	switch (kind(n))
	{
	case T_player:		return "Player";
	case T_gate:		return "Gate";
	case T_post:		return "Sign";
	}

	if (i = noble_item(n))
		return cap(plural_item_name(i, 1));

	return cap(subkind_s[subkind(n)]);
}


char *
display_kind(int n)
{
	char *s;

	switch (subkind(n))
	{
	case sub_city:
		if (is_port_city(n))
			s = "port city";
		else
			s = "city";
		break;

	case sub_fog:
	case sub_rain:
	case sub_wind:
	case sub_mist:
		s = "storm";
		break;

	case sub_guild:
	  if (rp_subloc(n) && rp_subloc(n)->guild &&
	      valid_box(rp_subloc(n)->guild)) 
	    s = sout("%s Guild",bx[rp_subloc(n)->guild]->name);
	  else
	    s = "";
	  break;

	default:
		s = subkind_s[subkind(n)];
	}

	return s;
}


/*
 *  Same as box code, less the brackets
 */

char *
box_code_less(int n)
{

	return int_to_code(n);
}


char *
box_code(int n)
{
	char *s;

	if (n == garrison_magic)
		return "Garrison";

	return sout("[%s]", int_to_code(n));
}


char *
box_name(int n)
{
	char *s;

	if (n == garrison_magic)
		return "Garrison";

	if (valid_box(n))
	{
		s = display_name(n);
		if (s && *s)
		{
		  if (options.output_tags > 0)
		    return sout("<tag type=box id=%d>%s~%s</tag type=box id=%d>",
				n,
				s, box_code(n),n);
		  else
		    return sout("%s~%s", s, box_code(n));
		}
	}

	return box_code(n);
}


char *
just_name(int n)
{
	char *s;

	if (n == garrison_magic)
		return "Garrison";

	if (valid_box(n))
	{
		s = display_name(n);
		if (s && *s)
			return s;
	}

	return box_code(n);
}


char *
plural_item_name(int item, int qty)
{
	char *s;

	if (qty == 1)
		return display_name(item);

	s = rp_item(item) ? rp_item(item)->plural_name : "";

	if (s == NULL || *s == '\0')
	{
		fprintf(stderr, "warning: plural name not set for "
				"item %s\n", box_code(item));
		s = display_name(item);
	}

	return s;
}


char *
plural_item_box(int item, int qty)
{
	char *s;

	if (qty == 1)
		return box_name(item);

	s = plural_item_name(item, qty);

	if (options.output_tags > 0)
	  return sout("<tag type=box id=%d link=%d>%s~%s</tag type=box id=%d>",
		      item, item,
		      s, box_code(item), item);
	else 
	  return sout("%s~%s", s, box_code(item));
}


char *
just_name_qty(int item, int qty)
{

	return sout("%s~%s", nice_num(qty), plural_item_name(item, qty));
}


char *
box_name_qty(int item, int qty)
{

	return sout("%s~%s", nice_num(qty), plural_item_box(item, qty));
}


char *
box_name_kind(int n)
{

	return sout("%s, %s", box_name(n), display_kind(n));
}



/*
 *  Routines for allocating entities and threading like entities
 *  together (kind_first, kind_next)
 */

static void
add_next_chain(int n)
{
	int kind;
	int i;
	static int cache_last = 0;
	static int cache_kind = 0;

	assert(bx[n] != NULL);
	kind = bx[n]->kind;

	if (kind == 0)
		return;

/*  optim! */

	if (cache_kind == kind &&
	    n > cache_last &&
	    bx[cache_last]->x_next_kind == 0)
	{
		bx[cache_last]->x_next_kind = n;
		bx[n]->x_next_kind = 0;
		cache_last = n;
		return;
	}

	cache_last = n;
	cache_kind = kind;

	if (box_head[kind] == 0)
	{
		box_head[kind] = n;
		bx[n]->x_next_kind = 0;
		return;
	}

	if (n < box_head[kind])
	{
		bx[n]->x_next_kind = box_head[kind];
		box_head[kind] = n;
		return;
	}

	i = box_head[kind];
	while (bx[i]->x_next_kind > 0 && bx[i]->x_next_kind < n)
		i = bx[i]->x_next_kind;

	bx[n]->x_next_kind = bx[i]->x_next_kind;
	bx[i]->x_next_kind = n;
}


static void
remove_next_chain(int n)
{
	int i;

	assert(bx[n] != NULL);

	i = box_head[bx[n]->kind];

	if (i == n)
	{
		box_head[bx[n]->kind] = bx[n]->x_next_kind;
	}
	else
	{
		while (i > 0 && bx[i]->x_next_kind != n)
			i = bx[i]->x_next_kind;

		assert(i > 0);

		bx[i]->x_next_kind = bx[n]->x_next_kind;
	}

	bx[n]->x_next_kind = 0;
}


static void
add_sub_chain(int n)
{
	int kind;
	int i;
	static int cache_last = 0;
	static int cache_kind = -1;

	assert(bx[n] != NULL);
	kind = bx[n]->skind;

/*  optim! */

	if (cache_kind == kind &&
	    n > cache_last &&
	    bx[cache_last]->x_next_sub == 0)
	{
		bx[cache_last]->x_next_sub = n;
		bx[n]->x_next_sub = 0;
		cache_last = n;
		return;
	}

	cache_last = n;
	cache_kind = kind;

	if (sub_head[kind] == 0)
	{
		sub_head[kind] = n;
		bx[n]->x_next_sub = 0;
		return;
	}

	if (n < sub_head[kind])
	{
		bx[n]->x_next_sub = sub_head[kind];
		sub_head[kind] = n;
		return;
	}

	i = sub_head[kind];
	while (bx[i]->x_next_sub > 0 && bx[i]->x_next_sub < n)
		i = bx[i]->x_next_sub;

	bx[n]->x_next_sub = bx[i]->x_next_sub;
	bx[i]->x_next_sub = n;
}


static void
remove_sub_chain(int n)
{
	int i;

	assert(bx[n] != NULL);

	i = sub_head[bx[n]->skind];

	if (i == n)
	{
		sub_head[bx[n]->skind] = bx[n]->x_next_sub;
	}
	else
	{
		while (i > 0 && bx[i]->x_next_sub != n)
			i = bx[i]->x_next_sub;

		assert(i > 0);

		bx[i]->x_next_sub = bx[n]->x_next_sub;
	}

	bx[n]->x_next_sub = 0;
}


void
delete_box(int n)
{

	remove_next_chain(n);
	remove_sub_chain(n);
	if (bx[n]->kind == T_char) {
	  bx[n]->kind = T_deadchar;
	} else {
	  bx[n]->kind = T_deleted;
	};
}


void
change_box_kind(int n, int kind)
{

	remove_next_chain(n);
	bx[n]->kind = kind;
	add_next_chain(n);
}


void
change_box_subkind(int n, int sk)
{

	if (subkind(n) == sk)
		return;

	remove_sub_chain(n);
	bx[n]->skind = sk;
	add_sub_chain(n);
}


void
alloc_box(int n, int kind, int sk)
{

	assert(n > 0 && n < MAX_BOXES);

	if (bx[n] != NULL)
	{
		fprintf(stderr, "alloc_box: DUP box %d\n", n);
		assert(FALSE);
	}

	bx[n] = (struct box *) my_malloc(sizeof(struct box));
	bx[n]->kind = kind;
	bx[n]->skind = sk;
	add_next_chain(n);
	add_sub_chain(n);
}
 

static int
rnd_alloc_num(int low, int high)
{
	int n;
	int i;

	n = rnd(low, high);

	for (i = n; i <= high; i++)
		if (bx[i] == NULL)
			return i;

	for (i = low; i < n; i++)
		if (bx[i] == NULL)
			return i;

	fprintf(stderr, "rnd_alloc_num(%d,%d) failed\n", low, high);

	return -1;
}



/*
 *  Entity coding system
 *
 *  range	      extent	use
 *  1-9999	       9999	reserved (items, skills)
 *
 *  10,000-19,999    10,000	provinces		(CCNN: AA00-DV99)
 *  20,000-26,759      6760	player entities		(CCN)
 *  26,760-33,519      6760	lucky characters	(CNC)
 *  33,520-36,119      2600     lucky locs		(CNN)
 *  36,120-102,400   66,279	sublocs, runoff		(CCNC)
 */


int new_ent_prime = FALSE;	/* allocate from prime real estate? */

int
new_ent(int kind, int sk)
{
	int n = -1;

	switch (kind)
	{
	case T_player:
		n = rnd_alloc_num(20000, 26759);

		if (n < 0)
			n = rnd_alloc_num(36120, MAX_BOXES-1);
		break;

	case T_char:
	case T_unform:
		if (new_ent_prime)
			n = rnd_alloc_num(26760, 33519);

		if (n < 0)
			n = rnd_alloc_num(36120, MAX_BOXES-1);
		break;

	case T_skill:
		assert(FALSE);
		n = rnd_alloc_num(8000, 8999);
		break;

	case T_loc:
		if (new_ent_prime)
			n = rnd_alloc_num(33520,36119);

		if (n < 0)
			n = rnd_alloc_num(36120, MAX_BOXES-1);
		break;

	default:
		n = rnd_alloc_num(36120, MAX_BOXES-1);
		break;
	}

	alloc_box(n, kind, sk);

	return n;
}

