// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <stdio.h>
#include <unistd.h>
#include "z.h"
#include "oly.h"
#include "forward.h"
#include "vectors/exit_view_list.h"
#include "vectors/item_ent_list.h"


int faery_region = 0;
int faery_player = 0;


#define    SZ    10        /* SZ x SZ is size of Faery */
#define    NCITIES    10        /* num cities to make in Faery */


void
create_faery() {
    int r, c;
    int map[SZ + 1][SZ + 1];
    int n;
    int i;
    int north, east, south, west;
    struct entity_loc *p;
    int sk;

    fprintf(stderr, "INIT: creating faery\n");

/*
 *  Create region wrapper for Faery
 */

    faery_region = new_ent(T_loc, sub_region);
    set_name(faery_region, "Faery");

/*
 *  Fill map[row,col] with locations.
 *  Border is ocean, inner locs are all forest.
 */

    for (r = 0; r <= SZ; r++) {
        for (c = 0; c <= SZ; c++) {
            if (c == 0 || c == SZ || r == 0 || r == SZ) {
                sk = sub_ocean;
            } else {
                sk = sub_forest;
            }

            n = new_ent(T_loc, sk);
            map[r][c] = n;

            set_where(n, faery_region);
        }
    }

/*
 *  Set the NSEW exit routes for every map location
 */

    for (r = 0; r <= SZ; r++) {
        for (c = 0; c <= SZ; c++) {
            p = p_loc(map[r][c]);

            n = (r == 0 ? SZ : r - 1);
            north = map[n][c];

            n = (r == SZ ? 0 : r + 1);
            south = map[n][c];

            n = (c == SZ ? 0 : c + 1);
            east = map[r][n];

            n = (c == 0 ? SZ : c - 1);
            west = map[r][n];

            ilist_append(&p->prov_dest, north);
            ilist_append(&p->prov_dest, east);
            ilist_append(&p->prov_dest, south);
            ilist_append(&p->prov_dest, west);
        }
    }

/*
 *  Make a ring of stones
 *  Randomly place it in Faery
 *  link with a gate to a Ring of Stones in the outside world
 *
 *  Thu May 27 08:27:18 1999 -- Scott Turner
 *
 *  We may not have any rings of stone...
 */

    {
        int gate;
        int ring;
        struct loc_info *li;
        int randloc;
        ilist l = NULL;
        int i;
        int other_ring;

        loop_loc(i)
                {
                    if (subkind(i) == sub_stone_cir) {
                        ilist_append(&l, i);
                    }
                }
        next_loc;

        if (ilist_len(l) > 0) {
            ilist_scramble(l);
            other_ring = l[0];

            li = rp_loc_info(faery_region);
            assert(li && ilist_len(li->here_list) > 0);

            randloc = li->here_list[rnd(0, ilist_len(li->here_list) - 1)];

            ring = new_ent(T_loc, sub_stone_cir);
            set_where(ring, randloc);

            gate = new_ent(T_gate, 0);
            set_where(gate, ring);

            p_gate(gate)->to_loc = other_ring;
            rp_gate(gate)->seal_key = rnd(111, 999);
        };

        ilist_reclaim(&l);
    }


/*
 *  Make a faery hill for every region on the map (except Faery itself).
 *  Place them randomly within Faery.
 *  Link them with the special road to a random location within the region.
 */

    loop_loc(i)
            {
                struct loc_info *li;
                int randloc;
                struct entity_subloc *sl;

                if (loc_depth(i) != LOC_region || i == faery_region) {
                    continue;
                }

                li = rp_loc_info(i);

                if (li == NULL || ilist_len(li->here_list) < 1) {
                    fprintf(stderr, "warning: loc info for %s is NULL\n",
                            box_name(i));
                    continue;
                }

                randloc = li->here_list[rnd(0, ilist_len(li->here_list) - 1)];

                if (subkind(randloc) == sub_ocean) {
                    continue;
                }

                r = rnd(1, SZ - 1);
                c = rnd(1, SZ - 1);

                n = new_ent(T_loc, sub_faery_hill);
                set_where(n, map[r][c]);

                sl = p_subloc(n);
                ilist_append(&sl->link_to, randloc);
#if 0
                sl->link_when = rnd(0, NUM_MONTHS-1);
#endif

                sl = p_subloc(randloc);
                ilist_append(&sl->link_from, n);

                bx[map[r][c]]->temp = 1;
            }
    next_loc;

/*
 *  Create some Faery cities.  Faery cities have markets which sell
 *  rare items.
 *
 *  Wed Dec 18 12:22:17 1996 -- Scott Turner
 *
 *  Faery cities produce "elven arrows" at 1/month...
 *
 */

    {
        ilist l = NULL;
        int i;
        int new;

        for (r = 1; r < SZ; r++) {
            for (c = 1; c < SZ; c++) {
                if (bx[map[r][c]]->temp == 0) {
                    ilist_append(&l, map[r][c]);
                }
            }
        }

        if (ilist_len(l) < NCITIES) {
            fprintf(stderr, "\twarning: space for Faery cities "
                            "only %d\n", ilist_len(l));
        }

        ilist_scramble(l);

        for (i = 0; i < ilist_len(l) && i < NCITIES; i++) {
            new = new_ent(T_loc, sub_city);
            set_where(new, l[i]);
            set_name(new, "Faery city");
            seed_city(new);
            /*
             *  Add an elven arrow production, 10 a month...
             *
             */
            add_city_trade(new, PRODUCE,
                           item_elvish_arrow, 10,
                           20 + rnd(0, 12) * 5, 0);
        }

        ilist_reclaim(&l);
    }

/*
 *  Create the Faery player
 */

    assert(faery_player == 0);

    faery_player = 204;
    alloc_box(faery_player, T_player, sub_pl_npc);
    set_name(faery_player, "Faery player");
    p_player(faery_player)->password = str_save(DEFAULT_PASSWORD);

    printf("faery loc is %s\n", box_name(map[1][1]));
}


