/*
 *  Tue Oct 12 06:16:40 1999 -- Scott Turner
 *
 *  This is the HTML filter, which works much as the text filter
 *  does, although it inserts a lot of HTML tags along the way.
 *
 *  Compile with:
 *                cc -g -o htmler htmler.c tags.o conv.o
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "tags.h"
#include "conv.h"
void process_string(char *line, int na);
void flush_output_buffer();

/*
 *  "Cur" is the current output line.  eocur is the actual end of the
 *  buffer; cur_col is the column not counting embedded tags.
 *
 */
#define LEN 2048
static char cur[LEN];
static char tmp[LEN];
static int eocur = 0;
static int cur_col = 0;
static int line_start = 0;
static char eol_string[LEN];  /* add this to the end of any line you print */

/*
 *  Prefix for external links.
 *
 */
static char prefix[LEN];

/*
 *  Structure to store "local ids" -- ones that will have internal
 *  linkage.
 *
 */
#define ID_LIMIT 10000
static int local_ids[ID_LIMIT];
static int eoids = 0;

static int is_local(int id)
{
  int i;
  for(i=0;i<eoids;i++)
    if (id == local_ids[i]) return 1;
  return 0;
};

/*
 *  Edison added this comment:

;.'                    ,/l[.]ft7,..............................................................................................

*
*/

/*
 *  Tags: are we passing through tags?
 *
 */
static int tags = 0;
static int found_tag = 0;

/*
 *  Mute the output of the ordinary text.
 *  No break: don't end a line...
 *  No advance: output doesn't advance the column.
 *  Pre: we're in "pre-formatted" output mode.
 *
 */
static int mute_counter=0;
static int no_break=0;
static int no_advance=0;

/*
 *  Turn mute on or off; we have to flush the buffer
 *  before changing the state!
 *
 */
#define ON 1
#define OFF 0
static void mute(int flag)
{
  flush_output_buffer();
  if (flag == ON)
    mute_counter++;
  else
    mute_counter--;
};

/*
 *  Turn pre-formatted text on and off, avoiding redundancies.
 *
 */
static int pre_flag=0;
void pre(int flag)
{
  if (flag == ON && !pre_flag) {
    process_string("<pre>",1);
    pre_flag = 1;
  } else if (flag == OFF && pre_flag) {
    process_string("</pre>",1);
    pre_flag = 0;
  };
};
    

/*
 *  indent::text
 *  indent:day:text
 *  indent/second-indent:day:text
 *  indent/second-indent::text
 */
static int indent = 0, second_indent = 0, day = -1, last_day = -1;
static char unit[100];
char *strip_stuff(char *s)
{
  char *p;

  /*
   *  Pull off unit string
   */
  for (p = unit; *s && *s != ':'; s++)
    *p++ = *s;

  if (p > unit) {
      *p++ = ':';
      *p++ = ' ';
    }
  *p = '\0';
  s++;

  /*
   *  Now get indentation:  %d or %d/%d
   */
  indent = atoi(s);

  while (*s && *s != ':' && *s != '/')
    s++;

  if (*s == '/') {
    s++;
    second_indent = atoi(s);
    while (*s && *s != ':')
      s++;
  } else
    second_indent = 0;

  if (*s == ':')
    s++;

  /*
   *  Now get day, if any
   */

  if (*s == ':') {
    last_day = day;
    day = -1;
  } else {
    last_day = day;
    day = atoi(s);
  };

  while (*s && *s != ':')
    s++;

  if (*s == ':')
    s++;

  return s;
}

/*
 *  Put the header on the front of the output buffer.
 *
 */
