// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include    <stdio.h>
#include    <sys/types.h>
#include    <dirent.h>
#include    <string.h>
#include <stdlib.h>
#include    "z.h"
#include    "oly.h"
#include "forward.h"


int monster_subloc_init = FALSE;
int population_init = FALSE;


/*
 *  io.c -- load and save the entity database
 */

/*
 * Mon May 10 07:19:21 1999 -- Scott Turner
 * Go through an ilist and substitute the new skill numbers.
 *
 */
int
convert_skill(int skill) {
    if (skill == 120) { return 1000; }
    if (skill == 9502) { return 1001; }
    if (skill == 9503) { return 1002; }
    if (skill == 9554) { return 1095; }
    if (skill == 9582) { return 1004; }
    if (skill == 9614) { return 1005; }
    if (skill == 9616) { return 1006; }
    if (skill == 9618) { return 1096; }
    if (skill == 9620) { return 1097; }
    if (skill == 9622) { return 1098; }
    if (skill == 9617) { return 1090; }
    if (skill == 9615) { return 1091; }
    if (skill == 9621) { return 1092; }
    if (skill == 9619) { return 1093; }
    if (skill == 9510) { return 1094; }

    if (skill == 121) { return 1100; }
    if (skill == 9501) { return 1193; }
    if (skill == 9505) { return 1102; }
    if (skill == 9541) { return 1194; }
    if (skill == 9570) { return 1104; }
    if (skill == 9580) { return 1105; }
    if (skill == 9581) { return 1106; }
    if (skill == 9594) { return 1107; }
    if (skill == 9598) { return 1108; }
    if (skill == 9507) { return 1195; }
    if (skill == 9595) { return 1131; }
    if (skill == 9596) { return 1132; }
    if (skill == 9612) { return 1190; }
    if (skill == 9613) { return 1191; }
    if (skill == 9599) { return 1192; }

    if (skill == 122) { return 1200; }
    if (skill == 9509) { return 1201; }
    if (skill == 9519) { return 1202; }
    if (skill == 9520) { return 1203; }
    if (skill == 9521) { return 1204; }
    if (skill == 9591) { return 1205; }
    if (skill == 9522) { return 1230; }
    if (skill == 9523) { return 1231; }
    if (skill == 9562) { return 1232; }
    if (skill == 9574) { return 1233; }
    if (skill == 9593) { return 1290; }
    if (skill == 9140) { return 1291; }
    if (skill == 9149) { return 1292; }

    if (skill == 124) { return 1300; }
    if (skill == 9515) { return 1301; }
    if (skill == 9537) { return 1302; }
    if (skill == 9538) { return 1303; }
    if (skill == 9592) { return 1304; }
    if (skill == 9536) { return 1330; }
    if (skill == 9539) { return 1331; }
    if (skill == 9585) { return 1332; }
    if (skill == 9586) { return 1333; }
    if (skill == 9610) { return 1390; }

    if (skill == 125) { return 1400; }
    if (skill == 9542) { return 1401; }
    if (skill == 9566) { return 1402; }
    if (skill == 9611) { return 1490; }
    if (skill == 9107) { return 1491; }
    if (skill == 9114) { return 1492; }
    if (skill == 9127) { return 1493; }

    if (skill == 126) { return 1500; }
    if (skill == 9551) { return 1501; }
    if (skill == 9552) { return 1502; }
    if (skill == 9584) { return 1503; }
    if (skill == 9549) { return 1530; }
    if (skill == 9550) { return 1531; }
    if (skill == 9573) { return 1532; }
    if (skill == 9590) { return 1533; }
    if (skill == 9553) { return 1590; }
    if (skill == 9623) { return 1591; }

    if (skill == 128) { return 1600; }
    if (skill == 9540) { return 1601; }
    if (skill == 9568) { return 1602; }
    if (skill == 9569) { return 1603; }
    if (skill == 9583) { return 1604; }
    if (skill == 9588) { return 1605; }
    if (skill == 9587) { return 1630; }
    if (skill == 9589) { return 1631; }

    if (skill == 129) { return 1700; }
    if (skill == 9563) { return 1701; }
    if (skill == 9564) { return 1702; }
    if (skill == 9128) { return 1703; }
    if (skill == 9565) { return 1730; }
    if (skill == 9567) { return 1731; }
    if (skill == 9129) { return 1790; }
    if (skill == 9130) { return 1791; }

    if (skill == 130) { return 1800; }
    if (skill == 9603) { return 1801; }
    if (skill == 9604) { return 1802; }
    if (skill == 9605) { return 1803; }
    if (skill == 9606) { return 1804; }
    if (skill == 9607) { return 1805; }
    if (skill == 9600) { return 1830; }
    if (skill == 9601) { return 1831; }
    if (skill == 9602) { return 1832; }
    if (skill == 9608) { return 1890; }
    if (skill == 9609) { return 1891; }

    if (skill == 131) { return 1900; }
    if (skill == 9145) { return 1901; }
    if (skill == 9579) { return 1902; }
    if (skill == 9150) { return 1903; }
    if (skill == 9517) { return 1930; }
    if (skill == 9529) { return 1931; }
    if (skill == 9530) { return 1932; }
    if (skill == 9146) { return 1990; }

    if (skill == 151) { return 2000; }
    if (skill == 9302) { return 2001; }
    if (skill == 9303) { return 2002; }
    if (skill == 9304) { return 2003; }
    if (skill == 9305) { return 2004; }
    if (skill == 9306) { return 2005; }
    if (skill == 9307) { return 2006; }
    if (skill == 9308) { return 2007; }
    if (skill == 9309) { return 2008; }
    if (skill == 9447) { return 2009; }
    if (skill == 9124) { return 2030; }
    if (skill == 9191) { return 2031; }
    if (skill == 9193) { return 2032; }
    if (skill == 9194) { return 2033; }
    if (skill == 9195) { return 2034; }
    if (skill == 9196) { return 2035; }
    if (skill == 9187) { return 2036; }
    if (skill == 9188) { return 2037; }
    if (skill == 9189) { return 2038; }
    if (skill == 9190) { return 2039; }
    if (skill == 9310) { return 2040; }
    if (skill == 9148) { return 2041; }
    if (skill == 9155) { return 2042; }

    if (skill == 152) { return 2100; }
    if (skill == 9312) { return 2101; }
    if (skill == 9313) { return 2102; }
    if (skill == 9314) { return 2103; }
    if (skill == 9315) { return 2104; }
    if (skill == 9316) { return 2105; }
    if (skill == 9317) { return 2106; }
    if (skill == 9318) { return 2107; }
    if (skill == 9319) { return 2108; }
    if (skill == 9440) { return 2109; }
    if (skill == 9311) { return 2130; }
    if (skill == 9400) { return 2131; }
    if (skill == 9401) { return 2132; }
    if (skill == 9402) { return 2133; }
    if (skill == 9403) { return 2134; }
    if (skill == 9404) { return 2135; }
    if (skill == 9405) { return 2136; }
    if (skill == 9406) { return 2137; }
    if (skill == 9433) { return 2138; }
    if (skill == 9156) { return 2139; }

    if (skill == 153) { return 2200; }
    if (skill == 9322) { return 2201; }
    if (skill == 9323) { return 2202; }
    if (skill == 9324) { return 2203; }
    if (skill == 9325) { return 2204; }
    if (skill == 9326) { return 2205; }
    if (skill == 9327) { return 2206; }
    if (skill == 9328) { return 2207; }
    if (skill == 9329) { return 2208; }
    if (skill == 9441) { return 2209; }
    if (skill == 9504) { return 2231; }
    if (skill == 9506) { return 2232; }
    if (skill == 9508) { return 2233; }
    if (skill == 9320) { return 2234; }
    if (skill == 9321) { return 2235; }
    if (skill == 9434) { return 2236; }
    if (skill == 9157) { return 2237; }

    if (skill == 154) { return 2300; }
    if (skill == 9332) { return 2301; }
    if (skill == 9333) { return 2302; }
    if (skill == 9334) { return 2303; }
    if (skill == 9335) { return 2304; }
    if (skill == 9336) { return 2305; }
    if (skill == 9337) { return 2306; }
    if (skill == 9338) { return 2307; }
    if (skill == 9339) { return 2308; }
    if (skill == 9442) { return 2309; }
    if (skill == 9407) { return 2330; }
    if (skill == 9408) { return 2331; }
    if (skill == 9409) { return 2332; }
    if (skill == 9410) { return 2333; }
    if (skill == 9411) { return 2334; }
    if (skill == 9412) { return 2335; }
    if (skill == 9413) { return 2336; }
    if (skill == 9435) { return 2337; }
    if (skill == 9158) { return 2338; }

    if (skill == 155) { return 2400; }
    if (skill == 9342) { return 2401; }
    if (skill == 9343) { return 2402; }
    if (skill == 9344) { return 2403; }
    if (skill == 9345) { return 2404; }
    if (skill == 9346) { return 2405; }
    if (skill == 9347) { return 2406; }
    if (skill == 9348) { return 2407; }
    if (skill == 9349) { return 2408; }
    if (skill == 9443) { return 2409; }
    if (skill == 9419) { return 2430; }
    if (skill == 9420) { return 2431; }
    if (skill == 9421) { return 2432; }
    if (skill == 9422) { return 2433; }
    if (skill == 9341) { return 2434; }
    if (skill == 9436) { return 2435; }
    if (skill == 9448) { return 2436; }
    if (skill == 9159) { return 2437; }

    if (skill == 156) { return 2500; }
    if (skill == 9352) { return 2501; }
    if (skill == 9353) { return 2502; }
    if (skill == 9354) { return 2503; }
    if (skill == 9355) { return 2504; }
    if (skill == 9356) { return 2505; }
    if (skill == 9357) { return 2506; }
    if (skill == 9358) { return 2507; }
    if (skill == 9359) { return 2508; }
    if (skill == 9444) { return 2509; }
    if (skill == 9414) { return 2530; }
    if (skill == 9415) { return 2531; }
    if (skill == 9416) { return 2532; }
    if (skill == 9417) { return 2533; }
    if (skill == 9418) { return 2534; }
    if (skill == 9437) { return 2535; }
    if (skill == 9162) { return 2536; }

    if (skill == 157) { return 2600; }
    if (skill == 9362) { return 2601; }
    if (skill == 9363) { return 2602; }
    if (skill == 9364) { return 2603; }
    if (skill == 9365) { return 2604; }
    if (skill == 9366) { return 2605; }
    if (skill == 9367) { return 2606; }
    if (skill == 9368) { return 2607; }
    if (skill == 9369) { return 2608; }
    if (skill == 9445) { return 2609; }
    if (skill == 9423) { return 2630; }
    if (skill == 9424) { return 2631; }
    if (skill == 9425) { return 2632; }
    if (skill == 9426) { return 2633; }
    if (skill == 9427) { return 2634; }
    if (skill == 9428) { return 2635; }
    if (skill == 9438) { return 2636; }
    if (skill == 9163) { return 2637; }

    if (skill == 158) { return 2700; }
    if (skill == 9372) { return 2701; }
    if (skill == 9373) { return 2702; }
    if (skill == 9374) { return 2703; }
    if (skill == 9375) { return 2704; }
    if (skill == 9376) { return 2705; }
    if (skill == 9377) { return 2706; }
    if (skill == 9378) { return 2707; }
    if (skill == 9379) { return 2708; }
    if (skill == 9446) { return 2709; }
    if (skill == 9429) { return 2730; }
    if (skill == 9430) { return 2731; }
    if (skill == 9431) { return 2732; }
    if (skill == 9432) { return 2733; }
    if (skill == 9439) { return 2734; }
    if (skill == 9166) { return 2735; }

    if (skill == 160) { return 2800; }
    if (skill == 9101) { return 2801; }
    if (skill == 9103) { return 2802; }
    if (skill == 9104) { return 2803; }
    if (skill == 9105) { return 2804; }
    if (skill == 9106) { return 2830; }
    if (skill == 9123) { return 2831; }
    if (skill == 9126) { return 2832; }
    if (skill == 9147) { return 2833; }
    if (skill == 9169) { return 2834; }
    if (skill == 9170) { return 2835; }
    if (skill == 9173) { return 2836; }
    if (skill == 9174) { return 2837; }
    if (skill == 9175) { return 2838; }
    if (skill == 9178) { return 2839; }
    if (skill == 9135) { return 2840; }
    if (skill == 9624) { return 2841; }

    if (skill == 162) { return 2900; }
    if (skill == 9141) { return 2901; }
    if (skill == 9142) { return 2930; }
    if (skill == 9143) { return 2931; }
    if (skill == 9151) { return 2932; }
    if (skill == 9160) { return 2933; }
    if (skill == 9161) { return 2934; }
    if (skill == 9167) { return 2935; }
    if (skill == 9171) { return 2936; }
    if (skill == 9172) { return 2937; }
    if (skill == 9177) { return 2938; }
    if (skill == 9197) { return 2939; }
    if (skill == 9136) { return 2940; }

    if (skill == 163) { return 3000; }
    if (skill == 9115) { return 3001; }
    if (skill == 9116) { return 3002; }
    if (skill == 9112) { return 3030; }
    if (skill == 9117) { return 3031; }
    if (skill == 9118) { return 3032; }
    if (skill == 9119) { return 3033; }
    if (skill == 9120) { return 3034; }
    if (skill == 9121) { return 3035; }
    if (skill == 9122) { return 3036; }
    if (skill == 9152) { return 3037; }
    if (skill == 9165) { return 3038; }
    if (skill == 9137) { return 3039; }

    if (skill == 164) { return 3100; }
    if (skill == 9108) { return 3101; }
    if (skill == 9109) { return 3102; }
    if (skill == 9164) { return 3103; }
    if (skill == 9168) { return 3104; }
    if (skill == 9102) { return 3130; }
    if (skill == 9110) { return 3131; }
    if (skill == 9111) { return 3132; }
    if (skill == 9113) { return 3133; }
    if (skill == 9131) { return 3134; }
    if (skill == 9132) { return 3135; }
    if (skill == 9133) { return 3136; }
    if (skill == 9144) { return 3137; }
    if (skill == 9153) { return 3138; }
    if (skill == 9176) { return 3139; }

    if (skill == 165) { return 3200; }
    if (skill == 9179) { return 3201; }
    if (skill == 9180) { return 3202; }
    if (skill == 9185) { return 3203; }
    if (skill == 9154) { return 3230; }
    if (skill == 9181) { return 3231; }
    if (skill == 9182) { return 3232; }
    if (skill == 9183) { return 3233; }
    if (skill == 9184) { return 3234; }
    if (skill == 9186) { return 3235; }
    if (skill == 9138) { return 3236; }
    if (skill == 9625) { return 3237; }
    if (skill == 9125) { return 3238; }

    if (skill == 170) { return 3300; }
    if (skill == 9201) { return 3330; }
    if (skill == 9202) { return 3331; }
    if (skill == 9139) { return 3332; }
    if (skill == 9626) { return 3333; }
    return skill;
};