#if 0
/*
 *  Mon Dec  9 12:31:41 1996 -- Scott Turner
 *
 *  This stuff is no longer used, since Faery operates differently
 *  now.
 *
 */

void
link_opener(int who, int where, int sk)
{
  struct entity_subloc *p, *pp;
  int i;
  int set_something = FALSE;

  p = rp_subloc(where);

  if (p == NULL)
  {
      wout(who, "Nothing happens.");
      return;
  }

  if (subkind(where) == sk && ilist_len(p->link_to) > 0)
  {
      if (p->link_open < 2 && p->link_open >= 0)
          p->link_open = 2;

      for (i = 0; i < ilist_len(p->link_to); i++)
          out(who, "A gateway to %s is here.",
                  box_name(p->link_to[i]));

      set_something = TRUE;
  }

  for (i = 0; i < ilist_len(p->link_from); i++)
  {
      if (subkind(p->link_from[i]) != sk)
          continue;

      pp = rp_subloc(p->link_from[i]);
      assert(pp);

      if (pp->link_open < 2)
          pp->link_open = 2;

      out(who, "A gateway to %s is here.",
                  box_name(p->link_from[i]));

      set_something = TRUE;
  }

  if (!set_something)
      wout(who, "Nothing happens.");
}
#endif

/*
 *  Wed Dec 18 20:53:40 1996 -- Scott Turner
 *
 *  The Wild Hunt rides through a province and possibly snatches a
 *  noble at random from amongst the present nobles and spirits him
 *  off to some random spot in Faery.
 *
 */