void do_header()
{
  int i;

  /*
   *   Perhaps the day changed, and we need to add in some stuff.
   *
   */
  if (last_day == -1 && day != -1) {
    pre(OFF);
    process_string("<table BORDER=0 CELLSPACING=0 CELLPADDING=0>", 1);
  };
  /*
   *  At the end of the daily reports, we want to finish the table
   *  and flush that out before we start on the current line.
   *
   */
  if (last_day != -1 && day == -1) {
    process_string("</table>", 1);
    pre(ON);
    flush_output_buffer();
  };

  if (day >= 0) {
    sprintf(&cur[eocur], "<tr><td ALIGN=RIGHT VALIGN=TOP><b>%2d:&nbsp&nbsp</b></td><td>", day);
    eocur += strlen("<tr><td ALIGN=RIGHT VALIGN=TOP><b>%2d:&nbsp&nbsp</b>"
		    "</td><td>") - 1;
    cur_col += 5;
    no_break++;
  };

  if (unit[0]) {
    strcat(cur, unit);
    eocur += strlen(unit)-1;
    cur_col += strlen(unit);
  };
  
  /*
   *  Possibly add in some indent.
   *
   *  Wed Oct 20 17:27:39 1999 -- Scott Turner
   *
   *  If we're not in <pre> then we need to add in &nbsp instead of
   *  regular spaces -- and we have to kick in a regular one to shield
   *  against following text.
   *
   *  Wed Oct 20 18:27:34 1999 -- Scott Turner
   *
   *  Also if we're not in <pre> we need to add in the second_indent,
   *  because on the first line it is in regular spaces so it will go away.
   *
   */
  cur[eocur] = 0;
  for(i=0;i< (pre_flag ? indent : (indent + second_indent));i++) {
      if (pre_flag) {
	cur[eocur++] = ' ';
      } else {
	strcat(cur,"&nbsp");
	eocur += 5;
      };
  };
  if (!pre_flag && indent) cur[eocur++] = ' ';
  cur_col += (pre_flag ? indent : (indent + second_indent));

  line_start = eocur;
};

/*
 *  Initialize the output buffer.
 *
 *  Strip the :: stuff off the front of the line, save it
 *  in global variables, reset the output buffer (cur), etc.
 *
 *  Mon Oct 11 09:50:01 1999 -- Scott Turner
 *
 *  Reset "found_tag".  We're not outputting empty lines
 *  with only a tag on them (unless we're passing tags, of course).
 *
 */
char *initialize_output_buffer(char *line)
{
  int front = 0;
  /*
   *  Reset the output buffer.
   *
   */
  eocur = 0;
  cur_col = 0;
  line_start = 0;
  cur[eocur] = '\0';
  /*
   *  Strip stuff off the front.
   *
   */
  line = strip_stuff(line);
  /*
   *  Put header on front of output buffer as appropriate.
   *
   */
  do_header();

  /*
   *  Reset found_tag.
   *
   */
  found_tag = 0;

  return line;
};

/*
 *  Check the output buffer to see if we need to output it.
 *
 *  The "right way" to wrap lines is unfortunately too hard for us
 *  to do correctly, because of the problems with embedded tags (which
 *  don't "count").  So what we do instead is output this line at the
 *  first break character (space) *after* the WRAP point.
 *
 *
 */
#define WRAP 68
void check_output()
{
  int i;
  if (cur_col > WRAP && cur[eocur-1] == ' ') {
    flush_output_buffer();
    eocur = 0;
    cur_col = 0;
    cur[eocur] = '\0';
    do_header();
    /*
     *  And add in any secondary indent.
     *
     */
    cur[eocur] = 0;
    for(i=0;i<second_indent;i++) {
      if (pre_flag) {
	cur[eocur++] = ' ';
      } else {
	strcat(cur,"&nbsp");
	eocur += 5;
      };
    };
    if (!pre_flag && indent) cur[eocur++] = ' ';
    cur_col += second_indent;
  };
};

/*
 *  Process a char: push it onto the output buffer and
 *  then see if you need to do anything with the buffer.
 *
 *  Tue Oct 12 07:25:38 1999 -- Scott Turner
 *
 *  If mute is on, just drop the char on the floor.
 */
char *process_char(char *line, int mute)
{
  if (!mute) {
    /*
     *  Put it on the output buffer.  Update the column, since
     *  this is a regular character.
     *
     */
    cur[eocur++] = (*line == '~') ? ' ' : *line;
    if (no_advance < 1) cur_col++;
    /*
     *  Check to see if we need to wrap, etc.
     *
     */
    if (*line != '~' && !no_break) check_output();
  };
  return ++line;
};

/*
 *  Generic add a cell to a table.
 *
 *  Tue Oct 12 08:40:49 1999 -- Scott Turner
 *
 *  Empty strings become a non-breaking space to force the table to render.
 */
void add_cell(char *value, char *align)
{
  char tmp[LEN];
  sprintf(tmp, "<td %s>%s</td>", align, strlen(value) ? value : "&nbsp");
  process_string(tmp,0);
};

/*
 *  Process a box tag.
 *
 */
void process_box_tag(struct tag *t)
{
  int id = atoi(get_key_value(t, "id"));
  if (id > 500) 
    if (t->start) {
      if (id >= 1000 && id <= 5000) 
	sprintf(tmp,"<a href=http://www.UPDATE/oly/tag/Manual/%d.html>", id);
      else
	if (is_local(id)) 
	  sprintf(tmp,"<a href=\"#%d\">", id);
	else
	  sprintf(tmp,"<a href=%s%d.html>", prefix, id);
      
      process_string(tmp,1);
      no_break++;
    } else {
      process_string("</a>",1);
      no_break--;
    };
};

