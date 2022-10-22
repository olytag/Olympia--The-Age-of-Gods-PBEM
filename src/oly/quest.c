// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    <string.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


int subloc_player = 0;


static char *art_att_s[] = {"sword", "dagger", "longsword"};
static char *art_def_s[] = {"helm", "shield", "armor"};
static char *art_mis_s[] = {"spear", "bow", "javelin", "dart"};
static char *art_mag_s[] = {"ring", "staff", "amulet"};

static char *pref[] = {"magic", "golden", "crystal", "enchanted", "elven"};

static char *of_names[] = {
        "Achilles", "Darkness", "Justice", "Truth", "Norbus", "Dirbrand",
        "Pyrellica", "Halhere", "Eadak", "Faelgrar", "Napich", "Renfast",
        "Ociera", "Shavnor", "Dezarne", "Roshun", "Areth Lorbin", "Anarth",
        "Vernolt", "Pentara", "Gravecarn", "Sardis", "Lethrys", "Habyn",
        "Obraed", "Beebas", "Bayarth", "Haim", "Balatea", "Bobbiek", "Moldarth",
        "Grindor", "Sallen", "Ferenth", "Rhonius", "Ragnar", "Pallia", "Kior",
        "Baraxes", "Coinbalth", "Raskold", "Lassan", "Haemfrith", "Earnberict",
        "Sorale", "Lorbin", "Osgea", "Fornil", "Kuneack", "Davchar", "Urvil",
        "Pantarastar", "Cyllenedos", "Echaliatic", "Iniera", "Norgar", "Broen",
        "Estbeorn", "Claunecar", "Salamus", "Rhovanth", "Illinod", "Pictar",
        "Elakain", "Antresk", "Kichea", "Raigor", "Pactra", "Aethelarn",
        "Descarq", "Plagcath", "Nuncarth", "Petelinus", "Cospera", "Sarindor",
        "Albrand", "Evinob", "Dafarik", "Haemin", "Resh", "Tarvik", "Odasgunn",
        "Areth Pirn", "Miranth", "Dorenth", "Arkaune", "Kircarth", "Perendor",
        "Syssale", "Aelbarik", "Drassa", "Pirn", "Maire", "Lebrus", "Birdan",
        "Fistrock", "Shotluth", "Aldain", "Nantasarn", "Carim", "Ollayos",
        "Hamish", "Sudabuk", "Belgarth", "Woodhead",
        NULL
};


/*
 *  Tue Oct 13 13:14:57 1998 -- Scott Turner
 *
 *  This borrows the old "monstermark" idea to grade the toughness of
 *  a monster stack, to let us give it appropriate treasure.
 *
 *  Tue Apr 27 06:40:02 1999 -- Scott Turner
 *
 *  MM provides a monstermark for a beast type, monstermark() for a stack.
 *
 *  Depending upon how MM is calculated, you can vary the relative numbers
 *  of beasts.  If MM = att * def, then a peasant gets a 1 and a dragon gets
 *  2250000 -- which means that dragons are going to be essentially unknown.
 *  If MM = att + def, then the range is 0 to 3000, which perhaps makes more
 *  sense.
 *
 *  MAX_MM is a calculated maximum MM (in io.c)
 *
 */
long MAX_MM = 1000l;

long MM(int item) {
    if (kind(item) == T_char && noble_item(item)) {
        item = noble_item(item);
    }
    return (item_attack(item) + item_defense(item));
};

long
monstermark(int unit) {
    long val = 0;
    struct item_ent *e;

    loop_inv(unit, e)
                {
                    if (item_attack(e->item) && item_defense(e->item)) {
                        val += e->qty * MM(e->item);
                    }
                }next_inv;

    val += char_attack(unit) + char_defense(unit);

    return val;
};

static int new_artifact(int who);

/*
 *  Tue Oct 13 13:22:30 1998 -- Scott Turner
 *
 *  Generate a single treasure for a monster.
 *
 */
