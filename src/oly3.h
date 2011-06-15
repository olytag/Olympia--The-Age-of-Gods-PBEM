/*
 *  Defines to turn on/control the Oly 3 Changes
 *  Thu Jun 27 11:20:01 1996 -- Scott Turner
 *
 *
 */
#define ATTACK_LIMIT 4   /* The number of attacks in a row permitted. */
#define CONTROL_SKILL          /* New skill to control men in battle. */
#define DEFAULT_CONTROLLED 10  /* Number controlled if you don't have the skill.*/
#define GARRISON_CONTROLLED 20  /* Default controlled by garrison */
#define DEFENDER_CONTROL_BONUS 20  /* On defense, control extra men. */
#define TACTICS_FACTOR 0.02    /* % given with each week of study. */
#define TACTICS_LIMIT 2.0      /* Maximum bonus from tactics. */
#define CITY_DEFENSE_BONUS 1.25      /* Terrain bonuses to defense */
#define FOREST_DEFENSE_BONUS 1.50
#define MOUNTAIN_DEFENSE_BONUS 2.00
#define SWAMP_DEFENSE_BONUS 0.75

/*
 *  Scrolls & Books
 *
 */
#define SCROLL_CHANCE 50  /* % for a scroll or book to appear in a city */

/*
 *  Shipcraft stuff
 *
 */
#define HULL_CAPACITY 2500
#define FORT_WEIGHT 250
#define SAIL_WEIGHT 250
#define KEEL_WEIGHT 100

#define SAILS_PER_HULL 3
#define KEELS_PER_HULL 1
#define PORTS_PER_HULL 3

#define SHIP_FORTS_PROTECT 10
#define SHIP_FORTS_BONUS 5

#define ROWERS_PER_PORT 4

/*
 *  Moat Constants
 *
 */
#define MOAT_MATERIAL 200
#define MOAT_EFFORT 4000