/*
 *  Insert a hrule
 *
 */
void process_h(struct tag *t)
{
  if (t->start) {
    process_string("<hr>",1);
  };
}

/*
 *  Process a turn tag
 *
 */
static int game_number=0;
void process_turn(struct tag *t)
{
  if (t->start) {
    game_number = atoi(get_key_value(t, "game"));
  };
}

/*
 *  Process an account_summary tag
 *
 */
void process_as(struct tag *t)
{
  if (t->start) {
    process_string("</font></center><p><hr><b>Account Summary</b>",0);
    pre(ON);
    eol_string[0] = '\0';
  } else {
    pre(OFF);
  };
}

/*
 *  Process a game_total tag
 *
 */
void process_gt(struct tag *t)
{
  if (t->start) {
    flush_output_buffer();
    indent=0;
    second_indent=0;
    process_string("</font></center><p><b>Game Summary</b><blockquote><table CELLSPACING=0 CELLPADDING=0 COLS=2 WIDTH=\"40%\"><tr><td>Players</td><td ALIGN=RIGHT>",0);
    sprintf(tmp,"%s</td></tr><tr><td>Controlled units</td>"
	    "<td ALIGN=RIGHT>%s</td></tr><tr><td>Other units</td>"
	    "<td ALIGN=RIGHT>%s</td></tr></table></blockquote>",
	    get_key_value(t,"players"),
	    get_key_value(t,"controlled"),
	    get_key_value(t,"other"));
    process_string(tmp,0);
    mute(ON);
  } else {
    mute(OFF);
  };
}

/*
 *  Process a faction_summary tag
 *
 */
void process_fs(struct tag *t)
{
  if (t->start) {
    indent=0;
    second_indent=0;
    sprintf(tmp,"<p><b>Faction Summary</b><blockquote>",
                "<table CELLSPACING=0 CELLPADDING=0 COLS=2 WIDTH=\"40%\">",
                "<tr><td><b>Faction [%s]</b></td><td></td><td ALIGN=RIGHT>",
                "<b>Rank</b></td></tr>",
		   int_to_code(atoi(get_key_value(t,"pl"))));
    process_string(tmp,0);
    mute(ON);
  } else {
    mute(OFF);
    process_string("</table></blockquote>",0);
  };
}

/*
 *  Process a capacity_summary tag
 *
 */
void process_cs(struct tag *t)
{
  if (t->start) {
    indent=0;
    second_indent=0;
    process_string("<b>Capacity Summary</b><blockquote>"
		   "<table BORDER CELLSPACING=0 CELLPADDING=3>"
		   "<tr>"
		   "<td><tt><b>STACK</b></tt></td>"
		   "<td><tt><b>WEIGHT</b></tt></td>"
		   "<td><tt><b>WALK</b></tt></td>"
		   "<td><tt><b>RIDE</b></tt></td>"
		   "<td><tt><b>FLY</b></tt></td></tr>",0);
    mute(ON);
  } else {
    mute(OFF);
    process_string("</table></blockquote>",0);
  };
}

/*
 *  Process a capacity_unit tag
 *
 */
void process_cu(struct tag *t)
{
  if (t->start) {
    int land_weight = atoi(get_key_value(t,"walk_weight"));
    int land_cap = atoi(get_key_value(t,"walk_cap"));
    int ride_weight = atoi(get_key_value(t,"ride_weight"));
    int ride_cap = atoi(get_key_value(t,"ride_cap"));
    int fly_weight = atoi(get_key_value(t,"fly_weight"));
    int fly_cap = atoi(get_key_value(t,"fly_cap"));
    int n;
    char land[64], ride[64], fly[64];

    strcpy(land,"&nbsp");
    if (land_cap > 0) {
      if (land_cap > land_weight) {
	sprintf(land, "%d %3d%%", (land_cap - land_weight),
		((land_weight * 100) / land_cap));
      } else {
	sprintf(land, "(%d) %3d%%", (land_weight - land_cap),
		((land_weight * 100) / land_cap));
      };
    };
      
    strcpy(ride,"&nbsp");
    if (ride_cap > 0) {
      if (ride_cap > ride_weight) {
	sprintf(ride, "%d %3d%%", (ride_cap - ride_weight),
		((ride_weight * 100) / ride_cap));
      } else {
	sprintf(ride, "(%d) %3d%%", (ride_weight - ride_cap),
		((ride_weight * 100) / ride_cap));
      };
    };

    strcpy(fly,"&nbsp");
    if (fly_cap > 0) {
      if (fly_cap > fly_weight) {
	sprintf(fly, "%d %3d%%", (fly_cap - fly_weight),
		((fly_weight * 100) / fly_cap));
      } else {
	sprintf(fly, "(%d) %3d%%", (fly_weight - fly_cap),
		((fly_weight * 100) / fly_cap));
      }
    }

    sprintf(tmp,"<tr><td>%s</td><td ALIGN=RIGHT>%s</td><td ALIGN=RIGHT>%s</td>"
	    "<td ALIGN=RIGHT>%s</td><td ALIGN=RIGHT>%s</td></tr>",
	    int_to_code(atoi(get_key_value(t,"unit"))),
	    get_key_value(t,"weight"),
	    land, ride, fly);
    mute(OFF);
    process_string(tmp,0);
    mute(ON);
  };
}