void
generate_one_treasure(int monster) {
    /*
     *  Tue Oct 13 17:58:12 1998 -- Scott Turner
     *
     *  45% -- gold (100 to 500)
     *  25% -- artifact
     *  10% -- rare trade items
     *  10% -- horses
     *   5% -- prisoner
     *   5% -- book
     */
    int choice = rnd(1, 100);

    if (choice < 45) {
        gen_item(monster, item_gold, rnd(100, 500));
    } else if (choice < 70) {
        int art = create_random_artifact(monster);
    } else if (choice < 80) {
        int item = random_trade_good();
        gen_item(monster, item,
                 rnd(rp_item(item)->trade_good / 2, rp_item(item)->trade_good * 2));
    } else if (choice < 90) {
        int item;

        switch (rnd(1, 4)) {
            case 1:
                item = item_pegasus;
                break;
            case 2:
                item = item_wild_horse;
                break;
            case 3:
                item = item_riding_horse;
                break;
            case 4:
                item = item_warmount;
                break;
        };
        gen_item(monster, item, rnd(3, 10));

    } else if (choice < 95) {
        int pris;
        char *name;

        switch (rnd(1, 8)) {
            case 1:
                name = "Old man";
                break;
            case 2:
                name = "Old man";
                break;
            case 3:
                name = "Knight";
                break;
            case 4:
                name = "Princess";
                break;
            case 5:
                name = "King's daughter";
                break;
            case 6:
                name = "Nobleman";
                break;
            case 7:
                name = "Merchant";
                break;
            case 8:
                name = "Distressed Lady";
                break;

            default:
                assert(FALSE);
        }

        pris = new_char(0, 0, monster, 100, indep_player,
                        LOY_unsworn, 0, name);

        p_magic(pris)->swear_on_release = TRUE;
        p_char(pris)->prisoner = TRUE;
    } else {
        (void) make_teach_book(monster, 0, 0, sub_book);
    }
};

/*
 *  Tue Oct 13 13:18:39 1998 -- Scott Turner
 *
 *  Generate treasures for a monster stack, based on the monstermark.
 *  There should be a guarantee of 1 treasure for each "50 orc equivalent".
 *  Lesser strength has a % chance of getting a treasure.
 *
 */
void
generate_treasure(int unit, int divisor) {
    int unit_mm = monstermark(unit) / divisor;
    int one_treasure = 25 * MM(item_orc);
    int count = 0, i;

    if (unit_mm < one_treasure * 0.80) {
        unit_mm = one_treasure * 0.80;
    }

    /*
     *  +/- 20% will give us a "range" of treasures.
     *
     */
    unit_mm = (unit_mm * (80 + rnd(1, 40))) / 100;
    /*
     *  And we have a small chance of being really out there.
     *
     */
    if (rnd(1, 100) < 10) {
        unit_mm *= rnd(5, 20);
    };

    while (unit_mm > 0) {
        if (rnd(1, one_treasure) < unit_mm) {
            generate_one_treasure(unit);
            count++;
        };
        unit_mm -= one_treasure;
        one_treasure += one_treasure;
    };

    /*
     *  Tue Dec 15 12:14:02 1998 -- Scott Turner
     *
     *  Some monsters may have specific treasures.
     *
     *  Dragons always have 1-6 power jewels.
     *
     */
    if (noble_item(unit) == item_dragon) {
        for (i = 1; i < rnd(1, 6); i++) {
            create_specific_artifact(unit, ART_POWER);
            count++;
        };
    } else if (noble_item(unit) == item_balrog) {
        create_specific_artifact(unit, ART_COMBAT);
        count++;
    };
};