void
do_wild_hunt(int where) {
    int i, who = 0, count = 0, dest, someone = 0, j;
    struct item_ent *e;
    struct loc_info *p;

    /*
     *  If called, we're going to grab someone if there's anyone
     *  here, so give it a shot.  Avoid priests of Timeid and anyone
     *  carrying an elfstone.
     *
     *  Thu Oct  8 18:08:59 1998 -- Scott Turner
     *
     *  The new elfstone is a charged artifact.
     */
    loop_here(where, i)
            {

                if (is_priest(i) == sk_timeid) { continue; }
                /* This can be removed at some point */
                if (has_use_key(i, use_faery_stone)) { continue; }
                /* Keep this one */
                if (has_artifact(i, ART_PROT_FAERY, 0, 0, 1)) { continue; }

                loop_stack(i, j)
                        {
                            if (is_priest(j) == sk_timeid) { continue; }
                            if (has_use_key(j, use_faery_stone)) { continue; }
                            if (has_artifact(i, ART_PROT_FAERY, 0, 0, 1)) { continue; }
                            count++;
                            if (rnd(1, count) == 1) { who = j; }
                        }next_stack;
            }next_here;

    if (!who) { return; }

    wout(who, "The Wild Hunt rides through the province and snatches you up!");
    wout(where,
         "The Wild Hunt rides through the province!  The leader snatches up %s!",
         box_name(who));

    /*
     *  Find a spot in Faery for him.  We want a place with no elves,
     *  no other other characters, and land.
     *
     */
    p = rp_loc_info(faery_region);
    assert(p);

    for (i = 0; i < 1000; i++) {
        dest = p->here_list[rnd(0, ilist_len(p->here_list) - 1)];
        /*
         *  No ocean.
         *
         */
        if (subkind(where) < sub_forest || subkind(where) > sub_swamp) { continue; }
        /*
         *  No one here...
         *
         */
        someone = 0;
        loop_here(i, j)
                {
                    if (kind(i) == T_char) {
                        someone = 1;
                        break;
                    };
                }next_here;
        if (someone) { continue; }
    };

    /*
     *  Maybe nowhere to put him?
     *
     */
    wout(who, "After a dizzying ride past many marvels, the Wild Hunt dumps you");
    if (i == 1000) {
        wout(who, "back in %s!", box_name(where));
        wout(where, "The Wild Hunt reappears and dumps %s unceremoniously to the ground.", box_name(who));
        return;
    };

    /*
     *  Otherwise grab "who", unstack him from wherever he is,
     *  strip him of all companions, and plop him into Faery.
     *
     */
    leave_stack(who);
    inventory_loop(who, e)
                {
                    /*
                     *  Delete all "man items"
                     *
                     */
                    if (man_item(e->item)) {
                        (void) drop_item(who, e->item, e->qty);
                    };
                }
    inventory_next;

    move_stack(who, dest);
    wout(who, "in %s!", box_name(dest));
    wout(who, "The hollow laughter and enchanting music of the Wild Hunt fades",
         "slowly away as you realize your predicament.");
};

/*
 *  Tue Dec 10 09:42:28 1996 -- Scott Turner
 *
 *  New version.
 *
 */
void
link_opener(int who, int where, int sk) {
    struct entity_subloc *p;
    int i, hill = 0, sum = 0, dest = 0;
    struct exit_view **l;
    int set_something = FALSE;

    /*
     *  Can't be civilization here.
     *
     */
    if (subkind(where) < sub_forest || subkind(where) > sub_swamp ||
        in_faery(where) || in_hades(where) || in_clouds(where) ||
        province_subloc(where, sub_faery_hill) ||
        province_subloc(where, sub_city) ||
        has_item(where, item_peasant) > 100) {
        wout(where, "A hill briefly appears but then fades away.");
        wout(who, "A hill briefly appears but then fades away.");
        return;
    };

    /*
     *  Find a closed hill to open.
     *
     */
    loop_subkind(sub_faery_hill, i)
            {
                l = exits_from_loc(0, i);
                if (ev_list_len(l) == 0) {
                    hill = i;
                    break;
                };
            }next_subkind;

    if (!hill) {
        wout(who, "The stone glows and fizzles but nothing happens.");
        return;
    };

    /*
     *  Select a location from all the provinces in faery; don't pick
     *  one that already has a hill or a city in it.
     *
     */
    loop_province(i)
                {
                    if (!in_faery(i)) { continue; }
                    if (subkind(i) < sub_forest || subkind(i) > sub_swamp) { continue; }
                    if (province_subloc(i, sub_faery_hill)) { continue; }
                    if (province_subloc(i, sub_city)) { continue; }
                    sum++;
                    if (rnd(1, sum) == 1) {
                        dest = i;
                    };
                }next_province;
    assert(dest);
    set_where(hill, dest);  /* It's now somewhere in faery... */

    /*
     *  And open a link into this province.
     *
     */
    p = p_subloc(hill);
    ilist_append(&p->link_to, where);
    p = p_subloc(where);
    ilist_append(&p->link_from, hill);
    wout(who, "The stone glows brightly and %s appears here!", box_name(hill));
    wout(where, "%s appears here.", box_name(hill));

    /*
     *  A chance for the Wild Hunt to make a visit.
     *
     */
    if (rnd(1, 20) == 20) {
        wout(where, "Hollow laughter and enchanting music fill the air!");
        wout(gm_player, "The wild hunt is riding in %s.", box_name(where));
        do_wild_hunt(where);
    };
}