/*
 *  Process a loc_summary tag
 *
 */
void process_ls(struct tag *t)
{
  if (t->start) {
    indent=0;
    second_indent=0;
    sprintf(tmp,"<b>Location Summary</b><blockquote>");
    process_string(tmp,0);
    pre(ON);
  } else {
    process_string("</blockquote>",0);
    pre(OFF);
  };
}

/*
 *  Process a faction_entry tag
 *
 */
void process_fe(struct tag *t)
{
  if (t->start) {
    sprintf(tmp,"<tr><td>%s</td><td ALIGN=RIGHT>%s</td>"
		   "<td ALIGN=RIGHT>%s</td></tr>",
		   get_key_value(t,"title"),
		   get_key_value(t,"value"),
		   get_key_value(t,"rank"));
    mute(OFF);
    process_string(tmp,0);
    mute(ON);
  };
}

/*
 *  Process a player_summary tag
 *
 */
void process_ps(struct tag *t)
{
  if (t->start) {
    pre(OFF);
    process_string("<b>Player Summary</b><blockquote>",0);
    pre(ON);
  } else {
    process_string("</blockquote>",0);
    pre(OFF);
  };
}

/*
 *  Process a header tag.  Just turns output off and then back on.
 *
 */
void process_eh(struct tag *t)
{
  if (t->start) {
    mute(ON);
  } else {
    mute(OFF);
  };
}

/*
 *  Garrison Report tag
 *
 */
void process_gr(struct tag *t)
{
  if (t->start) {
    process_string("<b>Garrison Report</b><blockquote>"
		   "<table BORDER CELLSPACING=0 CELLPADDING=3>",0);
    process_string("<tr><td><b><tt>GARR</tt></b></td>"
		   "<td><b><tt>WHERE</tt></b></td>"
		   "<td><b><tt>BORDER</tt></b></td>"
		   "<td><b><tt>MEN</tt></b></td>"
		   "<td><b><tt>COST</tt></b></td>"
		   "<td><b><tt>TAX</tt></b></td>"
		   "<td><b><tt>TAX RATE</tt><b></td>"
		   "<td><b><tt>CASTLE</tt></b></td>"
		   "<td><b><tt>RULER</tt></b></td>"
		   "</tr>",0);

  } else {
    process_string("</table></blockquote>",0);
    pre(ON);
  };
}

/*
 *  Unit Report tag
 *
 */
void process_ur(struct tag *t)
{
  if (t->start) {
    pre(OFF);
    sprintf(tmp,"\n<a NAME=%s></a><b>%s</b><hr>",
	    get_key_value(t, "id"),
	    get_key_value(t, "name"));
    process_string(tmp,1);
  } else {
    pre(ON);
 };
}

/*
 *  Char Report tag
 *
 */
void process_cr(struct tag *t)
{
  if (t->start) {
    pre(ON);
  } else {
    pre(OFF);
 };
}

/*
 *  Loc Report tag
 *
 */
void process_lr(struct tag *t)
{
  if (t->start) {
    pre(OFF);
    sprintf(tmp,"\n<a NAME=%s></a><b>%s</b><hr>",
	    get_key_value(t, "id"),
	    get_key_value(t, "name"));
    process_string(tmp,1);
  } else {
    pre(ON);
  };
}

/*
 *  Order Template tag
 *
 */