/*
 *  Create an Old book which offers instruction in a rare skill
 *
 *  Mon Nov  4 09:55:21 1996 -- Scott Turner
 *
 *  Genericized so it can produce any skill.  If rare flag is set,
 *  only produce the "rare" books - magic and religion.  Otherwise,
 *  produce the other books with twice the chance of the rare
 *  books.  If category is set, produce only category books.
 *
 *  Wed Apr 15 12:42:44 1998 -- Scott Turner
 *
 *  Give most books a descriptive title.
 *
 *  Thu Nov 19 18:37:38 1998 -- Scott Turner
 *
 *  Prevent any city controlled by a nation (or a capital) from
 *  producing a forbidden book.
 *
 *  Mon Dec  7 06:41:05 1998 -- Scott Turner
 *
 *  Can we improve this?  Run through all the cities in this region.
 *  Create the intersection of all proscribed books for nations that
 *  have capitals or controlled cities in the region.  Then use that
 *  proscribed list to avoid books.
 *
 *  Mon Dec  7 13:05:40 1998 -- Scott Turner
 *
 *  Better yet.  If a city is controlled, use that proscribed list.
 *  If it is a capital, use that proscribed list.  Otherwise do the
 *  intersection business.
 */

int
make_teach_book(int who, int rare, int category, int subkind) {
    int new, count = 0;
    char s[80];
    struct item_magic *p;
    int skill = 0;
    int i, j, nat, first = 1;
    ilist proscribed_skills = NULL;

    assert(subkind == sub_book || subkind == sub_scroll);

    /*
     *  If it is a controlled city, then only allow that nation's
     *  books.
     *
     */
    if (nation(player_controls_loc(who))) {
        proscribed_skills =
                ilist_copy(rp_nation(nation(player_controls_loc(who)))->proscribed_skills);
    } else {
        nat = 0;
        loop_nation(j)
                {
                    if (rp_nation(j)->capital == who) {
                        nat = j;
                        break;
                    };
                }next_nation;
#if 0
        for(j=1;j<=num_nations;j++)
          if (nations[j].capital == who) {
        nat = j;
        break;
          };
#endif
        /*
         *  If it is an uncontrolled capital...
         *
         */
        if (nat) {
            proscribed_skills =
                    ilist_copy(rp_nation(nat)->proscribed_skills);
        } else {
            /*
             *  Otherwise, create an ilist of the intersection of all
             *  proscribed skills.
             *
             */
            loop_city(i)
                    {
                        /*
                         *  Only care about cities in this region, that are controlled or
                         *  capitals.
                         */
                        if (region(i) == region(who)) {
                            /*
                             *  Is it a capital?
                             *
                             */
                            nat = 0;
                            loop_nation(j)
                                    {
                                        if (rp_nation(j)->capital == i) {
                                            nat = j;
                                            break;
                                        };
                                    }next_nation;
                            /*
                             *  Is it controlled?
                             *
                             */
                            if (!nat && player_controls_loc(who)) {
                                nat = nation(player_controls_loc(who));
                            }
                            /*
                             *  Nat is the nation, so if it exists intersect it into
                             *  our proscribed list.  If it's our first nation, then
                             *  just use the whole proscribed list.
                             *
                             */
                            if (nat) {
                                if (first) {
                                    proscribed_skills = ilist_copy(rp_nation(nat)->proscribed_skills);
                                    first = 0;
                                } else {
                                    /*
                                     *  Intersect by removing anything from proscribed skills
                                     *  that appears in the nation's proscribed skills.
                                     *
                                     */
                                    ilist tmp = ilist_copy(proscribed_skills);
                                    for (i = 0; i < ilist_len(tmp); i++) {
                                        if (ilist_lookup(rp_nation(nat)->proscribed_skills,
                                                         tmp[i]) == -1) {
                                            ilist_rem_value(&proscribed_skills, tmp[i]);
                                        }
                                    }
                                    ilist_reclaim(&tmp);
                                };
                            };
                        };
                    }next_city;
        };
    };
    /*
     *  At this point proscribed_skills are books we don't want to create.
     *
     */
    loop_skill(new)
            {
                if (new == sk_adv_sorcery) { continue; }
                if (new == sk_basic_religion) { continue; }
                /*
                 *  Sun Feb  6 10:31:08 2000 -- Scott Turner
                 *
                 *  No point in making books for skills that won't benefit from
                 *  instruction.
                 *
                 */
                if (new != skill_school(new) &&
                    ilist_lookup(rp_skill(skill_school(new))->offered, new) == -1) {
                    continue;
                }
                if ((!category || skill_school(new) == new)) {
                    /*
                     *  Choose only rare skills if rare is set.
                     *
                     */
                    if (rare &&
                        !magic_skill(new) &&
                        !religion_skill(new)) {
                        continue;
                    }
                    /*
                     *   Don't pick something if it violates proscribed_skills.
                     *
                     */
                    if (nat && ilist_lookup(proscribed_skills, new) != -1) {
                        continue;
                    }
                    /*
                     *  Would this line be picked if last?
                     *
                     */
                    if (rnd(1, ++count) == 1) {
                        skill = new;
                    };
                    /*
                     *  If this is a non-rare line, pretend it is in the list twice,
                     *  thereby doubling (roughly) it's chance of being selected.
                     *
                     */
                    if (!magic_skill(new) &&
                        !religion_skill(new) &&
                        rnd(1, ++count) == 1) {
                        skill = new;
                    };
                };
            }next_skill;

    new = create_unique_item(who, subkind);

    /*
     *  Thu Jun 22 06:46:09 2000 -- Scott Turner
     *
     *  Set an arbitrary base price of 200 gold for books.
     *
     */
    p_item(new)->base_price = 200;

    p = p_item_magic(new);

    if (subkind == sub_book) {
        p_item(new)->weight = 5;
        p->orb_use_count = rnd(7, 28);
    } else {
        p_item(new)->weight = 1;
        p->orb_use_count = 1;
    };

    /*
     *  Add the skill...
     *
     */
    ilist_append(&p->may_study, skill);
    if (p->orb_use_count < learn_time(skill)) {
        p->orb_use_count = learn_time(skill);
    }

    if (subkind == sub_book) {
        /*
         *  Might be a very special book.
         *
         */
        int chance = rnd(1, 300);
        if (chance == 1) {
            /*  Tome of magic... */
            set_name(new, "Tome of Magic");
            ilist_clear(&p->may_study);
            loop_skill(skill)
                    {
                        if (skill == sk_adv_sorcery) { continue; }
                        if (new == sk_basic_religion) { continue; }
                        if (magic_skill(skill) && skill_school(skill) == skill) {
                            ilist_append(&p->may_study, skill);
                        }
                    }next_skill;
        } else if (chance == 2) {
            /*  Bible */
            set_name(new, "Great Bible");
            ilist_clear(&p->may_study);
            loop_skill(skill)
                    {
                        if (religion_skill(skill) && skill_school(skill) == skill) {
                            ilist_append(&p->may_study, skill);
                        }
                    }next_skill;
        } else if (chance == 3) {
            /*  Tome of All Knowledge... */
            set_name(new, "Tome of All Knowledge");
            ilist_clear(&p->may_study);
            loop_skill(skill)
                    {
                        if (skill == sk_adv_sorcery) { continue; }
                        if (new == sk_basic_religion) { continue; }
                        if (ilist_lookup(proscribed_skills, skill) != -1) { continue; }
                        if (skill_school(skill) == skill) {
                            ilist_append(&p->may_study, skill);
                        }
                    }next_skill;
        } else if (rnd(1, 100) < 75) {
            sprintf(s, "Manual of %s", just_name(skill));
            set_name(new, s);
        } else {
            switch (rnd(1, 3)) {
                case 1:
                    strcpy(s, "old book");
                    break;
                case 2:
                    strcpy(s, "rare book");
                    break;
                case 3:
                    strcpy(s, "strange tome");
                    break;
                case 4:
                    strcpy(s, "ancient manual");
                    break;

                default:
                    assert(FALSE);
            }
            set_name(new, s);
        };
    };

    ilist_reclaim(&proscribed_skills);
    return new;
}

