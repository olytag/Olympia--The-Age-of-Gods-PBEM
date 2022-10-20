#include    <stdio.h>
#include    <sys/types.h>
#include    <dirent.h>
#include    "z.h"
#include    "oly.h"

/*
 *  Fri Apr 11 20:10:01 1997 -- Scott Turner
 *
 *  Has one or the other Nation won?
 *
 */
#define MIN_TURNS 12

static int
check_nation_win() {
    int k, i, flag, ruler, j, total;

    /*
     *  This is possibly redundant.
     *
     */
    calculate_nation_nps();

    /*
     *  Minimum # of turns.
     *
     */
    if (sysclock.turn < MIN_TURNS) { return FALSE; }

    loop_nation(k)
            {

                /*
                 *  Thu Mar 29 12:38:38 2001 -- Scott Turner
                 *
                 *  Ignore neutral nations.
                 *
                 */
                if (p_nation(k)->neutral) { continue; }

                flag = 0;
                loop_city(i)
                        {
                            ruler = player_controls_loc(i);
                            if (ruler && nation(ruler) && nation(ruler) != k) {
                                flag = 1;
                                break;
                            };
                        }next_city;
                if (flag) { continue; }

                loop_castle(i)
                        {
                            ruler = player_controls_loc(i);
                            if (ruler && nation(ruler) && nation(ruler) != k) {
                                flag = 1;
                                break;
                            };
                        }next_castle;
                if (flag) { continue; }

                total = 0;
                loop_nation(j)
                        {
                            if (p_nation(j)->neutral) { continue; }
                            if (k != j) { total += rp_nation(j)->nps; }
                        }next_nation;

                /*
                 *  You haven't met the win conditions this turn, so
                 *  we should zero you out.
                 *
                 */
                if (total * 2 >= rp_nation(k)->nps) {
                    rp_nation(k)->win = 0;
                    continue;
                };

                /*
                 *  Fri Apr 11 20:32:43 1997 -- Scott Turner
                 *
                 *  Add another turn...
                 *
                 */
                rp_nation(k)->win++;

                /*
                 *  Two nations cannot win simultaneously because of
                 *  the last condition.
                 *
                 */
                if (rp_nation(k)->win == 2) {
                    /*
                     *  We have a winner!  This really needs to go into
                     *  the front of the Times, but how?
                     *
                     */
                    return TRUE;
                };
            }next_nation;

    return FALSE;
};

/*
 *  Fri Apr 11 20:07:41 1997 -- Scott Turner
 *
 *  Is the game over?
 *
 *
 */
void
check_win_conditions() {
    /*
     *  Nation win
     *
     */
    if (check_nation_win()) {
        /*
         *  Do something here.
         *
         */
        fprintf(stderr, "Nation win!\n");
    };
};