void process_ot(struct tag *t)
{
  return;  /* UPDATE this section has a search box and submission form that has to be tailored to the web site.  For now, just return.  If you want to see what it does, comment out the return. */
  if (t->start) {
    process_string("<table CELLPADDING=0 CELLSPACING=0><tr><td>"
		   "<table BORDER=2 CELLSPACING=0 CELLPADDING=10 WIDTH=\"100%\">"
		   "<tr>"
		   "<td ALIGN=CENTER VALIGN=CENTER WIDTH=\"100%\" BGCOLOR=\"#FFFFCC\">"
		   "<br><form ACTION=\"http://search.freefind.com/find.html\""
		   "METHOD=\"GET\">Search "
		   "the <b>Olympia: The Age of Gods</b> rules:&nbsp;<input TYPE=\"HIDDEN\""
		   "NAME=\"id\" SIZE=\"-1\" VALUE=\"5012846\"><input TYPE=\"HIDDEN\" NAME=\"pageid\""
		   "SIZE=\"-1\" VALUE=\"r\"><input TYPE=\"HIDDEN\" NAME=\"mode\" SIZE=\"-1\""
		   "VALUE=\"ALL\"><input TYPE=\"TEXT\" NAME=\"query\" SIZE=\"19\"><input "
		   "TYPE=\"SUBMIT\" VALUE=\" Find \"></form></td>"
		   "</tr></table></td></tr>", 1);
    process_string("<tr><td><form Action=\"http://www.UPDATE/cgi-bin/mailto.pl\" Method=\"POST\">",1);
    process_string("<textarea name=\"body\" rows=20 cols=80>",1);
    pre_flag = 1;  /* Special case in the body of a form? */
  } else {
    process_string("</textarea>",1);
    sprintf(tmp,"<input type=hidden NAME=\"to\" VALUE=\"orders@tag%d.UPDATE\""
	    "size=0>", game_number);
    /*
    sprintf(tmp,"<input type=hidden NAME=\"to\" VALUE=\"UPDATE\""
	    "size=0>");
	    */
    process_string(tmp,1);
    process_string("<input type=hidden NAME=\"from\" VALUE=\"UPDATE\" "
		   "size=0>", 1);
    process_string("<br><input Type=submit Value=\"Send orders\">",1);
    process_string("</form></td></tr></table>",1);
    pre_flag = 0;
  };
}

/*
 *  Ship Report tag
 *
 */
void process_sr(struct tag *t)
{
  if (t->start) {
    process_string("<b>Ship Report</b><blockquote>"
		   "<table BORDER CELLSPACING=0 CELLPADDING=3>",0);
    process_string("<tr><td><b><tt>SHIP</tt></b></td>"
		   "<td><b><tt>HULLS</tt></b></td>"
		   "<td><b><tt>PORTS</tt></b></td>"
		   "<td><b><tt>SAILS</tt></b></td>"
		   "<td><b><tt>KEELS</tt></b></td>"
		   "<td><b><tt>FORTS</tt></b></td>"
		   "<td><b><tt>DAMGE</tt></b></td>"
		   "<td><b><tt>CAPACITY</tt></b></td>"
		   "</tr>",0);

    mute(ON);
  } else {
    mute(OFF);
    process_string("</table></blockquote>",0);
  };
}

/*
 *  Command tag -- make a link to the section in the rules.
 *
 */
void process_co(struct tag *t)
{
  if (t->start) {
    sprintf(tmp,"<a class=\"invis\" href=\"http://UPDATE/oly/tag/Manual/%s.html\">",
	    get_key_value(t, "name"));
    process_string(tmp, 1);
  } else {
    process_string("</a>",1);
  };
}

/*
 *  Process a garrison unit tag
 *
 */
void process_gu(struct tag *t)
{
  if (t->start) {
    process_string("<tr>",0);
    add_cell(int_to_code(atoi(get_key_value(t, "unit"))),"");
    add_cell(int_to_code(atoi(get_key_value(t, "loc"))),"");
    add_cell(atoi(get_key_value(t, "closed")) ? "Closed" : "Open", "");
    add_cell(get_key_value(t, "men"),"ALIGN=RIGHT");
    add_cell(get_key_value(t, "cost"),"ALIGN=RIGHT");
    add_cell(get_key_value(t, "tax"),"ALIGN=RIGHT");
    add_cell(get_key_value(t, "rate"),"ALIGN=RIGHT");
    add_cell(int_to_code(atoi(get_key_value(t, "castle"))),"");
    add_cell(int_to_code(atoi(get_key_value(t, "ruler"))),"");
    mute(ON);
  } else {
    mute(OFF);
    process_string("</tr>",0);
  };
}

/*
 *  Garrison inventory tag.
 *
 */
void process_gi(struct tag *t)
{
  if (t->start) {
    sprintf(tmp,"<a NAME=%s></a>", get_key_value(t, "id"));
    process_string(tmp, 1);
  };
};