/*
 *  Find an artifact in our region held by a subloc monster
 *  which is not only-defeatable by another artifact.
 */

static int
free_artifact(int where) {
    int reg = region(where);
    int i;
    int owner;
    ilist l = NULL;
    int ret;

    loop_item(i)
            {
                if (subkind(i) != sub_artifact) {
                    continue;
                }

                owner = item_unique(i);
                assert(owner);

                if (region(owner) != reg) {
                    continue;
                }

                if (!is_npc(owner) ||
                    npc_program(owner) != PROG_subloc_monster) {
                    continue;
                }

                if (only_defeatable(owner)) {
                    continue;
                }

                ilist_append(&l, i);
            }
    next_item;

    if (ilist_len(l) == 0) {
        return 0;
    }

    ret = l[rnd(0, ilist_len(l) - 1)];

    ilist_reclaim(&l);

    return ret;
}


#if 0
static int
new_artifact(int who)
{
    int new;
    char *s;

    new = create_unique_item(who, sub_artifact);

    switch (rnd(1,4))
    {
    case 1:
        s = art_att_s[rnd(0,2)];
        p_item_magic(new)->attack_bonus = rnd(1,10) * 5;
        break;

    case 2:
        s = art_def_s[rnd(0,2)];
        p_item_magic(new)->defense_bonus = rnd(1,10) * 5;
        break;

    case 3:
        s = art_mis_s[rnd(0,3)];
        p_item_magic(new)->missile_bonus = rnd(1,10) * 5;
        break;

    case 4:
        s = art_mag_s[rnd(0,2)];
        p_item_magic(new)->aura_bonus = rnd(1,3);
        break;

    default:
        assert(FALSE);
    }

    if (rnd(1,3) < 3)
    {
        s = sout("%s %s", pref[rnd(0,4)], s);
    }
    else
    {
        int i;

        for (i = 0; of_names[i]; i++)
            ;
        i = rnd(0, i-1);

        s = sout("%s of %s", cap(s), of_names[i]);
    }

    p_item(new)->weight = 10;
    set_name(new, s);

    return new;
}
#endif

