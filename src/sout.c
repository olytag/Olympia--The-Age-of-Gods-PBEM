
#include	<stdio.h>
#include	<limits.h>
#include	<string.h>
#include	"z.h"
#include	"oly.h"


#define		MAX_BUFFER	50
#define		BUF_LEN		LEN


static char *line_buffer[MAX_BUFFER];
static int buf_index = 0;
static int buffer_initialized =	FALSE;

int indent = 0;
int second_indent = 0;

char *spaces;
int spaces_len;


init_spaces()
{
	int i;
	/*VLN extern char *malloc(); */

	spaces_len = 150;
	spaces = my_malloc(spaces_len+1);

	for (i = 0; i <	spaces_len; i++)
		spaces[i] = ' ';

	spaces[spaces_len] = '\0';
}


int out_path = 0;
int out_alt_who = 0;


void
initialize_buffer()
{
	int i;

	for (i = 0; i <	MAX_BUFFER; i++)
		line_buffer[i] = my_malloc(BUF_LEN + 1);

	buffer_initialized = TRUE;

/*
 *  Make sure sout's arguments are appropriate to handle strings
 *  and	ints.
 */
	assert(sizeof(char *) == sizeof(long));
}


/*
 *  printf which returns a char	* which	does not need to
 *  be freed or	otherwise explicitly reclaimed.
 *
 *  Grabs the next buffer from the circular list, printfs
 *  into it, and returns it.  Buffer will eventually be	reused.
 */

char *
sout(format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac)
char *format;
long a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac;
{
	char *buf;

	assert(buffer_initialized);

	buf = line_buffer[buf_index++];
	if (buf_index >= MAX_BUFFER)
		buf_index = 0;

	sprintf(buf, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac);

	assert(buf[BUF_LEN] == '\0');

	return buf;
}


char *
comma_append(char *s, char *t)
{

	if (s && *s)
		return sout("%s, %s", s, t);

	return t;
}


/*
 *  Fan-out decollating	fp allocater
 */

#define OPEN_MAX	256	/* VLN */
#define	MAX_FP		(OPEN_MAX - 20)

struct fp_ent {
	FILE *fp;
	int player;
	struct fp_ent *next;
};

static struct fp_ent *head = NULL;
static int num_fp_open = 0;


static void
open_fp(struct fp_ent *p, int player)
{
	char fnam[LEN];
	extern int flush_always;

	p->player = player;

	sprintf(fnam, "%s/log/%d", libdir, player);

	p->fp =	fopen(fnam, "a+");
	if (p->fp == NULL)
	{
		fprintf(stderr,	"alloc_fp: can't open %s: ", fnam);
		perror("");
		assert(FALSE);
	}

	if (flush_always)
		setbuf(p->fp, NULL);
}


static struct fp_ent *
alloc_fp(int player)
{
	struct fp_ent *new;

	new = my_malloc(sizeof(*new));

	open_fp(new, player);
	num_fp_open++;

	return new;
}


static FILE *
grab_fp(int player)
{
	struct fp_ent *p, *pp, *ppp;

	for (ppp = NULL, pp = NULL, p =	head; p; ppp = pp, pp =	p, p = p->next)
		if (p->player == player)
		{
			if (pp != NULL)
			{
				pp->next = p->next;
				p->next	= head;
				head = p;
			}

			return p->fp;
		}

	if (num_fp_open	< MAX_FP)
	{
		p = alloc_fp(player);
		p->next	= head;
		head = p;
		return p->fp;
	}

	fclose(pp->fp);
	open_fp(pp, player);

	ppp->next = NULL;
	pp->next = head;
	head = pp;

	return pp->fp;
}


void
open_logfile_nondestruct()
{
	mkdir(sout("%s/log", libdir), 0755);
}


void
open_logfile()
{

	if (immediate)
		return;

	system(sout("rm	-rf %s/log", libdir));
	mkdir(sout("%s/log", libdir), 0755);
}


void
close_logfile()
{
	struct fp_ent *p;
	struct fp_ent *next;

	if (immediate)
		return;

	p = head;
	while (p)
	{
		next = p->next;
		if (p->fp)
		{
			fclose(p->fp);
			my_free(p);
		}
		p = next;
	}

	head = NULL;
	num_fp_open = 0;
}


/*
 *  Output formatters
 */


ilist out_vector = NULL;


static void
bottom_out(int pl, int who, int unit, char *s)
{
	FILE *fp;
	int bits = 0;

	if (immediate)
		fp = stdout;
	else
	{
#if 0
		assert(pl != 0);
#else
		if (pl == 0)
			return;
#endif

		fp = grab_fp(pl);
		set_bit(&(p_player(pl)->output), who);
	}

	if (show_day)
		bits |= 1;
	if (second_indent)
		bits |= 2;
	if (unit)
		bits |= 4;

	switch (bits)
	{
	case 0:
		fprintf(fp, "%d::%d::%s\n", who, indent, s);
		break;

	case 1:
		fprintf(fp, "%d::%d:%d:%s\n", who, indent, sysclock.day, s);
		break;

	case 2:
		fprintf(fp, "%d::%d/%d::%s\n", who, indent, second_indent, s);
		break;

	case 3:
		fprintf(fp, "%d::%d/%d:%d:%s\n", who, indent,
					second_indent, sysclock.day, s);
		break;

	case 4:
		fprintf(fp, "%d:%s:%d::%s\n",
					who, box_code_less(unit), indent, s);
		break;

	case 5:
		fprintf(fp, "%d:%s:%d:%d:%s\n",
					who, box_code_less(unit),
					indent, sysclock.day, s);
		break;

	case 6:
		fprintf(fp, "%d:%s:%d/%d::%s\n",
					who, box_code_less(unit), indent,
					second_indent, s);
		break;

	case 7:
		fprintf(fp, "%d:%s:%d/%d:%d:%s\n",
					who, box_code_less(unit), indent,
					second_indent, sysclock.day, s);
		break;

	default:
		assert(FALSE);
	}
}