#if 0
/*
 *  Tue Dec 17 14:59:35 1996 -- Scott Turner
 *
 *  Added use count.  Hmmm, elfstones aren't "unique items"...
 *
 */
int
v_use_faery_stone(struct command *c)
{
  struct exit_view **l;
  
  if (rp_item_magic(c->a)->orb_use_count < 1) {
    wout(c->who,"The stone gives off a blinding glow and suddenly vanishes!");
    destroy_unique_item(c->who, c->a);
    return FALSE;
  };
  rp_item_magic(c->a)->orb_use_count--;

  /*
   *  In a hill? f you're in a closed hill, then open the hill...
   *
   */
  if (subkind(subloc(c->who)) == sub_faery_hill) {
    l = exits_from_loc(0, subloc(c->who));
    if (ilist_len(l) == 0) {
      /*
       *  Inside a closed hill, let's reopen it.
       *
       */
      wout(c->who,"The stone glows brightly and the way to Faery is opened.");
      open_faery_hill(subloc(c->who));
    } else {
      wout(c->who,"The stone glows briefly, but nothing happens.");
    };
    return TRUE;
  } else {
    link_opener(c->who, subloc(c->who), sub_faery_hill);
  };
  
  return TRUE;
}
#endif

/*
 *  Thu Oct  8 18:10:22 1998 -- Scott Turner
 *
 *  Modified for artifact version...
 *
 */
int
v_use_faery_artifact(struct command *c) {
    struct exit_view **l;
    struct entity_artifact *a = is_artifact(c->a);

    if (!a) { return FALSE; }

    if (a->uses < 1) {
        wout(c->who, "%s gives off a blinding glow and suddenly vanishes!",
             box_name(c->a));
        destroy_unique_item(c->who, c->a);
        return FALSE;
    };
    a->uses--;

    /*
     *  In a hill? f you're in a closed hill, then open the hill...
     *
     */
    if (subkind(subloc(c->who)) == sub_faery_hill) {
        l = exits_from_loc(0, subloc(c->who));
        if (ev_list_len(l) == 0) {
            /*
             *  Inside a closed hill, let's reopen it.
             *
             */
            wout(c->who, "%s glows brightly and the way to Faery is opened.",
                 box_name(c->a));
            open_faery_hill(subloc(c->who));
        } else {
            wout(c->who, "%s glows briefly, but nothing happens.",
                 box_name(c->a));
        };
        return TRUE;
    } else {
        link_opener(c->who, subloc(c->who), sub_faery_hill);
    };

    return TRUE;
}

