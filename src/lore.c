
#include	<stdio.h>
#include	<string.h>
#include	"z.h"
#include	"oly.h"


static void
lore_function(int who, char *func)
{
	int i;

	if (strcmp(func, "capturable_animals") == 0)
	{
		loop_item(i)
		{
			if (item_animal(i) && item_capturable(i))
				out(who, "    %s", box_name(i));
		}
		next_item;

		return;
	}

	if (strcmp(func, "animal_fighters") == 0)
	{
		loop_item(i)
		{
			if (item_animal(i) && is_fighter(i))
				out(who, "    %-20s %s", box_name(i),
						sout("(%d,%d,%d)",
							item_attack(i),
							item_defense(i),
							item_missile(i)));
		}
		next_item;

		return;
	}

	fprintf(stderr, "bad lore sheet function: %s\n", func);
	assert(FALSE);
}

static void
do_skill_header(int who, int num, int use_texi)
{
  struct req_ent **l;
  int i, first;
  char *consume_string;
  char *continuation_string;

  wout(who, "Time to learn: %d days.", rp_skill(num)->time_to_learn);
  
  if (rp_skill(num)->np_req) {
      wout(who, "NPs to learn: %d.", rp_skill(num)->np_req);
  };
  if (rp_skill(num)->flags & COMBAT_SKILL) {
      wout(who, "Time to use: Automatic in combat");
  } else if (rp_skill(num)->time_to_use == -1) {
      wout(who, "Time to use: Variable.");
  } else if (rp_skill(num)->time_to_use) {
      wout(who, "Time to use: %d day%s.", rp_skill(num)->time_to_use,
	 rp_skill(num)->time_to_use > 1 ? "s" : "");
  };

  if (rp_skill(num)->practice_time) {
    wout(who, "Time to practice: %d day%s.", rp_skill(num)->practice_time,
	 rp_skill(num)->practice_time > 1 ? "s" : "");
    wout(who, "Cost to practice: %s.",
	 gold_s(rp_skill(num)->practice_cost));
  };
	 
  if (rp_skill(num)->piety) {
    if (magic_skill(num)) {
	wout(who, "Aura to use: %d.", rp_skill(num)->piety);
    } else {
	wout(who, "Piety to use: %d.", rp_skill(num)->piety);
    };
  };
  
  if (rp_skill(num)->flags & REQ_HOLY_SYMBOL &&
      rp_skill(num)->flags & REQ_HOLY_PLANT) {
      wout(who, "Required to use: Holy symbol, holy plant");
  } else if (rp_skill(num)->flags & REQ_HOLY_SYMBOL) {
      wout(who, "Required to use: Holy symbol");
  } else if (rp_skill(num)->flags & REQ_HOLY_PLANT) {
      wout(who, "Required to use: Holy plant");
  };
  
  if (rp_skill(num)->flags & COMBAT_SKILL) {
      wout(who, "This is a combat spell.");
  };
  /*
   *  "Requires"
   *
   */
  first = 1;
  if (rp_skill(num) && rp_skill(num)->req) {
    l = rp_skill(num)->req;
    for(i=0;i<ilist_len(l);i++) {
      consume_string = "";
      continuation_string = "";
      if (l[i]->consume == REQ_NO) {
	consume_string = " (not consumed)";
      };
      if (l[i]->consume == REQ_OR) {
	continuation_string = " *OR*";
      };
      if (first) {
	  wout(who,"Requires: %s %s%s%s",
	     nice_num(l[i]->qty),
	     plural_item_box(l[i]->item,l[i]->qty),
	     consume_string,
	     continuation_string);
      } else {
	wout(who,"          %s %s%s%s",
	     nice_num(l[i]->qty),
	     plural_item_box(l[i]->item,l[i]->qty),
	     consume_string,
	     continuation_string);
      };
    };
  };
  if (rp_skill(num) && rp_skill(num)->produced) {
      wout(who,"Produces: %s",box_name(rp_skill(num)->produced));
  };

  if (use_texi) {
    wout(who,"@end group");
    wout(who,"@end display");
    wout(who,"@end cartouche");
  };
};