int show_to_garrison = FALSE;

/*
 *  If output is sent to a location, show it to all characters in that
 *  location.  This includes characters one level deep in sublocations.
 *
 *  If we're not there, but we have a garrison there, and the event
 *  is one which a garrison would see, show it to them, unless it is
 *  in a hidden loc, which garrisons can't see into.
 */

static int
can_view_loc(int pl, struct entity_player *p, int where, int outer)
{
	ilist l = NULL;

	if (test_bit(p->locs, where))
		return TRUE;

	if (test_bit(p->locs, outer))
	{
		if (loc_hidden(where) && !test_known(pl, where))
			return FALSE;

		return TRUE;
	}

	if (show_to_garrison && loc_depth(outer) == LOC_province &&
	    player(province_admin(outer)) == pl)
	  {
		if (where != outer && loc_hidden(where))
			return FALSE;

		return TRUE;
	  }

	return FALSE;
}


static void
out_location(int where, char *s)
{
	int pl;
	struct entity_player *p;
	int outer = viewloc(where);

	loop_player(pl)
	{
		p = p_player(pl);

/*
 *  If we can see the subloc, and we've touched it or its outer
 *  viewloc, then we can see what goes on there.
 */

		if (can_view_loc(pl, p, where, outer))
		{
			bottom_out(pl, outer, 0, s);
		}
	}
	next_player;
}


static void
out_garrison(int garr, char *s)
{
  int pl = player(province_admin(garr));

  if (pl && subkind(pl) != sub_pl_silent)
    bottom_out(pl, OUT_GARR, garr, s);
}


static void
out_sup(int who, char *s)
{
	int i;

	if (who == VECT)
	{
		for (i = 0; i < ilist_len(out_vector); i++)
		{
			assert(out_vector[i] != VECT);
			out_sup(out_vector[i], s);
		}
		return;
	}

	if (is_prisoner(who))		/* prisoners don't report anything */
		return;

	if (out_path == MASTER)
	{
		bottom_out(who, out_alt_who, 0, s);
	}
	else if (subkind(who) == sub_garrison)
	{
		out_garrison(who, s);
	}
	else if (is_loc_or_ship(who))
	{
		out_location(who, s);
	}
	else
	{
		bottom_out(player(who), who, 0, s);
	}
}


out(who, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac)
int who;
char *format;
long a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac;
{
	char buf[LEN];

	sprintf(buf, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac);
	out_sup(who, buf);
}

void tags_off()
{
  options.output_tags--;
};

void tags_on()
{
  options.output_tags++;
};

tagout(who, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac)
int who;
char *format;
long a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac;
{
	char buf[LEN];

	if (options.output_tags < 1) return;

	sprintf(buf, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac);
	out_sup(who, buf);
}


wout(who, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac)
int who;
char *format;
long a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac;
{
	char buf[LEN];

	sprintf(buf, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac);
	out_sup(who, buf);
}


wiout(who, ind, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac)
int who;
int ind;
char *format;
long a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac;
{
	char buf[LEN];

	sprintf(buf, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac);

	second_indent = ind;
	out_sup(who, buf);
	second_indent = 0;
}



ilist
save_output_vector()
{
	ilist tmp;

	tmp = out_vector;
	out_vector = NULL;

	return tmp;
}


void
restore_output_vector(ilist t)
{

	ilist_reclaim(&out_vector);
	out_vector = t;
}


void
vector_clear()
{

	ilist_clear(&out_vector);
}


void
vector_players()
{
	int pl;

	ilist_clear(&out_vector);

	loop_player(pl)
	{
		if (pl != eat_pl && pl != skill_player)
		{
			ilist_append(&out_vector, pl);
		}
	}
	next_player;
}


void
vector_stack(int who, int clear)
{
	int i;

	if (clear)
		ilist_clear(&out_vector);

	if (clear || ilist_lookup(out_vector, who) < 0)
		ilist_append(&out_vector, who);

	loop_char_here(who, i)
	{
		if (clear || ilist_lookup(out_vector, i) < 0)
			ilist_append(&out_vector, i);
	}
	next_char_here;
}


void
vector_char_here(int where)
{
	int i;

	ilist_clear(&out_vector);

	loop_char_here(where, i)
	{
		ilist_append(&out_vector, i);
	}
	next_char_here;
}


void
vector_add(int who)
{

	ilist_add(&out_vector, who);
}


void
lines(int who, char *s)
{
	char buf[LEN];
	int i;

	for (i = 0; i < 72; i++)	/* used to be wrap_pos */
		buf[i] = '-';
	buf[i] = '\0';

	wout(who, "%s", s);
	tagout(who,"<tag type=header>");
	out(who, "%s", buf);
	tagout(who,"</tag type=header>");
}


void
match_lines(int who, char *s)
{
	char *p;
	char buf[LEN];

	out(who, "%s", s);

	strncpy(buf, s, LEN-1);
	buf[LEN-1] = '\0';

	for (p = buf; *p; p++)
		*p = '-';

	out(who, "%s", buf);
}


log_output(k, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac)
int k;
char *format;
long a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac;
{
	int save_out_path = out_path;
	int save_out_alt_who = out_alt_who;

	assert(k >= 10 && k <= 20);

	out_path = MASTER;
	out_alt_who = k;

	out(gm_player, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac);

	out_path = save_out_path;
	out_alt_who = save_out_alt_who;
}