/*
 *  linehash macro
 *
 *	data is stored in the form:
 *		xy data
 *	were 'xy' is a key followed by one character of whitespace
 *
 *	linehash macro returns 'xy' crunched into an int
 *
 *  Example:
 *
 *	s = "na Name field";
 *	c = linehash(s);
 *	assert(c == 'na');
 */

#define    linehash(t)    (strlen(t) < 2 ? 0 : ((t[0]) << 8 | (t[1])))
#define    t_string(t)    (strlen(t) >= 4 ? &t[3] : "")


/*
 *  advance lets us do a one-line lookahead in the scanning routines
 */

static char *line;

#define        advance        line = readlin()


static int ext_boxnum;


/*
 *  Routine to check if a structure is completely empty.
 *
 *  Since structures may have elements which are not saved by save_db(),
 *  this routine may return true when, in fact, no data from the structure
 *  will be saved.  However, the next turn run will clear this up, since
 *  the re-loaded empty structure will now pass zero_check.
 *
 *  Using zero_check is more reliable than element testing, since we might
 *  forget to add one to the check list.  Also, our concern is long-term
 *  buildup of unused empty structure, so keeping one around for a turn
 *  is not a big deal.
 */

static int
zero_check(void *ptr, unsigned size) {
    char *p = ptr;
    int sum = 0;

    while (size-- > 0) {
        sum |= *p++;
    }

    return sum == 0;
}


/*
 *  Returns the entity name in parenthesis, if available, to make the
 *  data files easier to read.
 */

static char *
if_name(int num)        /* to make the data files easier to read */
{
    char *s;
    extern int pretty_data_files;

    if (!pretty_data_files) {
        return "";
    }

    if (!valid_box(num)) {
        return "";
    }

    s = name(num);

    if (s && *s) {
        return sout(" (%s)", s);
    }

    return "";
}


static int
box_scan(char *t) {
    int n;

    n = atoi(t);

    if (valid_box(n)) {
        return n;
    }

#if 0
                                                                                                                            /* temp fix */
	if (convert_skill(n) != n)
	  return convert_skill(n);
#endif

    fprintf(stderr, "box_scan(%d): bad reference: %s\n", ext_boxnum, line);

    return 0;
}


static void
box_print(FILE *fp, char *header, int n) {
    /* assert(!n || valid_box(n)); */

    if (valid_box(n)) {
        fprintf(fp, "%s%d%s\n", header, n, if_name(n));
    }
}


/*
 *  boxlist0_scan, boxlist0_print:
 *  same as boxlist_xxx, but allows zero
 */

static void
boxlist0_scan(char *s, int box_num, ilist *l) {
    int n;

    while (1) {
        while (*s && iswhite(*s)) {
            s++;
        }

        if (isdigit(*s)) {
            n = atoi(s);

            if (n == 0 || valid_box(n)) {
                ilist_append(l, n);
            }
#if 0
                                                                                                                                        /* temp fix */
			else if (convert_skill(n) != n)
			  ilist_append(l, convert_skill(n));
#endif
            else {
                fprintf(stderr, "boxlist_scan(%d): bad box "
                                "reference: %d\n", box_num, n);
            }

            while (*s && isdigit(*s)) {
                s++;
            }
        } else if (*s == '\\') {        /* continuation line follows */
            s = readlin_ew();
        } else {
            break;
        }
    }
}


static void
boxlist0_print(FILE *fp, char *header, ilist l) {
    int i;
    int count = 0;

    for (i = 0; i < ilist_len(l); i++) {
        if (l[i] == 0 || valid_box(l[i])) {
            count++;
            if (count == 1) {
                fputs(header, fp);
            }

            if (count % 11 == 10) {        /* continuation line */
                fputs("\\\n\t", fp);
            }

            fprintf(fp, "%d ", l[i]);
        }
    }

    if (count) {
        fprintf(fp, "\n");
    }
}


static void
boxlist_scan(char *s, int box_num, ilist *l) {
    int n;

    while (1) {
        while (*s && iswhite(*s)) {
            s++;
        }

        if (isdigit(*s) || *s == '-') {
            n = atoi(s);

            if (valid_box(n) || n == MONSTER_ATT) {
                ilist_append(l, n);
            }
#if 0
                                                                                                                                        /* temp fix */
			else if (convert_skill(n) != n)
			  ilist_append(l, convert_skill(n));
#endif
            else {
                fprintf(stderr, "boxlist_scan(%d): bad box "
                                "reference: %d\n", box_num, n);
            }

            while (*s && (isdigit(*s) || *s == '-')) {
                s++;
            }
        } else if (*s == '\\') {        /* continuation line follows */
            s = readlin_ew();
        } else {
            break;
        }
    }
}


static void
boxlist_print(FILE *fp, char *header, ilist l) {
    int i;
    int count = 0;

    if (l == NULL) { return; }

    for (i = 0; i < ilist_len(l); i++) {
        if (valid_box(l[i]) || l[i] == MONSTER_ATT) {
            count++;
            if (count == 1) {
                fputs(header, fp);
            }

            if (count % 11 == 10) {        /* continuation line */
                fputs("\\\n\t", fp);
            }

            fprintf(fp, "%d ", l[i]);
        }
    }

    if (count) {
        fprintf(fp, "\n");
    }
}


static void
admit_print_sup(FILE *fp, struct admit *p) {
    int i;
    int count = 2;

    if (!valid_box(p->targ)) {
        return;
    }

    if (p->sense == 0 && ilist_len(p->l) == 0) {
        return;
    }

    fprintf(fp, " am %d %d ", p->targ, p->sense);

    for (i = 0; i < ilist_len(p->l); i++) {
        if (valid_box(p->l[i])) {
            count++;

            if (count % 11 == 10) {        /* continuation line */
                fputs("\\\n\t", fp);
            }

            fprintf(fp, "%d ", p->l[i]);
        }
    }

    if (count) {
        fprintf(fp, "\n");
    }
}


static void
admit_print(FILE *fp, struct entity_player *p) {
    int i;

    for (i = 0; i < ilist_len(p->admits); i++) {
        admit_print_sup(fp, p->admits[i]);
    }
}


static void
admit_scan(char *s, int box_num, struct entity_player *pp) {
    int n;
    int count = 0;
    struct admit *p;

    p = my_malloc(sizeof(*p));

    while (1) {
        while (*s && iswhite(*s)) {
            s++;
        }

        if (isdigit(*s)) {
            n = atoi(s);

            switch (count++) {
                case 0:
                    p->targ = n;
                    break;

                case 1:
                    p->sense = n;
                    break;

                default:
                    /* Temp fix for nations */
                    if (n <= 1002 && n >= 1000) { n -= 3; }
                    if (valid_box(n)) {
                        ilist_append(&p->l, n);
                    } else {
                        fprintf(stderr, "admit_scan(%d): "
                                        "bad box reference: %d\n", box_num, n);
                    }
            }

            while (*s && isdigit(*s)) {
                s++;
            }
        } else if (*s == '\\') {        /* continuation line follows */
            s = readlin_ew();
        } else {
            break;
        }
    }

    if (!valid_box(p->targ)) {
        fprintf(stderr, "admit_scan(%d): bad targ %d\n",
                box_num, p->targ);
        my_free(p);
        return;
    }

    ilist_append((ilist *) &(pp->admits), (int) p);
}


static void
ilist_print(FILE *fp, char *header, ilist l) {
    int i;

    if (ilist_len(l) > 0) {
        fputs(header, fp);

        for (i = 0; i < ilist_len(l); i++) {
            if (i % 11 == 10) {        /* continuation line */
                fprintf(fp, "\\\n\t");
            }

            fprintf(fp, "%d ", l[i]);
        }

        fprintf(fp, "\n");
    }
}


static void
ilist_scan(char *s, ilist *l) {

    while (1) {
        while (*s && iswhite(*s)) {
            s++;
        }

        if (isdigit(*s)) {
            ilist_append(l, atoi(s));

            while (*s && isdigit(*s)) {
                s++;
            }
        } else if (*s == '\\') {        /* continuation line follows */
            s = readlin_ew();
        } else {
            break;
        }
    }
}


static void
known_print(FILE *fp, char *header, sparse kn) {
    int i;
    int count = 0;
    int first = TRUE;

    loop_known(kn, i)
            {
                if (!valid_box(i)) {
                    continue;
                }

                if (first) {
                    fputs(header, fp);
                    first = FALSE;
                }

                if (count++ % 11 == 10) {
                    fprintf(fp, "\\\n\t");
                }

                fprintf(fp, "%d ", i);
            }
    next_known;

    if (!first) {
        fprintf(fp, "\n");
    }
}


static void
known_scan(char *s, sparse *kn, int box_num) {
    int n;

    while (1) {
        while (*s && iswhite(*s)) {
            s++;
        }

        if (isdigit(*s)) {
            n = atoi(s);

            if (valid_box(n)) {
                set_bit(kn, n);
            }
#if 0
                                                                                                                                        /* temp fix */
			else if (convert_skill(n) != n)
				set_bit(kn, convert_skill(n));
#endif
            else {
                fprintf(stderr, "known_scan(%d): bad box "
                                "reference: %d\n", box_num, n);
            }

            while (*s && isdigit(*s)) {
                s++;
            }
        } else if (*s == '\\') {        /* continuation line follows */
            s = readlin_ew();
        } else {
            break;
        }
    }
}


static void
skill_list_print(FILE *fp, char *header, struct skill_ent **l) {
    int i;
    int count = 0;

    for (i = 0; i < ilist_len(l); i++) {
        if (valid_box(l[i]->skill)) {
            count++;
            if (count == 1) {
                fputs(header, fp);
            }

            if (count > 1) {
                fputs(" \\\n\t", fp);
            }

            fprintf(fp, "%d %d %d %d 0",
                    l[i]->skill,
                    l[i]->know,
                    l[i]->days_studied,
                    l[i]->experience);
        }
    }

    if (count) {
        fputs("\n", fp);
    }
}


static void
skill_list_scan(char *s, struct skill_ent ***l, int box_num) {
    struct skill_ent *new;
    int dummy;
    int know, experience;

    while (1) {
        new = my_malloc(sizeof(*new));
        sscanf(s, "%d %d %d %d %d",
               &new->skill,
               &know,
               &new->days_studied,
               &experience,
               &dummy);

        new->know = know;
        new->experience = experience;

        new->skill = convert_skill(new->skill);
        if (valid_box(new->skill)) {
            ilist_append((ilist *) l, (int) new);
        } else {
            fprintf(stderr, "skill_list_scan(%d): bad skill %d\n",
                    box_num, new->skill);

            my_free(new);
        }

        if (s[strlen(s) - 1] == '\\') {    /* another entry follows */
            s = readlin_ew();
        } else {
            break;
        }
    }
}

/*
 *  Effect Functions
 *  Mon Aug  5 12:58:00 1996 -- Scott Turner
 *
 */
static void
effect_list_print(FILE *fp, char *header, struct effect **l) {
    int i;
    int count = 0;

    for (i = 0; i < ilist_len(l); i++) {
        count++;
        if (count == 1) {
            fputs(header, fp);
        }

        if (count > 1) {
            fputs(" \\\n\t", fp);
        }

        fprintf(fp, "%d %d %d %d",
                l[i]->type,
                l[i]->subtype,
                l[i]->days,
                l[i]->data);
    }

    if (count) {
        fputs("\n", fp);
    }
}

static void
effect_list_scan(char *s, struct effect ***l) {
    struct effect *new;

    while (1) {
        new = my_malloc(sizeof(*new));
        sscanf(s, "%d %d %d %d",
               &new->type,
               &new->subtype,
               &new->days,
               &new->data);

        ilist_append((ilist *) l, (int) new);

        if (s[strlen(s) - 1] == '\\') {    /* another entry follows */
            s = readlin_ew();
        } else {
            break;
        }
    }
}

/*
 *  Build Functions
 *  Mon Aug  5 12:58:00 1996 -- Scott Turner
 *
 */
static void
build_list_print(FILE *fp, char *header, struct entity_build **l) {
    int i;
    int count = 0;

    for (i = 0; i < ilist_len(l); i++) {
        count++;
        if (count == 1) {
            fputs(header, fp);
        }

        if (count > 1) {
            fputs(" \\\n\t", fp);
        }

        fprintf(fp, "%d %d %d %d",
                l[i]->type,
                l[i]->build_materials,
                l[i]->effort_required,
                l[i]->effort_given);
    }

    if (count) { fputs("\n", fp); }
}