/*
 *  Process a ship tag
 *
 */
void process_st(struct tag *t)
{
  if (t->start) {
    mute(OFF);
    process_string("<tr>",0);
    add_cell(int_to_code(atoi(get_key_value(t, "ship"))),"");
    add_cell(get_key_value(t, "hulls"),"ALIGN=RIGHT");
    add_cell(get_key_value(t, "ports"),"ALIGN=RIGHT");
    add_cell(get_key_value(t, "sails"),"ALIGN=RIGHT");
    add_cell(get_key_value(t, "keels"),"ALIGN=RIGHT");
    add_cell(get_key_value(t, "forts"),"ALIGN=RIGHT");
    sprintf(tmp,"%s%%",get_key_value(t, "damage"));
    add_cell(tmp, "ALIGN=RIGHT");
    {
      int weight = atoi(get_key_value(t, "weight"));
      int cap = atoi(get_key_value(t, "capacity"));
      sprintf(tmp,"%d/%d (%d%%)",weight,cap,(int) (weight*100)/cap);
      add_cell(tmp, "ALIGN=RIGHT");
    };
    mute(ON);
  } else {
    mute(OFF);
    process_string("</tr>",0);
    mute(ON);
  };
}

/*
 *  Process a unit_section tag
 *
 */
void process_uss(struct tag *t)
{
  if (t->start) {
    process_string("<b>Unit Summary</b><blockquote>"
		   "<table BORDER CELLSPACING=0 CELLPADDING=3>",0);
    process_string("<tr><td><b><tt>UNIT</tt></b></td>"
		   "<td><b><tt>WHERE</tt></b></td>"
		   "<td><b><tt>LOYAL</tt></b></td>"
		   "<td><b><tt>HEAL</tt></b></td>"
		   "<td><b><tt>B</tt></b></td>"
		   "<td><b><tt>AURA</tt></b></td>"
		   "<td><b><tt>GOLD</tt></b></td>"
		   "<td><b><tt>PEAS</tt></b></td>"
		   "<td><b><tt>WORK</tt></b></td>"
		   "<td><b><tt>SAIL</tt></b></td>"
		   "<td><b><tt>FIGHT</tt></b></td>"
		   "<td><b><tt>UNDER</tt></b></td>"
		   "<td><b><tt>NAME</tt></b></td>"
		   "</tr>",0);

    mute(ON);
  } else {
    mute(OFF);
    process_string("</table></blockquote>",0);
  };
}

/*
 *  Process a unit_summary tag
 *
 */
void process_us(struct tag *t)
{
  if (t->start) {
    mute(OFF);
    process_string("<tr>",0);
    add_cell(int_to_code(atoi(get_key_value(t, "unit"))),"");
    if (atoi(get_key_value(t, "loc")) == -1) {
      add_cell("??","");
    } else {
      add_cell(int_to_code(atoi(get_key_value(t, "loc"))),"");
    };
    add_cell(get_key_value(t, "loyal"),"");
    add_cell(get_key_value(t, "health"),"");
    add_cell(get_key_value(t, "behind"),"");
    add_cell(get_key_value(t, "aura"),"ALIGN=RIGHT");
    add_cell(get_key_value(t, "gold"),"ALIGN=RIGHT");
    add_cell(get_key_value(t, "peas"),"ALIGN=RIGHT");
    add_cell(get_key_value(t, "work"),"ALIGN=RIGHT");
    add_cell(get_key_value(t, "sail"),"ALIGN=RIGHT");
    add_cell(get_key_value(t, "fight"), "ALIGN=RIGHT");
    if (atoi(get_key_value(t, "under")) == -1)
      add_cell("??","");
    else if (atoi(get_key_value(t, "under")) == 0)
      add_cell("","");
    else 
      add_cell(int_to_code(atoi(get_key_value(t, "under"))),"");
    add_cell(get_key_value(t, "name"),"");
    mute(ON);
  } else {
    mute(OFF);
    process_string("</tr>",0);
    mute(ON);
  };
}

/*
 *  Process a (possible) tag.
 *
 */