static int
new_monster(int where) {
    int new;
    int item = 0;

    switch (subkind(where)) {
        case sub_graveyard:
        case sub_battlefield:
            switch (rnd(1, 3)) {
                case 1:
                    item = item_corpse;
                    break;
                case 2:
                    item = item_skeleton;
                    break;
                case 3:
                    item = item_spirit;
                    break;
                default:
                    assert(FALSE);
            }
            break;

        case sub_ench_forest:
            switch (rnd(1, 2)) {
                case 1:
                    item = item_elf;
                    break;
                case 2:
                    item = item_faery;
                    break;
                default:
                    assert(FALSE);
            }
            break;

        case sub_island:
            if (rnd(1, 2) == 1) {
                switch (rnd(1, 3)) {
                    case 1:
                        item = item_pirate;
                        break;
                    case 2:
                        item = item_spider;
                        break;
                    case 3:
                        item = item_cyclops;
                        break;
                    default:
                        assert(FALSE);
                };
                break;
            };

        default:
            item = random_beast(0);
    }

    /*
     *  Base this on the split size.
     *
     */
    new = create_monster_stack(item,
                               rnd(2, item_split(item) ? 10 : item_split(item) / 2),
                               where);
    p_char(new)->npc_prog = PROG_subloc_monster;
    return new;
}

int
seed_subloc_with_monster(int where, int limit) {
    int monster;

    monster = new_monster(where);
    /*
     *  Keep track of how many of each kind of monster we
     *  generate.
     *
     */
    bx[noble_item(monster)]->temp++;
    generate_treasure(monster, 1);

    if (rnd(1, 6) == 1) {
        int item;
        /*
         *  Temporarily set only_vulnerable for ourselves so we don't
         *  have a circular problem.  free_artifact() will take care of
         *  skipping over other only_vulnerable's.
         */
        p_misc(monster)->only_vulnerable = 1;
        item = free_artifact(monster);

        if (item)
            rp_misc(monster)->only_vulnerable = item;
        else
            rp_misc(monster)->only_vulnerable = 0;
    }
    /*
     *  Fri Nov 26 17:54:59 1999 -- Scott Turner
     *
     *  Make him a subloc monster.
     *
     */
    p_char(monster)->npc_prog = PROG_subloc_monster;

    /*
     *  Tue May 25 11:02:29 1999 -- Scott Turner
     *
     *  Close the border.
     *
     */
    p_subloc(where)->control.closed = 1;

    if (limit && rnd(1, 6) < 3) {
        p_subloc(where)->entrance_size = rnd(1, 4) + rnd(1, 4);
    };

    return monster;
}