static void
deliver_lore_sheet(int who, int num, int display_num, int use_texi)
{
	FILE *fp;
	char *fnam;
	char *line;
	char buf[20];
	char *q, *p, *t;
	char buf2[LEN];
	int first_blank = 0;

#if 1
	out(who, "");
	if (use_texi) {
	  wout(who, "@node %s", box_name(display_num));
	  if (skill_school(num) == num) {
	    wout(who, "@appendixsec %s", box_name(display_num));
	  } else {
	    wout(who, "@appendixsubsec %s", box_name(display_num));
	  };
	};

	strcpy(buf, box_code_less(display_num));
	if (!use_texi) match_lines(who, box_name(display_num));
	
#else
	if (kind(num) == T_skill && skill_school(num) != num)
		match_lines(who, sout("Skill:  %s", box_name(num)));
	else
		match_lines(who, sout("Lore for %s", box_name(num)));
#endif

	if (kind(num) == T_skill) {
		fnam = sout("%s/lore/%d/%d", libdir, skill_school(num), num);
	} else {
		fnam = sout("%s/lore/etc/%d", libdir, num);
	};

	fp = fopen(fnam, "r");

	if (fp == NULL)
	{
		out(who, "<lore sheet not available>");
		fprintf(stderr, "can't open %s: ", fnam);
		perror("");
		return;
	}

	/*  This is ended in do_skill_header */
	if (use_texi) {
	  wout(who,"@cartouche");
	  wout(who,"@display");
	  wout(who,"@group");
	};
	
	while (line = getlin(fp))
	{
		if (*line == '#')
			continue;

		if (*line == '$')
		{
			lore_function(who, line+1);
			continue;
		}

		if (strlen(line) < 2 && !first_blank && kind(num) == T_skill) {
		  /*
		   *  Thu Oct 31 11:01:33 1996 -- Scott Turner
		   *
		   *  Do the auto-header for skills before the first blank line.
		   *
		   */
		  do_skill_header(who, num, use_texi);
		  first_blank = 1;
		};

		p = line;
		q = buf2;

		while (*p)
		{
			if (*p == '@')
			{
				for (t = buf; *t; t++)
					*q++ = *t;
				p++;
				while (*p == '@')
					p++;
			}
			else
				*q++ = *p++;
		}
		*q = '\0';

		out(who, "%s", buf2);
	}

	fclose(fp);
}


char *
np_req_s(int skill)
{
	int np;

	np = skill_np_req(skill);

	if (np < 1)
		return "";

	if (np == 1)
		return ", 1 NP req'd";

	return sout(", %d NP req'd", np);
}


static void
out_skill_line(int who, int sk)
{

	out(who, "%-*s  %-34s %s%s",
			CHAR_FIELD, box_code_less(sk),
			just_name(sk),
			weeks(learn_time(sk)),
			np_req_s(sk));
}


static void
deliver_skill_lore(int who, int sk, int show_research, int use_texi)
{
  struct entity_skill *p;
  int i;

  deliver_lore_sheet(who, sk, sk, use_texi);

  p = rp_skill(sk);

  if (p == NULL) return;

  if (ilist_len(p->offered)) {
    out(who, "");
    wout(who, "The following teachable skills may be studied directly "
	 "once %s is known:", just_name(sk));
    out(who, "");

    if (use_texi) {
      wout(who,"@example");
      wout(who,"@group");
    };
    indent += 3;
    out(who, "%-*s  %-34s %13s",
	CHAR_FIELD, "num", "skill", "time to learn");
    out(who, "%-*s  %-34s %13s",
	CHAR_FIELD, "---", "-----", "-------------");

    for (i = 0; i < ilist_len(p->offered); i++)
      out_skill_line(who, p->offered[i]);

    indent -= 3;
    if (use_texi) {
      wout(who,"@end group");
      wout(who,"@end example");
    };

    out(who, "");
  };

  if (ilist_len(p->research))
    {
      wout(who, "The following unteachable skills may be studied directly "
	   "once %s is known:", just_name(sk));
      out(who, "");

      if (use_texi) {
	wout(who,"@example");
	wout(who,"@group");
      };
      indent += 3;
      out(who, "%-*s  %-34s %13s",
	  CHAR_FIELD, "num", "skill", "time to learn");
      out(who, "%-*s  %-34s %13s",
	  CHAR_FIELD, "---", "-----", "-------------");

      for (i = 0; i < ilist_len(p->research); i++)
	out_skill_line(who, p->research[i]);

      indent -= 3;
      if (use_texi) {
	wout(who,"@end group");
	wout(who,"@end example");
      };
		  
      out(who, "");
    }

  if (ilist_len(p->guild))
    {
      wout(who, "The following guild skills may be studied by guild"
	   "members:");
      out(who, "");

      if (use_texi) {
	wout(who,"@example");
	wout(who,"@group");
      };
      indent += 3;
      out(who, "%-*s  %-34s %13s",
	  CHAR_FIELD, "num", "skill", "time to learn");
      out(who, "%-*s  %-34s %13s",
	  CHAR_FIELD, "---", "-----", "-------------");

      for (i = 0; i < ilist_len(p->guild); i++)
	out_skill_line(who, p->guild[i]);

      indent -= 3;
      if (use_texi) {
	wout(who,"@end group");
	wout(who,"@end example");
      };
		  
      out(who, "");
    };
}