char *process_tag(char *line, int mute)
{
  struct tag t;
  char *s;
  int i;
  
  /*
   *  Try to read it.
   *
   */
  s = read_tag(line, &t);
  /*
   *  Maybe it wasn't a legitimate tag?
   *
   */
  if (s == line) {
    /*
     *  Treat this as a normal character
     *  and then return.
     *
     */
    line = process_char(line, mute);
    return line;
  };
  /*
   *  Note that we found one.
   *
   */
  found_tag++;
  
  /*
   *  See if there's any specific tag processing to be done.
   *
   */
  if (strcasecmp("tab", get_key_value(&t, "type")) == 0) {
    int col = atoi(get_key_value(&t, "col"));
    if (col && col > cur_col) {
      for(i=0;i<(col - cur_col);i++)
	cur[eocur++] = ' ';
      cur_col = col;
    };
  };

  /*
   *  If tags are on, we're going to dump this on the output, no
   *  moving the current_column.
   *
   */
  if (tags) {
    char *end;
    end = sprint_tag(&cur[eocur], &t);
    eocur += (end - &cur[eocur]);
  };

  /*
   *  Big, ugly dispatch.
   *
   */
  if (strcasecmp( "turn", get_key_value(&t, "type")) == 0)
    process_turn(&t);

  if (strcasecmp( "box", get_key_value(&t, "type")) == 0)
    process_box_tag(&t);
  
  if (strcasecmp("unit_section", get_key_value(&t, "type")) == 0)
    process_uss(&t);

  if (strcasecmp("unit_summary", get_key_value(&t, "type")) == 0)
    process_us(&t);

  if (strcasecmp("player_summary", get_key_value(&t, "type")) == 0)
    process_ps(&t);

  if (strcasecmp("account_summary", get_key_value(&t, "type")) == 0)
    process_as(&t);
  
  if (strcasecmp("game_totals", get_key_value(&t, "type")) == 0)
    process_gt(&t);
  
  if (strcasecmp("faction_summary", get_key_value(&t, "type")) == 0)
    process_fs(&t);

  if (strcasecmp("faction_entry", get_key_value(&t, "type")) == 0)
    process_fe(&t);
  
  if (strcasecmp("header", get_key_value(&t, "type")) == 0)
    process_eh(&t);
  
  if (strcasecmp( "loc_summary", get_key_value(&t, "type")) == 0)
    process_ls(&t);
  
  if (strcasecmp("capacity_summary", get_key_value(&t, "type")) == 0)
    process_cs(&t);
  
  if (strcasecmp("capacity_unit", get_key_value(&t, "type")) == 0)
    process_cu(&t);
  
  if (strcasecmp("ship_report", get_key_value(&t, "type")) == 0)
    process_sr(&t);

  if (strcasecmp("ship", get_key_value(&t, "type")) == 0)
    process_st(&t);
  
  if (strcasecmp("garr_report", get_key_value(&t, "type")) == 0)
    process_gr(&t);

  if (strcasecmp("garrison", get_key_value(&t, "type")) == 0)
    process_gu(&t);
  
  if (strcasecmp("garr_inv", get_key_value(&t, "type")) == 0)
    process_gi(&t);

  if (strcasecmp("hrule", get_key_value(&t, "type")) == 0)
    process_h(&t);

  if (strcasecmp("unit_report", get_key_value(&t, "type")) == 0)
    process_ur(&t);

  if (strcasecmp("char_report", get_key_value(&t, "type")) == 0)
    process_cr(&t);

  if (strcasecmp("loc_report", get_key_value(&t, "type")) == 0)
    process_lr(&t);

  if (strcasecmp("order_template", get_key_value(&t, "type")) == 0)
    process_ot(&t);

  if (strcasecmp("command", get_key_value(&t, "type")) == 0)
    process_co(&t);

  return s;
};

/*
 *  Mon Oct 11 09:54:00 1999 -- Scott Turner
 *
 *  Don't output empty lines that have a tag on them.
 *
 *  Tue Oct 12 08:10:51 1999 -- Scott Turner
 *
 *  Don't output empty lines if we have mute on...
 *
 *  Thu Oct 14 06:39:32 1999 -- Scott Turner
 *
 *  It seems to me like I should output any line that has
 *  advanced eocur beyond the indent, and empty lines that
 *  had no tags (because those are spacing lines), except when
 *  mute is on.
 *
 *  Fri Oct 22 07:17:22 1999 -- Scott Turner
 *
 *  Hmm, this doesn't account properly for lines with other junk
 *  at the beginning (e.g., "day" and "unit").
 *
 */
void flush_output_buffer()
{
  if (!mute_counter && (eocur > line_start+1 || !found_tag)) {
    cur[eocur] = '\0';
    if (cur[eocur-1] == '\n')
      cur[--eocur] = '\0';
    if (day >= 0) {
      /*
       * Started this line as a table..
       *
       */
      strcpy(&cur[eocur],"</td></tr>");
      eocur += strlen("</td></tr>");
      no_break--;
    };
    printf("%s%s\n",cur, eol_string);
    eocur = 0;
    cur_col = 0;
  };
};