static void
create_elven_hunt() {
    int new, new2, num;
    struct loc_info *p;
    int where;

    p = rp_loc_info(faery_region);
    assert(p);

    do {
        where = p->here_list[rnd(0, ilist_len(p->here_list) - 1)];
    } while (subkind(where) == sub_ocean);

    new = new_char(sub_ni, item_elf, where, 100, faery_player,
                   LOY_npc, 0, "Faery Hunt");

    if (new < 0) { return; }

    new2 = new_char(sub_ni, item_elf, where, 100, faery_player,
                    LOY_npc, 0, "Faery Hunt");

    if (new2 < 0) { return; }

    join_stack(new2, new);

    num = rnd(12, 50) + rnd(12, 50);
    gen_item(new, item_elf, num);
    gen_item(new2, item_elf, num);

    /*
     *  Set their "programs"
     *
     */
    p_char(new)->npc_prog = PROG_elf;
    p_char(new2)->npc_prog = PROG_elf;

    queue(new, "wait time 0");
#if 0
    init_load_sup(new);   /* make ready to execute commands immediately */
#endif
    queue(new2, "behind 9");  /* Become archers. */
#if 0
    init_load_sup(new2);   /* make ready to execute commands immediately */
#endif
}


void
update_faery() {
    int i;
    int n_faery = 0;
    struct entity_misc *p = NULL;

    loop_units(faery_player, i)
            {
                if (has_item(i, item_elf) < 3) {
                    kill_char(i, 0, S_nothing);
                } else {
                    n_faery++;
                };
            }
    next_unit;

    while (n_faery < 15) {
        create_elven_hunt();
        n_faery++;
    }

    /*
     *  Thu Oct  1 08:17:10 1998 -- Scott Turner
     *
     *  This code updates the elves by moving anyone who has been warned
     *  onto their "hostile" list.
     *
     */
    p = p_misc(faery_player);
    assert(p);

    clear_all_att(faery_player);

    for (i = 0; i < ilist_len(p->npc_memory); i++) {
        /*
         *  Delete anything that has become invalid.
         *
         */
        if (!valid_box(p->npc_memory[i]) ||
            kind(p->npc_memory[i]) != T_char) {
            ilist_delete(&p->npc_memory, i);
            continue;
        };
        /*
         *  And declare hostile to the rest.
         *
         */
        if (!is_hostile(faery_player, p->npc_memory[i])) {
            set_att(faery_player, p->npc_memory[i], HOSTILE);
            wout(gm_player,
                 "Faery declaring hostile to %s.", box_name(p->npc_memory[i]));
        };
    };
}

#if 0
void
swap_region_locs(int reg)
{
    ilist l = NULL;
    int i;
    int j;
    int who;
    int skip;

    loop_loc(i)
    {
        if (region(i) != reg)
            continue;

        if (loc_depth(i) != LOC_province)
            continue;

        skip = FALSE;
        loop_char_here(i, who)
        {
            if (char_moving(who) && player(who) == sub_pl_regular)
                skip = TRUE;
        }
        next_char_here;

        if (skip)
            continue;

        ilist_append(&l, i);
    }
    next_loc;

    if (ilist_len(l) < 2)
    {
        fprintf(stderr, "can't find two swappable locs for %s\n", box_name(reg));
        ilist_reclaim(&l);
        return;
    }

    ilist_scramble(l);

    loop_loc(i)
    {
        struct entity_loc *p;

        if (loc_depth(i) != LOC_province)
            continue;

        p = rp_loc(i);
        if (p == NULL)
            continue;

        for (j = 0; j < ilist_len(p->prov_dest); j++)
        {
            if (p->prov_dest[j] == l[0])
                p->prov_dest[j] = l[1];
            else if (p->prov_dest[j] == l[1])
                p->prov_dest[j] = l[0];
        }
    }
    next_loc;

    {
        ilist tmp;
        struct entity_loc *p1;
        struct entity_loc *p2;

        p1 = p_loc(l[0]);
        p2 = p_loc(l[1]);

        tmp = p1->prov_dest;
        p1->prov_dest = p2->prov_dest;
        p2->prov_dest = tmp;
    }

    log_output(LOG_CODE, "Swapped %s and %s in %s", box_name(l[0]), box_name(l[1]), box_name(reg));
}
#endif