void
deliver_lore(int who, int num)
{

	switch (kind(num))
	{
	case T_skill:
	  /*
	   * Thu Apr 16 10:06:10 1998 -- Scott Turner
	   *
	   *  Turned off -- in rules now.
	   *
	   *deliver_skill_lore(who, num, FALSE, FALSE);
	   *
	   */
		break;

	case T_item:
		deliver_lore_sheet(who, item_lore(num), num, FALSE);
		break;

	default:
		deliver_lore_sheet(who, num, num, FALSE);
	}

	out(who, "");
}


/*
 *  Show a player a lore sheet
 *  Set anyway to show them the lore sheet even if they've
 *  seen it before.
 */

void
queue_lore(int who, int num, int anyway)
{
	int pl = player(who);
	struct entity_player *p;

	assert(kind(pl) == T_player);

	if (!anyway && test_known(pl, num))
		return;

	p = p_player(pl);

	ilist_append(&p->deliver_lore, num);
	set_known(pl, num);
}


static int
lore_comp(a, b)
int *a;
int *b;
{

	return *a - *b;
}


void
show_lore_sheets()
{
	int pl;
	struct entity_player *p;
	int i;

	stage("show_lore_sheets()");

	out_path = MASTER;
	out_alt_who = OUT_LORE;

	loop_player(pl)
	{
		p = rp_player(pl);
		if (p == NULL || ilist_len(p->deliver_lore) <= 0)
			continue;

		qsort(p->deliver_lore, (unsigned) ilist_len(p->deliver_lore),
				sizeof(int), lore_comp);

		for (i = 0; i < ilist_len(p->deliver_lore); i++)
		{
/*
 *  Weed out duplicates in p->deliver_lore
 */
		    if (i > 0 && p->deliver_lore[i] == p->deliver_lore[i-1])
			continue;

		    deliver_lore(pl, p->deliver_lore[i]);
		}
	}
	next_player;

	out_path = 0;
	out_alt_who = 0;
}


void
gm_show_all_skills(int pl, int use_texi)
{
	int sk, subsk;
	int i;

	out_path = MASTER;
	out_alt_who = OUT_LORE;

	out(pl, "");
	out(pl, "Skill listing:");
	out(pl, "--------------");

	out(pl, "");
	out(pl, "Skill schools:");
	out(pl, "");
	if (use_texi) out(pl, "@itemize");

	loop_skill(sk)
	{
	  if (skill_school(sk) == sk) {
	    if (use_texi) out(pl,"@item");
	    out_skill_line(pl, sk);
	  };
	}
	next_skill;

	if (use_texi) out(pl, "@end itemize");
	out(pl, "");

	if (use_texi) out(pl, "@table @asis");
	loop_skill(sk)
	{
		if (skill_school(sk) == sk)
		{
		  if (use_texi) {
		    out(pl, "@item %s", box_name(sk));
		    out(pl, "@example");
		  } else out(pl, "%s", box_name(sk));
			indent += 3;

			loop_skill(i)
			{
				if (skill_school(i) == sk && i != sk)
					out_skill_line(pl, i);
			}
			next_skill;

			indent -= 3;
			out(pl, "");
			if (use_texi) {
			  out(pl, "@end example");
			  out(pl, "");
			};
		}
	}
	next_skill;
	if (use_texi) out(pl, "@end table");

#if 0
/*
 *  Output lore sheets for all skills
 */

	loop_skill(sk)
	{
		if (skill_school(sk) == sk)
		{
			deliver_skill_lore(pl, sk, TRUE, use_texi);
			out(pl, "");
			loop_skill(subsk) {
			  if (subsk != sk && skill_school(subsk) == sk) {
			    deliver_skill_lore(pl, subsk, TRUE, use_texi);
			    out(pl, "");
			  };
			} next_skill;
		}
	}
	next_skill;
#endif

#if 0
	loop_skill(sk)
	{
		if (skill_school(sk) != sk)
		{

			deliver_skill_lore(pl, sk, TRUE, use_texi);
			out(pl, "");
		}
	}
	next_skill;
#endif

	out_path = 0;
	out_alt_who = 0;
}


void
scan_char_skill_lore()
{
	int who;
	struct skill_ent *e;

	loop_char(who)
	{
		loop_char_skill(who, e)
		{
			queue_lore(who, e->skill, FALSE);
		}
		next_char_skill;
	}
	next_char;
}


void
scan_char_item_lore()
{
	int who;
	struct item_ent *e;
	int lore;

	loop_char(who)
	{
		loop_inv(who, e)
		{
			lore = item_lore(e->item);

			if (lore && !test_known(who, e->item))
			{
				queue_lore(who, e->item, FALSE);
			}
		}
		next_inv;
	}
	next_char;
}