/*
 *  Process a string (not necessarily starting a new line), using
 *  the mute.
 */

void process_string_mute(char *line)
{
  /*
   *  Now walk through the rest of the line.
   *
   */
 while(*line) {
    if (*line == '<') {
      /*
       *  Possible tag processing.
       *
       */
      line = process_tag(line, mute_counter);
    } else {
      /*
       *  A normal character.
       *
       */
      line =  process_char(line, mute_counter);
    };
 };
};

/*
 *  Process a string (not necessarily starting a new line)
 *  
 */
void process_string(char *line, int na)
{
  if (na) no_advance++;
  
  /*
   *  Now walk through the rest of the line.
   *
   */
 while(*line) {
    if (*line == '<') {
      /*
       *  Possible tag processing.
       *
       */
      line = process_tag(line, 0);
    } else {
      /*
       *  A normal character.
       *
       */
      line =  process_char(line, 0);
    };
 };
 
 if (na) no_advance--;
};

/*
 *  Process a line: Step through it char-by-char.  Normal chars
 *  you can buffer for output; for tags you need to parse the tag
 *  and see if there's anything to be done for that tag.
 *
 */
void process_line(char *line)
{
  int i = 0;
  
  /*
   *  Initialization at beginning of line.
   *
   */
 line = initialize_output_buffer(line);
 /*
  *  Do the processing.
  *
  */
 process_string_mute(line);
 /*
  *  End of line processing.
  *
  */
 flush_output_buffer();
};

/*
 *  The main function: process the input stream line by line.
 *  This works well because we promise not to split tags across
 *  lines.  At worst, you could have a very long line (containing the
 *  entire input).
 *
 */
void process_file(FILE *input)
{
  char line[LEN];
  /*
   *  Tue Oct 12 06:17:24 1999 -- Scott Turner
   *
   *  Insert the required initial HTML
   *
   */
  printf("<!doctype html public \"-//w3c//dtd html 4.0 transitional//en\">\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("   <meta http-equiv=\"Content-Type\" content=\"text/html;\n");
  printf("   charset=iso-8859-1\">\n");
  printf("   <title>Turn Report</title>\n");
  printf("   <style>\n");
  printf("     a.invis { color: black; text-decoration: none }\n");
  printf("   </style>\n");
  printf("</head>\n");
  printf("<body bgcolor=\"#FFFFFF\">\n");
  /*
   *  Matches up to "account summary".
   *
   */
  printf("<center><font size=+1>\n");
  sprintf(eol_string,"<br>");

  while(fgets(line, LEN, input) != NULL) 
    process_line(line);

  /*
   *  And the ending.
   *
   */
  printf("</pre>\n");
  printf("</body>\n");
  printf("</html>\n");
  
};

/*
 *  Thu Oct 14 08:38:28 1999 -- Scott Turner
 *
 *  Make a first pass through the file to find all the unit and
 *  location reports, so that we can build internal links to
 *  those.
 *
 */
void build_internal_links(FILE *fp)
{
  char line[LEN], *start;
  struct tag t;
  
  while(fgets(line, LEN, fp) != NULL) {
    /*
     *  unit_report and loc_report should always start a line.
     *
     */
    start = strip_stuff(line);
    if (*start == '<') {
      (void) read_tag(start,&t);
      if (strcasecmp("unit_report",get_key_value(&t,"type")) == 0 ||
	  strcasecmp("garr_inv",get_key_value(&t,"type")) == 0 ||
	  strcasecmp("loc_report",get_key_value(&t,"type")) == 0) {
	local_ids[eoids++] = atoi(get_key_value(&t,"id"));
	if (eoids >= ID_LIMIT) {
	  fprintf(stderr,"Overflow on local ids.\n");
	  return;
	};
      };
    };
  };
};

main(argc, argv)
int argc;
char **argv;
{
	FILE *fp = stdin;
	extern int optind;
	extern char *optarg;
	char c;
	
	strcpy(prefix,"");
	while ((c = getopt(argc, argv, "tp:f:")) != EOF)
	  {
	    switch (c)
	      {
	      case 't':
		tags = 1;
		break;
	      case 'b':
		strncpy(prefix, optarg, LEN);
		break;
	      case 'f':
		fp = fopen(optarg, "r");
		if (fp == NULL)
		  {
		    fprintf(stderr, "can't read %s: ", argv[1]);
		    perror("");
		    exit(1);
		  }
	      };
	    
	};

	build_internal_links(fp);
	rewind(fp);
	process_file(fp);
	fclose(fp);
	exit(0);
}