void
seed_monster_sublocs(int all) {
    int where;
    int i;
    stage("seed_monster_sublocs()");

    clear_temps(T_item);

    loop_loc(where)
            {
                if (loc_depth(where) != LOC_subloc) {
                    continue;
                }

                if (in_faery(where) || in_hades(where)) {
                    continue;
                }

                if (subkind(where) == sub_city) {
                    continue;
                }

                if (controlled_humans_here(province(where))) {
                    continue;
                }

                seed_subloc_with_monster(where, 1);
            }
    next_loc;

    loop_item(i)
            {
                if (bx[i]->temp) {
                    wout(gm_player, "Generated %d stacks of %s.",
                         bx[i]->temp, box_name(i));
                }
            }next_item;
}

/*
 *  Tue Oct 13 18:50:29 1998 -- Scott Turner
 *
 *  Randomly select an appropriate subloc for reseeding.
 *
 */
void
add_lair_monster() {
    int sum = 0, choice, owner, where;

    loop_loc(where)
            {
                if (loc_depth(where) != LOC_subloc ||
                    in_faery(where) || in_hades(where) ||
                    subkind(where) == sub_city ||
                    controlled_humans_here(province(where)) ||
                    has_item(province(where), item_peasant) > 100) {
                    continue;
                }

                /*
                 *  A laired beast is one that owns the location and
                 *  has the subloc monster program.
                 *
                 */
                owner = first_character(where);
                if (owner && is_real_npc(owner)) { continue; }
                /*
                 *  Possibly select.
                 *
                 */
                sum++;
                if (rnd(1, sum) == 1) { choice = where; }
            }next_loc;

    wout(gm_player, "Seeding %s with a monster.", box_name(where));
    seed_subloc_with_monster(where, 0);
};

/*
 *  Thu Sep 24 13:40:03 1998 -- Scott Turner
 *
 *  Like seed, except only sometimes and never where there
 *  is civilization.
 *
 *  Tue Oct 13 18:38:17 1998 -- Scott Turner
 *
 *  What this really needs to do is to try to keep the sublocs partially
 *  stocked.  What we need to do is count the number of sublocs, the number
 *  of sublocs with laired beasts and go from there...
 *
 */
void
reseed_monster_sublocs() {
    int where;
    int num_sublocs = 0;
    int num_laired = 0;
    int needed, i, owner;

    stage("reseed_monster_sublocs()");

    loop_loc(where)
            {

                if (loc_depth(where) != LOC_subloc) {
                    continue;
                }

                if (in_faery(where) || in_hades(where)) {
                    continue;
                }

                if (subkind(where) == sub_city) {
                    continue;
                }

                if (controlled_humans_here(province(where))) {
                    continue;
                }

                if (has_item(province(where), item_peasant) > 100) {
                    continue;
                }

                num_sublocs++;

                /*
                   *  A laired beast is one that owns the location and
                   *  has the subloc monster program.
                   *
                   */
                owner = first_character(where);
                if (owner && is_npc(owner) && npc_program(owner) == PROG_subloc_monster) {
                    num_laired++;
                }
            }next_loc;

    /*  We want 33% of the sublocs to have laired monsters. */
    needed = ((num_sublocs * 33) / 100) - num_laired;

    if (needed > 0) {
        log_output(LOG_MISC, "Reseeding %d sublocs.", needed);
    } else {
        log_output(LOG_MISC, "No need to reseed sublocs.");
    }

    for (i = 0; i < needed; i++) {
        add_lair_monster();
    }
}