static void
build_list_scan(char *s, struct entity_build ***l) {
    struct entity_build *new;

    while (1) {
        new = my_malloc(sizeof(*new));
        sscanf(s, "%d %d %d %d",
               &new->type,
               &new->build_materials,
               &new->effort_required,
               &new->effort_given);

        ilist_append((ilist *) l, (int) new);

        if (s[strlen(s) - 1] == '\\') {    /* another entry follows */
            s = readlin_ew();
        } else {
            break;
        }
    }
}

static int
item_list_print(FILE *fp, char *header, struct item_ent **l) {
    int i;
    int count = 0;

    for (i = 0; i < ilist_len(l); i++) {
        if (valid_box(l[i]->item) && l[i]->qty > 0) {
            count++;
            if (count == 1) {
                fputs(header, fp);
            }

            if (count > 1) {
                fputs(" \\\n\t", fp);
            }

            fprintf(fp, "%d %d",
                    l[i]->item,
                    l[i]->qty);
        }
    }

    if (count) {
        fputs("\n", fp);
    }

    return count;
}


static void
item_list_scan(char *s, struct item_ent ***l, int box_num) {
    struct item_ent *new;

    while (1) {
        new = my_malloc(sizeof(*new));
        sscanf(s, "%d %d", &new->item, &new->qty);

        if (valid_box(new->item)) {
            ilist_append((ilist *) l, (int) new);
        } else {
            fprintf(stderr, "item_list_scan(%d): bad item %d\n",
                    box_num, new->item);
            fprintf(stderr, "  bad item deleted from list.\n");

            my_free(new);
        }

        if (s[strlen(s) - 1] == '\\') {    /* another entry follows */
            s = readlin_ew();
        } else {
            break;
        }
    }
}


static void
trade_list_print(FILE *fp, char *header, struct trade **l) {
    int i;
    int count = 0;

    for (i = 0; i < ilist_len(l); i++) {
        if (valid_box(l[i]->item)) {
/*
 *  Weed out completed or cleared BUY and SELL trades, but don't
 *  touch PRODUCE or CONSUME zero-qty trades.
 *
 *  Tue Jun  6 13:29:32 2000 -- Scott Turner
 *
 *  Why not?  This causes problems because loop_trade ignores zero
 *  qty trades (as it probably should).
 *
 *      (l[i]->kind == BUY || l[i]->kind == SELL) &&
 */

            if (l[i]->qty <= 0) {
                continue;
            }

            count++;
            if (count == 1) {
                fputs(header, fp);
            }

            if (count > 1) {
                fputs(" \\\n\t", fp);
            }

            fprintf(fp, "%d %d %d %d %d %d %d %d %d",
                    l[i]->kind,
                    l[i]->item,
                    l[i]->qty,
                    l[i]->cost,
                    l[i]->cloak,
                    l[i]->have_left,
                    l[i]->month_prod,
                    l[i]->old_qty,
                    l[i]->counter);
        }
    }

    if (count) {
        fputs("\n", fp);
    }
}


static void
trade_list_scan(char *s, struct trade ***l, int box_num) {
    struct trade *new;

    while (1) {
        new = my_malloc(sizeof(*new));
        sscanf(s, "%d %d %d %d %d %d %d %d %d",
               &new->kind,
               &new->item,
               &new->qty,
               &new->cost,
               &new->cloak,
               &new->have_left,
               &new->month_prod,
               &new->old_qty,
               &new->counter);

        new->who = box_num;

        if (valid_box(new->item)) {
            ilist_append((ilist *) l, (int) new);
        } else {
            fprintf(stderr, "trade_list_scan(%d): bad item %d\n",
                    box_num, new->item);

            my_free(new);
        }

        if (s[strlen(s) - 1] == '\\') {    /* another entry follows */
            s = readlin_ew();
        } else {
            break;
        }
    }
}


static void
req_list_print(FILE *fp, char *header, struct req_ent **l) {
    int i;
    int count = 0;

    for (i = 0; i < ilist_len(l); i++) {
        if (valid_box(l[i]->item)) {
            count++;
            if (count == 1) {
                fputs(header, fp);
            }

            if (count > 1) {
                fputs(" \\\n\t", fp);
            }

            fprintf(fp, "%d %d %d",
                    l[i]->item,
                    l[i]->qty,
                    l[i]->consume);
        }
    }

    if (count) {
        fputs("\n", fp);
    }
}


static void
req_list_scan(char *s, struct req_ent ***l, int box_num) {
    struct req_ent *new;
    int consume;

    while (1) {
        new = my_malloc(sizeof(*new));
        sscanf(s, "%d %d %d", &new->item, &new->qty, &consume);
        new->consume = consume;

        if (valid_box(new->item)) {
            ilist_append((ilist *) l, (int) new);
        } else {
            fprintf(stderr, "req_list_scan(%d): bad item %d\n",
                    box_num, new->item);

            my_free(new);
        }

        if (s[strlen(s) - 1] == '\\') {    /* another entry follows */
            s = readlin_ew();
        } else {
            break;
        }
    }
}


void
olytime_scan(char *s, olytime *p) {

    sscanf(s, "%hd %hd %d", &p->turn, &p->day, &p->days_since_epoch);
}


void
olytime_print(FILE *fp, char *header, olytime *p) {

    if (p->turn || p->day || p->days_since_epoch) {
        fprintf(fp, "%s%d %d %d\n", header,
                p->turn, p->day, p->days_since_epoch);
    }
}


static void
print_loc_info(FILE *fp, struct loc_info *p) {

    if (zero_check(p, sizeof(*p))) {
        return;
    }

    fprintf(fp, "LI\n");
    box_print(fp, " wh ", p->where);
    boxlist_print(fp, " hl ", p->here_list);
}


static void
scan_loc_info(struct loc_info *p, int box_num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'wh':
                p->where = box_scan(t);
                break;

            case 'hl':
                boxlist_scan(t, box_num, (ilist *) &(p->here_list));
                break;

            case 0:
            default:
                fprintf(stderr, "scan_loc_info(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }
}


static void
print_magic(FILE *fp, struct char_magic *p) {

    if (zero_check(p, sizeof(*p))) {
        return;
    }

    fprintf(fp, "CM\n");

    if (p->magician) {
        fprintf(fp, " im %d\n", p->magician);
    }

    if (p->max_aura) {
        fprintf(fp, " ma %d\n", p->max_aura);
    }

    if (p->cur_aura) {
        fprintf(fp, " ca %d\n", p->cur_aura);
    }

    if (p->ability_shroud) {
        fprintf(fp, " as %d\n", p->ability_shroud);
    }

    if (p->hinder_meditation) {
        fprintf(fp, " hm %d\n", p->hinder_meditation);
    }

    if (p->quick_cast) {
        fprintf(fp, " qc %d\n", p->quick_cast);
    }

    if (p->aura_reflect) {
        fprintf(fp, " rb %d\n", p->aura_reflect);
    }

    if (p->hide_self) {
        fprintf(fp, " hs %d\n", p->hide_self);
    }

    if (p->hide_mage) {
        fprintf(fp, " cm %d\n", p->hide_mage);
    }

    if (p->knows_weather) {
        fprintf(fp, " kw %d\n", p->knows_weather);
    }

    if (p->swear_on_release) {
        fprintf(fp, " sr %d\n", p->swear_on_release);
    }

#if 0
                                                                                                                            /* Pledgee might have died! */
	if (valid_box(p->pledge)) box_print(fp, " pl ", p->pledge);
#endif
    if (valid_box(p->project_cast)) {
        box_print(fp, " pc ", p->project_cast);
    }
    box_print(fp, " ar ", p->auraculum);
    box_print(fp, " ot ", p->token);    /* our token artifact */
    known_print(fp, " vi ", p->visions);
}


static void
scan_magic(struct char_magic *p, int box_num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'im':
                p->magician = atoi(t);
                break;
            case 'ma':
                p->max_aura = atoi(t);
                break;
            case 'ca':
                p->cur_aura = atoi(t);
                break;
            case 'as':
                p->ability_shroud = atoi(t);
                break;
            case 'hm':
                p->hinder_meditation = atoi(t);
                break;
            case 'pc':
                p->project_cast = box_scan(t);
                break;
            case 'qc':
                p->quick_cast = atoi(t);
                break;
            case 'ot':
                p->token = box_scan(t);
                break;
            case 'pl':
                (void) box_scan(t);
                break;
            case 'ar':
                p->auraculum = box_scan(t);
                break;
            case 'rb':
                p->aura_reflect = atoi(t);
                break;
            case 'hs':
                p->hide_self = atoi(t);
                break;
            case 'cm':
                p->hide_mage = atoi(t);
                break;
            case 'sr':
                p->swear_on_release = atoi(t);
                break;
            case 'kw':
                p->knows_weather = atoi(t);
                break;
            case 'pr':  /* Old "prayer" flag */             break;

            case 'vi':
                known_scan(t, &p->visions, box_num);
                break;

            case 0:
            default:
                fprintf(stderr, "scan_magic(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }
}

static void
print_artifact(FILE *fp, struct entity_artifact *p) {
    if (zero_check(p, sizeof(*p))) {
        return;
    }

    fprintf(fp, "AR\n");

    if (p->type) {
        fprintf(fp, " ty %d\n", p->type);
    }

    if (p->param1) {
        fprintf(fp, " p1 %d\n", p->param1);
    }

    if (p->param2) {
        fprintf(fp, " p2 %d\n", p->param2);
    }

    if (p->uses) {
        fprintf(fp, " us %d\n", p->uses);
    }
};

static void
scan_artifact(struct entity_artifact *p, int box_num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'ty':
                p->type = atoi(t);
                break;
            case 'p1':
                p->param1 = atoi(t);
                break;
            case 'p2':
                p->param2 = atoi(t);
                break;
            case 'us':
                p->uses = atoi(t);
                break;
            case 0:
            default:
                fprintf(stderr, "scan_artifact(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }
};

static void
accept_print_sup(FILE *fp, struct accept_ent *p) {
    /*
   *  Trim out obviously bad "accepts".
   *
   */
    if (p->from_who && !valid_box(p->from_who)) {
        return;
    }

    fprintf(fp, " ac %d %d %d\n", p->item, p->from_who, p->qty);
}


static void
accept_print(FILE *fp, struct entity_char *p) {
    int i;

    for (i = 0; i < ilist_len(p->accept); i++) {
        accept_print_sup(fp, p->accept[i]);
    }
}


static void
print_char(FILE *fp, struct entity_char *p) {

    fprintf(fp, "CH\n");

    box_print(fp, " ni ", p->unit_item);
    box_print(fp, " lo ", p->unit_lord);

    if (p->health) {
        fprintf(fp, " he %d\n", p->health);
    }

    if (p->sick) {
        fprintf(fp, " si %d\n", p->sick);
    }

    if (p->loy_kind) {
        fprintf(fp, " lk %d\n", p->loy_kind);
    }
    if (p->loy_rate) {
        fprintf(fp, " lr %d\n", p->loy_rate);
    }

    skill_list_print(fp, " sl\t", p->skills);

#if 0
    effect_list_print(fp, " el\t", p->effects);
#endif

    if (p->prisoner) {
        fprintf(fp, " pr %d\n", p->prisoner);
    }

    if (p->moving) {
        fprintf(fp, " mo %d\n", p->moving);
    }

    if (p->behind) {
        fprintf(fp, " bh %d\n", p->behind);
    }

    if (p->guard) {
        fprintf(fp, " gu %d\n", p->guard);
    }

    if (p->time_flying) {
        fprintf(fp, " tf %d\n", p->time_flying);
    }

    if (p->break_point) {
        fprintf(fp, " bp %d\n", p->break_point);
    }

    if (p->personal_break_point) {
        fprintf(fp, " pb %d\n", p->personal_break_point);
    }

    if (p->rank) {
        fprintf(fp, " ra %d\n", p->rank);
    }

    if (p->attack) {
        fprintf(fp, " at %d\n", p->attack);
    }

    if (p->defense) {
        fprintf(fp, " df %d\n", p->defense);
    }

    if (p->missile) {
        fprintf(fp, " mi %d\n", p->missile);
    }

    if (p->npc_prog) {
        fprintf(fp, " po %d\n", p->npc_prog);
    }

    if (p->guild) {
        fprintf(fp, " gl %d\n", p->guild);
    }

    if (p->pay) {
        fprintf(fp, " pa %d\n", p->pay);
    }

    boxlist_print(fp, " ct ", p->contact);

    olytime_print(fp, " dt ", &p->death_time);

    /*
	 *  Religion stuff...
	 *
	 */
    if (p->religion.priest) {
        fprintf(fp, " pi %d\n", p->religion.priest);
    }

    if (p->religion.piety) {
        fprintf(fp, " pt %d\n", p->religion.piety);
    }

    boxlist_print(fp, " fl ", p->religion.followers);

    if (p->accept) {
        accept_print(fp, p);
    }
}


static void
accept_scan(char *s, struct entity_char *pp) {
    struct accept_ent *p;

    p = my_malloc(sizeof(*p));

    sscanf(s, "%d %d %d", &p->item, &p->from_who, &p->qty);

    ilist_append((ilist *) &(pp->accept), (int) p);
}

static void
scan_char(struct entity_char *p, int box_num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'ni':
                p->unit_item = box_scan(t);
                break;
            case 'lo':
                p->unit_lord = box_scan(t);
                break;
            case 'he':
                p->health = atoi(t);
                break;
            case 'si':
                p->sick = atoi(t);
                break;
            case 'pr':
                p->prisoner = atoi(t);
                break;
            case 'mo':
                p->moving = atoi(t);
                break;
            case 'bh':
                p->behind = atoi(t);
                break;
            case 'lk':
                p->loy_kind = atoi(t);
                break;
            case 'lr':
                p->loy_rate = atoi(t);
                break;
            case 'gu':
                p->guard = atoi(t);
                break;
            case 'tf':
                p->time_flying = atoi(t);
                break;
            case 'bp':
                p->break_point = atoi(t);
                break;
            case 'pb':
                p->personal_break_point = atoi(t);
                break;
            case 'ra':
                p->rank = atoi(t);
                break;
            case 'at':
                p->attack = atoi(t);
                break;
            case 'df':
                p->defense = atoi(t);
                break;
            case 'mi':
                p->missile = atoi(t);
                break;
            case 'po':
                p->npc_prog = atoi(t);
                break;
            case 'gl':
                p->guild = convert_skill(atoi(t));
                break;
            case 'pa':
                p->pay = atoi(t);
                break;
            case 'pi':
                p->religion.priest = atoi(t);
                break;
            case 'pt':
                p->religion.piety = atoi(t);
                break;

            case 'ct':
                boxlist_scan(t, box_num, &(p->contact));
                break;

            case 'fl':
                boxlist_scan(t, box_num, &(p->religion.followers));
                break;

            case 'sl':
                skill_list_scan(t, &p->skills, box_num);
                break;

#if 0
                                                                                                                                        case 'el':
			effect_list_scan(t, &p->effects);
			break;
#endif

            case 'dt':
                olytime_scan(t, &p->death_time);
                break;

            case 'ac':
                accept_scan(t, p);
                break;

            case 0:
            default:
                fprintf(stderr, "scan_char(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }
}


/*
 *  Mine Info Functions
 *  Fri Jan 24 12:35:16 1997 -- Scott Turner
 *
 */
static void
mine_info_print(FILE *fp, char *header, struct entity_mine *m) {
    int i;
    int count = 0;

    fprintf(fp, header);
    for (i = 0; i < MINE_MAX; i++) {
        if (item_list_print(fp, "    ml\t", m->mc[i].items) == 0) {
            fputs("    ml\t\n", fp);
        };
        fprintf(fp, "    ms %d\n", m->shoring[i]);
    };
    fputs("\n", fp);
};

static void
mine_info_scan(char *s, struct entity_mine **l, int bn) {
    int i = 0;
    struct entity_mine *m;

    /* Skip the initial line. */
    s = readlin_ew();

    m = (*l) = my_malloc(sizeof(**l));

    for (i = 0; i < MINE_MAX; i++) {
        if (strlen(&s[2]) > 2) {
            item_list_scan(&s[2], &m->mc[i].items, bn);
        };
        s = readlin_ew();
        sscanf(s, "ms %d", &m->shoring[i]);
        s = readlin_ew();
    };

}

static void
print_loc(FILE *fp, struct entity_loc *p) {

    if (zero_check(p, sizeof(*p))) {
        return;
    }

    fprintf(fp, "LO\n");

    boxlist0_print(fp, " pd ", p->prov_dest);

    if (p->hidden) {
        fprintf(fp, " hi %d\n", p->hidden);
    }

    if (p->shroud) {
        fprintf(fp, " sh %d\n", p->shroud);
    }

#if 0
                                                                                                                            if (p->barrier)
		fprintf(fp, " ba %d\n", p->barrier);
#endif

    if (p->dist_from_sea) {
        fprintf(fp, " ds %d\n", p->dist_from_sea);
    }

    if (p->dist_from_swamp) {
        fprintf(fp, " dw %d\n", p->dist_from_swamp);
    }

    if (p->dist_from_gate) {
        fprintf(fp, " dg %d\n", p->dist_from_gate);
    }

    if (p->sea_lane) {
        fprintf(fp, " sl %d\n", p->sea_lane);
    }

    if (p->tax_rate) {
        fprintf(fp, " tr %d\n", p->tax_rate);
    }

    if (p->mine_info) {
        mine_info_print(fp, " mi\n", p->mine_info);
    }

    if (p->control.weight) {
        fprintf(fp, " fw %d\n", p->control.weight);
    }

    if (p->control.men) {
        fprintf(fp, " fm %d\n", p->control.men);
    }

    if (p->control.nobles) {
        fprintf(fp, " fn %d\n", p->control.nobles);
    }

    if (p->control.closed) {
        fprintf(fp, " cd %d\n", p->control.closed);
    }

    box_print(fp, " ng ", p->near_grave);
#if 0
    effect_list_print(fp, " el\t", p->effects);
#endif
}


static void
scan_loc(struct entity_loc *p, int box_num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'hi':
                p->hidden = atoi(t);
                break;
            case 'sh':
                p->shroud = atoi(t);
                break;
#if 0
                case 'ba':	p->barrier = atoi(t);		break;
#endif
            case 'ds':
                p->dist_from_sea = atoi(t);
                break;
            case 'dw':
                p->dist_from_swamp = atoi(t);
                break;
            case 'dg':
                p->dist_from_gate = atoi(t);
                break;
            case 'ng':
                p->near_grave = box_scan(t);
                break;
            case 'sl':
                p->sea_lane = atoi(t);
                break;
            case 'tr':
                p->tax_rate = atoi(t);
                break;
            case 'fw':
                p->control.weight = atoi(t);
                break;
            case 'fm':
                p->control.men = atoi(t);
                break;
            case 'fn':
                p->control.nobles = atoi(t);
                break;
            case 'cd':
                p->control.closed = atoi(t);
                break;

            case 'pd':
                boxlist0_scan(t, box_num, &(p->prov_dest));
                break;

#if 0
                                                                                                                                        case 'el':
			effect_list_scan(t, &p->effects);
			break;
#endif

            case 'mi':
                mine_info_scan(t, &p->mine_info, box_num);
                break;

            case 0:
            default:
                fprintf(stderr, "scan_loc(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }
}

static void
print_ship(FILE *fp, struct entity_ship *p) {
    fprintf(fp, "SP\n");

    if (p->hulls) {
        fprintf(fp, " hu %d\n", p->hulls);
    }

    if (p->forts) {
        fprintf(fp, " fo %d\n", p->forts);
    }

    if (p->sails) {
        fprintf(fp, " sa %d\n", p->sails);
    }

    if (p->ports) {
        fprintf(fp, " po %d\n", p->ports);
    }

    if (p->keels) {
        fprintf(fp, " ke %d\n", p->keels);
    }

    if (p->galley_ram) {
        fprintf(fp, " gr %d\n", p->galley_ram);
    }
};

static void
scan_ship(struct entity_ship *p, int box_num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {

        if (*line == '#') { continue; }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'hu':
                p->hulls = atoi(t);
                break;
            case 'fo':
                p->forts = atoi(t);
                break;
            case 'sa':
                p->sails = atoi(t);
                break;
            case 'po':
                p->ports = atoi(t);
                break;
            case 'ke':
                p->keels = atoi(t);
                break;
            case 'gr':
                p->galley_ram = atoi(t);
                break;
        };
        advance;
    };
};

static void
print_subloc(FILE *fp, struct entity_subloc *p) {

    fprintf(fp, "SL\n");

    boxlist_print(fp, " te ", p->teaches);

    if (p->hp) {
        fprintf(fp, " hp %d\n", p->hp);
    }

    if (p->moat) {
        fprintf(fp, " mt %d\n", p->moat);
    }

    if (p->damage) {
        fprintf(fp, " da %d\n", p->damage);
    }

    if (p->defense) {
        fprintf(fp, " de %d\n", p->defense);
    }

/*
	if (p->capacity) {
	  fprintf(fp, " ca %d\n", p->capacity);
	};

	if (p->build_materials)
		fprintf(fp, " bm %d\n", p->build_materials);

	if (p->effort_required)
		fprintf(fp, " er %d\n", p->effort_required);

	if (p->effort_given)
		fprintf(fp, " eg %d\n", p->effort_given);

		*/

    if (p->moving) {
        fprintf(fp, " mo %d\n", p->moving);
    }

/*
	if (p->galley_ram) {
	  fprintf(fp, " gr %d\n", p->galley_ram);
	};

	if (p->shaft_depth)
		fprintf(fp, " sd %d\n", p->shaft_depth);
		*/

    if (p->safe) {
        fprintf(fp, " sh %d\n", p->safe);
    }

    if (p->major) {
        fprintf(fp, " mc %d\n", p->major);
    }

    if (p->opium_econ) {
        fprintf(fp, " op %d\n", p->opium_econ);
    }

    if (p->loot) {
        fprintf(fp, " lo %d\n", p->loot);
    }

    if (p->prominence) {
        fprintf(fp, " cp %d\n", p->prominence);
    }

    boxlist_print(fp, " nc ", p->near_cities);

    boxlist_print(fp, " lt ", p->link_to);
    boxlist_print(fp, " lf ", p->link_from);
/*
	boxlist_print(fp, " bs ", p->bound_storms);
	*/

    /*
	if (p->link_when)
		fprintf(fp, " lw %d\n", p->link_when);

	if (p->link_open)
		fprintf(fp, " lp %d\n", p->link_open);
	*/

    if (p->guild) {
        fprintf(fp, " gl %d\n", p->guild);
    }

    if (p->control.weight) {
        fprintf(fp, " fw %d\n", p->control.weight);
    }

    if (p->control.men) {
        fprintf(fp, " fm %d\n", p->control.men);
    }

    if (p->control.nobles) {
        fprintf(fp, " fn %d\n", p->control.nobles);
    }

    if (p->control.closed) {
        fprintf(fp, " cd %d\n", p->control.closed);
    }

    if (p->tax_market) {
        fprintf(fp, " tm %d\n", p->tax_market);
    }

    build_list_print(fp, " bl\t", p->builds);
#if 0
    effect_list_print(fp, " el\t", p->effects);
#endif

    if (p->entrance_size) {
        fprintf(fp, " es %d\n", p->entrance_size);
    }
}

static void
scan_subloc(struct entity_subloc *p, int box_num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'tm':
                p->tax_market = atoi(t);
                break;
            case 'hp':
                p->hp = atoi(t);
                break;
            case 'mt':
                p->moat = atoi(t);
                break;
            case 'da':
                p->damage = atoi(t);
                break;
            case 'de':
                p->defense = atoi(t);
                break;
/*
	case 'ca':   p->capacity = atoi(t);			break;
	case 'er':	p->effort_required = atoi(t);		break;
	case 'eg':	p->effort_given = atoi(t);		break;
	case 'bm':	p->build_materials = atoi(t);		break;
	*/
            case 'ca':
            case 'er':
            case 'eg':
            case 'bm':
                break;

            case 'mo':
                p->moving = atoi(t);
                break;
            case 'gr':
/*
	  p->galley_ram = atoi(t);
*/
                break;
/*
	case 'sd':	p->shaft_depth = atoi(t);		break;
*/
            case 'sh':
                p->safe = atoi(t);
                break;
            case 'mc':
                p->major = atoi(t);
                break;
            case 'op':
                p->opium_econ = atoi(t);
                break;
            case 'lo':
                p->loot = atoi(t);
                break;
            case 'cp':
                p->prominence = atoi(t);
                break;
/*
	case 'lw':	p->link_when = atoi(t);			break;
	case 'lp':	p->link_open = atoi(t);			break;
	*/
            case 'gl':
                p->guild = convert_skill(atoi(t));
                break;
            case 'es':
                p->entrance_size = atoi(t);
                break;

            case 'lt':
                boxlist_scan(t, box_num, &(p->link_to));
                break;

            case 'lf':
                boxlist_scan(t, box_num, &(p->link_from));
                break;

            case 'te':
                boxlist_scan(t, box_num, &(p->teaches));
                break;

            case 'nc':
                boxlist_scan(t, box_num, (ilist *) &(p->near_cities));
                break;

            case 'bs':
/*
	  boxlist_scan(t, box_num, (ilist *) &(p->bound_storms));
*/
                break;

            case 'bl':
                build_list_scan(t, &p->builds);
                break;

#if 0
                                                                                                                                        case 'el':
	  effect_list_scan(t, &p->effects);
	  break;
#endif

            case 'fw':
                p->control.weight = atoi(t);
                break;
            case 'fm':
                p->control.men = atoi(t);
                break;
            case 'fn':
                p->control.nobles = atoi(t);
                break;
            case 'cd':
                p->control.closed = atoi(t);
                break;

            case 0:
            default:
                fprintf(stderr, "scan_subloc(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }

}


static void
print_item(FILE *fp, struct entity_item *p) {

    if (zero_check(p, sizeof(*p))) {
        return;
    }

    fprintf(fp, "IT\n");

    if (p->trade_good) {
        fprintf(fp, " tg %d\n", p->trade_good);
    }

    if (p->plural_name && *p->plural_name) {
        fprintf(fp, " pl %s\n", p->plural_name);
    }

    if (p->weight) {
        fprintf(fp, " wt %d\n", p->weight);
    }

    if (p->land_cap) {
        fprintf(fp, " lc %d\n", p->land_cap);
    }

    if (p->ride_cap) {
        fprintf(fp, " rc %d\n", p->ride_cap);
    }

    if (p->fly_cap) {
        fprintf(fp, " fc %d\n", p->fly_cap);
    }

    if (p->is_man_item) {
        fprintf(fp, " mu %d\n", p->is_man_item);
    }

    if (p->prominent) {
        fprintf(fp, " pr %d\n", p->prominent);
    }

    if (p->animal) {
        fprintf(fp, " an %d\n", p->animal);
    }

    if (p->attack) {
        fprintf(fp, " at %d\n", p->attack);
    }

    if (p->defense) {
        fprintf(fp, " df %d\n", p->defense);
    }

    if (p->missile) {
        fprintf(fp, " mi %d\n", p->missile);
    }

    if (p->base_price) {
        fprintf(fp, " bp %d\n", p->base_price);
    }

    if (p->capturable) {
        fprintf(fp, " ca %d\n", p->capturable);
    }

    if (p->ungiveable) {
        fprintf(fp, " ug %d\n", p->ungiveable);
    }

    if (p->wild) {
        fprintf(fp, " wi %d\n", p->wild);
    }

    if (p->maintenance) {
        fprintf(fp, " mt %d\n", p->maintenance);
    }

    if (p->npc_split) {
        fprintf(fp, " sp %d\n", p->npc_split);
    }

    if (p->animal_part) {
        fprintf(fp, " ap %d\n", p->animal_part);
    }

    box_print(fp, " un ", p->who_has);
}


static void
scan_item(struct entity_item *p, int box_num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'pl':
                p->plural_name = str_save(t);
                break;
            case 'tg':
                p->trade_good = atoi(t);
                break;
            case 'wt':
                p->weight = atoi(t);
                break;
            case 'lc':
                p->land_cap = atoi(t);
                break;
            case 'rc':
                p->ride_cap = atoi(t);
                break;
            case 'fc':
                p->fly_cap = atoi(t);
                break;
            case 'mu':
                p->is_man_item = atoi(t);
                break;
            case 'pr':
                p->prominent = atoi(t);
                break;
            case 'an':
                p->animal = atoi(t);
                break;
            case 'un':
                p->who_has = box_scan(t);
                break;
            case 'at':
                p->attack = atoi(t);
                break;
            case 'df':
                p->defense = atoi(t);
                break;
            case 'mi':
                p->missile = atoi(t);
                break;
            case 'bp':
                p->base_price = atoi(t);
                break;
            case 'ca':
                p->capturable = atoi(t);
                break;
            case 'ug':
                p->ungiveable = atoi(t);
                break;
            case 'wi':
                p->wild = atoi(t);
                break;
            case 'mt':
                p->maintenance = atoi(t);
                break;
            case 'sp':
                p->npc_split = atoi(t);
                break;
            case 'ap':
                p->animal_part = atoi(t);
                break;

            case 0:
            default:
                fprintf(stderr, "scan_item(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }
}


static void
print_item_magic(FILE *fp, struct item_magic *p) {

    if (zero_check(p, sizeof(*p))) {
        return;
    }

    fprintf(fp, "IM\n");

    if (p->religion) {
        fprintf(fp, " rl %d\n", p->religion);
    }

    if (p->aura) {
        fprintf(fp, " au %d\n", p->aura);
    }

    if (p->curse_loyalty) {
        fprintf(fp, " cl %d\n", p->curse_loyalty);
    }

    if (p->cloak_region) {
        fprintf(fp, " cr %d\n", p->cloak_region);
    }

    if (p->cloak_creator) {
        fprintf(fp, " cc %d\n", p->cloak_creator);
    }

    if (p->use_key) {
        fprintf(fp, " uk %d\n", p->use_key);
    }

    if (p->quick_cast) {
        fprintf(fp, " qc %d\n", p->quick_cast);
    }

    if (p->attack_bonus) {
        fprintf(fp, " ab %d\n", p->attack_bonus);
    }

    if (p->defense_bonus) {
        fprintf(fp, " db %d\n", p->defense_bonus);
    }

    if (p->missile_bonus) {
        fprintf(fp, " mb %d\n", p->missile_bonus);
    }

    if (p->aura_bonus) {
        fprintf(fp, " ba %d\n", p->aura_bonus);
    }

    if (p->token_num) {
        fprintf(fp, " tn %d\n", p->token_num);
    }

    if (p->orb_use_count) {
        fprintf(fp, " oc %d\n", p->orb_use_count);
    }

    box_print(fp, " ti ", p->token_ni);

#if 0
    box_print(fp, " rc ", p->region_created);
#endif
    if (valid_box(p->project_cast)) {
        box_print(fp, " pc ", p->project_cast);
    }
    box_print(fp, " ct ", p->creator);
    if (p->lore) {
        fprintf(fp, " lo %d\n", p->lore);
    }

    boxlist_print(fp, " mu ", p->may_use);
    boxlist_print(fp, " ms ", p->may_study);
}


static void
scan_item_magic(struct item_magic *p, int box_num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'au':
                p->aura = atoi(t);
                break;
            case 'cl':
                p->curse_loyalty = atoi(t);
                break;
            case 'cr':
                p->cloak_region = atoi(t);
                break;
            case 'cc':
                p->cloak_creator = atoi(t);
                break;
            case 'uk':
                p->use_key = atoi(t);
                break;
            case 'rc':
                /* p->region_created = box_scan(t); */
                break;
            case 'pc':
                p->project_cast = box_scan(t);
                break;
            case 'ct':
                p->creator = box_scan(t);
                break;
            case 'lo':
                p->lore = box_scan(t);
                break;
            case 'qc':
                p->quick_cast = atoi(t);
                break;
            case 'ab':
                p->attack_bonus = atoi(t);
                break;
            case 'db':
                p->defense_bonus = atoi(t);
                break;
            case 'mb':
                p->missile_bonus = atoi(t);
                break;
            case 'ba':
                p->aura_bonus = atoi(t);
                break;
            case 'tn':
                p->token_num = atoi(t);
                break;
            case 'ti':
                p->token_ni = atoi(t);
                break;
            case 'oc':
                p->orb_use_count = atoi(t);
                break;
            case 'rl':
                p->religion = atoi(t);
                break;

            case 'mu':
                boxlist_scan(t, box_num, &(p->may_use));
                break;

            case 'ms':
                boxlist_scan(t, box_num, &(p->may_study));
                break;

            case 0:
            default:
                fprintf(stderr, "scan_item_magic(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }
}


static void
print_player(FILE *fp, struct entity_player *p) {

    fprintf(fp, "PL\n");

    if (p->full_name && *(p->full_name)) {
        fprintf(fp, " fn %s\n", p->full_name);
    }

    if (p->email && *(p->email)) {
        fprintf(fp, " em %s\n", p->email);
    }

    if (p->vis_email && *(p->vis_email)) {
        fprintf(fp, " ve %s\n", p->vis_email);
    }

    if (p->password && *(p->password)) {
        fprintf(fp, " pw %s\n", p->password);
    }

    if (p->noble_points) {
        fprintf(fp, " np %d\n", p->noble_points);
    }

    if (p->first_turn) {
        fprintf(fp, " ft %d\n", p->first_turn);
    }

    if (p->format) {
        fprintf(fp, " fo %d\n", p->format);
    }

    if (p->rules_path && *(p->rules_path)) {
        fprintf(fp, " rp %s\n", p->rules_path);
    }

    if (p->db_path && *(p->db_path)) {
        fprintf(fp, " db%s\n", p->db_path);
    }

    if (p->notab) {
        fprintf(fp, " nt %d\n", p->notab);
    }

    if (p->first_tower) {
        fprintf(fp, " tf %d\n", p->first_tower);
    }

    if (p->split_lines) {
        fprintf(fp, " sl %d\n", p->split_lines);
    }

    if (p->split_bytes) {
        fprintf(fp, " sb %d\n", p->split_bytes);
    }

    if (p->sent_orders) {
        fprintf(fp, " so %d\n", p->sent_orders);
    }

    if (p->dont_remind) {
        fprintf(fp, " dr %d\n", p->dont_remind);
    }

    if (p->compuserve) {
        fprintf(fp, " ci %d\n", p->compuserve);
    }

    if (p->broken_mailer) {
        fprintf(fp, " bm %d\n", p->broken_mailer);
    }

    if (p->last_order_turn) {
        fprintf(fp, " lt %d\n", p->last_order_turn);
    }

    if (p->nation) {
        fprintf(fp, " na %d\n", p->nation);
    }

    if (p->magic) {
        fprintf(fp, " ma %d\n", p->magic);
    }

    if (p->jump_start) {
        fprintf(fp, " js %d\n", p->jump_start);
    }

    known_print(fp, " kn ", p->known);
    boxlist_print(fp, " un ", p->units);
    boxlist_print(fp, " uf ", p->unformed);
    admit_print(fp, p);
}


static void
scan_player(struct entity_player *p, int box_num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'fn':
                p->full_name = str_save(t);
                break;
            case 'em':
                p->email = str_save(t);
                break;
            case 've':
                p->vis_email = str_save(t);
                break;
            case 'pw':
                p->password = str_save(t);
                break;
            case 'np':
                p->noble_points = atoi(t);
                break;
            case 'ft':
                p->first_turn = atoi(t);
                break;
            case 'fo':
                p->format = atoi(t);
                break;
            case 'rp':
                p->rules_path = str_save(t);
                break;
            case 'fp':
            case 'db':
                p->db_path = str_save(t);
                break;
            case 'nt':
                p->notab = atoi(t);
                break;
            case 'tf':
                p->first_tower = atoi(t);
                break;
            case 'so':
                p->sent_orders = atoi(t);
                break;
            case 'lt':
                p->last_order_turn = atoi(t);
                break;
            case 'sl':
                p->split_lines = atoi(t);
                break;
            case 'sb':
                p->split_bytes = atoi(t);
                break;
            case 'ci':
                p->compuserve = atoi(t);
                break;
            case 'ti':      /* ignore it... */              break;
            case 'bm':
                p->broken_mailer = atoi(t);
                break;
            case 'dr':
                p->dont_remind = atoi(t);
                break;
            case 'na': {
                p->nation = atoi(t);
                /* temp fix */
                if (p->nation <= 1002 && p->nation >= 1000) { p->nation -= 3; }
                break;
            };
            case 'ma':
                p->magic = atoi(t);
                break;
            case 'js':
                p->jump_start = atoi(t);
                break;

            case 'kn':
                known_scan(t, &p->known, box_num);
                break;

            case 'un':
                boxlist_scan(t, box_num, &(p->units));
                break;

            case 'uf':
                boxlist_scan(t, box_num, &(p->unformed));
                break;

            case 'am':
                admit_scan(t, box_num, p);
                break;

            case 0:
            default:
                fprintf(stderr, "scan_player(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }
}

static void print_religion(FILE *fp, struct entity_religion_skill *p) {
    fprintf(fp, " RL\n");
    fprintf(fp, " na %s\n", p->name);
    fprintf(fp, " st %d\n", p->strength);
    fprintf(fp, " wk %d\n", p->weakness);
    fprintf(fp, " pl %d\n", p->plant);
    fprintf(fp, " an %d\n", p->animal);
    fprintf(fp, " tr %d\n", p->terrain);
    fprintf(fp, " hp %d\n", p->high_priest);
    fprintf(fp, " b0 %d\n", p->bishops[0]);
    fprintf(fp, " b1 %d\n", p->bishops[1]);
    fprintf(fp, " ER\n");
};

static struct entity_religion_skill *scan_religion(int box_num) {
    char *t;
    int c;
    struct entity_religion_skill *new =
            my_malloc(sizeof(struct entity_religion_skill));

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);

        if (c == 'ER') {
            break;
        };

        t = t_string(line);

        switch (c) {
            case 'na':
                new->name = str_save(t);
                break;
            case 'st':
                new->strength = atoi(t);
                break;
            case 'wk':
                new->weakness = atoi(t);
                break;
            case 'pl':
                new->plant = atoi(t);
                break;
            case 'an':
                new->animal = atoi(t);
                break;
            case 'tr':
                new->terrain = atoi(t);
                break;
            case 'hp':
                new->high_priest = atoi(t);
                break;
            case 'b0':
                new->bishops[0] = atoi(t);
                break;
            case 'b1':
                new->bishops[1] = atoi(t);
                break;
        };
        advance;
    };
    return new;
};

static void
print_skill(FILE *fp, struct entity_skill *p) {
    int i;

    fprintf(fp, "SK\n");
    fprintf(fp, " tl %d\n", p->time_to_learn);
    fprintf(fp, " tu %d\n", p->time_to_use);
    /*
	 *  Flags
	 *  Mon Oct 21 12:50:46 1996 -- Scott Turner
	 *
	 */
    for (i = 0; i < MAX_FLAGS; i++) {
        if (p->flags & (1 << i)) {
            switch (1 << i) {
                case IS_POLLED:
                    fprintf(fp, " pl 1\n");
                    break;
                case REQ_HOLY_SYMBOL:
                    fprintf(fp, " hs 1\n");
                    break;
                case REQ_HOLY_PLANT:
                    fprintf(fp, " hp 1\n");
                    break;
                case COMBAT_SKILL:
                    fprintf(fp, " cs 1\n");
                    break;
            };
        };
    };

    box_print(fp, " rs ", p->required_skill);

    boxlist_print(fp, " of ", p->offered);
    boxlist_print(fp, " re ", p->research);
    boxlist_print(fp, " gl ", p->guild);
    req_list_print(fp, " rq\t", p->req);
    box_print(fp, " pr ", p->produced);

    if (p->practice_cost) {
        fprintf(fp, " pc %d\n", p->practice_cost);
    }
    if (p->practice_time) {
        fprintf(fp, " tp %d\n", p->practice_time);
    }
    if (p->practice_prog) {
        fprintf(fp, " pp %d\n", p->practice_prog);
    }

    if (p->np_req) {
        fprintf(fp, " np %d\n", p->np_req);
    }
    if (p->no_exp) {
        fprintf(fp, " ne %d\n", p->no_exp);
    }
    if (p->piety) {
        fprintf(fp, " pt %d\n", p->piety);
    }
    if (p->religion_skill) {
        print_religion(fp, p->religion_skill);
    }
}


static void
scan_skill(struct entity_skill *p, int box_num) {
    char *t;
    int c;

    advance;
    p->flags = 0;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'tl':
                p->time_to_learn = atoi(t);
                break;
            case 'tu':
                p->time_to_use = atoi(t);
                break;
            case 'ne':
                p->no_exp = atoi(t);
                break;
            case 'np':
                p->np_req = atoi(t);
                break;
            case 'rs':
                p->required_skill = box_scan(t);
                break;
            case 'pr':
                p->produced = box_scan(t);
                break;
            case 'pt':
                p->piety = atoi(t);
                break;
            case 'pc':
                p->practice_cost = atoi(t);
                break;
            case 'tp':
                p->practice_time = atoi(t);
                break;
            case 'pp':
                p->practice_prog = atoi(t);
                break;

            case 'of':
                boxlist_scan(t, box_num, &(p->offered));
                break;

            case 're':
                boxlist_scan(t, box_num, &(p->research));
                break;

            case 'gl':
                boxlist_scan(t, box_num, &(p->guild));
                break;

            case 'rq':
                req_list_scan(t, &p->req, box_num);
                break;

            case 'RL':
                p->religion_skill = scan_religion(box_num);
                break;

            case 'pl':
                p->flags |= IS_POLLED;
                break;

            case 'hs':
                p->flags |= REQ_HOLY_SYMBOL;
                break;

            case 'hp':
                p->flags |= REQ_HOLY_PLANT;
                break;

            case 'cs':
                p->flags |= COMBAT_SKILL;
                break;

            case 0:
            default:
                fprintf(stderr, "scan_skill(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }
}


static void
print_command(FILE *fp, struct command *p) {

    if (p->cmd == 0) {
        return;
    }

    fprintf(fp, "CO\n");
    fprintf(fp, " li %s\n", p->line);

    fprintf(fp, " ar %d %d %d %d %d %d %d %d %d\n",
            p->a, p->b, p->c, p->d, p->e, p->f, p->g, p->h, p->i);

/*
	fprintf(fp, " sv %d %d %d %d %d %d %d\n",
		p->v.direction,
		p->v.destination,
		p->v.road,
		p->v.dest_hidden,
		p->v.distance,
		p->v.orig,
		p->v.orig_hidden);
		*/

    if (p->state) {
        fprintf(fp, " cs %d\n", p->state);
    }

    if (p->wait) {
        fprintf(fp, " wa %d\n", p->wait);
    }

    if (p->status) {
        fprintf(fp, " st %d\n", p->status);
    }

    if (p->use_skill) {
        box_print(fp, " us ", p->use_skill);
    }

    if (p->use_exp) {
        fprintf(fp, " ue %d\n", p->use_exp);
    }

    if (p->days_executing) {
        fprintf(fp, " de %d\n", p->days_executing);
    }

    if (p->poll) {
        fprintf(fp, " po %d\n", p->poll);
    }

    if (p->pri) {
        fprintf(fp, " pr %d\n", p->pri);
    }

    if (p->inhibit_finish) {
        fprintf(fp, " if %d\n", p->inhibit_finish);
    }

    /*
	 *  Thu Oct 24 15:05:21 1996 -- Scott Turner
	 *
	 *  This must now be saved, since it can be > 1
	 *
	 */
    if (p->second_wait) {
        fprintf(fp, " sw %d\n", p->second_wait);
    }
}


static void
scan_command(struct command *p, int box_num) {
    char *t;
    int c;

    p->who = box_num;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'li':
                if (!oly_parse_cmd(p, t)) {
                    fprintf(stderr, "scan_command(%d): "
                                    "bad cmd %s\n", box_num, t);
                }
                break;

            case 'cs':
                p->state = atoi(t);
                break;
            case 'wa':
                p->wait = atoi(t);
                break;
            case 'st':
                p->status = atoi(t);
                break;
            case 'de':
                p->days_executing = atoi(t);
                break;
            case 'po':
                p->poll = atoi(t);
                break;
            case 'pr':
                p->pri = atoi(t);
                break;
            case 'if':
                p->inhibit_finish = atoi(t);
                break;
            case 'us':
                p->use_skill = box_scan(t);
                break;
            case 'ue':
                p->use_exp = atoi(t);
                break;
            case 'sw':
                p->second_wait = atoi(t);
                break;

            case 'ar':
                sscanf(t, "%d %d %d %d %d %d %d %d %d",
                       &p->a, &p->b, &p->c, &p->d,
                       &p->e, &p->f, &p->g, &p->h, &p->i);
                /* temp fix */
                p->a = convert_skill(p->a);
                p->b = convert_skill(p->b);
                break;

/*
		case 'sv':
			sscanf(t, "%d %d %d %d %d %d %d",
			       &p->v.direction,
			       &p->v.destination,
			       &p->v.road,
			       &p->v.dest_hidden,
			       &p->v.distance,
			       &p->v.orig,
			       &p->v.orig_hidden);
			       */

            case 0:
            default:
                fprintf(stderr, "scan_command(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }
}


static void
print_gate(FILE *fp, struct entity_gate *p) {

    fprintf(fp, "GA\n");

    box_print(fp, " tl ", p->to_loc);

    if (p->notify_jumps) {
        box_print(fp, " nj ", p->notify_jumps);
    }

    if (p->notify_unseal) {
        box_print(fp, " nu ", p->notify_unseal);
    }

    if (p->seal_key) {
        fprintf(fp, " sk %d\n", p->seal_key);
    }

    if (p->road_hidden) {
        fprintf(fp, " rh %d\n", p->road_hidden);
    }
}


static void
scan_gate(struct entity_gate *p, int box_num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'tl':
                p->to_loc = box_scan(t);
                break;
            case 'nj':
                p->notify_jumps = box_scan(t);
                break;
            case 'nu':
                p->notify_unseal = box_scan(t);
                break;
            case 'sk':
                p->seal_key = atoi(t);
                break;
            case 'rh':
                p->road_hidden = atoi(t);
                break;

            case 0:
            default:
                fprintf(stderr, "scan_gate(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }
}


static void
print_misc(FILE *fp, struct entity_misc *p) {

    if (zero_check(p, sizeof(*p))) {
        return;
    }

    fprintf(fp, "MI\n");

    if (valid_box(p->summoned_by)) {
        box_print(fp, " sb ", p->summoned_by);
    }

    if (p->npc_dir) {
        fprintf(fp, " di %d\n", p->npc_dir);
    }

    if (p->npc_created) {
        fprintf(fp, " mc %d\n", p->npc_created);
    }

    if (p->mine_delay) {
        fprintf(fp, " md %d\n", p->mine_delay);
    }

    if (p->storm_str) {
        fprintf(fp, " ss %d\n", p->storm_str);
    }

    if (p->cmd_allow) {
        fprintf(fp, " ca %c\n", p->cmd_allow);
    }

    box_print(fp, " gc ", p->garr_castle);
    box_print(fp, " mh ", p->npc_home);
    box_print(fp, " co ", p->npc_cookie);
    box_print(fp, " ov ", p->only_vulnerable);
    if (p->old_lord && valid_box(p->old_lord)) {
        box_print(fp, " ol ", p->old_lord);
    }
    box_print(fp, " bs ", p->bind_storm);

    if (p->save_name && *p->save_name) {
        fprintf(fp, " sn %s\n", p->save_name);
    }

    if (p->display && *p->display) {
        fprintf(fp, " ds %s\n", p->display);
    }

    known_print(fp, " nm ", p->npc_memory);
}


static void
scan_misc(struct entity_misc *p, int box_num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') {
            continue;
        }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'di':
                p->npc_dir = atoi(t);
                break;
            case 'mc':
                p->npc_created = atoi(t);
                break;
            case 'md':
                p->mine_delay = atoi(t);
                break;
            case 'ss':
                p->storm_str = atoi(t);
                break;
            case 'mh':
                p->npc_home = box_scan(t);
                break;
            case 'gc':
                p->garr_castle = box_scan(t);
                break;
            case 'sb':
                p->summoned_by = box_scan(t);
                break;
            case 'co':
                p->npc_cookie = box_scan(t);
                break;
            case 'ov':
                p->only_vulnerable = box_scan(t);
                break;
            case 'bs':
                p->bind_storm = box_scan(t);
                break;
            case 'ol':
                p->old_lord = box_scan(t);
                break;
            case 'sn':
                p->save_name = str_save(t);
                break;
            case 'ds':
                p->display = str_save(t);
                break;
            case 'ca':
                p->cmd_allow = *t;
                break;

            case 'nm':
                known_scan(t, &p->npc_memory, box_num);
                break;

            case 0:
            default:
                fprintf(stderr, "scan_misc(%d):  bad line: %s\n",
                        box_num, line);
        }
        advance;
    }
}

/*
 *  Thu Jan 21 07:15:54 1999 -- Scott Turner
 *
 *  Full-fledged type version.
 *
 */
static void
scan_nation2(struct entity_nation *n, int num) {
    char *t;
    int c;

    advance;
    while (line && iswhite(*line)) {
        if (*line == '#') { continue; }

        line++;
        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'na':
                n->name = str_save(t);
                break;
            case 'ci':
                n->citizen = str_save(t);
                break;
            case 'cp':
                n->capital = box_scan(t);
                break;
            case 'wi':
                n->win = atoi(t);
                break;
            case 'pl':
                n->player_limit = atoi(t);
                break;
            case 'js':
                n->jump_start = atoi(t);
                break;
            case 'ne':
                n->neutral = atoi(t);
                break;
            case 'ps':
                boxlist_scan(t, -1, &(n->proscribed_skills));
                break;

            case 0:
            default:
                fprintf(stderr, "scan_nation:  bad line: %s\n", line);
        }
        advance;
    }
}

static void
load_box(int n) {
    int c;
    char *t;
    struct box *p;

/*
 *  The fast scan of libdir/master should have allocated all of the boxes
 *  If one was later added manually, it won't have been pre-allocated by
 *  the master fast scan.  Remove master file and let io.c do the slow
 *  scan to allocate all the boxes.  Save the database to recreate master.
 */

    if (!valid_box(n)) {
        fprintf(stderr, "Unforseen box %d found in load phase.\n", n);
        fprintf(stderr, "Remove %s/master and retry.\n", libdir);
        assert(FALSE);
    }

    p = bx[n];

    ext_boxnum = n;

    advance;
    while (line && *line) {
        if (*line == '#') {
            advance;
            continue;
        }

        c = linehash(line);
        t = t_string(line);

        switch (c) {
            case 'na':
                set_name(n, t);
                advance;
                break;

            case 'il':
                item_list_scan(t, &bx[n]->items, n);
                advance;
                break;

            case 'tl':
                trade_list_scan(t, &bx[n]->trades, n);
                advance;
                break;

            case 'an':
                boxlist_scan(t, n, &(p_disp(n)->neutral));
                advance;
                break;

            case 'ad':
                boxlist_scan(t, n, &(p_disp(n)->defend));
                advance;
                break;

            case 'ah':
                boxlist_scan(t, n, &(p_disp(n)->hostile));
                advance;
                break;

            case 'el':
                effect_list_scan(t, &bx[n]->effects);
                advance;
                break;

            case 'CH':
                scan_char(p_char(n), n);
                break;
            case 'CM':
                scan_magic(p_magic(n), n);
                break;
            case 'LI':
                scan_loc_info(p_loc_info(n), n);
                break;
            case 'LO':
                scan_loc(p_loc(n), n);
                break;
            case 'SL':
                scan_subloc(p_subloc(n), n);
                break;
            case 'IT':
                scan_item(p_item(n), n);
                break;
            case 'PL':
                scan_player(p_player(n), n);
                break;
            case 'SK':
                scan_skill(p_skill(n), n);
                break;
            case 'GA':
                scan_gate(p_gate(n), n);
                break;
            case 'MI':
                scan_misc(p_misc(n), n);
                break;
            case 'IM':
                scan_item_magic(p_item_magic(n), n);
                break;
            case 'AR':
                scan_artifact(p_item_artifact(n), n);
                break;
            case 'CO':
                scan_command(p_command(n), n);
                break;
            case 'SP':
                scan_ship(p_ship(n), n);
                break;
            case 'NA':
                scan_nation2(p_nation(n), n);
                break;

            case 0:
            default:
                fprintf(stderr, "load_box(%d):  bad line: %s\n",
                        n, line);

                advance;
                while (line && iswhite(*line))
                    advance;
                continue;
        }
    }

    if (line) {
        assert(*line == '\0');
        advance;    /* advance over blank separating line */
    }
}

static void
print_nation(FILE *fp, struct entity_nation *n) {
    fprintf(fp, "NA\n");

    if (n->name) { fprintf(fp, " na %s\n", n->name); }
    if (n->citizen) { fprintf(fp, " ci %s\n", n->citizen); }
    if (n->capital) { box_print(fp, " cp ", n->capital); }
    if (n->win) { fprintf(fp, " wi %d\n", n->win); }
    if (n->player_limit) { fprintf(fp, " pl %d\n", n->player_limit); }
    if (n->jump_start) { fprintf(fp, " js %d\n", n->jump_start); }
    if (n->neutral) { fprintf(fp, " ne %d\n", n->neutral); }
    if (n->proscribed_skills) {
        boxlist_print(fp, " ps ", n->proscribed_skills);
    }
    fprintf(fp, "\n");
}


void
save_box(FILE *fp, int n) {
    struct box *p;
    struct att_ent *pd;
    void *vp;

    if (kind(n) == T_deleted) {
        return;
    }

    assert(valid_box(n));

    p = bx[n];

    if (bx[n]->skind) {
        fprintf(fp, "%d %s %s\n",
                n, kind_s[bx[n]->kind], subkind_s[bx[n]->skind]);
    } else {
        fprintf(fp, "%d %s 0\n",
                n, kind_s[bx[n]->kind]);
    }

    if (p->name && *(p->name)) {
        fprintf(fp, "na %s\n", p->name);
    }

    item_list_print(fp, "il\t", p->items);
    trade_list_print(fp, "tl\t", p->trades);
    effect_list_print(fp, "el\t", p->effects);

    if (pd = rp_disp(n)) {
        boxlist_print(fp, "an ", pd->neutral);
        boxlist_print(fp, "ad ", pd->defend);
        boxlist_print(fp, "ah ", pd->hostile);
    }

    if (vp = rp_loc_info(n)) { print_loc_info(fp, vp); }
    if (vp = rp_char(n)) { print_char(fp, vp); }
    if (vp = rp_magic(n)) { print_magic(fp, vp); }
    if (vp = rp_loc(n)) { print_loc(fp, vp); }
    if (vp = rp_subloc(n)) { print_subloc(fp, vp); }
    if (vp = rp_item(n)) { print_item(fp, vp); }
    if (vp = rp_item_magic(n)) { print_item_magic(fp, vp); }
    if (vp = rp_item_artifact(n)) { print_artifact(fp, vp); }
    if (vp = rp_player(n)) { print_player(fp, vp); }
    if (vp = rp_skill(n)) { print_skill(fp, vp); }
    if (vp = rp_gate(n)) { print_gate(fp, vp); }
    if (vp = rp_misc(n)) { print_misc(fp, vp); }
    if (vp = rp_command(n)) { print_command(fp, vp); }
    if (vp = rp_ship(n)) { print_ship(fp, vp); }
    /*
	 *  Thu Jan 21 07:13:13 1999 -- Scott Turner
	 *
	 *  Don't actually need to save this info, since it
	 *  should not change.
	 *
	 *  Fri Mar 30 17:35:08 2001 -- Scott Turner
	 *
	 *  It does now!
	 *
	 */
    if (vp = rp_nation(n)) { print_nation(fp, vp); }

    fprintf(fp, "\n");

    bx[n]->temp = 1;        /* mark for write_leftovers() */
}


static FILE *
open_write_fp(char *fnam) {
    char *path;
    FILE *fp;

    path = sout("%s/%s", libdir, fnam);

    fp = fopen(path, "w");

    if (fp == NULL) {
        fprintf(stderr, "open_write_fp: can't open %s: ", path);
        perror("");
    }

    return fp;
}


static void
write_kind(int box_kind, char *fnam) {
    FILE *fp;
    int i;

    fp = open_write_fp(fnam);

    if (fp != NULL) {
        loop_kind(box_kind, i)
                {
                    save_box(fp, i);
                }
        next_kind;

        fclose(fp);
    }
}


void
write_player(int pl) {
    int who;
    FILE *fp;

    fp = open_write_fp(sout("fact/%d", pl));

    if (fp == NULL) {
        return;
    }

    save_box(fp, pl);

    loop_units(pl, who)
            {
                assert(kind(who) == T_char || kind(who) == T_deleted);

                save_box(fp, who);
            }
    next_unit;

    fclose(fp);
}


static void
write_chars() {
    int i;

    loop_player(i)
            {
                write_player(i);
            }
    next_player;
}


static void
write_leftovers() {
    FILE *fp;
    int i;

    fp = open_write_fp("misc");

    if (fp != NULL) {
        for (i = 0; i < MAX_BOXES; i++) {
            if (bx[i] != NULL && kind(i) != T_nation &&
                bx[i]->temp == 0) {
                if (kind(i) != T_deleted) {
                    save_box(fp, i);
                }
            }
        }

        fclose(fp);
    }
}


static void
read_boxes(char *fnam) {
    int box_num;
    char *path;

    path = sout("%s/%s", libdir, fnam);
    if (!readfile(path)) {
        return;
    }

    advance;

    while (line) {
        if (*line == '#') {
            advance;
            continue;        /* skip comment lines */
        };

        box_num = atoi(line);
        if (box_num > 0) {
            load_box(box_num);
        } else {
            fprintf(stderr, "read_boxes(%s): unexpected line %s\n",
                    fnam, line);
            advance;
        };
    }
}


static void
read_chars() {
    DIR *d;
    struct dirent *e;
    char *fnam;

    fnam = sout("%s/fact", libdir);
    d = opendir(fnam);

    if (d == NULL) {
        fprintf(stderr, "read_chars: can't open %s: ", fnam);
        perror("");
        return;
    }

    while ((e = readdir(d)) != NULL) {
        if (*(e->d_name) >= '0' &&
            *(e->d_name) <= '9' &&
            /* Ignore anything ending in ~! */
            e->d_name[strlen(e->d_name) - 1] != '~') {
            read_boxes(sout("fact/%s", e->d_name));
        }
    }

    closedir(d);
}


static int
fast_scan() {
    char *path;
    char *s, *p, *q;
    int num;
    int kind, sk;

    path = sout("%s/master", libdir);
    if (!readfile(path)) {
        return FALSE;
    }

    while (s = readlin()) {
        num = atoi(s);

        for (p = s; *p && isdigit(*p); p++) {}
        while (*p && iswhite(*p)) {
            p++;
        }


        for (q = p; *q && *q != '.'; q++) {}
        if (*q == '.') {
            *q++ = '\0';
        }

        kind = atoi(p);
        sk = atoi(q);

        alloc_box(num, kind, sk);
    }

    return TRUE;
}


static void
scan_boxes(char *fnam) {
    int box_num;
    char *path;
    char *s, *t;
    int kind;
    int sk;

    path = sout("%s/%s", libdir, fnam);
    if (!readfile(path)) {
        return;
    }

    while (s = readlin()) {
        if (!isdigit(*s)) {
            continue;
        }

/*
		if (*s == '#')
			continue;
*/

/*
 *  Parse something of the form: box-number kind subkind
 *  example:  10 item artifact
 */

        box_num = atoi(s);

        while (*s && *s != ' ') {        /* skip over space to kind */
            s++;
        }
        if (*s == ' ') {
            s++;
        }

        for (t = s; *t && *t != ' '; t++) {    /* to subkind */
        }
        if (*t == ' ') {
            *t++ = '\0';
        }

        kind = lookup(kind_s, s);

        if (kind < 0) {
            fprintf(stderr, "read_boxes(%d): bad kind: %s\n",
                    box_num, s);
            kind = 0;
        }

        if (*t == '0') {
            sk = 0;
        } else {
            sk = lookup(subkind_s, t);
        }

        if (sk < 0) {
            fprintf(stderr, "read_boxes(%d): bad subkind: %s\n",
                    box_num, t);
            sk = 0;
        }

        alloc_box(box_num, kind, sk);

        while (s = readlin()) {
            if (*s == '#') {        /* skip comments */
                continue;
            }

            while (*s && iswhite(*s)) {
                s++;
            }

            if (*s == '\0') {        /* blank line, end of entry */
                break;
            }
        }
    }
}


static void
scan_chars() {
    DIR *d;
    struct dirent *e;
    char *fnam;

    fnam = sout("%s/fact", libdir);
    d = opendir(fnam);

    if (d == NULL) {
        fprintf(stderr, "scan_chars: can't open %s: ", fnam);
        perror("");
        return;
    }

    while ((e = readdir(d)) != NULL) {
        if (*(e->d_name) >= '0' && *(e->d_name) <= '9') {
            scan_boxes(sout("fact/%s", e->d_name));
        }
    }

    closedir(d);
}


/*
 *  Scan through all of the entity data files, calling alloc_box
 *  for each entity once its number and kind are known.
 *
 *  We do this so it is possible to perform type and sanity checking when
 *  the contents of the boxes are read in the second pass (read_boxes).
 */

static void
scan_all_boxes() {

    stage("fast_scan failed, scan_all_boxes()");

    scan_boxes("loc");
    scan_boxes("item");
    scan_boxes("skill");
    scan_boxes("gate");
    scan_boxes("road");
    scan_boxes("ship");
    scan_boxes("unform");
    scan_boxes("misc");
    scan_boxes("nation");

    scan_chars();
}


static void
read_all_boxes() {

    read_boxes("loc");
    read_boxes("item");
    read_boxes("skill");
    read_boxes("gate");
    read_boxes("road");
    read_boxes("ship");
    read_boxes("unform");
    read_boxes("misc");
    read_boxes("nation");

    read_chars();
}


static void
write_all_boxes() {
    int i;

    for (i = 0; i < MAX_BOXES; i++) {
        if (bx[i] != NULL) {
            bx[i]->temp = 0;
        }
    }

    system(sout("rm -rf %s/fact", libdir));
    makedir(sout("%s/fact", libdir), 0755);

    write_kind(T_loc, "loc");
    write_kind(T_item, "item");
    write_kind(T_skill, "skill");
    write_kind(T_gate, "gate");
    write_kind(T_road, "road");
    write_kind(T_ship, "ship");
    write_kind(T_unform, "unform");
    write_kind(T_nation, "nation");

    write_chars();
    write_leftovers();
}


static void
write_master() {
    FILE *fp;
    char *fnam;
    int i;
    char *s;

    fnam = sout("%s/master", libdir);
    fp = fopen(fnam, "w");

    if (fp == NULL) {
        fprintf(stderr, "can't write %s: ", fnam);
        perror("");
        return;
    }

    for (i = 0; i < MAX_BOXES; i++) {
        if (bx[i] != NULL) {
            bx[i]->temp = 0;
        }
    }

    for (i = 0; i < MAX_BOXES; i++) {
        if (kind(i) != T_deleted) {
            s = name(i);

            switch (kind(i)) {
                case 0:
                    break;

                case T_loc:
                case T_item:
                case T_skill:
                case T_gate:
                case T_ship:
                case T_unform:
                    fprintf(fp, "%d\t%d.%d\t%s\t\t%s\n",
                            i,
                            bx[i]->kind, bx[i]->skind,
                            kind_s[bx[i]->kind], s);
                    bx[i]->temp = 1;
                    break;

                case T_player:
                    fprintf(fp, "%d\t%d.%d\tfact/%d\t%s\n",
                            i, bx[i]->kind, bx[i]->skind, i, s);
                    bx[i]->temp = 1;
                    break;

                case T_char:
                    fprintf(fp, "%d\t%d.%d\tfact/%d\t%s\n",
                            i,
                            bx[i]->kind, bx[i]->skind,
                            player(i), s);
                    bx[i]->temp = 1;
                    break;
            }
        }
    }

    for (i = 0; i < MAX_BOXES; i++) {
        if (kind(i) != T_deleted && bx[i]->temp == 0) {
            s = name(i);
            fprintf(fp, "%d\t%d.%d\tmisc\t%s\n",
                    i, bx[i]->kind, bx[i]->skind, s);
        }
    }

    fclose(fp);
}

#if 0
                                                                                                                        static void
scan_nation(struct entity_nation *n)
{
  char *t;
  int c;

  advance;
  while (line && iswhite(*line)) {
    if (*line == '#') continue;

    line++;
    c = linehash(line);
    t = t_string(line);

    switch (c) {
    case 'na':	{
      n->name = str_save(t);
      break;
    };
    case 'ci':	n->citizen = str_save(t); break;
    case 'cp':	n->capital = box_scan(t); break;
    case 'wi':	n->win = atoi(t); break;
    case 'pl':  n->player_limit = atoi(t); break;
    case 'js':  n->jump_start = atoi(t); break;
    case 'ps':
      boxlist_scan(t, -1, &(n->proscribed_skills));
      break;

    case 0:
    default:
      fprintf(stderr, "scan_nation:  bad line: %s\n", line);
    }
    advance;
  }
}

/*
 *  Tue Apr  8 11:55:15 1997 -- Scott Turner
 *
 *  The nations file.
 *
 *  Fri Jan 15 14:20:53 1999 -- Scott Turner
 *
 */
static void
scan_nations()
{
  char *s;
  char *fname;
  int i;

  fname = sout("%s/nations", libdir);
  if (!readfile(fname)) {
    fprintf(stderr, "scan_nations: can't read %s: ", fname);
    perror("");
    return;
  };

  s = readlin();

  if (s == NULL || (num_nations = atoi(s)) == 0) {
    fprintf(stderr,"No nations number in nations file?\n");
    return;
  };

  if (num_nations >= MAX_NATIONS) {
    fprintf(stderr,"Too many nations (%d) in nations file.\n",num_nations);
    return;
  };

  nations[0].name = "";
  nations[0].citizen = "";

  for(i=1;i<=num_nations;i++) {
    scan_nation(&nations[i]);
  };
};

/*
 *  Tue Apr  8 11:55:15 1997 -- Scott Turner
 *
 *  The nations file.
 *
 */
static void
save_nations()
{
  FILE *fp;
  int i;

  fp = open_write_fp("nation");
  if (fp == NULL) return;

  fprintf(fp,"%d\n",num_nations);
  for(i=1;i<=num_nations;i++)
    save_nation(fp, &nations[i]);

  fclose(fp);

};
#endif

/*
 *  The sysclock and the random number seed are stored separately
 *  from the entity data, in a file called "system".
 */

static void
load_system() {
    FILE *fp;
    char *s;
    char *fname;
    extern short seed[3];
    char tmp[80];

    fname = sout("%s/system", libdir);

    fp = fopen(fname, "r");

    if (fp == NULL) {
        fprintf(stderr, "load_system: can't read %s: ", fname);
        perror("");
        return;
    }

    while (s = getlin_ew(fp)) {
        if (*s == '\0' || *s == '#') {    /* comments and blank lines */
            continue;
        }

        if (strncmp(s, "sysclock:", 9) == 0) {
            olytime_scan(&s[9], &sysclock);
        } else if (strncmp(s, "game_num=", 9) == 0) {
            game_number = atoi(&s[9]);
        } else if (strncmp(s, "from_host=", 10) == 0) {
            from_host = str_save(&s[10]);
        } else if (strncmp(s, "reply_host=", 11) == 0) {
            reply_host = str_save(&s[11]);
        } else if (strncmp(s, "indep_player=", 13) == 0) {
            indep_player = atoi(&s[13]);
        } else if (strncmp(s, "gm_player=", 10) == 0) {
            gm_player = atoi(&s[10]);
        } else if (strncmp(s, "skill_player=", 13) == 0) {
            skill_player = atoi(&s[13]);
        } else if (strncmp(s, "seed0=", 6) == 0) {
            seed[0] = atoi(&s[6]);
        } else if (strncmp(s, "seed1=", 6) == 0) {
            seed[1] = atoi(&s[6]);
        } else if (strncmp(s, "seed2=", 6) == 0) {
            seed[2] = atoi(&s[6]);
        } else if (strncmp(s, "post=", 5) == 0) {
            post_has_been_run = atoi(&s[5]);
        } else if (strncmp(s, "init=", 5) == 0) {
            seed_has_been_run = atoi(&s[5]);
        } else if (strncmp(s, "ds=", 3) == 0) {
            dist_sea_compute = atoi(&s[3]);
        } else if (strncmp(s, "nc=", 3) == 0) {
            near_city_init = atoi(&s[3]);
        } else if (strncmp(s, "mi=", 3) == 0) {
            cookie_init = atoi(&s[3]);
        } else if (strncmp(s, "fr=", 3) == 0) {
            faery_region = atoi(&s[3]);
        } else if (strncmp(s, "fp=", 3) == 0) {
            faery_player = atoi(&s[3]);
        } else if (strncmp(s, "hr=", 3) == 0) {
            hades_region = atoi(&s[3]);
        } else if (strncmp(s, "hp=", 3) == 0) {
            hades_pit = atoi(&s[3]);
        } else if (strncmp(s, "hl=", 3) == 0) {
            hades_player = atoi(&s[3]);
        } else if (strncmp(s, "np=", 3) == 0) {
            npc_pl = atoi(&s[3]);
        } else if (strncmp(s, "ms=", 3) == 0) {
            monster_subloc_init = atoi(&s[3]);
        } else if (strncmp(s, "pi=", 3) == 0) {
            population_init = atoi(&s[3]);
        } else if (strncmp(s, "cr=", 3) == 0) {
            cloud_region = atoi(&s[3]);
        } else if (strncmp(s, "cp=", 3) == 0) {
            combat_pl = atoi(&s[3]);
        } else if (strncmp(s, "ysize=", 6) == 0) {
            ysize = atoi(&s[6]);
        } else if (strncmp(s, "xsize=", 6) == 0) {
            xsize = atoi(&s[6]);
        } else if (strncmp(s, "turn_limit=", 11) == 0) {
            options.turn_limit = atoi(&s[11]);
        } else if (strncmp(s, "autodrop=", 9) == 0) {
            options.auto_drop = atoi(&s[9]);
        } else if (strncmp(s, "free=", 5) == 0) {
            options.free = atoi(&s[5]);
        } else if (strncmp(s, "turn_charge=", 12) == 0) {
            strncpy(options.turn_charge, &s[12], 23);
        } else if (strncmp(s, "mp_antipathy=", 13) == 0) {
            options.mp_antipathy = atoi(&s[13]);
        } else if (strncmp(s, "survive_np=", 11) == 0) {
            options.survive_np = atoi(&s[11]);
        } else if (strncmp(s, "death_nps=", 10) == 0) {
            options.death_nps = atoi(&s[10]);
        } else if (strncmp(s, "guild_teaching=", 15) == 0) {
            options.guild_teaching = atoi(&s[15]);
        } else if (strncmp(s, "deserted_player=", 16) == 0) {
            deserted_player = atoi(&s[16]);
        } else if (strncmp(s, "accounting_dir=", 15) == 0) {
            options.accounting_dir = str_save(&s[15]);
            /* strncpy(options.accounting_dir, &s[15], 80); */
        } else if (strncmp(s, "accounting_prog=", 16) == 0) {
            options.accounting_prog = str_save(&s[16]);
            /* strncpy(options.accounting_prog, &s[16], 80); */
        } else if (strncmp(s, "html_path=", 10) == 0) {
            options.html_path = str_save(&s[10]);
        } else if (strncmp(s, "html_passwords=", 15) == 0) {
            options.html_passwords = str_save(&s[15]);
        } else if (strncmp(s, "times_pay=", 10) == 0) {
            options.times_pay = atoi(&s[10]);
        } else if (strncmp(s, "open_ended=", 11) == 0) {
            options.open_ended = atoi(&s[11]);
        } else if (strncmp(s, "cpp=", 4) == 0) {
            strncpy(options.cpp, &s[4], 80);
        } else if (strncmp(s, "full_markets=", 13) == 0) {
            options.full_markets = atoi(&s[13]);
        } else if (strncmp(s, "output_tags=", 12) == 0) {
            options.output_tags = atoi(&s[12]);
        } else if (strncmp(s, "num_books=", 10) == 0) {
            options.num_books = atoi(&s[10]);
        } else if (strncmp(s, "market_age=", 11) == 0) {
            options.market_age = atoi(&s[11]);
        } else if (strncmp(s, "min_piety=", 10) == 0) {
            options.min_piety = atoi(&s[10]);
        } else if (strncmp(s, "piety_limit=", 12) == 0) {
            options.piety_limit = atoi(&s[12]);
        } else if (strncmp(s, "head_priest_piety_limit=", 24) == 0) {
            options.head_priest_piety_limit = atoi(&s[24]);
        } else if (strncmp(s, "top_piety=", 10) == 0) {
            options.top_piety = atoi(&s[10]);
        } else if (strncmp(s, "middle_piety=", 13) == 0) {
            options.middle_piety = atoi(&s[13]);
        } else if (strncmp(s, "bottom_piety=", 13) == 0) {
            options.bottom_piety = atoi(&s[13]);
        } else if (strncmp(s, "claim_give=", 11) == 0) {
            options.claim_give = atoi(&s[11]);
        } else if (strncmp(s, "check_balance=", 14) == 0) {
            options.check_balance = atoi(&s[14]);
        } else if (strncmp(s, "free_np_limit=", 14) == 0) {
            options.free_np_limit = atoi(&s[14]);
        } else {
            fprintf(stderr, "load_system: unrecognized "
                            "line: %s\n", s);
        }
    }

    /*
	 *  Provide default values for piety if necessary.
	 *
	 */
    if (!options.top_piety &&
        !options.middle_piety &&
        !options.bottom_piety) {
        options.top_piety = 12;
        options.middle_piety = 6;
        options.bottom_piety = 3;
    };


    fclose(fp);
}


static void
save_system() {
    FILE *fp;
    char *fname;
    extern short seed[3];

    fname = sout("%s/system", libdir);

    fp = fopen(fname, "w");

    if (fp == NULL) {
        fprintf(stderr, "load_system: can't write %s: ", fname);
        perror("");
        return;
    }

    olytime_print(fp, "sysclock: ", &sysclock);
    fprintf(fp, "game_num=%d\n", game_number);
    fprintf(fp, "indep_player=%d\n", indep_player);
    fprintf(fp, "gm_player=%d\n", gm_player);
    fprintf(fp, "skill_player=%d\n", skill_player);
    fprintf(fp, "from_host=%s\n", from_host);
    fprintf(fp, "reply_host=%s\n", reply_host);
    fprintf(fp, "seed0=%d\n", seed[0]);
    fprintf(fp, "seed1=%d\n", seed[1]);
    fprintf(fp, "seed2=%d\n", seed[2]);
    fprintf(fp, "post=%d\n", post_has_been_run);
    fprintf(fp, "init=%d\n", seed_has_been_run);
    fprintf(fp, "ds=%d\n", dist_sea_compute);
    fprintf(fp, "nc=%d\n", near_city_init);
    fprintf(fp, "mi=%d\n", cookie_init);
    fprintf(fp, "fr=%d\n", faery_region);
    fprintf(fp, "fp=%d\n", faery_player);
    fprintf(fp, "hr=%d\n", hades_region);
    fprintf(fp, "hp=%d\n", hades_pit);
    fprintf(fp, "hl=%d\n", hades_player);
    fprintf(fp, "np=%d\n", npc_pl);
    fprintf(fp, "ms=%d\n", monster_subloc_init);
    fprintf(fp, "pi=%d\n", population_init);
    fprintf(fp, "cr=%d\n", cloud_region);
    fprintf(fp, "cp=%d\n", combat_pl);
    fprintf(fp, "xsize=%d\n", xsize);
    fprintf(fp, "ysize=%d\n", ysize);
    fprintf(fp, "turn_limit=%d\n", options.turn_limit);
    fprintf(fp, "autodrop=%d\n", options.auto_drop);
    fprintf(fp, "free=%d\n", options.free);
    fprintf(fp, "turn_charge=%s\n", options.turn_charge);
    fprintf(fp, "mp_antipathy=%d\n", options.mp_antipathy);
    fprintf(fp, "survive_np=%d\n", options.survive_np);
    fprintf(fp, "death_nps=%d\n", options.death_nps);
    fprintf(fp, "guild_teaching=%d\n", options.guild_teaching);
    fprintf(fp, "accounting_dir=%s\n", options.accounting_dir);
    fprintf(fp, "accounting_prog=%s\n", options.accounting_prog);
    fprintf(fp, "html_path=%s\n", options.html_path);
    fprintf(fp, "html_passwords=%s\n", options.html_passwords);
    fprintf(fp, "times_pay=%d\n", options.times_pay);
    fprintf(fp, "open_ended=%d\n", options.open_ended);
    fprintf(fp, "cpp=%s\n", options.cpp);
    fprintf(fp, "full_markets=%d\n", options.full_markets);
    fprintf(fp, "output_tags=%d\n", options.output_tags);
    fprintf(fp, "num_books=%d\n", options.num_books);
    fprintf(fp, "market_age=%d\n", options.market_age);
    fprintf(fp, "min_piety=%d\n", options.min_piety);
    fprintf(fp, "piety_limit=%d\n", options.piety_limit);
    fprintf(fp, "head_priest_piety_limit=%d\n", options.head_priest_piety_limit);
    fprintf(fp, "top_piety=%d\n", options.top_piety);
    fprintf(fp, "middle_piety=%d\n", options.middle_piety);
    fprintf(fp, "bottom_piety=%d\n", options.bottom_piety);
    fprintf(fp, "claim_give=%d\n", options.claim_give);
    fprintf(fp, "check_balance=%d\n", options.check_balance);
    fprintf(fp, "free_np_limit=%d\n", options.free_np_limit);

    fclose(fp);
}


static void
delete_deadchars() {
    int i;

    loop_kind(T_deadchar, i)
            {
/*
 *  Loc should have been zeroed already in kill_char
 */

                if (loc(i)) {
                    set_where(i, 0);
                }
                delete_box(i);
            }
    next_kind;
}


void
load_db() {

    stage("load_db()");

/*
 *  Assertions to verify the sanity of the linehash macro
 *  Switch the byte ordering if this fails
 */

    assert(linehash("ab f") == 'ab');
    assert(linehash("") == 0);
    assert(linehash("na") == 'na');
    assert(linehash("ab ") == 'ab');

    load_system();

    if (!fast_scan()) {    /* pass 1: call alloc_box for each entity */
        scan_all_boxes();
    }

    read_all_boxes();    /* pass 2: read the entity attributes */
    /*
	 *  At this point we should be able to set the MAX_MM
	 *  to be that of a dragon.
	 *
	 *  Mon Jun 14 07:27:07 1999 -- Scott Turner
	 *
	 *  Using nazgul, since dragon is an outlier.
	 *
	 */
    MAX_MM = MM(item_nazgul);

    load_orders();

#if 0
    scan_nations();         /* Here so the boxes are valid... */
#endif

    check_db();            /* check database integrity */
    determine_map_edges();        /* initialization for map routines */

    if (!post_has_been_run) {
        stage("INIT: post_production()");
        post_production();
    }

    if (!seed_has_been_run) {
        stage("INIT: seed_initial_locations()");
        seed_initial_locations();
        seed_orcs();
    }

    if (faery_region == 0) {
        create_faery();
    }

    if (hades_region == 0) {
        create_hades();
    }

    if (cloud_region == 0) {
        create_cloudlands();
    }

    if (!dist_sea_compute) {
        compute_dist();
        dist_sea_compute = TRUE;
    }

    if (!near_city_init) {
        seed_city_near_lists();
        near_city_init = TRUE;
    }

    if (!cookie_init) {
        seed_cookies();
        cookie_init = TRUE;
    }

    if (monster_subloc_init == 0) {
        seed_monster_sublocs();
        monster_subloc_init = TRUE;
    }

    if (population_init == 0) {
        seed_population();
        population_init = TRUE;
    };

    if (combat_pl == 0 || bx[combat_pl] == 0) {
        combat_pl = 210;

        alloc_box(combat_pl, T_player, sub_pl_npc);
        set_name(combat_pl, "Combat log");
        p_player(combat_pl)->password = str_save(DEFAULT_PASSWORD);
        fprintf(stderr, "\tcreated combat player %d\n", combat_pl);
    }

    init_ocean_chars();
    delete_deadchars();
}


void
cleanup_posts() {
    int i;

    loop_post(i)
            {
                set_where(i, 0);
                delete_box(i);
            }
    next_post;
}


void
save_logdir() {
    int ret;
    char *s, *t;

    system(sout("rm -rf %s/save/%d", libdir, sysclock.turn));
    makedir(sout("%s/save", libdir), 0755);

    s = sout("%s/log", libdir);
    t = sout("%s/save/%d", libdir, sysclock.turn);

    ret = rename(s, t);

    if (ret < 0) {
        fprintf(stderr, "couldn't rename %s to %s:", s, t);
        perror("");
    }

    s = sout("%s/players.html", libdir);
    t = sout("%s/save/%d/players.html", libdir, sysclock.turn);

    ret = rename(s, t);

    if (ret < 0) {
        fprintf(stderr, "couldn't rename %s to %s:", s, t);
        perror("");
    }

    makedir(sout("%s/log", libdir), 0755);
}


void
save_db() {

    stage("save_db()");
    cleanup_posts();
    save_system();
    write_all_boxes();
    write_master();
    save_orders();
    rename_act_join_files();
}

